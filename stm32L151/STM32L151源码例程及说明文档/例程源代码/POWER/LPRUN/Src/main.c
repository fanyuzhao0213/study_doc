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
void Power_Enter_LPRun(void);
void Power_Exit_LPRun(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
	
	/* USER CODE BEGIN 1 */
	__IO uint8_t sec = 0;
	/* USER CODE END 1 */
	
	/* MCU Configuration----------------------------------------------------------*/
	
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	
	/* Configure the system clock */
	SystemClock_Config();
	
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	
	/* USER CODE BEGIN 2 */
	
	// 禁用比较器
	__COMP_CLK_DISABLE();
	
	// 禁止 PVD
	HAL_PWR_DisablePVD();
	
	/* Disable Prefetch Buffer */
	__HAL_FLASH_PREFETCH_BUFFER_DISABLE();
	
	LED_ON;
	
	// 允许/禁用 调试端口 少800uA
	HAL_DBGMCU_DisableDBGStopMode();
	
	
	
	
	
	/* USER CODE END 2 */
	
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		// 等待Walkup 按下
		while(WK_UP != GPIO_PIN_RESET)
		{
		}
		// 等待Walkup 释放
		while(WK_UP == GPIO_PIN_RESET)
		{
		}
		if(sec==RESET){
			sec=SET;
			printf("Go To Low Power Run\r\n");
			Power_Enter_LPRun();
		}else{
			sec=RESET;
			Power_Exit_LPRun();
			printf("Go To Main Run\r\n");
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
	
	__PWR_CLK_ENABLE();
	
	// 最低电压范围 0~4Mhz
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void Power_Enter_LPRun(void)
{
	LED_OFF;
	// 系统时钟必须128Khz以下
	// 设置时钟为 MSI = 65.536 KHz
	__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_0);
	HAL_PWREx_EnableLowPowerRunMode();
	// 等待进入低功耗运行模式
	while(__HAL_PWR_GET_FLAG(PWR_FLAG_REGLP) == RESET)
	{
	}
}
void Power_Exit_LPRun(void)
{
	
	HAL_PWREx_DisableLowPowerRunMode();
	// 等待退出低功耗运行模式
	while(__HAL_PWR_GET_FLAG(PWR_FLAG_REGLP) != RESET)
	{
	}
	// 设置时钟为 MSI = 2.097 MHz
	__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_5);
	LED_ON;
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
