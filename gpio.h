#ifndef BLUE_PILL_GPIO_H

#define BLUE_PILL_GPIO_H

#include <stdint.h>

struct gpio {
  volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};

// GPIO pin mode
enum {
  GPIO_INPUT,
  GPIO_OUTPUT_10MHZ,
  GPIO_OUTPUT_2MHZ,
  GPIO_OUTPUT_50MHZ
};

// GPIO pin configuration
enum {
  GPIO_IN_ANALOG,
  GPIO_IN_FLOATING,
  GPIO_IN_PULL_UP_DOWN
};
enum {
  GPIO_OUT_PUSH_PULL,
  GPIO_OUT_OPEN_DRAIN,
  GPIO_OUT_ALT_PUSH_PULL,
  GPIO_OUT_ALT_OPEN_DRAIN
};

#define GPIO(port) ((struct gpio *) 0x40010800 + (0x400 * (port - 'A')))
#define PIN(port, num) (((port) << 8) | num) // uint16_t
#define PIN_PORT(pin) ((pin) >> 8)
#define PIN_NUM(pin) ((pin) & 255)


static inline void gpio_set(uint16_t pin, uint8_t mode, uint8_t configuration) {
  gpio *gpio = GPIO(PIN_PORT(pin));
  uint8_t pin_num = PIN_NUM(pin);

  if(pin_num < 8) {
    gpio->CRL &= ~(7 << (pin_num * 4));
    gpio->CRL |= (((configuration << 2) | mode) << (pin_num * 4));
  }
  else {
    gpio->CRH &= ~(7 << (pin * 4));
    gpio->CRH |= (((configuration << 2) | mode) << (pin_num * 4));
  }
}


#endif
