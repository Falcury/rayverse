#include <stdbool.h>

typedef int bool32;
typedef unsigned char u8;
typedef char i8;
typedef unsigned short u16;
typedef short i16;
typedef unsigned int u32;
typedef int i32;
#ifdef _WIN32
typedef unsigned __int64 u64;
typedef __int64 i64;
#else
typedef unsigned long long u64;
typedef long long i64;
#endif

#define COUNT(array) (sizeof(array) / sizeof((array)[0]))
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define pi32 3.14159265358979323846f
#define two_pi32 (2.f*pi32)
#define inv_sqrt_2 0.70710678118f

#ifdef _WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

static inline void fatal_error() {
	abort();
}

#define ASSERT(expr) do { if(!((expr))) fatal_error(); } while (0)
