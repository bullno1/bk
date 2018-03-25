#ifndef BK_PRINTF_H
#define BK_PRINTF_H

#include <stdarg.h>
#include "decl.h"
#include "macro.h"

struct bk_file_s;

BK_DECL int
bk_vprintf(struct bk_file_s* out, const char* fmt, va_list args);

BK_INLINE BK_PRINTF_LIKE(2, 3) int
bk_printf(struct bk_file_s* out, const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	int result = bk_vprintf(out, fmt, args);
	va_end(args);

	return result;
}

#endif
