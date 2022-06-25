#ifndef __BSP_DRIVE_H
#define __BSP_DRIVE_H

// 最后修改日期 2015 11 16

#define BSP_I2C		1
#define BSP_SPI		1
#define BSP_USART	1

#ifdef __cplusplus
extern "C" {
#endif
	
	
#ifdef __STM32F0xx_HAL_H
#include "stm32f0xx_hal.h"
#endif
	
#ifdef __STM32F1xx_HAL_H
#include "stm32f1xx_hal.h"
#endif
	
#ifdef __STM32L1xx_HAL_H
#include "stm32l1xx_hal.h"
#endif
	
#if defined (BSP_USART)	// USART
#include "usart.h"
#endif
	
#if defined (BSP_I2C)	// I2C
#include "i2c.h"
#endif
	
#if defined (BSP_SPI)	// SPI
#include "spi.h"
#endif
	
	// 兼容部分代码
#ifdef __INT8_T_TYPE__
	typedef __UINT8_T_TYPE__ 	u8;
	typedef __UINT16_T_TYPE__ 	u16;
	typedef __UINT32_T_TYPE__  u32;
#else
    /* exact-width unsigned integer types */
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;
#endif
#if defined (BSP_I2C)	// I2C
//#include "i2c.h"
	uint8_t I2C_Write(uint8_t slaveAddress , uint8_t regAddress , uint8_t regData);
	uint8_t I2C_WriteBulk(uint8_t slaveAddress , uint8_t regAddress , uint8_t size , uint8_t *data);
	uint8_t I2C_Read(uint8_t slaveAddress , uint8_t regAddress);
	uint8_t I2C_ReadBulk(uint8_t slaveAddress , uint8_t regAddress , uint8_t size , uint8_t *data);
	void SeachI2cDev(void);
#endif
	
	
#if defined (BSP_SPI)	// SPI
//#include "spi.h"
	uint8_t BSP_SPI_Byte(SPI_HandleTypeDef *hspi,uint8_t txData);
	HAL_StatusTypeDef BSP_SPI_Bulk(SPI_HandleTypeDef *hspi, uint8_t *txData, uint8_t *rxData, uint8_t size);
#endif
	
	
	
	
	
#ifdef __cplusplus
}
#endif

#endif /* __BSP_DRIVE_H */
