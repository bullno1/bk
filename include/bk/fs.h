#ifndef BK_FS_H
#define BK_FS_H

#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include "macro.h"

typedef struct bk_fs_s bk_fs_t;
typedef struct bk_file_s bk_file_t;
typedef int64_t bk_off_t;

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
	int(*seek)(bk_fs_t* fs, bk_file_t* file, bk_off_t pos, int origin);
};

BK_INLINE int
bk_fopen(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	if(fs->open == NULL) { return EOPNOTSUPP; }

	return fs->open(fs, path, mode, file);
}

BK_INLINE void
bk_fclose(bk_file_t* file)
{
	if(file->fs->close == NULL) { return; }

	file->fs->close(file->fs, file);
}

BK_INLINE int
bk_fread(bk_file_t* file, void* buf, size_t* len)
{
	if(file->fs->read == NULL) { return EOPNOTSUPP; }

	return file->fs->read(file->fs, file, buf, len);
}

BK_INLINE int
bk_fwrite(bk_file_t* file, const void* buf, size_t* len)
{
	if(file->fs->write == NULL) { return EOPNOTSUPP; }

	return file->fs->write(file->fs, file, buf, len);
}

BK_INLINE int
bk_ftell(bk_file_t* file, size_t* pos)
{
	if(file->fs->tell == NULL) { return EOPNOTSUPP; }

	return file->fs->tell(file->fs, file, pos);
}

BK_INLINE int
bk_fseek(bk_file_t* file, bk_off_t pos, int origin)
{
	if(file->fs->seek == NULL) { return EOPNOTSUPP; }

	return file->fs->seek(file->fs, file, pos, origin);
}

#endif
