#include <cstdint>

namespace screen {
    void init();
    int width();
    int height();
    void putpixel(int row, int col, uint8_t r, uint8_t g, uint8_t b);
}