#include <mini_uart.h>
#include <mmio.h>
#include <fb.h>
#include <queue.h>
#include <memory.h>
#include <interrupts.h>
#include <timer.h>

void intToWriteBytes(uint64 num) {
    if (num == 0) {
        uart_writeByteBlocking('0');
        return;
    }

    uint64 rev_num = 0;
    uint64 count = 0;

    while (num > 0) {
        rev_num = rev_num * 10 + num % 10;
        num /= 10;
        count++;
    }

    while (count > 0)
    {
        int digit = rev_num % 10;
        rev_num /= 10;
        uart_writeByteBlocking('0' + digit);
        count--;
    }
    return;
}

void handle_irq() {
    uart_writeText("interrupt!\n");

    // TODO: make a generic irq router
    timer_handle_irq();  
}

void main()
{
    intr_init();
    timer_init();

    mem_init();
    uart_init();
    timer_enable_interrupts();
    intr_enable();
    
    // uart_writeText("-------------------------------------------------------\n");
    // uart_writeText("-  welcome to                                         -\n");
    // uart_writeText("-                             _____     _____         -\n");
    // uart_writeText("-                            /     \\  /     \\       -\n");
    // uart_writeText("-                           /  __   \\|   ___//       -\n");
    // uart_writeText("-                    |     |  || |  |||   \\__        -\n");
    // uart_writeText("-                __  | _   |  ||_|  |||\____  \\      -\n");
    // uart_writeText("-               /    |  |   \       //  ___/   ||     -\n");
    // uart_writeText("-               \__  |  |    \_____//   \_____// !    -\n");
    // uart_writeText("-------------------------------------------------------\n");
    uart_writeText("-------------------------------------------------------\n");
    uart_writeText("-  welcome to                                         -\n");
    uart_writeText("-                             _____     _____         -\n");
    uart_writeText("-                            /     \\\\  /     \\\\       -\n");
    uart_writeText("-                           /  __   \\\\|   ___//       -\n");
    uart_writeText("-                    |     |  || |  |||   \\\\__        -\n");
    uart_writeText("-                __  | _   |  ||_|  || \\____  \\\\      -\n");
    uart_writeText("-               /    |  |   \\       //  ___/   ||     -\n");
    uart_writeText("-               \\__  |  |    \\_____//   \\_____// !    -\n");
    uart_writeText("-------------------------------------------------------\n");
    uart_writeText("\n");
    uart_writeText("\n");

    uart_writeText("Booted in exception level: ");
    intToWriteBytes(intr_getExLevel());
    uart_writeText("\n");
    uart_writeText("\n");

    while (1) {
        uint8 ch = uart_readByteBlocking();
        uart_writeByteBlocking(ch);
        if (ch == '\r')
        {
            uart_writeByteBlocking('\n');
        }
    }
}