typedef int bool32;
typedef unsigned char u8;
typedef char i8;
typedef unsigned short u16;
typedef short i16;
typedef unsigned int u32;
typedef int i32;
typedef unsigned __int64 u64;
typedef __int64 i64;

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define pi32 3.14159265358979323846f
#define two_pi32 (2.f*pi32)
#define inv_sqrt_2 0.70710678118f

static inline void fatal_error() {
	abort();
}

#define ASSERT(expr) do { if(!((expr))) fatal_error(); } while (0)
