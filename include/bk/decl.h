#ifndef BK_DECL

#include "macro.h"

#if BK_DYNAMIC == 1
#	if BK_BUILD == 1
#		define BK_DECL BK_EXTERN BK_DYNAMIC_EXPORT
#	else
#		define BK_DECL BK_EXTERN BK_DYNAMIC_IMPORT
#	endif
#else
#	define BK_DECL BK_EXTERN
#endif

#endif
