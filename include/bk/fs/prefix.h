#ifndef BK_PREFIX_FS_H
#define BK_PREFIX_FS_H

#include "../decl.h"

struct bk_fs_s;
struct bk_allocator_s;

BK_DECL struct bk_fs_s*
bk_prefix_fs_create(
	struct bk_allocator_s* allocator,
	struct bk_fs_s* backing_fs,
	const char* prefix
);

BK_DECL void
bk_prefix_fs_destroy(struct bk_fs_s* fs);

#endif
