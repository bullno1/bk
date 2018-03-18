#ifndef BK_ASSERT_H
#define BK_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define BK_ASSERT(COND, MSG)
#else
#define BK_ASSERT(COND, MSG) BK_CRITICAL_ASSERT(COND, MSG)
#endif

#define BK_CRITICAL_ASSERT(COND, MSG) \
	do { \
		if(!(COND)) { \
			fprintf(stderr, "Assertion failed: %s\nMessage: %s\nFile: %s\nLine: %d\nFunction: %s\n", #COND, MSG, __FILE__, __LINE__, __func__); \
			abort(); \
		} \
	} while(0)

#endif
