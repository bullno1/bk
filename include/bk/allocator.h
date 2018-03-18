#ifndef BK_ALLOCATOR_H
#define BK_ALLOCATOR_H

#include <stddef.h>
#include "macro.h"
#include "assert.h"

#define BK_NEW(ALLOCATOR, TYPE) (TYPE*)(bk_malloc(ALLOCATOR, sizeof(TYPE)))
#define BK_REALLOC_FREE(PTR, SIZE) (ptr != 0 && size == 0)

typedef struct bk_allocator_s bk_allocator_t;

struct bk_allocator_s
{
	void*(*realloc)(bk_allocator_t* allocator, void* ptr, size_t size);
};

BK_INLINE void*
bk_unsafe_realloc(bk_allocator_t* allocator, void* ptr, size_t size)
{
	return allocator->realloc(allocator, ptr, size);
}

BK_INLINE void*
bk_unsafe_malloc(bk_allocator_t* allocator, size_t size)
{
	return bk_unsafe_realloc(allocator, NULL, size);
}

BK_INLINE void
bk_free(bk_allocator_t* allocator, void* ptr)
{
	bk_unsafe_realloc(allocator, ptr, 0);
}

BK_INLINE void*
bk_realloc(bk_allocator_t* allocator, void* ptr, size_t size)
{
	void* result = bk_unsafe_realloc(allocator, ptr, size);
	BK_CRITICAL_ASSERT(size == 0 || result != NULL, "Out of memory");

	return result;
}

BK_INLINE void*
bk_malloc(bk_allocator_t* allocator, size_t size)
{
	return bk_realloc(allocator, NULL, size);
}

#endif
