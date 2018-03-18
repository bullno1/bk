#include <bk/fs/ro.h>
#include <bk/fs.h>
#include <bk/fs/utils.h>
#include <bk/allocator.h>
#include <errno.h>

typedef struct
{
	bk_fs_t bk_fs;
	bk_allocator_t* allocator;
	bk_fs_t* backing_fs;
} bk_ro_fs_t;

static int
bk_ro_fs_open(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);

	if(!bk_is_mode_ro(mode)) { return EROFS; }

	int err = bk_fopen(self->backing_fs, path, mode, file);

	(*file)->fs = fs;

	return err;
}

static void
bk_ro_fs_close(bk_fs_t* fs, bk_file_t* file)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	self->backing_fs->close(self->backing_fs, file);
}

static int
bk_ro_fs_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	return self->backing_fs->read(self->backing_fs, file, buf, len);
}

static int
bk_ro_fs_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len)
{
	(void)fs;
	(void)file;
	(void)buf;
	(void)len;
	return EROFS;
}

static int
bk_ro_fs_seek(bk_fs_t* fs, bk_file_t* file, size_t offset, int origin)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	return self->backing_fs->seek(self->backing_fs, file, offset, origin);
}

static int
bk_ro_fs_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	return self->backing_fs->tell(self->backing_fs, file, pos);
}

BK_DECL struct bk_fs_s*
bk_ro_fs_create(struct bk_allocator_s* allocator, struct bk_fs_s* backing_fs)
{
	bk_ro_fs_t* fs = BK_NEW(allocator, bk_ro_fs_t);
	*fs = (bk_ro_fs_t){
		.bk_fs = {
			.open = &bk_ro_fs_open,
			.close = &bk_ro_fs_close,
			.read = &bk_ro_fs_read,
			.write = &bk_ro_fs_write,
			.tell = &bk_ro_fs_tell,
			.seek = &bk_ro_fs_seek,
		},
		.allocator = allocator,
		.backing_fs = backing_fs
	};

	return &fs->bk_fs;
}

BK_DECL void
bk_ro_fs_destroy(struct bk_fs_s* fs)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	bk_free(self->allocator, self);
}
