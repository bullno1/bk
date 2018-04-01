#include <bk/fs/mem.h>
#include <bk/array.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int
bk_mem_fs_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	size_t file_size = bk_array_len(*(mem_file->bufferp));

	if((size_t)mem_file->pos >= file_size)
	{
		*len = 0;
		return 0;
	}

	size_t bytes_left = file_size - mem_file->pos;
	size_t bytes_to_read = BK_MIN(bytes_left, *len);

	memcpy(buf, &(*(mem_file->bufferp))[mem_file->pos], bytes_to_read);
	mem_file->pos += bytes_to_read;
	*len = bytes_to_read;

	return 0;
}

int
bk_mem_fs_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	size_t file_size = bk_array_len(*(mem_file->bufferp));
	size_t write_size = *len;

	size_t required_size = mem_file->pos + write_size;
	if(required_size > file_size)
	{
		*(mem_file->bufferp) = bk_array__prepare_push(*(mem_file->bufferp));
		bk_array_resize(*(mem_file->bufferp), required_size);
	}

	memcpy(&(*(mem_file->bufferp))[mem_file->pos], buf, write_size);
	mem_file->pos += write_size;

	return 0;
}

int
bk_mem_fs_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	*pos = (size_t)mem_file->pos;
	return 0;
}

int
bk_mem_fs_seek(bk_fs_t* fs, bk_file_t* file, bk_off_t pos, int origin)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	bk_off_t new_pos = 0;

	switch(origin)
	{
		case SEEK_SET:
			new_pos = pos;
			break;
		case SEEK_CUR:
			new_pos = mem_file->pos + pos;
			break;
		case SEEK_END:
			new_pos = (bk_off_t)(bk_array_len(*(mem_file->bufferp))) + pos;
			break;
		default:
			return EINVAL;
	}

	if(new_pos < 0) { return EINVAL; }

	mem_file->pos = new_pos;

	return 0;
}

static bk_fs_t bk_mem_fs = {
	.read = bk_mem_fs_read,
	.write = bk_mem_fs_write,
	.tell = bk_mem_fs_tell,
	.seek = bk_mem_fs_seek,
};

bk_file_t*
bk_mem_fs_wrap(bk_mem_file_t* mem_file, BK_ARRAY(char)* bufferp)
{
	*mem_file = (bk_mem_file_t){
		.bk_file = { .fs = &bk_mem_fs },
		.bufferp = bufferp,
		.pos = 0
	};

	return &mem_file->bk_file;
}
