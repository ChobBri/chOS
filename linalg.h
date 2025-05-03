#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat2.h"
#include "mat4.h"
#include "math.h"

mat4 translate(float x, float y, float z) {
    mat4 T = mat4::identity();
    T[0][3] = x;
    T[1][3] = y;
    T[2][3] = z;
    return T;
}

mat4 translate(const vec3& v) {
    mat4 T = mat4::identity();
    T[0][3] = v.x;
    T[1][3] = v.y;
    T[2][3] = v.z;
    return T;
}

/* Credit: https://en.wikipedia.org/wiki/Rotation_matrix */
mat4 rotate(float angle, const vec3& axis) {
    float c = cos(angle);
    float s = sin(angle);
    float C = 1 - c;

    vec3 axisnorm = axis.normalize();
    float x = axisnorm.x;
    float y = axisnorm.y;
    float z = axisnorm.z;

    mat4 R;
    R[0][0] = x * x * C + c;
    R[0][1] = x * y * C - z * s;
    R[0][2] = x * z * C + y * s;

    R[1][0] = x * y * C + z * s;
    R[1][1] = y * y * C + c;
    R[1][2] = y * z * C - x * s;

    R[2][0] = x * z * C - y * s;
    R[2][1] = y * z * C + x * s;
    R[2][2] = z * z * C + c;

    R[3][3] = 1;
    return R;
}

mat4 scale(float x, float y, float z) {
    mat4 S;
    S[0][0] = x;
    S[1][1] = y;
    S[2][2] = z;
    S[3][3] = 1;
    return S;
}

mat4 viewing(const vec3& eye, const vec3& point, const vec3& up) {
    mat4 M;

    vec3 z = -(point - eye).normalize();
    vec3 x = (up ^ z).normalize();
    vec3 y = (z ^ x).normalize();

    M.rows[0] = vec4(x, 0);
    M.rows[1] = vec4(y, 0);
    M.rows[2] = vec4(z, 0);
    M.rows[3] = vec4(0, 0, 0, 1);

    mat4 V = M * translate(-eye);

    return V;
}

/* left, right, bottom, top, near, far */
mat4 projection(float l, float r, float b, float t, float n, float f) {
    mat4 P;
    float E = (2 * n) / (r - l);
    float A = (r + l) / (r - l);
    float F = (2 * n) / (t - b);
    float B = (t + b) / (t - b);
    float C = (n + f) / (n - f);
    float D = (2 * f * n) / (n - f);
    P[0][0] = E;
    P[0][2] = A;
    P[1][1] = F;
    P[1][2] = B;
    P[2][2] = C;
    P[2][3] = D;
    P[3][2] = -1;
    return P;
}

mat4 perspective( float fovy, float aspect, float n, float f )
{
  mat4 out;

  float s = 1 / atan2( fovy, 2.0 );

  out.rows[0] = vec4( s/aspect,          0,           0,           0 );
  out.rows[1] = vec4(         0,         s,           0,           0 );
  out.rows[2] = vec4(         0,         0, (f+n)/(n-f), 2*f*n/(n-f) );
  out.rows[3] = vec4(         0,         0,          -1,           0 );

  return out;
}

vec2 proj(const vec2& base, const vec2& v) {
    vec2 normalizedBase = base.normalize();
    return (v * normalizedBase) * normalizedBase;
}

vec3 proj(const vec3& base, const vec3& v) {
    vec3 normalizedBase = base.normalize();
    return (v * normalizedBase) * normalizedBase;
}

bool pointInTriangle(const vec2& point, const vec2& v0, const vec2& v1, const vec2& v2) {
    const vec2& a = v1 - v0;
    const vec2& b = v2 - v0;

    const vec2& relativePoint = point - v0;

    vec2 vars = mat2::colSpace(a, b).inverse() * relativePoint;
    float alpha = vars.x;
    float beta = vars.y;

    bool inTriangle = alpha >= 0 && beta >= 0 && alpha + beta <= 1;
    return inTriangle;
}
