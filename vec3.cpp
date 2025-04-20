#include "vec3.h"

vec3 operator * (float k, vec3 const& p) {
    return vec3(k * p.x, k * p.y, k * p.z);
}
