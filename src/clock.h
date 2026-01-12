#ifndef STM32F1XX_CLOCK_H
#define STM32F1XX_CLOCK_H

#include "rcc.h"

#define SYSCLK_FREQ 48000000

static inline void clock_init() {
  // Set 8 MHz HSI clock on
  RCC->CR |= (1 << 0);

  // Set PLL clock source to HSI divided by 2 (4 MHz)
  RCC->CFGR &= ~(1u << 16);

  // Multiple PLL by 12 (48 MHz)
  RCC->CFGR &= (0b1111u << 18);
  RCC->CFGR |= (0b1010 << 18);

  // USB prescale PLL no division (48 MHz)
  RCC->CFGR |= (1u << 22);

  // APB low speed prescaler of HCLK / 2 (24 MHz) [Cannot exceed this 36 MHz]
  RCC->CFGR &= ~(0b111u << 8);
  RCC->CFGR |= (0b100 << 8);
 
  // Enable PLL clock
  RCC->CR |= (1 << 24);
  // Wait for PLL to become available
  while(!(RCC->CR & (1 << 25))) (void) 0;

  // Set SYSCLK to use PLL
  RCC->CFGR &= ~(0b11u << 0);
  RCC->CFGR |= (0b10 << 0);
}

#endif

