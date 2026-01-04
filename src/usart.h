#ifndef STM32F1XX_USART_H
#define STM32F1XX_USART_H

#include <stdint.h>

#include "rcc.h"

struct usart {
  volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};

#define USART1 ((struct usart *) (0x40013800))
#define USART2 ((struct usart *) (0x40004400))
#define USART3 ((struct usart *) (0x40004800))
#define UART4  ((struct usart *) (0x40004C00))
#define UART5  ((struct usart *) (0x40005000))

static inline void usart_init() {
  // USART1
  RCC->APB2ENR |= (1 << 14);

  // USART2-3, UART4-5
  for(uint8_t i = 17; i < 21; i++) {
    RCC->APB1ENR |= (1 << i);
  }
}

#endif
