/*
SHT2x - Library for the SHT2x series of Sensirion sensors.

Copyright 2015 Waiman

Supported devices:
SHT20*
SHT21
SHT25
*/

#include "SHT2x.h"
#include "BSP_drive.h"


SHT_HandleTypeDef hsht2x;


/****************************************************************************
* 名    称： void SHT2x_CMD(uint8_t cmd)
* 功    能： 向SHT发送命令
* 入口参数： cmd 指令
* 出口参数：无
* 说    明：
****************************************************************************/
uint8_t SHT2x_CMD(uint8_t cmd)
{
	return HAL_I2C_Master_Transmit(hsht2x.SHT_i2c,SHT2xADDR,&cmd,1,50);
}


/****************************************************************************
* 名    称：void BSP_SHT2X_Init(void)
* 功    能：初始化 SHT
* 入口参数：
* 出口参数：无
* 说    明：指定SHT的I2C接口，复位芯片
****************************************************************************/
void BSP_SHT2X_Init(I2C_HandleTypeDef *I2C_interface)
{
	hsht2x.SHT_i2c = I2C_interface;
	if(SHT2x_CMD(SOFT_RESET) == 0) hsht2x.Status = HAL_SHT2x_STATE_READY;
	HAL_Delay(15);
}


/****************************************************************************
* 名    称：uint16_t readMeasure(void)
* 功    能：读取测量得出的数据
* 入口参数：无
* 出口参数：返回读取的数据 uint16_t
* 说    明：
****************************************************************************/
uint16_t readMeasure(void)
{
	uint8_t data[3]= {0};
	//	uint8_t stat;
	uint16_t value = 0;
	HAL_I2C_Master_Receive(hsht2x.SHT_i2c,SHT2xADDR,data,3,50);
	value = ((uint16_t)(data[0])<<8 | data[1]);
	//value &= ~0x0003;
	
	//	stat = data[1] &0x03;
	//	printf("stat:%x\r\n",stat );
	//	if (stat==0){	// 0 = Temperature
	//	}else{	// 0x02 = Humidity
	//	}
	return value;
}


/****************************************************************************
* 名    称：float readT(void)
* 功    能：读取温度
* 入口参数：无
* 出口参数：转换成摄氏度 float
* 说    明：先启动温度测量，然后再读取
****************************************************************************/
float readT(void)
{
	uint16_t data =readMeasure();
	data &= ~0x0003; // 低两位是数据类型标识，
	//	return (-46.85+175.72/65536.00*(float)data);
	return (-46.85+ 0.0026812744140625 *(float)data);
}


/****************************************************************************
* 名    称：float readRH(void)
* 功    能：读取湿度
* 入口参数：无
* 出口参数：转换成相对湿度百分比 float
* 说    明：先启动湿度测量，然后再读取
****************************************************************************/
float readRH(void)
{
	uint16_t data =readMeasure();
	data &= ~0x0003; // 低两位是数据类型标识，
	//return (-6.0+125.0/65536.00*(float)data);
	return (-6.0+ 0.0019073486328125 *(float)data);
}


/****************************************************************************
* 名    称：void setHeater(uint8_t on)
* 功    能：打开加热功能
* 入口参数：on=0 关闭，相反则打开加热功能
* 出口参数：无
* 说    明：
****************************************************************************/
void setHeater(uint8_t on)
{
	uint8_t userRegister;
	userRegister=I2C_Read(SHT2xADDR,USER_REG_R);
	if (on!=0)
	{
		userRegister=(userRegister&~SHT2x_HEATER_MASK) | SHT2x_HEATER_ON;
	}
	else
	{
		userRegister=(userRegister&~SHT2x_HEATER_MASK) | SHT2x_HEATER_OFF;
	}
	I2C_Write(SHT2xADDR,USER_REG_W,userRegister);
}


/****************************************************************************
* 名    称：uint8_t getResolution(void)
* 功    能：获取SHT的测量分辨率
* 入口参数：无
* 出口参数：测量分辨率
* 说    明：
****************************************************************************/
uint8_t getResolution(void)
{
	uint8_t userRegister;
	userRegister=I2C_Read(SHT2xADDR,USER_REG_R);
	hsht2x.Resolutions =userRegister & SHT2x_RES_MASK ;
	return hsht2x.Resolutions;
}


/****************************************************************************
* 名    称：void setResolution(SHT2xResolution resolution)
* 功    能：设置SHT的测量分辨率
* 入口参数：测量分辨率，请参考枚举: SHT2xResolution
* 出口参数：无
* 说    明：
****************************************************************************/
void setResolution(SHT2xResolution resolution)
{
	uint8_t userRegister;
	userRegister=I2C_Read(SHT2xADDR,USER_REG_R);
	hsht2x.Resolutions=(userRegister&~SHT2x_RES_MASK) | resolution;
	I2C_Write(SHT2xADDR,USER_REG_W,hsht2x.Resolutions);
}


/****************************************************************************
* 名    称：uint8_t autoMeasure(float * th)
* 功    能：读取测量得出的数据，并根据数据的类型进行转化，再自动启动下一个类型的测量
* 入口参数：th 返回已经转换的数据
* 出口参数：0=温度，0x02=湿度
* 说    明：先启动一次测量（无论温度还是湿度），然后才执行这个函数
****************************************************************************/
uint8_t autoMeasure(float * th)
{
	if (hsht2x.Status < 0x0f){	// 器件不存在或未初始化，或者未启动测量
		*th =0.0;
		return hsht2x.Status;
	}
	
	uint16_t data = readMeasure();
	uint8_t redata = (uint8_t)data & 0x03; // 低两位是数据类型标识，
	data &= ~0x0003;
	
	if (redata==0)  	// 0 = Temperature
	{
		*th = (-46.85+ 0.0026812744140625 *(float)data);
		redata = HAL_SHT2x_STATE_Temper;
		measureRH();	// start Humidity measure
	}
	else  	// 0x02 = Humidity
	{
		*th = (-6.0+ 0.0019073486328125 *(float)data);
		redata = HAL_SHT2x_STATE_Humi;
		measureT();	// start Temperature measure		
	}
	return redata;
}

__INLINE void measureRH (void)
{
	SHT2x_CMD(MEASUREMENT_RH_POLL);hsht2x.Status = HAL_SHT2x_STATE_Humi;	// start Humidity measure
}
__INLINE void  measureT (void)
{
	SHT2x_CMD(MEASUREMENT_T_POLL);hsht2x.Status = HAL_SHT2x_STATE_Temper;	// start Temperature measure
}
__INLINE void softReset (void)
{
	SHT2x_CMD(SOFT_RESET);			// reset SHT2x
}