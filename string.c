
#include "string.h"

char* strncpy(char* dest, const char* src, size_t len) {
    for (size_t i = 0; i < len; i++) {
        char c = src[i];
        dest[i] = c;
        if (c == '\0') {
            return dest;
        }
    }
    return dest;
}

char* strncat(char* dest, const char* src, size_t len) {
    size_t offset = 0;
    while (dest[offset] != '\0') {
        offset++;
    }

    for (size_t i = 0; i < len; i++) {
        char c = src[i];
        dest[i + offset] = c;
        if (c == '\0') {
            return dest;
        }
    }
    return dest;
}
