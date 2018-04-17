#ifndef BK_PRINTF_H
#define BK_PRINTF_H

#include <stdarg.h>
#include "decl.h"

#if defined(__GNUC__) || defined(__clang__)
#	define BK_PRINTF_LIKE(x, y) __attribute__((format(printf, x, y)))
#else
#	define BK_PRINTF_LIKE(x, y)
#endif

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

#ifdef BK_IMPLEMENTATION
#include "../../src/printf.c"
#include "../../src/vendor/format/format.c"
#endif

#endif
