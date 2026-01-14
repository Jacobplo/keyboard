#ifndef STM32F1XX_USART_H
#define STM32F1XX_USART_H

#include <stdint.h>

#include "stm32f1xx.h"
#include "rcc.h"

struct usart {
  volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};

#define USART1 ((struct usart *) (0x40013800))
#define USART2 ((struct usart *) (0x40004400))
#define USART3 ((struct usart *) (0x40004800))
#define UART4  ((struct usart *) (0x40004C00))
#define UART5  ((struct usart *) (0x40005000))

static inline void usart_init(void) {
  // USART1
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // USART2-3, UART4-5
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_USART3EN
                | RCC_APB1ENR_UART4EN | RCC_APB1ENR_UART5EN;
}

#endif
