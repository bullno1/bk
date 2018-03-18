#ifndef BK_FS_H
#define BK_FS_H

#include <stddef.h>
#include "macro.h"

typedef struct bk_fs_s bk_fs_t;
typedef struct bk_file_s bk_file_t;

struct bk_file_s
{
	bk_fs_t* fs;
};

struct bk_fs_s
{
	int(*open)(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file);
	void(*close)(bk_fs_t* fs, bk_file_t* file);
	int(*read)(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len);
	int(*write)(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len);
	int(*tell)(bk_fs_t* fs, bk_file_t* file, size_t* pos);
	int(*seek)(bk_fs_t* fs, bk_file_t* file, size_t pos, int origin);
	const char*(*strerror)(bk_fs_t* fs, int code);
};

BK_INLINE int
bk_fopen(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	return fs->open(fs, path, mode, file);
}

BK_INLINE void
bk_fclose(bk_file_t* file)
{
	file->fs->close(file->fs, file);
}

BK_INLINE int
bk_fread(bk_file_t* file, void* buf, size_t* len)
{
	return file->fs->read(file->fs, file, buf, len);
}

BK_INLINE int
bk_fwrite(bk_file_t* file, const void* buf, size_t* len)
{
	return file->fs->write(file->fs, file, buf, len);
}

BK_INLINE int
bk_ftell(bk_file_t* file, size_t* pos)
{
	return file->fs->tell(file->fs, file, pos);
}

BK_INLINE int
bk_fseek(bk_file_t* file, size_t pos, int origin)
{
	return file->fs->seek(file->fs, file, pos, origin);
}

BK_INLINE const char*
bk_fstrerror(bk_fs_t* fs, int code)
{
	return fs->strerror(fs, code);
}

#endif
