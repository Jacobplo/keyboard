#include <stdint.h>

#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "stm32f103bluepillplus.h"

#include "gpio.h"
#include "systick.h"
#include "usb.h"
#include "tusb.h"



int main(void) {
  systick_init();
  gpio_init();
  usb_init();

  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_FULL
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);


  uint16_t led = PIN_LED;
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);

  uint16_t btn = PIN_BTN;
  gpio_set(btn, GPIO_INPUT, GPIO_IN_PULL_UP_DOWN);



  while(1) {
    tud_task();
  };
  return 0;
}
