#include <stdint.h>

#include "stm32f103xb.h"
#include "stm32f103bluepillplus.h"

#include "gpio.h"
#include "systick.h"
#include "clock.h"
#include "usb.h"


void SystemInit(void);


int main(void) {
  uint16_t led = PIN_LED;
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);

  uint16_t btn = PIN_BTN;
  gpio_set(btn, GPIO_INPUT, GPIO_IN_PULL_UP_DOWN);



  while(1) {
    if(gpio_read(btn)) {
      gpio_toggle(led);
      delay_ticks(100);
    }
    else {
      gpio_write(led, GPIO_LOW);
    }
  };
  return 0;
}

// Order of initialization is important here.
void SystemInit(void) {
  clock_init();
  SysTick_Config(SYSCLK_FREQ / 1000);
  usb_init();
  gpio_init();
}
