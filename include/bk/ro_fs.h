#ifndef BK_RO_FS_H
#define BK_RO_FS_H

#include "decl.h"

struct bk_fs_s;
struct bk_allocator_s;

BK_DECL struct bk_fs_s*
bk_ro_fs_create(struct bk_allocator_s* allocator, struct bk_fs_s* backing_fs);

BK_DECL void
bk_ro_fs_destroy(struct bk_fs_s* fs);

#endif
