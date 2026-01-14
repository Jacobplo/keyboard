#ifndef STM32F1XX_FLASH_H
#define STM32F1XX_FLASH_H

#include <stdint.h>

struct flash {
  volatile uint32_t ACR; 
};

#define FLASH ((struct flash *) (0x40022000))

#endif
