#include <bk/fs/vfs.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <bk/assert.h>
#include <string.h>
#include <errno.h>

typedef struct
{
	bk_fs_t bk_fs;
	bk_allocator_t* allocator;
	bk_dlist_link_t mounts;
} bk_vfs_t;

static int
bk_vfs_open(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	bk_vfs_t* self = BK_CONTAINER_OF(fs, bk_vfs_t, bk_fs);

	if(strstr(path, "..") != NULL) { return EOPNOTSUPP; }

	for(
		const bk_dlist_link_t* mount_link = self->mounts.next;
		mount_link != &self->mounts;
		mount_link = mount_link->next
	)
	{
		const bk_vfs_mount_t* mount = BK_CONTAINER_OF(mount_link, bk_vfs_mount_t, link);
		size_t mount_len = strlen(mount->path);
		if(strncmp(path, mount->path, mount_len) == 0 && path[mount_len] == '/')
		{
			return bk_fopen(mount->subfs, path + mount_len + 1, mode, file);
		}
	}

	return ENOENT;
}

struct bk_fs_s*
bk_vfs_create(struct bk_allocator_s* allocator)
{
	bk_vfs_t* fs = BK_NEW(allocator, bk_vfs_t);
	*fs = (bk_vfs_t){
		.bk_fs = { .open = &bk_vfs_open },
		.allocator = allocator
	};

	bk_dlist_init(&fs->mounts);

	return &fs->bk_fs;
}

void
bk_vfs_destroy(struct bk_fs_s* vfs)
{
	bk_vfs_t* self = BK_CONTAINER_OF(vfs, bk_vfs_t, bk_fs);
	bk_free(self->allocator, self);
}

void
bk_vfs_mount(struct bk_fs_s* vfs, bk_vfs_mount_t* mount)
{
	bk_vfs_t* self = BK_CONTAINER_OF(vfs, bk_vfs_t, bk_fs);
	BK_ASSERT(mount->path[0] == '/', "Mount path must be absolute");
	bk_dlist_prepend(&self->mounts, &mount->link);
}

void
bk_vfs_umount(struct bk_fs_s* vfs, bk_vfs_mount_t* mount)
{
	(void)vfs;
	bk_dlist_unlink(&mount->link);
}
