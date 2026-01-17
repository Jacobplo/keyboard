#include <stdint.h>

#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "stm32f103bluepillplus.h"

#include "gpio.h"
#include "systick.h"
#include "usb.h"



int main(void) {
  systick_init();
  gpio_init();
  usb_init();

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
