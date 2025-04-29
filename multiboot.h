#pragma once
#include <cstdint>

/* Magic field of the header */
static constexpr uint32_t MULTIBOOT_HEADER_MAGIC = 0x1BADB002;

/* Should be in %eax after GRUB hands over control */
static constexpr uint32_t MULTIBOOT_BOOTLOADER_MAGIC = 0x2BADB002;

/* Multiboot Info Flags */
static constexpr uint32_t MULTIBOOT_INFO_MEMORY = (1 << 0);
static constexpr uint32_t MULTIBOOT_INFO_BOOTDEV = (1 << 1);
static constexpr uint32_t MULTIBOOT_INFO_CMDLINE = (1 << 2);
static constexpr uint32_t MULTIBOOT_INFO_MODS = (1 << 3);
static constexpr uint32_t MULTIBOOT_INFO_AOUT_SYMS = (1 << 4);
static constexpr uint32_t MULTIBOOT_INFO_ELF_SHDR = (1 << 5);
static constexpr uint32_t MULTIBOOT_INFO_MEM_MAP = (1 << 6);
static constexpr uint32_t MULTIBOOT_INFO_DRIVE_INFO = (1 << 7);
static constexpr uint32_t MULTIBOOT_INFO_CONFIG_TABLE = (1 << 8);
static constexpr uint32_t MULTIBOOT_INFO_BOOT_LOADER_NAME = (1 << 9);
static constexpr uint32_t MULTIBOOT_INFO_APM_TABLE = (1 << 10);
static constexpr uint32_t MULTIBOOT_INFO_VBE_INFO = (1 << 11);
static constexpr uint32_t MULTIBOOT_INFO_FRAMEBUFFER_INFO = (1 << 12);


struct multiboot_mod_entry {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved; /* set to 0 by bootloader */
};

struct multiboot_aout_symbol_table {
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved; /* set to 0 by bootloader */
};

struct multiboot_elf_section_header_table {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
};

struct multiboot_mmap_entry {
    uint32_t size;
    uint32_t addr_low;
    uint32_t addr_high;
    uint32_t len_low;
    uint32_t len_high;
    uint32_t type;

    /* Constants for 'type' */
    static constexpr uint32_t TYPE_AVAILABLE             = 1;
    static constexpr uint32_t TYPE_RESERVED              = 2;
    static constexpr uint32_t TYPE_ACPI_RECLAIMABLE      = 3;
    static constexpr uint32_t TYPE_NVS                   = 4;
    static constexpr uint32_t TYPE_BADRAM                = 5;
} __attribute__((packed));

struct multiboot_drive_entry {
    uint32_t size;
    uint32_t num;
    uint8_t  mode;
    uint16_t cylinders;
    uint8_t  heads;
    uint8_t  sectors;
    uint16_t ports[0]; /* use size to determine the number of ports */

    /* Constants for 'mode' */
    static constexpr uint8_t MODE_CHS = 0;
    static constexpr uint8_t MODE_LBA = 1;
};

struct multiboot_apm_table {
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg_16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_len;
    uint16_t cseg_16_len;
    uint16_t dseg_len;
};

struct multiboot_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} __attribute__((packed));

struct multiboot_info
{
    /* Multiboot info version number */
    uint32_t flags;

    /* Available memory from BIOS */
    uint32_t mem_lower;
    uint32_t mem_upper;

    /* "root" partition */
    uint32_t boot_device;

    /* Kernel command line */
    uint32_t cmdline;

    /* Boot-Module list */
    uint32_t mods_count;
    uint32_t mods_addr;

    /* mut-ex aout-sym-tbl or elf-section-hdr-tbl */
    union syms
    {
        struct multiboot_aout_symbol_table {
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t reserved; /* set to 0 by bootloader */
        } aout_sym;
        struct multiboot_elf_section_header_table {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } elf_sec;
    } syms;

    /* Memory Mapping buffer */
    uint32_t mmap_length;
    uint32_t mmap_addr;

    /* Drive Info buffer */
    uint32_t drives_length;
    uint32_t drives_addr;

    /* ROM configuration table */
    uint32_t config_table;

    /* Boot Loader Name */
    uint32_t boot_loader_name;

    /* APM table */
    uint32_t apm_table_addr;

    /* Video */
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint32_t framebuffer_addr_low;
    uint32_t framebuffer_addr_high;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    static constexpr uint8_t MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED  = 0;
    static constexpr uint8_t MULTIBOOT_FRAMEBUFFER_TYPE_RGB      = 1;
    static constexpr uint8_t MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT = 2;
    uint8_t framebuffer_type;
    union color_info
    {
        struct multiboot_index_color_info
        {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        } index_color_info;
        struct multiboot_direct_color_info
        {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        } direct_color_info;
    } color_info;
};

const multiboot_info* get_multiboot_info();
bool isFlagSet(uint32_t flag);
