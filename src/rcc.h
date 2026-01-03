#ifndef BLUE_PILL_RCC_H
#define BLUE_PILL_RCC_H

#include <stdint.h>

struct rcc {
  volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR, BDCR, CSR;
};

enum {
  RCC_CR,
  RCC_CFGR,
  RCC_CIR,
  RCC_APB2RSTR,
  RCC_APB1RSTR,
  RCC_AHBENR,
  RCC_APB2ENR,
  RCC_APB1ENR,
  RCC_BDCR,
  RCC_CSR
};

#define RCC ((struct rcc *) 0x40021000)

#endif
