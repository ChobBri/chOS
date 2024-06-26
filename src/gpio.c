#include <gpio.h>
#include <mmio.h>

uint32 gpio_call(uint32 pin_number, uint32 value, uint32 base, uint32 field_size, uint32 field_max) {
    uint32 field_mask = (1 << field_size) - 1;
  
    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0; 

    uint32 num_fields = 32 / field_size;
    uint32 reg = base + ((pin_number / num_fields) * 4);
    uint32 shift = (pin_number % num_fields) * field_size;

    uint32 curval = mmio_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}

uint32 gpio_set     (uint32 pin_number, uint32 value) { return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN); }
uint32 gpio_clear   (uint32 pin_number, uint32 value) { return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN); }
uint32 gpio_pull    (uint32 pin_number, uint32 value) { return gpio_call(pin_number, value, GPIO_PUP_PDN_CNTRL_REG0, 2, GPIO_MAX_PIN); }
uint32 gpio_function(uint32 pin_number, uint32 value) { return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); }

void gpio_useAsAlt5(uint32 pin_number) {

}