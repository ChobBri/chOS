#include <mini_uart.h>
#include <fb.h>
#include <queue.h>
#include <memory.h>

void intToWriteBytes(int num) {
    if (num == 0) {
        uart_writeByteBlocking('0');
        return;
    }

    while (num > 0)
    {
        int digit = num % 10;
        num /= 10;
        uart_writeByteBlocking('0' + digit);
    }
    return;
}



void main()
{
    mem_init();
    uart_init();
    
    // uart_writeText("-------------------------------------------------------\n");
    // uart_writeText("-  welcome to                                         -\n");
    // uart_writeText("-                            _____      _____         -\n");
    // uart_writeText("-                           /     \\   /     \\       -\n");
    // uart_writeText("-                          /   __  \\ |   ___//       -\n");
    // uart_writeText("-                    |     |  || |  |||   \\__        -\n");
    // uart_writeText("-                __  | _   |  ||_|  |||\____  \\      -\n");
    // uart_writeText("-               /    |  |   \       //  ___/   ||     -\n");
    // uart_writeText("-               \__  |  |    \_____//   \_____// !    -\n");
    // uart_writeText("-------------------------------------------------------\n");
    uart_writeText("-------------------------------------------------------\n");
    uart_writeText("-  welcome to                                         -\n");
    uart_writeText("-                            _____      _____         -\n");
    uart_writeText("-                           /     \\\\   /     \\\\       -\n");
    uart_writeText("-                          /   __  \\\\ |   ___//       -\n");
    uart_writeText("-                    |     |  || |  |||   \\\\__        -\n");
    uart_writeText("-                __  | _   |  ||_|  || \\____  \\\\      -\n");
    uart_writeText("-               /    |  |   \\       //  ___/   ||     -\n");
    uart_writeText("-               \\__  |  |    \\_____//   \\_____// !    -\n");
    uart_writeText("-------------------------------------------------------\n");
    uart_writeText("\n");
    uart_writeText("\n");

    struct s {
        int a;
    };

    uart_writeText("=== Testing mem ===\n");

    uart_writeText("Free list loc: ");
    uart_writeNum((uint64) (mem_getFreeList()->next));
    uart_writeText("\n");

    uart_writeText("Free list len: ");
    uart_writeNum((uint64) (mem_getFreeList()->length));
    uart_writeText("\n");

    for (int i = 0; i < 10; i++) {
        struct s *s1 = mem_allocMem(sizeof(struct s));
        struct s *s2 = mem_allocMem(sizeof(struct s));
        struct s *s3 = mem_allocMem(sizeof(struct s));
        mem_freeMem(s1, sizeof(struct s));
        mem_freeMem(s2, sizeof(struct s));
        mem_freeMem(s3, sizeof(struct s));
    }

    for (int i = 0; i < 10; i++) {
        struct s *s1 = mem_allocStack(sizeof(struct s));
        struct s *s2 = mem_allocStack(sizeof(struct s));
        struct s *s3 = mem_allocStack(sizeof(struct s));
        mem_freeStack(s1, sizeof(struct s));
        mem_freeStack(s2, sizeof(struct s));
        mem_freeStack(s3, sizeof(struct s));
    }

    for (int i = 0; i < 10; i++) {
        struct s *s1 = mem_allocStack(sizeof(struct s));
        struct s *s2 = mem_allocMem(sizeof(struct s));
        struct s *s3 = mem_allocStack(sizeof(struct s));
        struct s *s4 = mem_allocMem(sizeof(struct s));
        struct s *s5 = mem_allocMem(sizeof(struct s));
        struct s *s6 = mem_allocStack(sizeof(struct s));
        mem_freeStack(s1, sizeof(struct s));
        mem_freeMem(s2, sizeof(struct s));
        mem_freeStack(s3, sizeof(struct s));
        mem_freeMem(s4, sizeof(struct s));
        mem_freeMem(s5, sizeof(struct s));
        mem_freeStack(s6, sizeof(struct s));
    }

    uart_writeText("Free list loc: ");
    uart_writeNum((uint64) (mem_getFreeList()->next));
    uart_writeText("\n");

    uart_writeText("Free list len: ");
    uart_writeNum((uint64) (mem_getFreeList()->length));
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