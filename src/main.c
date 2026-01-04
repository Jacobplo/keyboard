#include <stdint.h>

#include "gpio.h"
#include "rcc.h"
#include "systick.h"


int main(void) {
  // Initialize systick with 1 ms = 1 tick
  systick_init(9000000 / 1000);

  gpio_init();

  
  uint16_t led = PIN('B', 2);
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);


  while(1) {
    gpio_write(led, GPIO_ON);
    delay_ticks(100);
    gpio_write(led, GPIO_OFF);
    delay_ticks(100);
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
  _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};
