#include <bk/default_allocator.h>
#include <stdlib.h>
#include <bk/allocator.h>

static void*
bk_default_realloc(bk_allocator_t* allocator, void* ptr, size_t size)
{
	(void)allocator;

	if(size == 0)
	{
		free(ptr);
		return NULL;
	}
	else
	{
		void* result = realloc(ptr, size);
		return result;
	}
}

static bk_allocator_t bk_default_allocator_instance = { .realloc = bk_default_realloc };
bk_allocator_t* bk_default_allocator = &bk_default_allocator_instance;
