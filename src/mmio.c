#include <mmio.h>

void mmio_write(uint64 reg, uint32 val)
{
    *(reg32 *)reg = val;
}

void mmio_set(uint64 reg, uint32 val)
{
    *(reg32 *)reg |= val;
}

uint32 mmio_read(uint64 reg)
{
    return *(reg32 *)reg;
}
