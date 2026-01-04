#ifndef STM32F1XX_SYSTICK_H
#define STM32F1XX_SYSTICK_H

#include <stdint.h>

struct systick {
  volatile uint32_t CSR, RVR, CVR, CALIB;
};

#define SYSTICK ((struct systick *) (0xe000e010))

static inline int8_t systick_init(uint32_t ticks) {
  // SYST_RVR holds a 24 bit value
  if(ticks > 0xffffff) return -1;
  SYSTICK->RVR = ticks;

  SYSTICK->CVR = 0;

  SYSTICK->CSR = 0b111;

  return 0;
};

void SysTick_Handler(void);

void delay_ticks(uint32_t ticks);

#endif
