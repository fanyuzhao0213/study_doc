/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(E, 5)


#define OS_THREAD_STRUCT OsThreadInitStruct *		/*�����߳̽ṹ���ض���*/
#define OS_THREAD_FUNC_ID_END  0X05					/*����̸߳���������ʵ����Ŀ�޸�*/


typedef void(*pOsThreadFuncInit)(rt_uint8_t,rt_uint32_t);	/*��������һ������ָ��*/

typedef struct
{
	const rt_uint8_t FuncID;				/*ID*/
	const pOsThreadFuncInit ThreadInit;		/*����*/
	const rt_uint16_t Priority;				/*���ȼ�*/
	const rt_uint32_t Tick;					/*tick��*/
}OsThreadInitStruct;

static OsThreadInitStruct OsThreadInitTable[]=
{
	//Task ID    					TaskInitPointer     	Thread Priority  	Thread Tick  
	{0,								MyTestThreadInit,		3,					10},
	{OS_THREAD_FUNC_ID_END,			RT_NULL,				1,					5},
};


/*�̳߳�ʼ������*/
static void OsThreadInit(void)
{
	/*OS_THREAD_STRUCT  #define�������߳̽ṹ��*/
	OS_THREAD_STRUCT pOsThread = (OsThreadInitStruct *)&OsThreadInitTable[0]; //ȡ���߳��б�ĵ�һ���̳߳�ʼ��
	while(pOsThread->FuncID < OS_THREAD_FUNC_ID_END)
	{
		if(pOsThread->ThreadInit != NULL)
		{
			pOsThread->ThreadInit(pOsThread->Priority,pOsThread->Tick);	/*�̳߳�ʼ��,���Ϊ���ȼ��Լ�tick*/
		}
		pOsThread++;
	}
}



int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	EepromHwInit();
	
	/*�����߳��б�*/
	OsThreadInit();
	
    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}
