#include <sysregs.h>

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, main_core
    // We're not on the main core, so hang in an infinite wait loop
proc_hang:  wfe
    b       proc_hang
main_core:  // We're on the main core!
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1, x0        

    ldr     x0, =HCR_VALUE
    msr     hcr_el2, x0

    ldr     x0, =SCR_VALUE
    msr     scr_el3, x0

    ldr     x0, =SPSR_VALUE
    msr     spsr_el3, x0

    adr     x0, bss_clean        
    msr     elr_el3, x0

    adr     x0, bss_clean        
    msr     elr_el2, x0
    eret

bss_clean:
    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
bss_clean_loop:
    cbz     w2, enter_main       // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, bss_clean_loop   // Loop if non-zero
enter_main:  // Jump to our main() routine in C (make sure it doesn't return)
    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1
    bl      main
    // In case it does return, halt the master core too
    b       proc_hang
