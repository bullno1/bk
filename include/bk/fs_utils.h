#ifndef BK_FS_UTILS_H
#define BK_FS_UTILS_H

#include <stddef.h>
#include "decl.h"

struct bk_fs_s;
struct bk_allocator_s;

BK_DECL int
bk_read_file(
	struct bk_fs_s* fs, const char* path,
	struct bk_allocator_s* allocator, void** bufp, size_t* sizep
);

BK_DECL int
bk_write_file(struct bk_fs_s* fs, const char* path, const void* buf, size_t size);

#endif
