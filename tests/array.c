#include "munit/munit.h"
#include <bk/array.h>
#include <bk/default_allocator.h>
#include <emmintrin.h>
#include <stdbool.h>

static MunitResult
retention(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(int) array = bk_array_create(bk_default_allocator, int, 2);
	munit_assert_size(0, ==, bk_array_len(array));

	for(int i = 0; i < 10000; ++i)
	{
		munit_assert_size(i, ==, bk_array_len(array));
		bk_array_push(array, i);
	}

	munit_assert_size(10000, ==, bk_array_len(array));

	for(int i = 0; i < 10000; ++i)
	{
		munit_assert_int(array[i], ==, i);
	}

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitResult
alignment_sse(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(__m128i) array = bk_array_create(bk_default_allocator, __m128i, 1);
	munit_assert_true((uintptr_t)array % BK_ALIGN_OF(__m128i) == 0);

	for(int i = 0; i < 10000; ++i)
	{
		bk_array_push(array, _mm_set_epi32(i, i + 1, i + 2, i + 3));
		munit_assert_true((uintptr_t)array % BK_ALIGN_OF(__m128i) == 0);
	}

	for(int i = 0; i < 10000; ++i)
	{
		__m128i expected = _mm_set_epi32(i, i + 1, i + 2, i + 3);
		munit_assert_memory_equal(sizeof(expected), &expected, &array[i]);
	}

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitResult
alignment_long_double(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(long double) array = bk_array_create(bk_default_allocator, long double, 1);
	munit_assert_true((uintptr_t)array % BK_ALIGN_OF(long double) == 0);

	for(int i = 0; i < 10000; ++i)
	{
		bk_array_push(array, i);
		munit_assert_true((uintptr_t)array % BK_ALIGN_OF(long double) == 0);
	}

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitResult
remove(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(uint32_t) array = bk_array_create(bk_default_allocator, uint32_t, 0);

	bk_array_push(array, 0);
	bk_array_push(array, 1);
	bk_array_push(array, 2);
	bk_array_push(array, 3);
	munit_assert_size(4, ==, bk_array_len(array));

	bk_array_remove(array, 2);

	munit_assert_size(3, ==, bk_array_len(array));
	munit_assert_uint32(0, ==, array[0]);
	munit_assert_uint32(1, ==, array[1]);
	munit_assert_uint32(3, ==, array[2]);

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitResult
quick_remove(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	uint32_t num_elements = munit_rand_uint32() % 1000 + 1;
	uint32_t deleted_element = munit_rand_uint32() % num_elements;

	BK_ARRAY(uint32_t) array = bk_array_create(bk_default_allocator, uint32_t, 0);

	for(uint32_t i = 0; i < num_elements; ++i)
	{
		bk_array_push(array, i);
	}

	munit_assert_size(num_elements, ==, bk_array_len(array));
	bk_array_quick_remove(array, deleted_element);
	munit_assert_size(num_elements - 1, ==, bk_array_len(array));

	for(uint32_t i = 0; i < num_elements; ++i)
	{
		bool present = false;

		for(uint32_t j = 0; j < bk_array_len(array); ++j)
		{
			if(array[j] == i)
			{
				present = true;
				break;
			}
		}

		if(i == deleted_element)
		{
			munit_assert_false(present);
		}
		else
		{
			munit_assert_true(present);
		}
	}

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitResult
pop(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(int) array = bk_array_create(bk_default_allocator, int, 2);
	munit_assert_size(0, ==, bk_array_len(array));

	bk_array_push(array, 42);
	bk_array_push(array, 43);
	munit_assert_size(2, ==, bk_array_len(array));
	munit_assert_int(43, ==, bk_array_pop(array));
	munit_assert_int(42, ==, bk_array_pop(array));
	munit_assert_size(0, ==, bk_array_len(array));

	bk_array_destroy(array);

	return MUNIT_OK;
}

static MunitTest tests[] = {
	{ .name = "/retention", .test = retention },
	{ .name = "/alignment/sse", .test = alignment_sse },
	{ .name = "/alignment/long_double", .test = alignment_long_double },
	{ .name = "/remove", .test = remove },
	{ .name = "/quick_remove", .test = quick_remove },
	{ .name = "/pop", .test = pop },
	{ 0 }
};

MunitSuite array = {
	.prefix = "/array",
	.tests = tests
};
