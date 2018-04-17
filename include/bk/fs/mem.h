#ifndef BK_MEM_FS_H
#define BK_MEM_FS_H

#include "../fs.h"
#include "../decl.h"

typedef struct bk_mem_file_s bk_mem_file_t;

struct bk_mem_file_s
{
	bk_file_t bk_file;
	bk_off_t pos;
	bk_off_t size;
	union
	{
		char* fixed;
		char** flexible;
	} buf;
};

BK_DECL bk_file_t*
bk_mem_fs_wrap_flexible(bk_mem_file_t* mem_file, char** buffer);

BK_DECL bk_file_t*
bk_mem_fs_wrap_fixed(bk_mem_file_t* mem_file, char* buffer, size_t size);

#ifdef BK_IMPLEMENTATION
#include "../../../src/fs/mem.c"
#endif

#endif
