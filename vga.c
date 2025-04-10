#include "vga.h"
#include "x86.h"

/* Read from attribute register */
uint8_t vga_read_atrb_reg(uint8_t index) {
    cli();
    (void) inb(0x3DA);  // reset
    outb(0x3C0, index | (1 << 5));
    uint8_t data = inb(0x3C1);
    sti();
    return data;
}

/* Write to attribute register */
void vga_write_atrb_reg(uint8_t index, uint8_t data) {
    cli();
    (void) inb(0x3DA);  // reset
    outb(0x3C0, index | (1 << 5));
    outb(0x3C0, data);
    sti();
}

/* Read from miscellaneous output register */
uint8_t vga_read_misc_output_reg() {
    cli();
    uint8_t data = inb(0x3CC);
    sti();
    return data;
}

/* Write to miscellaneous output register */
void vga_write_misc_output_reg(uint8_t data) {
    cli();
    outb(0x3C2, data);
    sti();
}

/* Read from graphics register */
uint8_t vga_read_gfx_reg(uint8_t index) {
    cli();
    outb(0x3CE, index);
    uint8_t data = inb(0x3CF);
    sti();
    return data;
}

/* Write to graphics register */
void vga_write_gfx_reg(uint8_t index, uint8_t data) {
    cli();
    outb(0x3CE, index);
    outb(0x3CF, data);
    sti();
}

/* Read from sequencer register */
uint8_t vga_read_seq_reg(uint8_t index) {
    cli();
    outb(0x3C4, index);
    uint8_t data = inb(0x3C5);
    sti();
    return data;
}

/* Write to sequencer register */
void vga_write_seq_reg(uint8_t index, uint8_t data) {
    cli();
    outb(0x3C4, index);
    outb(0x3C5, data);
    sti();
}

/* Read from CRT controller register */
/* assumes bit 0 of misc output reg is set */
uint8_t vga_read_crt_ctrl_reg(uint8_t index) {
    cli();
    outb(0x3D4, index);
    uint8_t data = inb(0x3D5);
    sti();
    return data;
}

/* Write to CRT controller register */
/* assumes bit 0 of misc output reg is set */
void vga_write_crt_ctrl_reg(uint8_t index, uint8_t data) {
    cli();
    outb(0x3D4, index);
    outb(0x3D5, data);
    sti();
}
