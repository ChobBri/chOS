#pragma once 
#include "vec2.h"

struct mat2 {

    vec2 rows[2];

    mat2() {}

    vec2& operator[](int index) const {
        return ((vec2*)(&rows[0]))[index];
    }

    float determinant() const {
        return rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0];
    }

    mat2 inverse() const;

    static mat2 identity() {
        mat2 I;
        I[0][0] = 1;
        I[1][1] = 1;
        return I;
    }

    static mat2 colSpace(const vec2& col1, const vec2& col2) {
        mat2 M;
        M[0][0] = col1[0];
        M[0][1] = col2[0];
        M[1][0] = col1[1];
        M[1][1] = col2[1];
        return M;
    }

    static mat2 rowSpace(const vec2& row1, const vec2& row2) {
        mat2 M;
        M.rows[0] = row1;
        M.rows[1] = row2;
        return M;
    }
};

mat2 operator * (float k, const mat2& m);
vec2 operator * (const mat2& m, const vec2& v);
mat2 operator * (const mat2& m, const mat2& n);
