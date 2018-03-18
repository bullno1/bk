#include <bk/fs_utils.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <stdio.h>

int
bk_read_file(
	struct bk_fs_s* fs, const char* path,
	struct bk_allocator_s* allocator, void** bufp, size_t* sizep
)
{
	bk_file_t* file = NULL;
	int err;
	if((err = bk_fopen(fs, path, "rb", &file)) != 0) { goto end; }

	if((err = bk_fseek(file, 0, SEEK_END)) != 0) { goto end; }

	size_t file_size = 0;
	if((err = bk_ftell(file, &file_size)) != 0) { goto end; }

	if((err = bk_fseek(file, 0, SEEK_SET)) != 0) { goto end; }

	void* buf = bk_malloc(allocator, file_size);

	if((err = bk_fread(file, buf, &file_size)) != 0)
	{
		bk_free(allocator, buf);
		goto end;
	}

	*bufp = buf;
	*sizep = file_size;

end:
	if(file != NULL) { bk_fclose(file); }
	return err;
}

int
bk_write_file(struct bk_fs_s* fs, const char* path, const void* buf, size_t size)
{
	bk_file_t* file = NULL;
	int err;
	if((err = bk_fopen(fs, path, "wb", &file)) != 0) { goto end; }

	if((err = bk_fwrite(file, buf, &size)) != 0) { goto end; }

end:
	if(file != NULL) { bk_fclose(file); }
	return err;
}
