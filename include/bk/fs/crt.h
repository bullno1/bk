#ifndef BK_CRT_FS_H
#define BK_CRT_FS_H

#include "../decl.h"

struct bk_fs_s;
struct bk_allocator_s;

BK_DECL struct bk_fs_s*
bk_crt_fs_create(struct bk_allocator_s* allocator);

BK_DECL void
bk_crt_fs_destroy(struct bk_fs_s* fs);

#ifdef BK_IMPLEMENTATION
#include "../../../src/fs/crt.c"
#endif

#endif
