#pragma once

#include <cstdint>
#include <cstddef>

namespace vga {

/* Graphics Registers */
static constexpr uint16_t GFX_ADDR_REG = 0x3CE;
static constexpr uint16_t GFX_DATA_REG = 0x3CF;
static constexpr uint16_t GFX_IDX_SET_RST_REG = 0x0;
static constexpr uint16_t GFX_IDX_ENABLE_SET_RST_REG = 0x1;
static constexpr uint16_t GFX_IDX_CLR_CMP_REG = 0x2;
static constexpr uint16_t GFX_IDX_DATA_ROT_REG = 0x3;
static constexpr uint16_t GFX_IDX_READ_MAP_SEL_REG = 0x4;
static constexpr uint16_t GFX_IDX_GFX_MODE_REG = 0x5;
static constexpr uint16_t GFX_IDX_MISC_GFX_REG = 0x6;
static constexpr uint16_t GFX_IDX_COLOR_DNT_CARE_REG = 0x7;
static constexpr uint16_t GFX_IDX_BIT_MASK_REG = 0x8;

/* Sequencer Registers */
static constexpr uint16_t SEQ_ADDR_REG = 0x3C4;
static constexpr uint16_t SEQ_DATA_REG = 0x3C5;
static constexpr uint16_t SEQ_IDX_RST_REG = 0x0;
static constexpr uint16_t SEQ_IDX_CLK_MODE_REG = 0x1;
static constexpr uint16_t SEQ_IDX_MAP_MASK_REG = 0x2;
static constexpr uint16_t SEQ_IDX_CHAR_MAP_SEL_REG = 0x3;
static constexpr uint16_t SEQ_IDX_SEQ_MEM_MODE_REG = 0x4;

/* Attribute Controller Registers */
static constexpr uint16_t ATRB_CTRL_ADDR_DATA_REG = 0x3C0;
static constexpr uint16_t ATRB_CTRL_DATA_READ_REG = 0x3C1;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG0 = 0x0;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG1 = 0x1;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG2 = 0x2;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG3 = 0x3;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG4 = 0x4;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG5 = 0x5;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG6 = 0x6;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG7 = 0x7;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG8 = 0x8;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG9 = 0x9;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG10 = 0xA;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG11 = 0xB;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG12 = 0xC;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG13 = 0xD;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG14 = 0xE;
static constexpr uint16_t ATRB_CTRL_IDX_PALET_REG15 = 0xF;
static constexpr uint16_t ATRB_CTRL_IDX_ATRB_MODE_CTRL_REG = 0x10;
static constexpr uint16_t ATRB_CTRL_IDX_OVERSCAN_COLOR_REG = 0x11;
static constexpr uint16_t ATRB_CTRL_IDX_COLOR_PLANE_ENABLE_REG = 0x12;
static constexpr uint16_t ATRB_CTRL_IDX_HORZ_PXL_PAN_REG = 0x13;
static constexpr uint16_t ATRB_CTRL_IDX_COLOR_SELECT_REG = 0x14;

/* CRT Controller Registers */
static constexpr uint16_t CRTC_ADDR_REG = 0x3D4;
static constexpr uint16_t CRTC_DATA_REG = 0x3D5;
static constexpr uint16_t CRTC_ADDR_ALT_REG = 0x3B4;
static constexpr uint16_t CRTC_DATA_ALT_REG = 0x3B5;
static constexpr uint16_t CRTC_IDX_HORZ_TOTAL_REG = 0x0;
static constexpr uint16_t CRTC_IDX_END_HORZ_DISP_REG = 0x1;
static constexpr uint16_t CRTC_IDX_START_HORZ_BLANK_REG = 0x2;
static constexpr uint16_t CRTC_IDX_END_HORZ_BLANK_REG = 0x3;
static constexpr uint16_t CRTC_IDX_START_HORZ_RETRACE_REG = 0x4;
static constexpr uint16_t CRTC_IDX_END_HORZ_RETRACE_REG = 0x5;
static constexpr uint16_t CRTC_IDX_VERT_TOTAL_REG = 0x6;
static constexpr uint16_t CRTC_IDX_OVERFLOW_REG = 0x7;
static constexpr uint16_t CRTC_IDX_PRESET_ROW_SCAN_REG = 0x8;
static constexpr uint16_t CRTC_IDX_MAX_SCAN_LINE_REG = 0x9;
static constexpr uint16_t CRTC_IDX_CURSOR_START_REG = 0xA;
static constexpr uint16_t CRTC_IDX_CURSOR_END_REG = 0xB;
static constexpr uint16_t CRTC_IDX_START_ADDR_HIGH_REG = 0xC;
static constexpr uint16_t CRTC_IDX_START_ADDR_LOW_REG = 0xD;
static constexpr uint16_t CRTC_IDX_CURSOR_LOC_HIGH_REG = 0xE;
static constexpr uint16_t CRTC_IDX_CURSOR_LOC_LOW_REG = 0xF;
static constexpr uint16_t CRTC_IDX_VERT_RETRACE_START_REG = 0x10;
static constexpr uint16_t CRTC_IDX_VERT_RETRACE_END_REG = 0x11;
static constexpr uint16_t CRTC_IDX_VERT_DISP_END_REG = 0x12;
static constexpr uint16_t CRTC_IDX_OFFSET_REG = 0x13;
static constexpr uint16_t CRTC_IDX_UNDERLINE_LOC_REG = 0x14;
static constexpr uint16_t CRTC_IDX_START_VERT_BLANK_REG = 0x15;
static constexpr uint16_t CRTC_IDX_END_VERT_BLANK_REG = 0x16;
static constexpr uint16_t CRTC_IDX_CRTC_MODE_CTRL_REG = 0x17;
static constexpr uint16_t CRTC_IDX_LINE_COMP_REG = 0x18;

/* Color Registers */
static constexpr uint16_t DAC_ADDR_WRITE_MODE_REG = 0x3C8;
static constexpr uint16_t DAC_ADDR_READ_MODE_REG = 0x3C7;
static constexpr uint16_t DAC_ADDR_DATA_REG = 0x3C9;
static constexpr uint16_t DAC_ADDR_STATE_REG = 0x3C7;

/* External Registers */
static constexpr uint16_t MISC_OUTPUT_READ_REG = 0x3CC;
static constexpr uint16_t MISC_OUTPUT_WRITE_REG = 0x3C2;
static constexpr uint16_t FEATURE_CTRL_READ_REG = 0x3CA;
static constexpr uint16_t FEATURE_CTRL_WRITE_MONO_REG = 0x3BA;
static constexpr uint16_t FEATURE_CTRL_WRITE_COLOR_REG = 0x3DA;
static constexpr uint16_t INPUT_STATUS_REG0 = 0x3C2;
static constexpr uint16_t INPUT_STATUS_MONO_REG1 = 0x3BA;
static constexpr uint16_t INPUT_STATUS_COLOR_REG1 = 0x3DA;

static constexpr int VGA_WIDTH = 80;
static constexpr int VGA_HEIGHT = 25;
static constexpr int VGA_MEMORY_BASE = 0xB8000;

/* Hardware text mode color constants. */
typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

uint8_t read_atrb_reg(uint8_t index);
void write_atrb_reg(uint8_t index, uint8_t data);
uint8_t read_misc_output_reg();
void write_misc_output_reg(uint8_t data);
uint8_t read_gfx_reg(uint8_t index);
void write_gfx_reg(uint8_t index, uint8_t data);
uint8_t read_seq_reg(uint8_t index);
void write_seq_reg(uint8_t index, uint8_t data);
uint8_t read_crt_ctrl_reg(uint8_t index);
void write_crt_ctrl_reg(uint8_t index, uint8_t data);
uint32_t read_color_reg(uint8_t index, uint8_t& r, uint8_t& g, uint8_t& b);
void write_color_reg(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

static inline uint8_t entry_color(vga_color fg, vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t entry(char c, uint8_t color) 
{
	return (uint16_t) c | (uint16_t) color << 8;
}

}