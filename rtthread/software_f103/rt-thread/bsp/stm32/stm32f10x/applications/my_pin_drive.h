#ifndef __MY_PIN_DRIVE_H__
#define __MY_PIN_DRIVE_H__

#include <rtthread.h>
#include <stm32f1xx.h>
#include <rtdevice.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ���ű�ţ�ͨ���鿴�豸�����ļ�drv_gpio.cȷ�� */
#ifndef BEEP_PIN_NUM
//    #define BEEP_PIN_NUM            35  /* PB0 */
	#define BEEP_PIN_NUM  			GET_PIN(C, 8)  //PC8 led
#endif
#ifndef KEY0_PIN_NUM
//    #define KEY0_PIN_NUM            55  /* PD8 */
	#define KEY0_PIN_NUM            GET_PIN(E, 4)  //PE4 key0
#endif
#ifndef KEY1_PIN_NUM
    #define KEY1_PIN_NUM            56  /* PD9 */
#endif

extern void MyTestThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick);
#endif