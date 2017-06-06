/* See LICENSE file for copyright and license details. */
#include <math.h>

static inline double
nnpow(double a, double b)
{
	int neg = a < 0;
	a = pow(neg ? -a : a, b);
	return neg ? -a : a;
}

static inline float
nnpowf(float a, float b)
{
	int neg = a < 0;
	a = powf(neg ? -a : a, b);
	return neg ? -a : a;
}

#define GENERIC(TYPE, FUNC, ...)\
	TYPE:           FUNC(__VA_ARGS__),\
	TYPE *:         FUNC(__VA_ARGS__),\
	TYPE **:        FUNC(__VA_ARGS__),\
	TYPE ***:       FUNC(__VA_ARGS__),\
	const TYPE:     FUNC(__VA_ARGS__),\
	const TYPE *:   FUNC(__VA_ARGS__),\
	const TYPE **:  FUNC(__VA_ARGS__),\
	const TYPE ***: FUNC(__VA_ARGS__)

#define MATH_GENERIC_1(FUNC, A)       (_Generic((A),\
						GENERIC(double, FUNC,    A),\
						GENERIC(float,  FUNC##f, A)))

#define MATH_GENERIC_N(FUNC, A, ...)  (_Generic((A),\
						GENERIC(double, FUNC,    A, __VA_ARGS__),\
						GENERIC(float,  FUNC##f, A, __VA_ARGS__)))

#define BLIND_GENERIC_1(FUNC, A)      (_Generic((A),\
						GENERIC(double, FUNC##_d, A),\
						GENERIC(float,  FUNC##_f, A)))

#define BLIND_GENERIC_N(FUNC, A, ...) (_Generic((A),\
						GENERIC(double, FUNC##_d, A, __VA_ARGS__), \
						GENERIC(float,  FUNC##_f, A, __VA_ARGS__)))

#define pow(...)         MATH_GENERIC_N(pow,      __VA_ARGS__)
#define log2(...)        MATH_GENERIC_1(log2,     __VA_ARGS__)
#define abs(...)         MATH_GENERIC_1(fabs,     __VA_ARGS__)
#define sqrt(...)        MATH_GENERIC_1(sqrt,     __VA_ARGS__)
#define exp(...)         MATH_GENERIC_1(exp,      __VA_ARGS__)
#define g_isnan(...)     MATH_GENERIC_1(isnan,    __VA_ARGS__)
#define g_isinf(...)     MATH_GENERIC_1(isinf,    __VA_ARGS__)
#define g_isfinite(...)  MATH_GENERIC_1(isfinite, __VA_ARGS__)
#define nnpow(...)       MATH_GENERIC_N(nnpow,    __VA_ARGS__)

#define srgb_encode(...) BLIND_GENERIC_1(srgb_encode, __VA_ARGS__)
#define srgb_decode(...) BLIND_GENERIC_1(srgb_decode, __VA_ARGS__)

#define yuv_to_srgb(a, b, c, d, e, f)\
	BLIND_GENERIC_N(yuv_to_srgb, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))
#define srgb_to_yuv(a, b, c, d, e, f)\
	BLIND_GENERIC_N(srgb_to_yuv, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))
#define ciexyz_to_srgb(a, b, c, d, e, f)\
	BLIND_GENERIC_N(ciexyz_to_srgb, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))
#define srgb_to_ciexyz(a, b, c, d, e, f)\
	BLIND_GENERIC_N(srgb_to_ciexyz, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))
#define scaled_yuv_to_ciexyz(a, b, c, d, e, f)\
	BLIND_GENERIC_N(scaled_yuv_to_ciexyz, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))
#define ciexyz_to_scaled_yuv(a, b, c, d, e, f)\
	BLIND_GENERIC_N(ciexyz_to_scaled_yuv, (a), (b), (c), (void *)(d), (void *)(e), (void *)(f))

#define htole(A) (_Generic((A),\
			   uint8_t: (A),\
			    int8_t: (uint8_t)(A),\
			   uint16_t: htole16(A),\
			    int16_t: (uint16_t)htole16((uint16_t)(A)),\
			   uint32_t: htole32(A),\
			    int32_t: (uint32_t)htole32((uint32_t)(A)),\
			   uint64_t: htole64(A),\
			    int64_t: (uint64_t)htole64((uint64_t)(A))))

#define letoh(A) (_Generic((A),\
			   uint8_t: (A),\
			    int8_t: (uint8_t)(A),\
			   uint16_t: le16toh(A),\
			    int16_t: (uint16_t)le16toh((uint16_t)(A)),\
			   uint32_t: le32toh(A),\
			    int32_t: (uint32_t)le32toh((uint32_t)(A)),\
			   uint64_t: le64toh(A),\
			    int64_t: (uint64_t)le64toh((uint64_t)(A))))