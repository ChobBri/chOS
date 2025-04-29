#pragma once

#include <stddef.h>
#include <stdint.h>

namespace terminal {
char* itoa( int value, char* str, int base );
void initialize(void);
void writestring(const char* data);
void write(const char* data, size_t size);
void appendChar(char c);
}