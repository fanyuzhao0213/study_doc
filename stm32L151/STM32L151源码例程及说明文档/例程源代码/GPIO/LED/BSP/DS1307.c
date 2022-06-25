#include "DS1307.h"


// PRIVATE FUNCTIONS

// Aquire data from the RTC chip in BCD format
// refresh the buffer

DSRTC_HandleTypeDef DSRTC;

void DS1307_read(void)
{
  I2C_ReadBulk(DS1307_I2C_ADDR , DS1307_SEC , 7 , DSRTC.BCD);
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307_save(void)
{
  I2C_WriteBulk(DS1307_I2C_ADDR , DS1307_SEC , 7 , DSRTC.BCD);
}

void DS1307_stop(void)
{
    DSRTC.BCD[DS1307_SEC]=DSRTC.BCD[DS1307_SEC] | DS1307_CLOCKHALT;
    I2C_Write(DS1307_I2C_ADDR , DS1307_SEC , DSRTC.BCD[DS1307_SEC]);
}

void DS1307_start(void)
{
    DSRTC.BCD[DS1307_SEC]=DSRTC.BCD[DS1307_SEC] & (~DS1307_CLOCKHALT);
	I2C_Write(DS1307_I2C_ADDR , DS1307_SEC , DSRTC.BCD[DS1307_SEC]);
}

void DS1307_SetOutput(HAL_DS_SqOutTypeDef SqOut)  
{
	I2C_Write(DS1307_I2C_ADDR , DS1307_CONTROL , SqOut);
}

uint8_t DS1307_GetOutput(void)  
{
	return I2C_Read(DS1307_I2C_ADDR , DS1307_CONTROL);
}

uint8_t Bcd2Dec(uint8_t bcd)
{
	return ( (bcd>>4)*10 + bcd&0x0f );
}

uint8_t Dec2Bcd(uint8_t dec)
{
	return ( ((uint8_t)(dec/10))<<4 | (dec%10) );
}

void updataBCD(void)
{
	DSRTC.BCD[DS1307_SEC]= Dec2Bcd(DSRTC.DEC.tm_sec)&0x7F;// 启动时间
	DSRTC.BCD[DS1307_MIN]= Dec2Bcd(DSRTC.DEC.tm_min);
	DSRTC.BCD[DS1307_HR]= Dec2Bcd(DSRTC.DEC.tm_hour)&0x3F;// 24小时
	DSRTC.BCD[DS1307_DATE]= Dec2Bcd(DSRTC.DEC.tm_mday);
	DSRTC.BCD[DS1307_MTH]= Dec2Bcd(DSRTC.DEC.tm_mon+1);	// BCD:月 1~12
	DSRTC.BCD[DS1307_YR]= Dec2Bcd(DSRTC.DEC.tm_year-100);	// 从2000年算起
	if(DSRTC.DEC.tm_wday==0) DSRTC.BCD[DS1307_DAY]=7;	// 星期 1~7 BCD:7=星期日
	else DSRTC.BCD[DS1307_DAY]=DSRTC.DEC.tm_wday;	
}


void updataDEC(void)
{
	DSRTC.DEC.tm_sec  =Bcd2Dec(DSRTC.BCD[DS1307_SEC]&0x7F);	// 秒 0~59
	DSRTC.DEC.tm_min  =Bcd2Dec(DSRTC.BCD[DS1307_MIN]);	// 分 0~59
	DSRTC.DEC.tm_hour =Bcd2Dec(DSRTC.BCD[DS1307_HR]&0x3F);	// 时 0~23
	DSRTC.DEC.tm_mday =Bcd2Dec(DSRTC.BCD[DS1307_DATE]);	// 日 1~31
	DSRTC.DEC.tm_mon  =Bcd2Dec(DSRTC.BCD[DS1307_MTH])-1;	// 月 0~11
	DSRTC.DEC.tm_year =Bcd2Dec(DSRTC.BCD[DS1307_YR])+100;	// 年 从1900算起
	if (DSRTC.BCD[DS1307_DAY] == 7)	DSRTC.DEC.tm_wday=0;	// 星期 0~6 DEC:0=星期日
	else DSRTC.DEC.tm_wday=DSRTC.BCD[DS1307_DAY];
	DSRTC.DEC.tm_isdst=0;	// 夏令时
}

