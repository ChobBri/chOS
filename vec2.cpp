#include "vec2.h"

vec2 operator * (float k, vec2 const& p) {
    return vec2(k * p.x, k * p.y);
}
