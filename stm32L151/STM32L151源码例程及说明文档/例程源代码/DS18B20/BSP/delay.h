#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#ifdef __STM32F0xx_HAL_H
#include "stm32f0xx_hal.h"
#endif

#ifdef __STM32F1xx_HAL_H
#include "stm32f1xx_hal.h"
#endif

//#ifdef __STM32L1xx_HAL_H
#include "stm32l1xx_hal.h"
//#endif

#ifdef _CMSIS_OS_H
#include "cmsis_os.h"
#endif

#ifdef  configTICK_RATE_HZ

#endif

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos
#endif


void delay_init(void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */





























