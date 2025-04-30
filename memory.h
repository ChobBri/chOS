#pragma once

#include <cstddef>

void init_memory();

void* malloc(size_t size);
void free(void* ptr);
