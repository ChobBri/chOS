.align 4
.global irq_stub_33
irq_stub_33:
    pusha
    movb $33, %al              # IRQ Number (33 - Keyboard)
    pushl %eax
    call interrupt_handler
    add $4, %esp
    movb $0x20, %al
    outb %al, $0x20
    popa
    iret
