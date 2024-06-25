#include <mmio.h>

void mmio_write(u64 reg, u32 val)
{
    *(reg32 *)reg = val;
}

u32 mmio_read(u64 reg)
{
    return *(reg32 *)reg;
}
