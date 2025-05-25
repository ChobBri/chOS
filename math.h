#pragma once
#include <cstdint>
static constexpr float PI = 3.1415926535f;
static constexpr float TAU = 2 * PI;
float sqrt(float z);
float invSqrt(float x);
float exp(float x);
float ln(float x);
float pow(float b, float e);

int floor(float x);
int ceil(float x);
int round(float x);
int clamp(int val, int lower, int upper);
int abs(int x);
float abs(float f);

int max(int a, int b);
float max(float a, float b);
int min(int a, int b);
float min(float a, float b);

float sin(float x);
float cos(float x);
float atan2(float y, float x);


