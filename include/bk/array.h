#ifndef BK_ARRAY_H
#define BK_ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include "decl.h"

struct bk_allocator_s;

/**
 * @brief Create a new array.
 * @param ALLOCATOR The allocator to use.
 * @param TYPE Type of elements.
 * @param CAPACITY Initial capacity.
 */
#define bk_array_create(ALLOCATOR, TYPE, CAPACITY) \
	((TYPE*)(bk_array__create(\
		(ALLOCATOR), \
		sizeof(TYPE), \
		BK_ALIGN_OF(TYPE), \
		(CAPACITY))))

/// Push an element to the end of the array.
#define bk_array_push(ARRAY, ITEM) \
	((ARRAY) = bk_array__prepare_push((ARRAY)), \
	 ((ARRAY))[bk_array_len((ARRAY)) - 1] = (ITEM))

/// Resize an array.
#define bk_array_resize(ARRAY, NEW_LENGTH) \
	((ARRAY) = bk_array__resize((ARRAY), (NEW_LENGTH)))

/// Get the begin iterator the array.
#define bk_array_begin(ARRAY) (ARRAY)

/// Get the end iterator of an array.
#define bk_array_end(ARRAY) ((ARRAY) + bk_array_len((ARRAY)))

/**
 * @brief Loop through an array.
 * @param TYPE element type.
 * @param VAR iterator variable.
 * @param ARRAY array to loop through.
 */
#define bk_array_foreach(TYPE, VAR, ARRAY) \
	for(TYPE* VAR = bk_array_begin((ARRAY)); VAR != bk_array_end((ARRAY)); ++(VAR))

/// Remove an item from an array by overwriting it with the last item.
#define bk_array_quick_remove(ARRAY, INDEX) \
	((ARRAY)[(INDEX)] = (ARRAY)[bk_array_len((ARRAY)) - 1], \
	 bk_array_resize((ARRAY), bk_array_len((ARRAY)) - 1))

/// Reserve space at the end of the array and return pointer to that slot.
#define bk_array_alloc(ARRAY) \
	((ARRAY) = bk_array__prepare_push((ARRAY)), \
	 &(ARRAY)[bk_array_len((ARRAY)) - 1])

/// Destroy an array.
BK_DECL void
bk_array_destroy(void* array);

/// Remove all items from an array.
BK_DECL void
bk_array_clear(void* array);

/// Get the length of an array.
BK_DECL size_t
bk_array_len(const void* array);

/// Delete an element from the array while keeping other elements in order
BK_DECL void
bk_array_remove(void* array, size_t index);

// private
BK_DECL void*
bk_array__create(
	struct bk_allocator_s* allocator,
	size_t elem_size,
	uint8_t alignment,
	size_t capacity
);

BK_DECL void*
bk_array__prepare_push(void* array);

BK_DECL void*
bk_array__resize(void* array, size_t new_length);

#ifdef BK_IMPLEMENTATION
#include "../../src/array.c"
#endif

#endif
