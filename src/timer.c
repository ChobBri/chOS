#include <timer.h>
#include <peripherals/base.h>
#include <interrupts.h>
#include <mmio.h>

uint32 timer1_int = 1000000;
uint32 timer1_val = 0;

void timer_init(void) {
    timer1_val = mmio_read(PERIPHERAL_BASE + 0x3004);  // global timer low bits
    timer1_val += timer1_int;
    mmio_write(PERIPHERAL_BASE + 0x3010, timer1_val);  // timer 1 compare val
}

void timer_handle_irq(void) {
    timer1_val += timer1_int;
    mmio_write(PERIPHERAL_BASE + 0x3010, timer1_val);
    mmio_set(PERIPHERAL_BASE + 0x3000, 1 << VC_IRQ_TIMER1);
}

void timer_enable_interrupts(void) {
    mmio_set(IRQ0_SET_EN_0, 1 << VC_IRQ_TIMER1);
}