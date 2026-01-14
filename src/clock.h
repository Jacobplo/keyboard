#ifndef STM32F1XX_CLOCK_H
#define STM32F1XX_CLOCK_H

#include "stm32f1xx.h"
#include "rcc.h"

#define SYSCLK_FREQ 48000000

static inline void clock_init(void) {
  // Set 8 MHz HSI clock on
  RCC->CR |= RCC_CR_HSION;
  while(!(RCC->CR & RCC_CR_HSIRDY)) (void) 0;

  // Set PLL clock source to HSI divided by 2 (4 MHz)
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC);

  // Multiple PLL by 12 (48 MHz)
  RCC->CFGR &= ~(RCC_CFGR_PLLMUL_16);
  RCC->CFGR |= (RCC_CFGR_PLLMUL_12);

  // USB prescale PLL no division (48 MHz)
  RCC->CFGR |= RCC_CFGR_USBPRE;

  // APB low speed prescaler of HCLK / 2 (24 MHz) [Cannot exceed this 36 MHz]
  RCC->CFGR &= ~(RCC_CFGR_PPRE1_16);
  RCC->CFGR |= (RCC_CFGR_PPRE1_2);
 
  // Enable PLL clock
  RCC->CR |= RCC_CR_PLLON;
  // Wait for PLL to become available
  while(!(RCC->CR & RCC_CR_PLLRDY)) (void) 0;

  // Set SYSCLK to use PLL
  RCC->CFGR &= ~(RCC_CFGR_SW_NONE);
  RCC->CFGR |= (RCC_CFGR_SW_PLL);
}

#endif

