.text

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.global disable
disable:
    mrs x0, DAIF
    msr DAIFSet, #2
    ret

.global enable
enable:
    mrs x0, DAIF
    msr DAIFClr, #2
    ret

.global restore
restore:
    msr DAIF, x0
    ret

/* Exceptions vector table */
/* Table has 4 sets of the entries (in the following order): Synchronous, IRQ, FIQ, SError */
.align 11               // aarch64 requires 2 KB alignment (2KB = 2^10 * 2 = 2^11)
.global exp_vec_table 
exp_vec_table:
    /* Each entry in the vector table is 16 instructions long. */
    /* 1 instruction = 8 bytes, 8B * 16 = 128B, log_2(128) = 7 */
    // Current EL SP0
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .
    // Current EL SPx
    .align 7
        b .
    .align 7
        b handle_cur_el_spx_irq
    .align 7
        b .
    .align 7
        b .
    // Lower EL AArch64
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .
    // Lower EL AArch32
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .
    .align 7
        b .

.macro kernel_entry
    str	x30,      [sp, #-16]!
    stp	x28, x29, [sp, #-16]!
    stp	x26, x27, [sp, #-16]!
    stp	x24, x25, [sp, #-16]!
    stp	x22, x23, [sp, #-16]!
    stp	x20, x21, [sp, #-16]!
    stp	x18, x19, [sp, #-16]!
    stp	x16, x17, [sp, #-16]!
    stp	x14, x15, [sp, #-16]!
    stp	x12, x13, [sp, #-16]!
    stp	x10, x11, [sp, #-16]!
    stp	x8 , x9 , [sp, #-16]!
    stp	x6 , x7 , [sp, #-16]!
    stp	x4 , x5 , [sp, #-16]!
    stp	x2 , x3 , [sp, #-16]!
    stp	x0 , x1 , [sp, #-16]!
.endm

.macro kernel_exit
    ldp	x0 , x1 ,  [sp], #16
    ldp	x2 , x3 ,  [sp], #16
    ldp	x4 , x5 ,  [sp], #16
    ldp	x6 , x7 ,  [sp], #16
    ldp	x8 , x9 ,  [sp], #16
    ldp	x10, x11,  [sp], #16
    ldp	x12, x13,  [sp], #16
    ldp	x14, x15,  [sp], #16
    ldp	x16, x17,  [sp], #16
    ldp	x18, x19,  [sp], #16
    ldp	x20, x21,  [sp], #16
    ldp	x22, x23,  [sp], #16
    ldp	x24, x25,  [sp], #16
    ldp	x26, x27,  [sp], #16
    ldp	x28, x29,  [sp], #16
    ldr x30,       [sp], #16
	eret
.endm

handle_cur_el_spx_irq:
	kernel_entry
	bl	handle_irq
	kernel_exit

.global init_exp_vec
init_exp_vec:
    adr x0, exp_vec_table
    msr VBAR_EL1, x0
    ret
