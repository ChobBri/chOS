#include "math.h"

float sqrt(float z)
{
	union { float f; uint32_t i; } val = {z};	/* Convert type, preserving bit pattern */
	/*
	 * To justify the following code, prove that
	 *
	 * ((((val.i / 2^m) - b) / 2) + b) * 2^m = ((val.i - 2^m) / 2) + ((b + 1) / 2) * 2^m)
	 *
	 * where
	 *
	 * b = exponent bias
	 * m = number of mantissa bits
	 */
	val.i -= 1 << 23;	/* Subtract 2^m. */
	val.i >>= 1;		/* Divide by 2. */
	val.i += 1 << 29;	/* Add ((b + 1) / 2) * 2^m. */

	return val.f;		/* Interpret again as float */
}

float invSqrt(float x) {
    float xhalf = 0.5f * x;
    union {
        float x;
        int i;
    } u;
    u.x = x;
    u.i = 0x5f375a86 - (u.i >> 1);
    /* The next line can be repeated any number of times to increase accuracy */
    u.x = u.x * (1.5f - xhalf * u.x * u.x);
    return u.x;
}

int round(float x) {
    if (x > 0.0f) {
        x += 0.5f;
        return (int) x;
    }
    else {
        x -= 0.5f;
        return (int) x;
    }
}

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}
