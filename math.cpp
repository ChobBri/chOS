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

float exp(float x) {
    float result = 1.0f;
    const int iterationNum = 10;  // increase for accuracy
    float xExp = 1;
    int factorial = 1;
    for (int i = 1; i < iterationNum; i++) {
        factorial *= i;
        xExp *= x;
        result += xExp / factorial;
    }
    return result;
}

float ln(float x) {
    const int iterationNum = 5;
    float result = 0.0f;
    for (int i = 1; i <= iterationNum; i++) {
        float e = exp(result);
        result = result + 2 * (x - e) / (x + e);
    }
    return result;
}

float pow(float b, float e) {
    return exp(ln(b) * e);
}

int floor(float x) {
    if (x >= 0.0f) {
        return (int) x;
    }

    int intx = (int) x;
    if (x == (float)intx) {
        return intx;
    }
    return intx - 1;
}

int ceil(float x) {
    if (x <= 0.0f) {
        return -(int) (-x);
    }

    int intx = (int) x;
    if (x == (float) intx) {
        return intx;
    }
    return intx + 1;
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

int clamp(int val, int lower, int upper) {
    if (val < lower) {
        return lower;
    }
    else if (val > upper) {
        return upper;
    }
    else {
        return val;
    }
}

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

float abs(float x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

float max(float a, float b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

float min(float a, float b) {
    if (a < b) {
        return a;
    }
    return b;
}


/* Bhaskara I approximation */
float sin(float x) {
    int offsetFactor = floor(x / TAU);
    x -= offsetFactor * TAU;
    if (x < PI) {
        float sinx = (16 * x * (PI - x)) / (5 * PI * PI - 4 * x * (PI - x));
        return sinx;
    }
    else {
        x -= PI;
        float sinx = -(16 * x * (PI - x)) / (5 * PI * PI - 4 * x * (PI - x));
        return sinx;
    }
}

float cos(float x) {
    return sin(x + PI / 2);
}

float atan2(float y, float x) {
    if (x == 0.0) {
        if (y >= 0.f) {
            return PI / 2;
        }
        else {
            return -PI / 2;
        }
    }

    float m = y / x;
    float absm = abs(m);

    float arctan = 0;

    if (absm < 1.f) {
        arctan = absm / (1 + 0.28f * absm * absm);
    }
    else {
        arctan = PI / 2 - absm / (absm * absm + 0.28);
    }

    if (x < 0.0f) {
        arctan += PI;
    }

    if (m < 0.0f) {
        return -arctan;
    }
    return arctan;
}
