#pragma once
#include <cstdint>
#include <cstddef>

namespace screen {
    void init();
    int width();
    int height();
    void putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void drawline(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);  
    void swapBuffers();
    void fillScreen(int r, int g, int b);
    void write(const char* data, size_t size, int x, int y);
    void writestring(const char* data, int x, int y);
}