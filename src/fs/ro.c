#include <bk/fs/ro.h>
#include <bk/fs.h>
#include <bk/fs/utils.h>
#include <bk/allocator.h>
#include <errno.h>

typedef struct
{
	bk_fs_t bk_fs;
	bk_allocator_t* allocator;
	bk_fs_t* backing_fs;
} bk_ro_fs_t;

static int
bk_ro_fs_open(bk_fs_t* fs, const char* path, const char* mode, bk_file_t** file)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);

	if(!bk_is_mode_ro(mode)) { return EROFS; }

	return bk_fopen(self->backing_fs, path, mode, file);
}

struct bk_fs_s*
bk_ro_fs_create(struct bk_allocator_s* allocator, struct bk_fs_s* backing_fs)
{
	bk_ro_fs_t* fs = BK_NEW(allocator, bk_ro_fs_t);
	*fs = (bk_ro_fs_t){
		.bk_fs = { .open = &bk_ro_fs_open, },
		.allocator = allocator,
		.backing_fs = backing_fs
	};

	return &fs->bk_fs;
}

void
bk_ro_fs_destroy(struct bk_fs_s* fs)
{
	bk_ro_fs_t* self = BK_CONTAINER_OF(fs, bk_ro_fs_t, bk_fs);
	bk_free(self->allocator, self);
}
