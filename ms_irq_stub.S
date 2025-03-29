.align 4
.global ms_irq_stub_32
ms_irq_stub_32:
    pusha
    movb $32, %al              # IRQ Number (33 - Timer)
    push %eax
    call ms_interrupt_handler
    pop %eax
    movb $0x20, %al
    outb %al, $0x20
    popa
    iret
.align 4
.global ms_irq_stub_33
ms_irq_stub_33:
    pusha
    movb $33, %al              # IRQ Number (33 - Keyboard)
    push %eax
    call ms_interrupt_handler
    pop %eax
    movb $0x20, %al
    outb %al, $0x20
    popa
    iret
