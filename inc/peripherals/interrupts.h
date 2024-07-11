#pragma once

#include <peripherals/base.h>

// Base Address
#define ARMC_REG_BASE           (PERIPHERAL_BASE + 0xb000)
#define IRQ0_PENDING0           (ARMC_REG_BASE + 0x200)
#define IRQ0_PENDING1           (ARMC_REG_BASE + 0x204)
#define IRQ0_PENDING2           (ARMC_REG_BASE + 0x208)
#define IRQ0_SET_EN_0           (ARMC_REG_BASE + 0x210)
#define IRQ0_SET_EN_1           (ARMC_REG_BASE + 0x214)
#define IRQ0_SET_EN_2           (ARMC_REG_BASE + 0x218)
#define IRQ0_CLR_EN_0           (ARMC_REG_BASE + 0x220)
#define IRQ0_CLR_EN_1           (ARMC_REG_BASE + 0x224)
#define IRQ0_CLR_EN_2           (ARMC_REG_BASE + 0x228)
#define IRQ_STATUS0             (ARMC_REG_BASE + 0x230)
#define IRQ_STATUS1             (ARMC_REG_BASE + 0x234)
#define IRQ_STATUS2             (ARMC_REG_BASE + 0x238)
