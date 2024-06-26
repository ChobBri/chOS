#include <mini_uart.h>
#include <fb.h>
#include <queue.h>

void main()
{
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

    
    qid16 newq = queue_getNew();

    queue_enqueue(newq, 4);
    queue_enqueue(newq, 7);
    pid32 pid;
    pid = queue_dequeue(newq);
    uart_writeByteBlocking('0' + pid);

    qid16 newq2 = queue_getNew();
    queue_enqueue(newq2, 2);
    queue_enqueue(newq2, 3);

    pid = queue_dequeue(newq);
    uart_writeByteBlocking('0' + pid);

    pid = queue_dequeue(newq);
    if (pid == QUEUE_EMPTY) {
        uart_writeText("queue was empty!");
    } else {
        uart_writeText("queue was not empty?");
    }

    pid = queue_dequeue(newq2);
    uart_writeByteBlocking('0' + pid);

    pid = queue_dequeue(newq2);
    uart_writeByteBlocking('0' + pid);

    pid = queue_dequeue(newq2);
    if (pid == QUEUE_EMPTY) {
        uart_writeText("queue was empty!");
    } else {
        uart_writeText("queue was not empty?");
    }

    // fb_init();

    // drawRect(150,150,400,400,0x03,0);
    // drawRect(300,300,350,350,0x2e,1);

    // drawCircle(960,540,250,0x0e,0);
    // drawCircle(960,540,50,0x13,1);

    // drawPixel(250,250,0x0e);

    // drawChar('O',500,500,0x05);
    // drawString(100,100,"Hello world!",0x0f);

    // drawLine(100,500,350,700,0x0c);

    while (1) {
        uint8 ch = uart_readByteBlocking();
        uart_writeByteBlocking(ch);
        if (ch == '\r')
        {
            uart_writeByteBlocking('\n');
        }
    }
}