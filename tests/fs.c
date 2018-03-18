#include "munit/munit.h"
#include <string.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <bk/default_allocator.h>
#include <bk/crt_fs.h>
#include <bk/fs_utils.h>

static MunitResult
crt_basic(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* fs = bk_crt_fs_create(bk_default_allocator);

	const char* test = "Hello";
	munit_assert_int(0, ==, bk_write_file(fs, "bin/test", test, strlen(test)));

	void* buf;
	size_t len;
	munit_assert_int(0, ==, bk_read_file(fs, "bin/test", bk_default_allocator, &buf, &len));

	munit_assert_size(strlen(test), ==, len);
	munit_assert_memory_equal(len, test, buf);

	bk_free(bk_default_allocator, buf);

	bk_crt_fs_destroy(fs);

	return MUNIT_OK;
}

static MunitTest crt_tests[] = {
	{ .name = "/basic", .test = crt_basic },
	{ 0 }
};

MunitSuite fs()
{
	static MunitSuite suites[] = {
		{ .prefix = "/crt", .tests = crt_tests },
		{ 0 }
	};

	return (MunitSuite) {
		.prefix = "/fs",
		.suites = suites
	};
}
