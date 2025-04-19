#pragma once

#include <stddef.h>
#include <stdint.h>

namespace terminal {
char* itoa( int value, char* str, int base );
void init_mode3();
void init_mode13();
void initialize(void);
void writestring(const char* data);
void write(const char* data, size_t size);
void putchar(char c);
}