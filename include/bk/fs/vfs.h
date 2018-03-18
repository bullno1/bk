#ifndef BK_VFS_H
#define BK_VFS_H

#include "../decl.h"
#include "../dlist.h"

struct bk_allocator_s;
struct bk_fs_s;

typedef struct bk_vfs_mount_s bk_vfs_mount_t;

struct bk_vfs_mount_s
{
	bk_dlist_link_t link;
	const char* path;
	struct bk_fs_s* subfs;
};

BK_DECL struct bk_fs_s*
bk_vfs_create(struct bk_allocator_s* allocator);

BK_DECL void
bk_vfs_destroy(struct bk_fs_s* vfs);

BK_DECL void
bk_vfs_mount(struct bk_fs_s* vfs, bk_vfs_mount_t* mount);

BK_DECL void
bk_vfs_umount(struct bk_fs_s* vfs, bk_vfs_mount_t* mount);

#endif
