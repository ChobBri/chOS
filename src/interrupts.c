#include <interrupts.h>

extern void init_exp_vec();
extern void restore(imask);
extern imask enable(void);
extern imask disable(void);
extern uint32 get_el(void);

void intr_init(void) {
    init_exp_vec();
}

imask intr_enable(void) {
    return enable();
}

imask intr_disable(void) {
    return disable();
}

void intr_restore(imask mask) {
    restore(mask);
}

uint32 intr_getExLevel(void) {
    return get_el();
}
