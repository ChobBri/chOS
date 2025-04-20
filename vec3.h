#pragma once
#include "math.h"

struct vec3 {
    float x;
    float y;
    float z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    vec3( float _x, float _y, float _z )
        : x(_x), y(_y), z(_z) {}
  
    bool operator == (const vec3& p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator != (const vec3& p) const {
        return !(*this == p);
    }

    vec3 operator + (const vec3& p) const {
        return vec3(x + p.x, y + p.y, z + p.z);
    }

    vec3 operator - (const vec3& p) const {
        return vec3(x - p.x, y - p.y, z - p.z);
    }

    vec3 operator - () const {
        return vec3(-x, -y, -z);
    }

    /* dot product */
    float operator * (const vec3& p) const {
        return x * p.x + y * p.y + z * p.z;
    }

    /* cross product */
    vec3 operator ^ (const vec3& p) const {
        return vec3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }

    /* component-wise product */
    vec3 operator % (const vec3& p) const {
        return vec3(x * p.x,  y * p.y, z * p.z);
    }

    vec3 normalize() const {
        float invLen = invSqrt(x * x + y * y + z * z);
        return vec3(invLen * x, invLen * y, invLen * z);
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float squaredLength() const {
        return x * x + y * y + z * z;
    }
};

vec3 operator * (float k, vec3 const& p);
