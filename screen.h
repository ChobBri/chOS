#pragma once
#include <cstdint>

namespace screen {
    void init();
    int width();
    int height();
    void putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void drawline(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);  
}