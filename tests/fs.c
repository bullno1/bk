#include "munit/munit.h"
#include <string.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <bk/default_allocator.h>
#include <bk/fs/crt.h>
#include <bk/fs/ro.h>
#include <bk/fs/utils.h>

static MunitResult
crt_fs_test(const MunitParameter params[], void* fixture)
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

static MunitResult
ro_fs_test(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* backing_fs = bk_crt_fs_create(bk_default_allocator);
	bk_fs_t* fs = bk_ro_fs_create(bk_default_allocator, backing_fs);

	const char* test = "Hello";
	munit_assert_int(0, !=, bk_write_file(fs, "bin/test", test, strlen(test)));

	void* buf;
	size_t len;
	munit_assert_int(0, ==, bk_write_file(backing_fs, "bin/test", test, strlen(test)));
	munit_assert_int(0, ==, bk_read_file(fs, "bin/test", bk_default_allocator, &buf, &len));

	munit_assert_size(strlen(test), ==, len);
	munit_assert_memory_equal(len, test, buf);

	bk_free(bk_default_allocator, buf);

	bk_ro_fs_destroy(fs);
	bk_crt_fs_destroy(backing_fs);

	return MUNIT_OK;
}

MunitSuite fs()
{
	static MunitTest tests[] = {
		{ .name = "/crt_fs", .test = crt_fs_test },
		{ .name = "/ro_fs", .test = ro_fs_test },
		{ 0 }
	};

	return (MunitSuite) {
		.prefix = "/fs",
		.tests = tests
	};
}
