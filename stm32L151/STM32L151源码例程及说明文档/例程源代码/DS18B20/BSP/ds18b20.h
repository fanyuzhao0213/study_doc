////////////////////////////////////////////////////////////////////////////////
//DS18B20驱动代码
//修改日期:2016/4/10
//版本：V1.1
//版权所有，盗版必究。
////////////////////////////////////////////////////////////////////////////////

#ifndef __DS18B20_H
#define __DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STM32F0xx_HAL_H
#include "stm32f0xx_hal.h"
#endif

#ifdef __STM32F1xx_HAL_H
#include "stm32f1xx_hal.h"
#endif

//#ifdef __STM32L1xx_HAL_H
#include "stm32l1xx_hal.h"
//#endif


#include "delay.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


//ROM操作指令
#define  READ_ROM            0x33     //读ROM
#define  MATCH_ROM           0x55     //匹配ROM
#define  SKIP_ROM            0xcc     //跳过ROM
#define  SEARCH_ROM          0xf0     //搜索ROM
#define  ALARM_SEARCH        0xec     //告警搜索


//存储器操作指令
#define  WRITE_SCRATCHPAD    0x4e     //写暂存存储器
#define  READ_SCRATCHPAD     0xbe     //读暂存存储器
#define  COPY_SCRATCHPAD     0x48     //复制暂存存储器
#define  CONVERT_TEMPERATURE 0x44     //温度变换
#define  RECALL_EPROM        0xb8     //重新调出
#define  READ_POWER_SUPPLY   0xb4     //读电源


#define DS18B20_MAX_LEN		2

//#define DS18B20_0 	GPIO_ResetBits(DS18B20_PORT, DS18B20_PIN)
//#define DS18B20_1   	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN)

#define DS18B20_0       HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin,GPIO_PIN_RESET)
#define DS18B20_1       HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin,GPIO_PIN_SET)
#define DS18B20_STATE   HAL_GPIO_ReadPin(DQ_GPIO_Port, DQ_Pin)

typedef struct
{
	uint8_t device;
	uint8_t romAddr[8];
	float temp;
}TDs18TypeDef;


extern TDs18TypeDef  tDs18b[DS18B20_MAX_LEN];
extern uint8_t Ds18b_Len;


extern uint8_t DS18B20_Init(void);			//初始化DS18B20
extern float DS18B20_Get_Temp(uint8_t device);	//获取温度
extern void DS18B20_Start(uint8_t device);		//开始温度转换
extern void DS18B20_Write_bit(uint8_t dat) ;
extern void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
extern uint8_t DS18B20_Read_Byte(void);		//读出一个字节
extern uint8_t DS18B20_Read_Bit(void);		//读出一个位
extern uint8_t DS18B20_Rst(void);			//复位DS18B20
extern uint8_t DS18B20_search(uint8_t *newAddr);
extern void DS18B20_reset_search(void);
extern void DS18B20_skip(void);
extern void DS18B20_Select( uint8_t rom[8]);
extern void delay_us(uint32_t dly);
#ifdef _CMSIS_OS_H
extern void DS18Read_Task(void const * argument);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */


