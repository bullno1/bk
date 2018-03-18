#include <bk/fs/prefix.h>
#include <string.h>
#include <errno.h>
#include <bk/fs.h>
#include <bk/allocator.h>
#include <bk/macro.h>

typedef struct
{
	bk_fs_t bk_fs;
	bk_allocator_t* allocator;
	bk_fs_t* backing_fs;
	const char* prefix;
	size_t prefix_len;
} bk_prefix_fs_t;

static int
bk_prefix_fs_open(
	bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file
)
{
	bk_prefix_fs_t* self = BK_CONTAINER_OF(fs, bk_prefix_fs_t, bk_fs);

	size_t path_len = strlen(path);
	char* prefixed_path = bk_unsafe_malloc(self->allocator, path_len + self->prefix_len + 1);
	if(prefixed_path == NULL) { return ENOMEM; }

	memcpy(prefixed_path, self->prefix, self->prefix_len);
	memcpy(prefixed_path + self->prefix_len, path, path_len);
	prefixed_path[path_len + self->prefix_len] = '\0';

	int err = bk_fopen(self->backing_fs, prefixed_path, mode, file);

	bk_free(self->allocator, prefixed_path);
	return err;
}

struct bk_fs_s*
bk_prefix_fs_create(
	struct bk_allocator_s* allocator,
	struct bk_fs_s* backing_fs,
	const char* prefix
)
{
	bk_prefix_fs_t* fs = BK_NEW(allocator, bk_prefix_fs_t);
	*fs = (bk_prefix_fs_t){
		.bk_fs = {
			.open = &bk_prefix_fs_open,
		},
		.allocator = allocator,
		.backing_fs = backing_fs,
		.prefix = prefix,
		.prefix_len = strlen(prefix)
	};

	return &fs->bk_fs;
}

void
bk_prefix_fs_destroy(struct bk_fs_s* vfs)
{
	bk_prefix_fs_t* self = BK_CONTAINER_OF(vfs, bk_prefix_fs_t, bk_fs);
	bk_free(self->allocator, self);
}
