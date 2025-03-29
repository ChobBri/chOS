#pragma once

#include <stddef.h>
#include <stdint.h>

char* itoa( int value, char* str, int base );
void terminal_initialize(void);
void terminal_writestring(const char* data);
void terminal_write(const char* data, size_t size);
void terminal_putchar(char c);
