#pragma once

#include <types.h>
#include <peripherals/gpio.h>

enum {
  GPIO_MAX_PIN       = 53,
};

typedef enum {
    GPIO_FUNCTION_INPUT = 0b000,
    GPIO_FUNCTION_OUTPUT = 0b001,
    GPIO_FUNCTION_ALT0 = 0b100,
    GPIO_FUNCTION_ALT1 = 0b101,
    GPIO_FUNCTION_ALT2 = 0b110,
    GPIO_FUNCTION_ALT3 = 0b111,
    GPIO_FUNCTION_ALT4 = 0b011,
    GPIO_FUNCTION_ALT5 = 0b010,
} gpio_function_t;

typedef enum {
    GPIO_PULL_OFF = 0b00,
    GPIO_PULL_UP = 0b01,
    GPIO_PULL_DOWN = 0b10,
} gpio_pull_t;

uint32 gpio_call(uint32 pin_number, uint32 value, uint32 base, uint32 field_size, uint32 field_max);

uint32 gpio_set     (uint32 pin_number, uint32 value);
uint32 gpio_clear   (uint32 pin_number, uint32 value);
uint32 gpio_pull    (uint32 pin_number, uint32 value);
uint32 gpio_function(uint32 pin_number, uint32 value);

void gpio_useAsAlt5(uint32 pin_number);