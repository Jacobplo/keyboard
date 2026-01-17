#ifndef STM32F1XX_SYSTICK_H
#define STM32F1XX_SYSTICK_H

#include <stdint.h>

#include "stm32f103xb.h"

void SysTick_Handler(void);

void delay_ticks(uint32_t ticks);

#endif
