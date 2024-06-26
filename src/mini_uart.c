#include <mini_uart.h>
#include <mmio.h>
#include <gpio.h>
#include <peripherals/aux.h>

// UART
#define TXD 14
#define RXD 15

enum {
    AUX_UART_CLOCK  = 500000000,
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init() {
    gpio_pull(TXD, GPIO_PULL_OFF);
    gpio_function(TXD, GPIO_FUNCTION_ALT5);

    gpio_pull(RXD, GPIO_PULL_OFF);
    gpio_function(RXD, GPIO_FUNCTION_ALT5);

    mmio_write(AUX_ENABLES, 1);                       // enable UART1
    mmio_write(AUX_MU_CNTL_REG, 0);                   // disable UART RX/TX
    mmio_write(AUX_MU_LCR_REG, 3);                    // 8 bits mode
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200)); // set baud rate
    mmio_write(AUX_MU_CNTL_REG, 3);                   // enable UART RX/TX
}

u32 uart_isWriteByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x20; }
u32 uart_isReadByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x01; }

void uart_writeByte(u8 ch) { mmio_write(AUX_MU_IO_REG, (u32)ch); }
u8 uart_readByte() { return mmio_read(AUX_MU_IO_REG) & 0xFF; }

void uart_writeByteBlocking(u8 ch) {
    while (!uart_isWriteByteReady()); 
    uart_writeByte(ch);
}

u8 uart_readByteBlocking() {
    while (!uart_isReadByteReady());
    return uart_readByte();
}

void uart_writeText(char *buffer) {
    while (*buffer) {
        if (*buffer == '\n') uart_writeByteBlocking('\r');
        uart_writeByteBlocking(*buffer++);
    }
}
