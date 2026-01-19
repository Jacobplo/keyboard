#include <stdint.h>

#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "stm32f103bluepillplus.h"

#include "gpio.h"
#include "afio.h"
#include "systick.h"
#include "usb.h"

#include "tusb.h"
#include "usb_descriptors.h"
#include "hid.h"


#define DEBUG 0


static uint16_t led = PIN_LED;
static uint16_t btn = PIN_BTN;

#define NUM_ROWS 2
#define NUM_COLS 4

static uint16_t row[NUM_ROWS] = { PIN('B', 11), PIN('B', 10) };
static uint16_t col[NUM_COLS] = { PIN('B', 3), PIN('B', 4), PIN('B', 5), PIN('B', 6) };

static uint8_t keys[NUM_ROWS][NUM_COLS] = {
  { HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4 },
  { HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D }
};

enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED     = 1000,
  BLINK_SUSPENDED   = 2500,
};
static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

void select_row(uint8_t row_num) {
  gpio_write(row[row_num], GPIO_LOW);
  for(int8_t i = 0; i < NUM_ROWS; i++) {
    if(i == row_num) continue;
    
    gpio_write(row[i], GPIO_HIGH);
  }
}


int main(void) {
  systick_init();
  afio_init();
  afio_disable_jtag();
  gpio_init();
  usb_init();

  // On-board peripheral initialization
  gpio_set(led, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL); 
  gpio_set(btn, GPIO_INPUT, GPIO_IN_PULL_UP_DOWN);


  // Keyboard Matrix Initialization
  for(int8_t i = 0; i < 2; i++) {
    gpio_set(row[i], GPIO_OUTPUT_50MHZ, GPIO_OUT_OPEN_DRAIN);
    gpio_write(row[i], GPIO_HIGH);
  }
  for(int8_t i = 0; i < 4; i++) {
    gpio_set(col[i], GPIO_INPUT, GPIO_IN_PULL_UP_DOWN);
  }

 // Tusb initialization.
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_FULL
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);
 


  // Main loop
  while(1) {
    tud_task();
    #if DEBUG
    led_blinking_task();
    #endif
    
    hid_task();
  };
  return 0;
}

#if DEBUG
// Invoked when device is mounted
void tud_mount_cb(void) {
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}
#endif

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void) {
  // Poll every 10ms
  const uint32_t  interval_ms = 10;
  static uint32_t start_ms    = 0;

  if (get_ticks() - start_ms < interval_ms) {
    return; // not enough time
  }
  start_ms += interval_ms;

  uint32_t const btn2 = gpio_read(btn);

  if (tud_suspended() && btn2) {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  } 
  else {
    // keyboard interface
    if (tud_hid_n_ready(ITF_NUM_KEYBOARD)) {
      static bool key_pressed = false;
      static bool nothing_pressed = true;
      uint8_t const report_id = 0;  // only one hid interface, so can be left as 0
      uint8_t const modifier = 0;   // bitmask of modifier keys
      uint8_t keycode[6] = {0};     // 6 keys can be pressed at once
      
      uint8_t key_index = 0;

      for(int8_t i = 0; i < NUM_ROWS; i++) {
        select_row(i);
        delay_ticks(1);
        for(int8_t j = 0; j < NUM_COLS; j++) {
          if(key_index == 6) {
            break;
          }
          if(!gpio_read(col[j])) {
            keycode[key_index] = keys[i][j];
            key_index++; 
            key_pressed = true;
            nothing_pressed = false;
          }
        }
      }
      
      if(key_pressed) { 
        tud_hid_n_keyboard_report(ITF_NUM_KEYBOARD, report_id, modifier, keycode);
        key_pressed = false;
        nothing_pressed = true;
      }
      else if(nothing_pressed) {
        tud_hid_n_keyboard_report(ITF_NUM_KEYBOARD, report_id, 0, NULL);
        nothing_pressed = false;
      }
    }
  }
}

// Invoked when received SET_PROTOCOL request
// protocol is either HID_PROTOCOL_BOOT (0) or HID_PROTOCOL_REPORT (1)
void tud_hid_set_protocol_cb(uint8_t instance, uint8_t protocol) {
  (void)instance;
  (void)protocol;

  // nothing to do since we use the same compatible boot report for both Boot and Report mode.
  // TODO set a indicator for user
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len) {
  (void)instance;
  (void)report;
  (void)len;

  // nothing to do
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(
    uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(
    uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
  (void)report_id;

  // keyboard interface
  if (instance == ITF_NUM_KEYBOARD) {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_type == HID_REPORT_TYPE_OUTPUT) {
      // bufsize should be (at least) 1
      if (bufsize < 1) {
        return;
      }

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        gpio_write(led, true);
      } else {
        // Caplocks Off: back to normal blink
        gpio_write(led, false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  static uint32_t start_ms  = 0;
  static bool     led_state = false;

  // blink is disabled
  if (!blink_interval_ms) {
    return;
  }

  // Blink every interval ms
  if (get_ticks() - start_ms < blink_interval_ms) {
    return; // not enough time
  }
  start_ms += blink_interval_ms;

  gpio_write(led, led_state);
  led_state = 1 - led_state; // toggle
}
