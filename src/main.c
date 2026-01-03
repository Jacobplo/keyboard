#include <stdint.h>

#include "gpio.h"
#include "rcc.h"


static inline void wait(volatile uint32_t count) {
  for(volatile uint32_t i = 0; i < count; i++) {
    (void) 0;
  }
}

int main(void) {
  uint16_t led = PIN('B', 2);
  RCC->APB2ENR |= (1 << 3);
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);


  for(;;) {
    gpio_write(led, GPIO_ON);
    wait(999999);
    gpio_write(led, GPIO_OFF);
    wait(999999);
  };
  return 0;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
  // memset .bss to zero, and copy .data section to RAM region
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

  main();             // Call main()
  for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);

__attribute__((section(".isr_vector"))) void (*const tab[16 + 91])(void) = {
  _estack, _reset
};
