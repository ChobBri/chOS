#pragma once

#include <types.h>

#define PERIPHERAL_BASE 0xFE000000

void uart_init();
void uart_writeText(char *buffer);
void uart_loadOutputFifo();
u8 uart_readByte();
u16 uart_isReadByteReady();
void uart_writeByteBlocking(u8 ch);
void uart_update();
