#include <bk/array.h>
#include <string.h>
#include <bk/allocator.h>

typedef struct
{
	bk_allocator_t* allocator;
	size_t elem_size;
	size_t length;
	size_t capacity;
	uint8_t alignment;
	uint8_t padding;
} bk_array_t;

static inline bk_array_t*
bk_array_head(const void* ptr)
{
	const char* body = ptr;
	return (bk_array_t*)(body - (*(body - 1)));
}

void*
bk_array__create(
	bk_allocator_t* allocator,
	size_t elem_size,
	uint8_t alignment,
	size_t capacity
)
{
	size_t mem_required = sizeof(bk_array_t) + elem_size * capacity + alignment - 1;
	bk_array_t* array = bk_malloc(allocator, mem_required);
	*array = (bk_array_t){
		.allocator = allocator,
		.elem_size = elem_size,
		.alignment = alignment,
		.capacity = capacity
	};

	char* body = BK_ALIGN_PTR((char*)array + sizeof(bk_array_t), alignment);
	char offset = body - (char*)array;
	*(body - 1) = offset;
	return body;
}

void
bk_array_destroy(void* array)
{
	bk_array_t* head = bk_array_head(array);
	bk_free(head->allocator, head);
}

static
void* bk_array__realloc(void* array, size_t new_capacity)
{
	bk_array_t* head = bk_array_head(array);
	char old_offset = (char*)array - (char*)head;

	size_t mem_required =
		sizeof(bk_array_t) + head->elem_size * new_capacity + head->alignment - 1;
	head = bk_realloc(head->allocator, head, mem_required);
	head->capacity = new_capacity;

	char* new_body = BK_ALIGN_PTR((char*)head + sizeof(bk_array_t), head->alignment);
	if(new_body != (char*)head + old_offset)
	{
		memmove(new_body, (char*)head + old_offset, head->elem_size * head->length);
	}
	*(new_body - 1) = new_body - (char*)head;

	return new_body;
}

void*
bk_array__resize(void* array, size_t new_length)
{
	bk_array_t* head = bk_array_head(array);
	if(new_length <= head->capacity)
	{
		head->length = new_length;
		return array;
	}
	else
	{
		void* new_array = bk_array__realloc(array, new_length);
		bk_array_head(new_array)->length = new_length;
		return new_array;
	}
}

void*
bk_array__prepare_push(void* array)
{
	bk_array_t* head = bk_array_head(array);
	size_t new_length = head->length + 1;
	if(new_length <= head->capacity)
	{
		head->length = new_length;
		return array;
	}
	else
	{
		void* new_array = bk_array__realloc(array, BK_MAX(head->capacity * 2, new_length));
		bk_array_head(new_array)->length = new_length;
		return new_array;
	}
}

size_t
bk_array_len(const void* array)
{
	return bk_array_head(array)->length;
}

void
bk_array_clear(void* array)
{
	bk_array_head(array)->length = 0;
}

void
bk_array_remove(void* array, size_t index)
{
	bk_array_t* head = bk_array_head(array);
	void* dst = (char*)array + head->elem_size * index;
	void* src = (char*)array + head->elem_size * (index + 1);
	size_t size = (head->length - index - 1) * head->elem_size;
	memmove(dst, src, size);
	--head->length;
}

void*
bk_array__prepare_insert(void* array, size_t index)
{
	bk_array_resize(array, bk_array_len(array) + 1);
	bk_array_t* head = bk_array_head(array);

	void* dst = (char*)array + head->elem_size * (index + 1);
	void* src = (char*)array + head->elem_size * index;
	size_t size = (head->length - index - 1) * head->elem_size;
	memmove(dst, src, size);
	return array;
}
