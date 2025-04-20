#include "mat4.h"

mat4 operator * (float k, const mat4& m) {
    mat4 n;
    n.rows[0] = k * m.rows[0];
    n.rows[1] = k * m.rows[1];
    n.rows[2] = k * m.rows[2];
    n.rows[3] = k * m.rows[3];
    return n;
}

vec4 operator * (const mat4& m, const vec4& v) {
    vec4 u;

    u[0] = m.rows[0] * v;
    u[1] = m.rows[1] * v;
    u[2] = m.rows[2] * v;
    u[3] = m.rows[3] * v;

    return u;
}

mat4 operator * (mat4 const& m, mat4 const& n) {
    mat4 p;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float sum=0;
            for (int k = 0; k < 4; k++) {
                sum += m[i][k] * n[k][j];
            }
            p[i][j] = sum;
        }
    }
  
    return p;
}
