#pragma once

#include <types.h>

void timer_init(void);
void timer_handle_irq(void);
void timer_enable_interrupts(void);