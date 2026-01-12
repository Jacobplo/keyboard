#ifndef STM32F1XX_USB_H
#define STM32F1XX_USB_H

#include "rcc.h"
#include "gpio.h"

#define USB_DN PIN('A', 11)
#define USB_DP PIN('A', 12)

static inline void usb_init() {
  // Enable USB clock. Clock must be initialized properly first.
  RCC->APB1ENR |= (1 << 23);
}

#endif
