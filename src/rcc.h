#ifndef STM32F1XX_RCC_H
#define STM32F1XX_RCC_H

#include <stdint.h>

struct rcc {
  volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR, BDCR, CSR;
};

#define RCC ((struct rcc *) 0x40021000)

#endif
