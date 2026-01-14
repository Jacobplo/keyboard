/*
* This file contains only definitions used in this project, and should not be used
* as an overview for the MCU.
*/

#ifndef STM32F1XX_H
#define STM32F1XX_H

#define BIT(pos) (1u << (pos))

/*
* RCC
*/
#define RCC_CR_HSION              BIT(0)
#define RCC_CR_HSIRDY             BIT(1)
#define RCC_CR_HSEON              BIT(16)
#define RCC_CR_HSERDY             BIT(17)
#define RCC_CR_HSEBYP             BIT(18)
#define RCC_CR_PLLON              BIT(24)
#define RCC_CR_PLLRDY             BIT(25)

#define RCC_CFGR_PLLSRC           BIT(16)
#define RCC_CFGR_PLLMUL_16        (0b1111u << 18)
#define RCC_CFGR_PLLMUL_12        (0b1010u << 18)
#define RCC_CFGR_PLLMUL_9         (0b0111u << 18)
#define RCC_CFGR_USBPRE           BIT(22)
#define RCC_CFGR_PPRE1_16         (0b111u << 8)
#define RCC_CFGR_PPRE1_2          (0b100u << 8)
#define RCC_CFGR_SW_HSI           (0b00u << 0)
#define RCC_CFGR_SW_HSE           (0b01u << 0)
#define RCC_CFGR_SW_PLL           (0b10u << 0)
#define RCC_CFGR_SW_NONE          (0b11u << 0)

#define RCC_APB2ENR_IOPAEN        BIT(2)
#define RCC_APB2ENR_IOPBEN        BIT(3)
#define RCC_APB2ENR_IOPCEN        BIT(4)
#define RCC_APB2ENR_IOPDEN        BIT(5)
#define RCC_APB2ENR_IOPEEN        BIT(6)
#define RCC_APB2ENR_USART1EN      BIT(14)

#define RCC_APB1ENR_USART2EN      BIT(17)
#define RCC_APB1ENR_USART3EN      BIT(18)
#define RCC_APB1ENR_UART4EN       BIT(19)
#define RCC_APB1ENR_UART5EN       BIT(20)
#define RCC_APB1ENR_USBEN         BIT(23)


/*
* SysTick
*/
#define SYST_CSR_ENABLE           BIT(0)
#define SYST_CSR_TICKINT          BIT(1)
#define SYST_CSR_CLKSOURCE        BIT(2)


/*
* Flash
*/
#define FLASH_ACR_LATENCY         (0b111u << 0)
#define FLASH_ACR_LATENCY_2       (0b010u << 0)
#define FLASH_ACR_PRFTBE          BIT(4)

#endif
