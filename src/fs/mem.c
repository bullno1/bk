#include <bk/fs/mem.h>
#include <bk/array.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static inline int
bk_mem_fs_is_flexible(bk_mem_file_t* file)
{
	return file->size == -1;
}

static size_t
bk_mem_fs_file_size(bk_mem_file_t* file)
{
	return bk_mem_fs_is_flexible(file) ? bk_array_len(*file->buf.flexible) : (size_t)file->size;
}

static char*
bk_mem_fs_buf(bk_mem_file_t* file)
{
	return bk_mem_fs_is_flexible(file) ? *file->buf.flexible : file->buf.fixed;
}

static int
bk_mem_fs_read(bk_fs_t* fs, bk_file_t* file, void* buf, size_t* len)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	size_t file_size = bk_mem_fs_file_size(mem_file);

	if((size_t)mem_file->pos >= file_size)
	{
		*len = 0;
		return 0;
	}

	size_t bytes_left = file_size - mem_file->pos;
	size_t bytes_to_read = BK_MIN(bytes_left, *len);

	memcpy(buf, &(bk_mem_fs_buf(mem_file)[mem_file->pos]), bytes_to_read);
	mem_file->pos += bytes_to_read;
	*len = bytes_to_read;

	return 0;
}

static int
bk_mem_fs_write(bk_fs_t* fs, bk_file_t* file, const void* buf, size_t* len)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	size_t file_size = bk_mem_fs_file_size(mem_file);
	size_t write_size = *len;

	size_t required_size = mem_file->pos + write_size;
	if(required_size > file_size)
	{
		if(bk_mem_fs_is_flexible(mem_file))
		{
			*(mem_file->buf.flexible) = bk_array__prepare_push(*mem_file->buf.flexible);
			bk_array_resize(*(mem_file->buf.flexible), required_size);
		}
		else
		{
			return EOPNOTSUPP;
		}
	}

	memcpy(&(bk_mem_fs_buf(mem_file)[mem_file->pos]), buf, write_size);
	mem_file->pos += write_size;

	return 0;
}

static int
bk_mem_fs_tell(bk_fs_t* fs, bk_file_t* file, size_t* pos)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	*pos = (size_t)mem_file->pos;
	return 0;
}

static int
bk_mem_fs_seek(bk_fs_t* fs, bk_file_t* file, bk_off_t pos, int origin)
{
	(void)fs;
	bk_mem_file_t* mem_file = BK_CONTAINER_OF(file, bk_mem_file_t, bk_file);
	bk_off_t new_pos = 0;
	bk_off_t file_size = (bk_off_t)bk_mem_fs_file_size(mem_file);

	switch(origin)
	{
		case SEEK_SET:
			new_pos = pos;
			break;
		case SEEK_CUR:
			new_pos = mem_file->pos + pos;
			break;
		case SEEK_END:
			new_pos = file_size + pos;
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
bk_mem_fs_wrap_flexible(bk_mem_file_t* mem_file, char** bufferp)
{
	*mem_file = (bk_mem_file_t){
		.bk_file = { .fs = &bk_mem_fs },
		.buf = { .flexible = bufferp },
		.size = -1,
		.pos = 0
	};

	return &mem_file->bk_file;
}

bk_file_t*
bk_mem_fs_wrap_fixed(bk_mem_file_t* mem_file, char* buffer, size_t size)
{
	*mem_file = (bk_mem_file_t){
		.bk_file = { .fs = &bk_mem_fs },
		.buf = { .fixed = buffer },
		.size = (bk_off_t)size,
		.pos = 0
	};

	return &mem_file->bk_file;
}
