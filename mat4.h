#pragma once 
#include "vec4.h"

struct mat4 {

    vec4 rows[4];

    mat4() {}

    vec4& operator[](int index) const {
        return ((vec4*)(&rows[0]))[index];
    }

    static mat4 identity() {
        mat4 I;
        I[0][0] = 1;
        I[1][1] = 1;
        I[2][2] = 1;
        I[3][3] = 1;
        return I;
    }
};

mat4 operator * (float k, const mat4& m);
vec4 operator * (const mat4& m, const vec4& v);
mat4 operator * (const mat4& m, const mat4& n);
