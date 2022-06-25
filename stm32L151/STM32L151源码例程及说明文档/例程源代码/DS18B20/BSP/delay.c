#include "delay.h"

static uint8_t  fac_us=0;  //us延时倍乘数

/*******************************************************************************
* Function Name : delay_init
* Description   : 初始化延时参数
* Input         : None.
* Output        : None.
* Return value  :
*******************************************************************************/
void delay_init(void)
{
  // ST默认使用HCLK时钟作为SysTick时钟
  // 官方HAL库正常不使用8分频
  // 默认SysTick->CTRL = 0x00 00 00 07
    if(SysTick->CTRL & SYSTICK_CLKSOURCE_HCLK){ // 根据分频设置，选择us节拍数
        fac_us=HAL_RCC_GetHCLKFreq()/1000000UL;
    }else{
        fac_us=HAL_RCC_GetHCLKFreq()/8000000UL;
    }
}

/*******************************************************************************
* Function Name : delay_us
* Description   : us延时函数
* Input         : 时间，单位us
*******************************************************************************/
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//LOAD的值
	ticks=nus*fac_us-10; 			//需要的节拍数
	tcnt=0;
	told=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;//时间超过/等于要延迟的时间,则退出.
		}
	};
}

/*******************************************************************************
* Function Name : delay_ms
* Description   : ms延时函数
* Input         : 时间，单位ms
*******************************************************************************/
void delay_ms(uint32_t nms)
{
#ifdef _CMSIS_OS_H // 使用FreeRTOS
	osDelay(nms);
#else
	HAL_Delay(nms);
#endif
}

