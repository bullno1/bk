#ifndef _WIN32
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <sys/types.h>
#endif

#include <bk/crt_fs.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <bk/fs.h>
#include <bk/allocator.h>

typedef struct
{
	bk_fs_t bk_fs;
	bk_allocator_t* allocator;
} bk_crt_fs_t;

typedef struct
{
	bk_file_t bk_file;
	FILE* crt_file;
} bk_crt_file_t;

static int
bk_crt_fs_open(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	bk_crt_fs_t* self = BK_CONTAINER_OF(fs, bk_crt_fs_t, bk_fs);

	bk_crt_file_t* bk_file = BK_NEW(self->allocator, bk_crt_file_t);

	bk_file->bk_file.fs = fs;

	bk_file->crt_file = fopen(path, mode);
	if(bk_file->crt_file == NULL)
	{
		bk_free(self->allocator, bk_file);
		return errno;
	}

	*file = &bk_file->bk_file;
	return 0;
}

static void
bk_crt_fs_close(bk_fs_t* fs, bk_file_t* file)
{
	bk_crt_fs_t* self = BK_CONTAINER_OF(fs, bk_crt_fs_t, bk_fs);
	bk_crt_file_t* bk_file = BK_CONTAINER_OF(file, bk_crt_file_t, bk_file);
	fclose(bk_file->crt_file);
	bk_free(self->allocator, bk_file);
}

static int
bk_crt_fs_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len)
{
	(void)fs;
	bk_crt_file_t* bk_file = BK_CONTAINER_OF(file, bk_crt_file_t, bk_file);

	*len = fread(buf, 1, *len, bk_file->crt_file);

	return ferror(bk_file->crt_file) ? EIO : 0;
}

static int
bk_crt_fs_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len)
{
	(void)fs;
	bk_crt_file_t* bk_file = BK_CONTAINER_OF(file, bk_crt_file_t, bk_file);

	*len = fwrite(buf, 1, *len, bk_file->crt_file);

	return ferror(bk_file->crt_file) ? EIO : 0;
}

static int
bk_crt_fs_seek(bk_fs_t* fs, bk_file_t* file, size_t offset, int origin)
{
	(void)fs;
	bk_crt_file_t* bk_file = BK_CONTAINER_OF(file, bk_crt_file_t, bk_file);

#ifdef _WIN32
	int result = _fseeki64(bk_file->crt_file, offset, origin);
#else
	int result = fseeko(bk_file->crt_file, offset, origin);
#endif

	return result == 0 ? 0 : errno;
}

static int
bk_crt_fs_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos)
{
	(void)fs;
	bk_crt_file_t* bk_file = BK_CONTAINER_OF(file, bk_crt_file_t, bk_file);

#ifdef _WIN32
	__int64 offset = _ftelli64(bk_file->crt_file);
#else
	off_t offset = (size_t)ftello(bk_file->crt_file);
#endif

	*pos = (size_t)offset;

	return offset == -1 ? errno : 0;
}

bk_fs_t*
bk_crt_fs_create(bk_allocator_t* allocator)
{
	bk_crt_fs_t* fs = BK_NEW(allocator, bk_crt_fs_t);
	*fs = (bk_crt_fs_t){
		.allocator = allocator,
		.bk_fs = {
			.open = &bk_crt_fs_open,
			.close = &bk_crt_fs_close,
			.read = &bk_crt_fs_read,
			.write = &bk_crt_fs_write,
			.tell = &bk_crt_fs_tell,
			.seek = &bk_crt_fs_seek,
		}
	};

	return &fs->bk_fs;
}

void
bk_crt_fs_destroy(bk_fs_t* fs)
{
	bk_crt_fs_t* self = BK_CONTAINER_OF(fs, bk_crt_fs_t, bk_fs);
	bk_free(self->allocator, self);
}
