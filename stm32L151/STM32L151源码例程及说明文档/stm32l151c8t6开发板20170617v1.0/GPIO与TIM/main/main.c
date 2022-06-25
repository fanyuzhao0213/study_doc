#include "stm32l1xx.h"
#include "stm32l1xx_tim.h"
#include "misc.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
u8 flag;
int main(void)
{ 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = 200;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM IT enable */
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
	
		/* Enable the TIM2 Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    delay_init(8);
    LED_Init(); 
		delay_ms(100);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		delay_ms(100);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		delay_ms(10);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		delay_us(100);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
    while(1)
    {
      led_on(flag);
    }
}



