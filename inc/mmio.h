#pragma once

#include <types.h>

void mmio_write(u64 reg, u32 val);
u32 mmio_read(u64 reg);
