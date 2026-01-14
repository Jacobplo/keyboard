#ifndef STM32F1XX_SYSTICK_H
#define STM32F1XX_SYSTICK_H

#include <stdint.h>

#include "stm32f1xx.h"

struct systick {
  volatile uint32_t CSR, RVR, CVR, CALIB;
};

#define SYST ((struct systick *) (0xe000e010))


#define SYST_CVR_RELOAD 0u


static inline int8_t systick_init(uint32_t ticks) {
  // SYST_RVR holds a 24 bit value
  if(ticks > 0xffffff) return -1;
  SYST->RVR = ticks;

  SYST->CVR = SYST_CVR_RELOAD;

  SYST->CSR |= SYST_CSR_ENABLE | SYST_CSR_TICKINT | SYST_CSR_CLKSOURCE;

  return 0;
};

void SysTick_Handler(void);

void delay_ticks(uint32_t ticks);

#endif
