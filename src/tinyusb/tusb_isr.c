#include "tusb.h"


void USB_HP_CAN1_TX_IRQHandler(void) {
  return;
}

void USB_LP_CAN1_RX0_IRQHandler(void) {
  tusb_int_handler(BOARD_TUD_RHPORT, true);
  return;
}

void USBWakeUp_IRQHandler(void) {
  return;
}
