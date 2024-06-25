#include <io.h>
#include <mmio.h>

// GPIO 

// Base Address
enum {
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4,
};

enum {
    GPIO_MAX_PIN       = 53,
    GPIO_FUNCTION_INPUT = 0b000,
    GPIO_FUNCTION_OUTPUT = 0b001,
    GPIO_FUNCTION_ALT0 = 0b100,
    GPIO_FUNCTION_ALT1 = 0b101,
    GPIO_FUNCTION_ALT2 = 0b110,
    GPIO_FUNCTION_ALT3 = 0b111,
    GPIO_FUNCTION_ALT4 = 0b011,
    GPIO_FUNCTION_ALT5 = 0b010,
};

enum {
    GPIO_PUPPDN_NONE = 0b00,
    GPIO_PUPPDN_UP = 0b01,
    GPIO_PUPPDN_DOWN = 0b10,
};

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
u32 gpio_pull    (u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN); }
u32 gpio_function(u32 pin_number, u32 value) { return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); }

void gpio_useAsAlt5(u32 pin_number) {
    gpio_pull(pin_number, GPIO_PUPPDN_NONE);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}

// UART

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK  = 500000000,
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init() {
    mmio_write(AUX_ENABLES, 1); //enable UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); //8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); //disable interrupts
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200)); // 9600, 115200
    gpio_useAsAlt5(14);
    gpio_useAsAlt5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

u32 uart_isWriteByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x20; }

void uart_writeByteBlockingActual(u8 ch) {
    while (!uart_isWriteByteReady()); 
    mmio_write(AUX_MU_IO_REG, (u32)ch);
}

void uart_writeText(char *buffer) {
    while (*buffer) {
       if (*buffer == '\n') uart_writeByteBlockingActual('\r');
       uart_writeByteBlockingActual(*buffer++);
    }
}