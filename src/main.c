#include <stdint.h>

#include "gpio.h"
#include "systick.h"
#include "clock.h"
#include "usb.h"


int main(void) {
  clock_init();
  usb_init();

  // Initialize systick with 1 ms = 1 tick
  systick_init(SYSCLK_FREQ / 1000);

  gpio_init();


  uint16_t led = PIN('B', 2);
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);

  uint16_t btn = PIN('A', 0);
  gpio_set(btn, GPIO_INPUT, GPIO_IN_PULL_UP_DOWN);

  while(1) {
    if(gpio_read(btn)) {
      gpio_write(led, GPIO_LOW); 
      delay_ticks(100);
      gpio_write(led, GPIO_HIGH);
      delay_ticks(100);
    }
    else {
      gpio_write(led, GPIO_LOW);
    }
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
  while(1) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);

__attribute__((section(".isr_vector"))) void (*const tab[16 + 91])(void) = {
  _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};
