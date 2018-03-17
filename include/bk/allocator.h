#ifndef BK_ALLOCATOR
#define BK_ALLOCATOR

#include <stddef.h>
#include "macro.h"

typedef struct bk_allocator_s bk_allocator_t;

struct bk_allocator_s
{
	void*(*realloc)(bk_allocator_t* allocator, void* ptr, size_t size);
};

BK_INLINE void*
bk_realloc(bk_allocator_t* allocator, void* ptr, size_t size)
{
	return allocator->realloc(allocator, ptr, size);
}

BK_INLINE void*
bk_malloc(bk_allocator_t* allocator, size_t size)
{
	return bk_realloc(allocator, NULL, size);
}

BK_INLINE void
bk_free(bk_allocator_t* allocator, void* ptr)
{
	bk_realloc(allocator, ptr, 0);
}

#endif
