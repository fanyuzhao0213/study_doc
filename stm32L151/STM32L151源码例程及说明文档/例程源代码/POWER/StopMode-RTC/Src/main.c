/**
******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
*
* COPYRIGHT(c) 2015 STMicroelectronics
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void power_exit_stop(void);
void power_enter_stop(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
__IO uint8_t sec = 0;

/* USER CODE END 0 */

int main(void)
{
	
	/* USER CODE BEGIN 1 */
	
	/* USER CODE END 1 */
	
	/* MCU Configuration----------------------------------------------------------*/
	
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	
	/* Configure the system clock */
	SystemClock_Config();
	
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	
	/* USER CODE BEGIN 2 */
	// 禁用比较器
	__COMP_CLK_DISABLE();
	
		/* 禁止唤醒时钟计数 */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	
	// 低功耗模式下RTC 允许访问 ，RTC唤醒需要
	HAL_PWR_EnableBkUpAccess();
	//HAL_PWR_DisableBkUpAccess();
	
	// 禁止 PVD
	HAL_PWR_DisablePVD();
	
	
	// 动态电压调整范围 低功耗睡眠、低功耗运行模式才有效果
	/* Set Voltage scale1 as MCU will run at 32MHz */
	//__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
	//while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};
	
	
	// 关闭VREFINT	少3uA
	HAL_PWREx_EnableUltraLowPower();
	// 忽略VREFINT 加快启动速度
	HAL_PWREx_EnableFastWakeUp();
	
	
	// 允许/禁用 调试端口 少800uA
	HAL_DBGMCU_DisableDBGStopMode();
	//HAL_DBGMCU_EnableDBGStopMode();
	
	/* USER CODE END 2 */
	
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		LED_ON;
		HAL_Delay(1000);
		printf("Run!\r\n");
		sec++;
		if(sec>5){
			sec=0;
			power_enter_stop();
		}
		/* USER CODE END WHILE */
		
		/* USER CODE BEGIN 3 */
		
	}
	/* USER CODE END 3 */
	
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
	
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;
	
	__PWR_CLK_ENABLE();
	
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**	Enter Stop Mode
***************************************/
void power_enter_stop(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	printf("=== Power Stop ===\r\n");
	LED_OFF;
	HAL_UART_MspDeInit(&huart1);
	
	//UART1;	减少16uA
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	__GPIOA_CLK_DISABLE();
	__GPIOB_CLK_DISABLE();
	__GPIOC_CLK_DISABLE();
	__GPIOH_CLK_DISABLE();
	
	// PIN1 连接到 PWR->CR 的 PWR_FLAG_WU 标记
	// 用于唤醒后，判断WKUP按钮状态
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
		
	/*## Configure the Wake up timer ###########################################*/
	/*  RTC Wakeup Interrupt Generation:
	Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI))
	Wakeup Time = Wakeup Time Base * WakeUpCounter 
	= (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI)) * WakeUpCounter
	==> WakeUpCounter = Wakeup Time / Wakeup Time Base
	
	To configure the wake up timer to 4s the WakeUpCounter is set to 0x242B:
	RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
	Wakeup Time Base = 16 /(~37KHz) = ~0,432 ms
	Wakeup Time = ~5s = 0,432ms  * WakeUpCounter
	==> WakeUpCounter = ~5s/0,432ms = 11562 */
	// rtc 唤醒采用LSI时钟，第一参数，计数溢出唤醒；第二参数：进行16分配，
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 11562, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	
	/* Enter Stop Mode */
	//	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	//	/* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
	//	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};
	// 进入停机模式 
	// PWR_LOWPOWERREGULATOR_ON 稳压器工作在动态低功耗模式 减少2uA
	// PWR_STOPENTRY_WFI	中断唤醒
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	
}


/**	Exit STOP mode
***************************************/
void power_exit_stop(void)
{
	/* 禁止唤醒时钟计数 */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	
	//STOP模式唤醒之后，默认运行MSI 时钟，需要重新配置系统时钟
	SystemClock_Config();	
	
	// 清除唤醒标记 减少重复唤醒电流，减少3uA
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	
	// 停止 PIN1 连接到 PWR->CR 的 PWR_FLAG_WU 标记
	// 用于在工作状态判断WKUP按钮状态
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	
	HAL_UART_MspInit(&huart1);	//重新初始化串口
}

/**
* @brief  RTC 唤醒回调函数
* @param  None
* @retval None
*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	power_exit_stop();
	printf("  \r\n\r\nRTC Walkup!\r\n");
}


/**
* @brief  外部中断回调函数
* @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==KEY_Pin){
		// 判断按钮是在普通工作状态、还是唤醒状态
		if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU)!=RESET){
			power_exit_stop();
			printf("  \r\n\r\nExti Walkup!\r\n");
		}else{
			printf("  \r\nExti 0!\r\n");
		}
	}
}

//===================================================  printf uart1	
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
PUTCHAR_PROTOTYPE
{
	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
#ifdef __STM32F0xx_HAL_H
	
	huart1.Instance->TDR = (uint8_t)ch;
#endif
	
#ifdef __STM32F1xx_HAL_H
	huart1.Instance->DR = (uint8_t)ch;
#endif
	
#ifdef __STM32L1xx_HAL_H
	huart1.Instance->DR = (uint8_t)ch;
#endif
	
	return ch;
}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
* @brief Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
	
}

#endif

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
