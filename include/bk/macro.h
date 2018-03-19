#ifndef BK_MACRO_H
#define BK_MACRO_H

#ifdef __cplusplus
#	define BK_ALIGN_OF(TYPE) alignof(TYPE)
#else
#	define BK_ALIGN_OF(TYPE) offsetof(struct { char c; TYPE t;}, t)
#endif

#ifdef __cplusplus
#	define BK_FLEXIBLE_ARRAY_MEMBER(TYPE, NAME) TYPE NAME[1]
#else
#	define BK_FLEXIBLE_ARRAY_MEMBER(TYPE, NAME) TYPE NAME[]
#endif

#define BK_MIN(A, B) ((A) < (B) ? (A) : (B))
#define BK_MAX(A, B) ((A) > (B) ? (A) : (B))

#if defined(__GNUC__) || defined(__clang__)
#	define BK_MAYBE_UNUSED __attribute__((unused))
#	define BK_PRINTF_LIKE(x, y) __attribute__((format(printf, x, y)))
#else
#	define BK_MAYBE_UNUSED
#	define BK_PRINTF_LIKE(x, y)
#endif

#define BK_INLINE BK_MAYBE_UNUSED static inline

#define BK_CONTAINER_OF(PTR, TYPE, MEMBER) \
	(TYPE*)((char*)PTR - offsetof(TYPE, MEMBER))

#define BK_ENUM(NAME, X) \
	BK_DEFINE_ENUM(NAME, X) \
	BK_DEFINE_ENUM_TO_STR(NAME, X)

#define BK_DEFINE_ENUM(NAME, X) \
	typedef enum { X(BK_GEN_ENUM) } NAME;
#define BK_GEN_ENUM(ENUM) ENUM,

#define BK_DEFINE_ENUM_TO_STR(NAME, X) \
	BK_INLINE const char* NAME##_to_str(NAME value) { \
		switch(value) { \
			X(BK_DEFINE_ENUM_CASE) \
			default: return 0; \
		} \
	}
#define BK_DEFINE_ENUM_CASE(ENUM) case ENUM : return #ENUM ;

#define BK_STATIC_ARRAY_LEN(ARRAY) (sizeof((ARRAY)) / sizeof((ARRAY)[0]))

#define BK_ALIGN_PTR(PTR, ALIGNMENT) \
	(void*)(((uintptr_t)(PTR) + (ALIGNMENT) - 1) / (ALIGNMENT) * (ALIGNMENT))

/// Make it clear that it's an array and not a pointer to a single element
#define BK_ARRAY(T) T*

#ifdef __cplusplus
#	define BK_EXTERN extern "C"
#else
#	define BK_EXTERN extern
#endif

#ifdef _WIN32
#	ifdef __GNUC__
#		define BK_DYNAMIC_EXPORT __attribute__((dllexport))
#		define BK_DYNAMIC_IMPORT __attribute__((dllimport))
#	else
#		define BK_DYNAMIC_EXPORT __declspec(dllexport)
#		define BK_DYNAMIC_IMPORT __declspec(dllimport)
#	endif
#else
#	define BK_DYNAMIC_EXPORT __attribute__((visibility("default")))
#	define BK_DYNAMIC_IMPORT
#endif

#endif
