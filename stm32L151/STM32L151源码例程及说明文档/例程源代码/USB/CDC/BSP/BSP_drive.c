#include "BSP_drive.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//*=========================================================== I2C
#if defined (BSP_I2C)
//***********************************
//写入一个字节
//***********************************
uint8_t I2C_Write(uint8_t slaveAddress , uint8_t regAddress , uint8_t regData)
{
    uint8_t rxData[2] = {regAddress,regData};
    return HAL_I2C_Master_Transmit(&hi2c1,slaveAddress,rxData,2,50);
}


//***********************************
//写入多个字节
//***********************************
uint8_t I2C_WriteBulk(uint8_t slaveAddress , uint8_t regAddress , uint8_t size , uint8_t *data)
{
    uint8_t reStr = HAL_ERROR;
//	size++;
//    uint8_t *txData = (uint8_t*)malloc(sizeof(uint8_t)*size);	//临时申请内存区域
//    if(txData!=NULL)
//    {
//        txData[0]=regAddress;
//        memcpy(txData+1,data,size-1);
//        reStr = HAL_I2C_Master_Transmit(&hi2c1,slaveAddress,txData,size,50);
//        free(txData);	// 释放内存
//    }
	reStr = HAL_I2C_Mem_Write(&hi2c1,slaveAddress, regAddress, I2C_MEMADD_SIZE_8BIT ,data, size,50);
    return reStr;
	
	
	
}

//***********************************
//读取一个字节
//***********************************
uint8_t I2C_Read(uint8_t slaveAddress , uint8_t regAddress)
{
    uint8_t reStr;
    uint8_t data=0;
    reStr=I2C_ReadBulk(slaveAddress , regAddress ,  1 , &data);
    if(reStr) return 0; // 错误返回0
    return data;	// 正确返回数据
}


//***********************************
//读取多个字节
//***********************************
uint8_t I2C_ReadBulk(uint8_t slaveAddress , uint8_t regAddress , uint8_t size , uint8_t *data)
{
    uint8_t reStr;
    reStr=HAL_I2C_Master_Transmit(&hi2c1,slaveAddress,&regAddress,1,50);
    if(reStr) return reStr;
    reStr=HAL_I2C_Master_Receive(&hi2c1,slaveAddress,data,size,50);
    if(reStr) return reStr;
    return HAL_OK;
}



//***********************************
//搜索I2C设备地址
//***********************************
void SeachI2cDev(void)
{
    uint8_t i;
    printf("\r\n\r\n --------- I2C DEVICE ---------\r\n");
    for (i=0; i<127; i++)
    {
        if(HAL_I2C_IsDeviceReady(&hi2c1,(i<<1),3,50)==HAL_OK)
            printf("Device %d : 0x%02X \r\n",i,(i<<1));
    }
}
#endif	//=================================================== I2C END






//*=========================================================== SPI
#if defined (BSP_SPI)	// SPI

//***********************************
//读写一个字节
//***********************************
uint8_t BSP_SPI_Byte(SPI_HandleTypeDef *hspi,uint8_t txData)
{
    uint8_t rxData;
    uint16_t retry=0;

    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET)
    {
        retry++;
        if (retry>500) break;
    };
    retry=0;
    hspi->Instance->DR = txData;
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE) == RESET)
    {
        retry++;
        if (retry>500) break;
    };
    rxData = hspi->Instance->DR;
    return rxData;

}

//***********************************
//读写多个字节
//***********************************
HAL_StatusTypeDef BSP_SPI_Bulk(SPI_HandleTypeDef *hspi, uint8_t *txData, uint8_t *rxData, uint8_t size)
{
    uint16_t retry=0;
    __HAL_SPI_ENABLE(hspi);
//	do{
//		if(hspi->Lock == HAL_UNLOCKED)
//		{
//			hspi->Lock = HAL_LOCKED;
//			break;
//		}
//	}while (1);
//	hspi->State       = HAL_SPI_STATE_BUSY_TX;
//	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;

    for(uint16_t i = 0; i < size; i++)
    {
        while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET)
        {
            retry++;
            if (retry>500) break;
        };
        hspi->Instance->DR = txData[i];
        retry=0;
        while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE) == RESET)
        {
            retry++;
            if (retry>500) break;
        };
        rxData[i] = hspi->Instance->DR;
    }

//	hspi->State = HAL_SPI_STATE_READY;
//	__HAL_UNLOCK(hspi);
//	__HAL_SPI_DISABLE(hspi);
    return HAL_OK;
}


#endif	//=================================================== SPI END





//===================================================  printf uart1	
#if defined (BSP_USART)	
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
#endif

