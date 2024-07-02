#pragma once

#include <types.h>

void uart_init();
void uart_writeText(char *buffer);
void uart_loadOutputFifo();

uint32 uart_isWriteByteReady();
uint32 uart_isReadByteReady();

void uart_writeByte(uint8);
uint8 uart_readByte();

void uart_writeByteBlocking(uint8 ch);
uint8 uart_readByteBlocking();

void uart_writeNum(uint64);
