#ifndef STM32F1XX_USB_H
#define STM32F1XX_USB_H

#include "rcc.h"
#include "gpio.h"

static inline void usb_init(void) {
  // Enable USB clock. Clock must be initialized properly first.
  RCC->APB1ENR |= (1 << 23);
}

#endif
