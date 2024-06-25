#pragma once

#include <types.h>

void uart_init();
void uart_writeText(char *buffer);
void uart_loadOutputFifo();

u32 uart_isWriteByteReady();
u32 uart_isReadByteReady();

void uart_writeByte(u8);
u8 uart_readByte();

void uart_writeByteBlocking(u8 ch);
u8 uart_readByteBlocking();

