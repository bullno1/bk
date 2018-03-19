#include <bk/stdstream.h>
#include <bk/fs.h>
#include <stdio.h>
#include <errno.h>

static int
bk_stdstream_open(
	bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file
);

static void
bk_stdstream_close(bk_fs_t* fs, bk_file_t* file);

static int
bk_stdstream_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len);

static int
bk_stdstream_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len);

static int
bk_stdstream_seek(bk_fs_t* fs, bk_file_t* file, size_t offset, int origin);

static int
bk_stdstream_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos);

static bk_fs_t bk_stdstream = {
	.open = bk_stdstream_open,
	.close = bk_stdstream_close,
	.read = bk_stdstream_read,
	.write = bk_stdstream_write,
	.seek = bk_stdstream_seek,
	.tell = bk_stdstream_tell
};

static bk_file_t bk_stdin_instance = { .fs = &bk_stdstream };
static bk_file_t bk_stdout_instance = { .fs = &bk_stdstream };
static bk_file_t bk_stderr_instance = { .fs = &bk_stdstream };

bk_file_t* bk_stdin = &bk_stdin_instance;
bk_file_t* bk_stdout = &bk_stdout_instance;
bk_file_t* bk_stderr = &bk_stderr_instance;

static int
bk_stdstream_open(
	bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file
)
{
	(void)fs;
	(void)path;
	(void)mode;
	(void)file;
	return EOPNOTSUPP;
}

static void
bk_stdstream_close(bk_fs_t* fs, bk_file_t* file)
{
	(void)fs;
	if(file == bk_stdin)
	{
		fclose(stdin);
	}
	else if(file == bk_stdout)
	{
		fclose(stdout);
	}
	else if(file == bk_stderr)
	{
		fclose(stderr);
	}
}

static int
bk_stdstream_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len)
{
	(void)fs;

	if(file == bk_stdin)
	{
		*len = fread(buf, 1, *len, stdin);
		return ferror(stdin) ? EIO : 0;
	}
	else
	{
		return EOPNOTSUPP;
	}
}

static int
bk_stdstream_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len)
{
	(void)fs;

	FILE* stdfile = NULL;

	if(file == bk_stdout)
	{
		stdfile = stdout;
	}
	else if(file == bk_stderr)
	{
		stdfile = stderr;
	}
	else
	{
		return EOPNOTSUPP;
	}

	*len = fwrite(buf, 1, *len, stdfile);
	return ferror(stdfile) ? EIO : 0;
}

static int
bk_stdstream_seek(bk_fs_t* fs, bk_file_t* file, size_t offset, int origin)
{
	(void)fs;
	(void)file;
	(void)offset;
	(void)origin;

	return EOPNOTSUPP;
}

static int
bk_stdstream_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos)
{
	(void)fs;
	(void)file;
	(void)pos;

	return EOPNOTSUPP;
}
