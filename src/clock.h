#ifndef STM32F1XX_CLOCK_H
#define STM32F1XX_CLOCK_H

#include "stm32f1xx.h"
#include "rcc.h"
#include "flash.h"

#define SYSCLK_FREQ 72000000

static inline void clock_init(void) {
  // Set 8 MHz HSE clock on.
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR & RCC_CR_HSERDY)) (void) 0;

  // Reconfigure flash wait state for 72 MHz
  FLASH->ACR |= FLASH_ACR_PRFTBE;
  FLASH->ACR &= ~(FLASH_ACR_LATENCY);
  FLASH->ACR |= FLASH_ACR_LATENCY_2;

  // Set PLL clock source to HSE (8 MHz)
  RCC->CFGR = (RCC_CFGR_PLLSRC);

  // Multiply PLL by 9 (72 MHz)
  RCC->CFGR &= ~(RCC_CFGR_PLLMUL_16);
  RCC->CFGR |= (RCC_CFGR_PLLMUL_9);

  // USB prescale PLL / 1.5 (48 MHz)
  RCC->CFGR &= ~(RCC_CFGR_USBPRE);

  // APB low speed prescaler of HCLK / 2 (36 MHz) [Cannot exceed 36 MHz]
  RCC->CFGR &= ~(RCC_CFGR_PPRE1_16);
  RCC->CFGR |= (RCC_CFGR_PPRE1_2);
 
  // Enable PLL clock
  RCC->CR |= RCC_CR_PLLON;
  while(!(RCC->CR & RCC_CR_PLLRDY)) (void) 0;

  // Set SYSCLK to use PLL
  RCC->CFGR &= ~(RCC_CFGR_SW_NONE);
  RCC->CFGR |= (RCC_CFGR_SW_PLL);
}

#endif

