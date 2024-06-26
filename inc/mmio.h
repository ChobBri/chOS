#pragma once

#include <types.h>

void mmio_write(uint64 reg, uint32 val);
uint32 mmio_read(uint64 reg);
