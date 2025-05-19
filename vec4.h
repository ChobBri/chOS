#pragma once
#include "math.h"
#include "vec3.h"

struct vec4 {
    float x;
    float y;
    float z;
    float w;

    vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    
    vec4( float _x, float _y, float _z, float _w )
    : x(_x), y(_y), z(_z), w(_w) {}

    vec4(const vec3& v, float _w)
    : x(v.x), y(v.y), z(v.z), w(_w) {}
  
    bool operator == (const vec4& p) const {
        return x == p.x && y == p.y && z == p.z && w == p.w;
    }

    bool operator != (const vec4& p) const {
        return !(*this == p);
    }

    vec4 operator + (const vec4& p) const {
        return vec4(x + p.x, y + p.y, z + p.z, w + p.w);
    }

    vec4 operator - (const vec4& p) const {
        return vec4(x - p.x, y - p.y, z - p.z, w - p.w);
    }

    /* dot product */
    float operator * (const vec4& p) const {
        return x * p.x + y * p.y + z * p.z + w * p.w;
    }

    /* component-wise product */
    vec4 operator % (const vec4& p) const {
        return vec4(x * p.x,  y * p.y, z * p.z, w * p.w);
    }

    vec4 normalize() const {
        float invLen = invSqrt(x * x + y * y + z * z + w * w);
        return vec4(invLen * x, invLen * y, invLen * z, invLen * w);
    }

    float length() const {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    float squaredLength() const {
        return x * x + y * y + z * z + w * w;
    }

    vec3 xyz() const {
        return vec3(x, y, z);
    }

    vec3 perspDiv() const {
        return vec3(x / w, y / w, z / w);
    }

    float& operator[](int index) {
        return (&x)[index];
    }

    float operator[](int index) const {
        return (&x)[index];
    }
};

vec4 operator * (float k, const vec4& p);
