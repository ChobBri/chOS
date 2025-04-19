
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

int strncmp(const char* str1, const char* str2, size_t len) {
    for (size_t i = 0; i < len; i++) {
        char c1 = str1[i];
        char c2 = str2[i];

        if (c1 < c2) {
            return -1;
        }
        else if (c1 > c2) {
            return 1;
        }

        if (c1 == '\0') { // && c2 == '\0'
            return 0;
        }
    }
    return 0;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
