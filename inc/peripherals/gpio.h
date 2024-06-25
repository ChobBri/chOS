#pragma once

#include <peripherals/base.h>

// Base Address
#define GPIO_REG_BASE           (PERIPHERAL_BASE + 0x200000)
#define GPFSEL0                 (GPIO_REG_BASE + 0x00)
#define GPFSEL1                 (GPIO_REG_BASE + 0x04)
#define GPFSEL2                 (GPIO_REG_BASE + 0x08)
#define GPFSEL3                 (GPIO_REG_BASE + 0x0c)
#define GPFSEL4                 (GPIO_REG_BASE + 0x10)
#define GPFSEL5                 (GPIO_REG_BASE + 0x14)
#define GPSET0                  (GPIO_REG_BASE + 0x1c)
#define GPSET1                  (GPIO_REG_BASE + 0x20)
#define GPCLR0                  (GPIO_REG_BASE + 0x28)
#define GPCLR1                  (GPIO_REG_BASE + 0x2C)
#define GPIO_PUP_PDN_CNTRL_REG0 (GPIO_REG_BASE + 0xe4)
#define GPIO_PUP_PDN_CNTRL_REG1 (GPIO_REG_BASE + 0xe8)
#define GPIO_PUP_PDN_CNTRL_REG2 (GPIO_REG_BASE + 0xeC)
#define GPIO_PUP_PDN_CNTRL_REG3 (GPIO_REG_BASE + 0xf0)
