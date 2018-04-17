#ifndef BK_DEFAULT_ALLOCATOR_H
#define BK_DEFAULT_ALLOCATOR_H

#include "decl.h"

BK_DECL struct bk_allocator_s* bk_default_allocator;

#ifdef BK_IMPLEMENTATION
#include "../../src/default_allocator.c"
#endif

#endif
