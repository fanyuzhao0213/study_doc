/*
  DS1307.h - library for DS1307 rtc
*/

#ifndef DS1307_h
#define DS1307_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "i2c.h"
#include "BSP_drive.h"



#define DS1307_I2C_ADDR		0xD0

#define DS1307_BASE_YR 		2000

#define DS1307_SEC		0x00
#define DS1307_MIN		0x01
#define DS1307_HR 		0x02
#define DS1307_DAY 		0x03
#define DS1307_DATE 	0x04
#define DS1307_MTH 		0x05
#define DS1307_YR 		0x06
#define DS1307_CONTROL 	0x07

typedef enum
{
	DS1307_SQW1HZ  		=0x10,
	DS1307_SQW4KHZ 		=0x11,
	DS1307_SQW8KHZ 		=0x12,
	DS1307_SQW32KHZ 	=0x13,
	DS1307_SQWOUT_HIGH 	=0x80,
	DS1307_SQWOUT_LOW 	=0x00,
}HAL_DS_SqOutTypeDef;

 // Define register bit masks
#define DS1307_CLOCKHALT 0x80

#define DS1307_LO_BCD  0X0F
#define DS1307_HI_BCD  0XF0

#define DS1307_HI_SEC  0X70
#define DS1307_HI_MIN  0X70
#define DS1307_HI_HR   0x30
#define DS1307_LO_DAY  0x07
#define DS1307_HI_DATE 0X30
#define DS1307_HI_MTH  0X10
#define DS1307_HI_YR   0xF0

typedef struct
{
	uint8_t Sec;
	uint8_t Min;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
}RTC_Leaguer_HandleTypeDef;


// library interface description
typedef union 
{
	uint8_t Array[7];
	RTC_Leaguer_HandleTypeDef  Leag;
}RTC_UNL_HandleTypeDef;


typedef struct
{
	I2C_HandleTypeDef 	    *DS_i2c;		/* i2c 接口*/
	uint8_t 				BCD[7];			/* DS1307 BCD格式日期*/
	struct tm 				DEC;			/* time.h 格式日期*/
}DSRTC_HandleTypeDef;

//**************************************************************************************
//变量

extern DSRTC_HandleTypeDef DSRTC;

void DS1307_read(void);
void DS1307_save(void);
void DS1307_stop(void);
void DS1307_start(void);
void DS1307_SetOutput(HAL_DS_SqOutTypeDef SqOut);
uint8_t DS1307_GetOutput(void);
uint8_t Bcd2Dec(uint8_t bcd);
uint8_t Dec2Bcd(uint8_t dec);
void updataBCD(void);
void updataDEC(void);


#endif
