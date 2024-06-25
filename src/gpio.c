#include <gpio.h>
#include <mmio.h>

u32 gpio_call(u32 pin_number, u32 value, u32 base, u32 field_size, u32 field_max) {
    u32 field_mask = (1 << field_size) - 1;
  
    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0; 

    u32 num_fields = 32 / field_size;
    u32 reg = base + ((pin_number / num_fields) * 4);
    u32 shift = (pin_number % num_fields) * field_size;

    u32 curval = mmio_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}

u32 gpio_set     (u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN); }
u32 gpio_clear   (u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN); }
u32 gpio_pull    (u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPIO_PUP_PDN_CNTRL_REG0, 2, GPIO_MAX_PIN); }
u32 gpio_function(u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); }

void gpio_useAsAlt5(u32 pin_number) {
    gpio_pull(pin_number, GPIO_PULL_OFF);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}