#ifndef STM32F1XX_GPIO_H

#define STM32F1XX_GPIO_H

#include <stdint.h>

struct gpio {
  volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};

// GPIO pin mode
enum {
  GPIO_INPUT = 0,
  GPIO_OUTPUT_10MHZ,
  GPIO_OUTPUT_2MHZ,
  GPIO_OUTPUT_50MHZ
};

// GPIO pin configuration
enum {
  GPIO_IN_ANALOG = 0,
  GPIO_IN_FLOATING,
  GPIO_IN_PULL_UP_DOWN
};
enum {
  GPIO_OUT_PUSH_PULL = 0,
  GPIO_OUT_OPEN_DRAIN,
  GPIO_OUT_ALT_PUSH_PULL,
  GPIO_OUT_ALT_OPEN_DRAIN
};

// GPIO State
enum {
  GPIO_OFF = 0,
  GPIO_ON
};

#define GPIO(port) ((struct gpio *) (0x40010800 + (0x400 * ((port) - 'A'))))
#define PIN(port, num) (((port) << 8) | (num)) // uint16_t
#define PIN_PORT(pin) ((pin) >> 8)
#define PIN_NUM(pin) (uint8_t)((pin) & 255)


static inline void gpio_set(uint16_t pin, uint8_t mode, uint8_t configuration) {
  struct gpio *gpio = GPIO(PIN_PORT(pin));
  uint8_t pin_num = PIN_NUM(pin);

  if(pin_num < 8) {
    gpio->CRL &= ~(15U << (pin_num * 4));
    gpio->CRL |= (uint32_t)(((configuration << 2) | mode) << (pin_num * 4));
  }
  else {
    gpio->CRH &= ~(15U << ((pin_num - 8) * 4));
    gpio->CRH |= (uint32_t)(((configuration << 2) | mode) << ((pin_num - 8) * 4));
  }
}

static inline void gpio_write(uint16_t pin, uint8_t state) {
  struct gpio *gpio = GPIO(PIN_PORT(pin));
  uint8_t pin_num = PIN_NUM(pin);

  gpio->BSRR = (uint32_t)(1 << pin_num) << (state ? 0 : 16);
}


#endif
