#include "vec4.h"

vec4 operator * (float k, const vec4& p) {
    return vec4(k * p.x, k * p.y, k * p.z, k * p.w);
}
