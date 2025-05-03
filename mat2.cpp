#include "mat2.h"

mat2 mat2::inverse() const {
    mat2 inv;
    inv[0][0] = rows[1][1];
    inv[1][1] = rows[0][0];
    inv[0][1] = -rows[0][1];
    inv[1][0] = -rows[1][0];
    float det = determinant();
    return (1 / det) * inv;
}

mat2 operator * (float k, const mat2& m) {
    mat2 n;
    n.rows[0] = k * m.rows[0];
    n.rows[1] = k * m.rows[1];
    return n;
}

vec2 operator * (const mat2& m, const vec2& v) {
    vec2 u;

    u[0] = m.rows[0] * v;
    u[1] = m.rows[1] * v;

    return u;
}

mat2 operator * (mat2 const& m, mat2 const& n) {
    mat2 p;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            float sum=0;
            for (int k = 0; k < 2; k++) {
                sum += m[i][k] * n[k][j];
            }
            p[i][j] = sum;
        }
    }
  
    return p;
}
