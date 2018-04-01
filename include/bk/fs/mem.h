#ifndef BK_MEM_FS_H
#define BK_MEM_FS_H

#include "../fs.h"
#include "../decl.h"

typedef struct bk_mem_file_s bk_mem_file_t;

struct bk_mem_file_s
{
	bk_file_t bk_file;
	BK_ARRAY(char)* bufferp;
	bk_off_t pos;
};

BK_DECL bk_file_t*
bk_mem_fs_wrap(bk_mem_file_t* mem_file, BK_ARRAY(char)* buffer);

#endif
