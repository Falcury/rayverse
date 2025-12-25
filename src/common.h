#ifndef __cplusplus
// If we don't have access to C99, stdbool.h is not available and we need to supply 'bool' and 'inline'
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else 
typedef unsigned char bool;
#define true 1
#define false 0
#ifdef _MSC_VER
#define inline __inline
#else
#define inline __inline__
#endif
#endif
#endif

typedef int bool32;
typedef unsigned char u8;
typedef char i8;
typedef char s8;
typedef unsigned short u16;
typedef short i16;
typedef short s16;
typedef unsigned int u32;
typedef int i32;
typedef int s32;
#ifdef _WIN32
typedef unsigned __int64 u64;
typedef __int64 i64;
typedef __int64 s64;
#else
typedef unsigned long long u64;
typedef long long i64;
typedef long long s64;
#endif

#define U8_MAX 0xFF

#define COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define LEN COUNT
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#define MAX_2(x, y) if (x < y) x = y;
#define MAX_3(x, y) if (y > x) x = y;
#define MIN_2(x, y) if (x > y) x = y;
#define MIN_3(x, y) if (y < x) x = y;
#define ABS(x) (x >= 0 ? (x) : -(x))
#define SGN(x) (x >= 0 ? (x > 0 ? 1 : 0) : -1)

#define LERP(a, b, f, m) ((a) * ((m) - (f)) + ((b) * (f)))

#define pi32 3.14159265358979323846f
#define two_pi32 (2.f*pi32)
#define inv_sqrt_2 0.70710678118f

#ifdef _WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

static inline void fatal_error(void) {
	abort();
}

#define print_once(msg) do { static bool done; if (!done) { puts(msg); done = true; }} while (0)

#ifdef NOTIFY_UNIMPLEMENTED_DOS_FUNCS
#define print_once_dos(msg) print_once(msg)
#else
#define print_once_dos(msg)
#endif

#define ASSERT(expr) do { if(!((expr))) fatal_error(); } while (0)
#define BREAKPOINT do { int _ = 5; } while (0)
