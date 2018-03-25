#include <bk/printf.h>
#include <errno.h>
#include <bk/fs.h>
#include "vendor/format/format.h"

static void*
bk_format_cons(void* out, const char* buf, size_t n)
{
	return bk_fwrite(out, buf, &n) == 0 ? out : NULL;
}

int
bk_vprintf(struct bk_file_s* out, const char* fmt, va_list args)
{
	int err = format(bk_format_cons, out, fmt, args);
	return err == EXBADFORMAT ? EINVAL : 0;
}
