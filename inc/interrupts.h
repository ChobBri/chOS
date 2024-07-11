#pragma once

#include <types.h>
#include <peripherals/interrupts.h>

#define VC_IRQ_TIMER0 (0)
#define VC_IRQ_TIMER1 (1)
#define VC_IRQ_TIMER2 (2)
#define VC_IRQ_TIMER3 (3)

typedef uint64 imask;

void intr_init(void);
imask intr_enable(void);
imask intr_disable(void);
void intr_restore(imask);

uint32 intr_getExLevel(void);