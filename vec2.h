#pragma once
#include "math.h"

struct vec2 {
    float x;
    float y;

    vec2() : x(0.0f), y(0.0f) {}

    vec2( float _x, float _y )
        : x(_x), y(_y) {}
  
    bool operator == (const vec2& p) const {
        return x == p.x && y == p.y;
    }

    bool operator != (const vec2& p) const {
        return !(*this == p);
    }

    vec2 operator + (const vec2& p) const {
        return vec2(x + p.x, y + p.y);
    }

    vec2 operator - (const vec2& p) const {
        return vec2(x - p.x, y - p.y);
    }

    /* dot product */
    float operator * (const vec2& p) const {
        return x * p.x + y * p.y;
    }

    /* component-wise product */
    vec2 operator % (const vec2& p) const {
        return vec2(x * p.x,  y * p.y);
    }

    vec2 normalize() const {
        float invLen = invSqrt(x * x + y * y);
        return vec2(invLen * x, invLen * y);
    }

    float length() const {
        return sqrt(x * x + y * y);
    }

    float squaredLength() const {
        return x * x + y * y;
    }
};

vec2 operator * (float k, vec2 const& p);
