#include "vga.h"
#include "x86.h"

namespace vga {

static constexpr uint8_t PAS_BIT = (1 << 5);
/* Read from attribute register */
uint8_t read_atrb_reg(uint8_t index) {
    cli();
    (void) inb(INPUT_STATUS_COLOR_REG1);  // reset
    outb(ATRB_CTRL_ADDR_DATA_REG, index | PAS_BIT);
    uint8_t data = inb(ATRB_CTRL_ADDR_DATA_REG);
    sti();
    return data;
}

/* Write to attribute register */
void write_atrb_reg(uint8_t index, uint8_t data) {
    cli();
    (void) inb(INPUT_STATUS_COLOR_REG1);  // reset
    outb(ATRB_CTRL_ADDR_DATA_REG, index | PAS_BIT);
    outb(ATRB_CTRL_ADDR_DATA_REG, data);
    sti();
}

/* Read from miscellaneous output register */
uint8_t read_misc_output_reg() {
    cli();
    uint8_t data = inb(MISC_OUTPUT_READ_REG);
    sti();
    return data;
}

/* Write to miscellaneous output register */
void write_misc_output_reg(uint8_t data) {
    cli();
    outb(MISC_OUTPUT_WRITE_REG, data);
    sti();
}

/* Read from graphics register */
uint8_t read_gfx_reg(uint8_t index) {
    cli();
    outb(GFX_ADDR_REG, index);
    uint8_t data = inb(GFX_DATA_REG);
    sti();
    return data;
}

/* Write to graphics register */
void write_gfx_reg(uint8_t index, uint8_t data) {
    cli();
    outb(GFX_ADDR_REG, index);
    outb(GFX_DATA_REG, data);
    sti();
}

/* Read from sequencer register */
uint8_t read_seq_reg(uint8_t index) {
    cli();
    outb(SEQ_ADDR_REG, index);
    uint8_t data = inb(SEQ_DATA_REG);
    sti();
    return data;
}

/* Write to sequencer register */
void write_seq_reg(uint8_t index, uint8_t data) {
    cli();
    outb(SEQ_ADDR_REG, index);
    outb(SEQ_DATA_REG, data);
    sti();
}

/* Read from CRT controller register */
/* assumes bit 0 of misc output reg is set */
uint8_t read_crt_ctrl_reg(uint8_t index) {
    cli();
    outb(CRTC_ADDR_REG, index);
    uint8_t data = inb(CRTC_DATA_REG);
    sti();
    return data;
}

/* Write to CRT controller register */
/* assumes bit 0 of misc output reg is set */
void write_crt_ctrl_reg(uint8_t index, uint8_t data) {
    cli();
    outb(CRTC_ADDR_REG, index);
    outb(CRTC_DATA_REG, data);
    sti();
}

uint32_t read_color_reg(uint8_t index, uint8_t& r, uint8_t& g, uint8_t& b) {
    cli();
    outb(vga::DAC_ADDR_WRITE_MODE_REG, index);
    r = inb(vga::DAC_ADDR_DATA_REG);
    g = inb(vga::DAC_ADDR_DATA_REG);
    b = inb(vga::DAC_ADDR_DATA_REG);
    sti();
    return ((b & 0x3F) << 12) | ((g & 0x3F) << 6) | (r & 0x3F);
}

void write_color_reg(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    cli();
    outb(vga::DAC_ADDR_WRITE_MODE_REG, index);
    outb(vga::DAC_ADDR_DATA_REG, r);
    outb(vga::DAC_ADDR_DATA_REG, g);
    outb(vga::DAC_ADDR_DATA_REG, b);
    sti();
}


}
