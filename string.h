#pragma once
#include <stddef.h>

char* strncpy(char* dest, const char* src, size_t len);
char* strncat(char* dest, const char* src, size_t len);
int strncmp(const char* str1, const char* str2, size_t len);
size_t strlen(const char* str);
