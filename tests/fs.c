#include "munit/munit.h"
#include <string.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <bk/array.h>
#include <bk/default_allocator.h>
#include <bk/fs/crt.h>
#include <bk/fs/ro.h>
#include <bk/fs/vfs.h>
#include <bk/fs/prefix.h>
#include <bk/fs/mem.h>
#include <bk/fs/utils.h>
#include <errno.h>

static MunitResult
crt(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* fs = bk_crt_fs_create(bk_default_allocator);

	const char* test = __func__;
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
ro(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* backing_fs = bk_crt_fs_create(bk_default_allocator);
	bk_fs_t* fs = bk_ro_fs_create(bk_default_allocator, backing_fs);

	const char* test = __func__;
	munit_assert_int(EROFS, ==, bk_write_file(fs, "bin/test", test, strlen(test)));

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

static MunitResult
prefix(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* backing_fs = bk_crt_fs_create(bk_default_allocator);
	bk_fs_t* fs = bk_prefix_fs_create(bk_default_allocator, backing_fs, "bin");

	const char* test = __func__;
	munit_assert_int(0, ==, bk_write_file(fs, "/test", test, strlen(test)));

	void* buf;
	size_t len;
	munit_assert_int(0, ==, bk_read_file(fs, "/test", bk_default_allocator, &buf, &len));

	munit_assert_size(strlen(test), ==, len);
	munit_assert_memory_equal(len, test, buf);

	bk_free(bk_default_allocator, buf);

	bk_prefix_fs_destroy(fs);
	bk_crt_fs_destroy(backing_fs);

	return MUNIT_OK;
}

static MunitResult
vfs(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	bk_fs_t* backing_fs = bk_crt_fs_create(bk_default_allocator);
	bk_fs_t* fs = bk_vfs_create(bk_default_allocator);

	bk_vfs_mount_t mount = {
		.path = "/data",
		.subfs = backing_fs
	};
	bk_vfs_mount(fs, &mount);

	const char* test = __func__;
	munit_assert_int(ENOENT, ==, bk_write_file(fs, "bin/test", test, strlen(test)));
	munit_assert_int(0, ==, bk_write_file(fs, "/data/bin/test", test, strlen(test)));

	void* buf;
	size_t len;
	munit_assert_int(ENOENT, ==, bk_read_file(fs, "bin/test", bk_default_allocator, &buf, &len));
	munit_assert_int(0, ==, bk_read_file(fs, "/data/bin/test", bk_default_allocator, &buf, &len));

	munit_assert_size(strlen(test), ==, len);
	munit_assert_memory_equal(len, test, buf);

	bk_free(bk_default_allocator, buf);

	bk_vfs_umount(fs, &mount);

	munit_assert_int(ENOENT, ==, bk_read_file(fs, "/data/bin/test", bk_default_allocator, &buf, &len));

	bk_vfs_destroy(fs);
	bk_crt_fs_destroy(backing_fs);

	return MUNIT_OK;
}

static MunitResult
mem(const MunitParameter params[], void* fixture)
{
	(void)params;
	(void)fixture;

	BK_ARRAY(char) buf = bk_array_create(bk_default_allocator, char, 1);

	const char test[] = "bk_mem_fs_test";
	bk_mem_file_t mem_file;
	bk_file_t* file = bk_mem_fs_wrap_flexible(&mem_file, &buf);

	size_t size = sizeof(test);
	munit_assert_int(0, ==, bk_fwrite(file, test, &size));
	munit_assert_size(sizeof(test), ==, size);

	char read_buf[sizeof(test)];
	munit_assert_int(0, ==, bk_fseek(file, 0, SEEK_SET));
	munit_assert_int(0, ==, bk_fread(file, read_buf, &size));
	munit_assert_size(sizeof(test), ==, size);

	munit_assert_memory_equal(sizeof(test), test, read_buf);

	file = bk_mem_fs_wrap_fixed(&mem_file, read_buf, sizeof(test));

	char read_buf2[sizeof(test)];
	munit_assert_int(0, ==, bk_fread(file, read_buf2, &size));
	munit_assert_memory_equal(sizeof(test), read_buf, read_buf2);

	bk_array_destroy(buf);

	return MUNIT_OK;
}

static MunitTest tests[] = {
	{ .name = "/crt", .test = crt },
	{ .name = "/ro", .test = ro },
	{ .name = "/vfs", .test = vfs },
	{ .name = "/prefix", .test = prefix },
	{ .name = "/mem", .test = mem },
	{ 0 }
};

MunitSuite fs = {
	.prefix = "/fs",
	.tests = tests
};
