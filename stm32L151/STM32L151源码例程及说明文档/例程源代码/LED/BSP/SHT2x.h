/*
SHT2x - Library for the SHT2x series of Sensirion sensors.

Copyright 2015 Waiman

Supported devices:
SHT20*
SHT21
SHT25
*/    

#ifndef SHT2X_H
#define SHT2X_H

#include "i2c.h"
#include "BSP_drive.h"

//  Address
#define SHT2xADDR   0x80

//  CRC
//const uint16_t POLYNOMIAL = 0x131;  //P(x)=x^8+x^5+x^4+1 = 100110001

// sensor commands
#define MEASUREMENT_T_HM     0xE3 // Temperature measurement, hold master
#define MEASUREMENT_RH_HM    0xE5 // Humidity measurement,  hold master
#define MEASUREMENT_T_POLL   0xF3 // Temperature measurement, no hold master, currently not used
#define MEASUREMENT_RH_POLL  0xF5 // Humidity measurement, no hold master, currently not used
#define USER_REG_W           0xE6 // write user register
#define USER_REG_R           0xE7 // read user register
#define SOFT_RESET           0xFE  // soft reset


#define SHT2x_HEATER_ON       0x04 // heater on
#define SHT2x_HEATER_OFF      0x00 // heater off
#define SHT2x_HEATER_MASK     0x04 // Mask for Heater bit(2) in user reg.

// sensor resolutions
typedef enum {
	SHT2x_RES_12_14BIT       = 0x00, // RH=12bit, T=14bit
	SHT2x_RES_8_12BIT        = 0x01, // RH= 8bit, T=12bit
	SHT2x_RES_10_13BIT       = 0x80, // RH=10bit, T=13bit
	SHT2x_RES_11_11BIT       = 0x81, // RH=11bit, T=11bit
	SHT2x_RES_MASK           = 0x81  // Mask for res. bits (7,0) in user reg.
} SHT2xResolution;


typedef enum {
  HAL_SHT2x_STATE_RESET      = 0x00,  /*!< SPI not yet initialized or disabled                */
  HAL_SHT2x_STATE_READY      = 0x01,  /*!< SPI initialized and ready for use                  */
  HAL_SHT2x_STATE_Temper     = 0x10,  /*!< SPI process is ongoing                             */
  HAL_SHT2x_STATE_Humi    	 = 0x20,  /*!< Data Transmission process is ongoing               */
} SHT2x_StateTypeDef;

typedef struct
{
	I2C_HandleTypeDef 	    *SHT_i2c;				/* i2c 接口*/
	uint8_t 				Resolutions;
	SHT2x_StateTypeDef 		Status;
}SHT_HandleTypeDef;

//**************************************************************************************
//变量

extern SHT_HandleTypeDef hsht2x;


uint8_t SHT2x_CMD(uint8_t cmd);
/*
#define measureRH 	SHT2x_CMD(MEASUREMENT_RH_POLL);hsht2x.Status = HAL_SHT2x_STATE_Humi;	// start Humidity measure
#define measureT 	SHT2x_CMD(MEASUREMENT_T_POLL);hsht2x.Status = HAL_SHT2x_STATE_Temper;	// start Temperature measure
#define softReset 	SHT2x_CMD(SOFT_RESET)			// reset SHT2x
*/

__INLINE void measureRH (void);
__INLINE void  measureT (void);
__INLINE void softReset (void);

void BSP_SHT2X_Init(I2C_HandleTypeDef *I2C_interface);

uint16_t readMeasure(void);
float readRH(void);
float readT(void);

void setHeater(uint8_t on);

uint8_t getResolution(void);
void setResolution(SHT2xResolution resolution);

uint8_t autoMeasure(float * th);

#endif
