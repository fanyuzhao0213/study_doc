////////////////////////////////////////////////////////////////////////////////
//DS18B20驱动代码
//修改日期:2016/4/10
//版本：V1.1
//版权所有，盗版必究。
////////////////////////////////////////////////////////////////////////////////

#include "ds18b20.h"

TDs18TypeDef  tDs18b[DS18B20_MAX_LEN];
uint8_t Ds18b_Len=0;

// global search state
uint8_t ROM_NO[8];
uint8_t LastDiscrepancy;
uint8_t LastFamilyDiscrepancy;
uint8_t LastDeviceFlag;


/*******************************************************************************
* Function Name : DS18B20_Init
* Description   : 初始化DS18B20的IO口 DQ 同时检测DS的存在
* Input         : None.
* Output        : None.
* Return value  : 返回器件数量  返回0:不存在
*******************************************************************************/
uint8_t DS18B20_Init(void)
{
  uint8_t i=0;

  /*Configure GPIO pin : DS18B20 */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(DQ_GPIO_Port, &GPIO_InitStruct);

  if(!DS18B20_Rst())
  {
	while(DS18B20_search(tDs18b[i].romAddr)){ //搜索DS18B20器件
	  tDs18b[i].device=i;
	  i++;
	  if (i>=DS18B20_MAX_LEN) break;
	}
	Ds18b_Len = i;
	DS18B20_reset_search();
	return Ds18b_Len;
  }else{
	return 0;
  }
}


/*******************************************************************************
* Function Name : DS18B20_Rst
* Description   : 复位DS18B20
* Input         : None.
* Output        : None.
* Return value  : 返回1:不存在  返回0:存在
*******************************************************************************/
uint8_t DS18B20_Rst(void)
{
  uint8_t retry=0;

  DS18B20_0; //拉低DQ
  delay_us(750);    //拉低750us
  DS18B20_1; //DQ=1
  delay_us(80);     //80US

  //等待DS18B20的回应

  while (DS18B20_STATE&&retry<200)
  {
	retry++;
	delay_us(1);
  };
  if(retry>=200)return 1;
  else retry=0;
  while (!DS18B20_STATE&&retry<240)
  {
	retry++;
	delay_us(1);
  };
  if(retry>=240)return 1;
  return 0;
}

/*******************************************************************************
* Function Name : DS18B20_Read_Bit
* Description   : 从DS18B20读取一个位
* Input         : None.
* Output        : None.
* Return value  : 返回值：读到的数据位，第一位有效
*******************************************************************************/
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
  uint8_t data;
  DS18B20_0;
  delay_us(2);
  DS18B20_1;

  delay_us(12);
  if(DS18B20_STATE)data=1;
  else data=0;
  delay_us(50);
  return data;
}


/*******************************************************************************
* Function Name : DS18B20_Read_Byte
* Description   : 从DS18B20读取一个字节
* Input         : None.
* Output        : None.
* Return value  : 返回值：读到的字节
*******************************************************************************/
uint8_t DS18B20_Read_Byte(void)    // read one byte
{
  uint8_t i,j,dat;
  dat=0;
  for (i=1;i<=8;i++)
  {
	j=DS18B20_Read_Bit();
	dat=(j<<7)|(dat>>1);
  }
  return dat;
}


/*******************************************************************************
* Function Name : DS18B20_Write_bit
* Description   : 写一个字节到DS18B20
* Input         : dat：要写入的bit，第一位有效
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_Write_bit(uint8_t dat)
{
  uint8_t testb;

  testb=dat&0x01;
  if (testb)
  {
	DS18B20_0;// Write 1
	delay_us(2);
	DS18B20_1;
	delay_us(60);
  }
  else
  {
	DS18B20_0;// Write 0
	delay_us(60);
	DS18B20_1;
	delay_us(2);
  }
}


/*******************************************************************************
* Function Name : DS18B20_Write_Byte
* Description   : 写一个字节到DS18B20
* Input         : dat=要写入的字节
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_Write_Byte(uint8_t dat)
{
  uint8_t j;
  uint8_t testb;
  //DS18B20_IO_OUT();//SET PA0 OUTPUT;
  for (j=1;j<=8;j++)
  {
	testb=dat&0x01;
	dat=dat>>1;
	DS18B20_Write_bit(testb) ;
  }
}

//
/*******************************************************************************
* Function Name : DS18B20_Start
* Description   : 开始温度转换
* Input         : rom器件的地址，如需要跳过rom[0]=0 或 rom[0]=0xCC.
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_Start(uint8_t device)// ds1820 start convert
{
  DS18B20_Rst();
  if (device==SKIP_ROM){
	DS18B20_Write_Byte(SKIP_ROM);// skip rom
  }else{
	DS18B20_Select(tDs18b[device].romAddr);
  }
  DS18B20_Write_Byte(CONVERT_TEMPERATURE);// convert
}


/*******************************************************************************
* Function Name : DS18B20_Get_Temp
* Description   : 从ds18b20读取温度值
* Input         : rom器件的地址，如需要跳过rom[0]=0 或 rom[0]=0xCC.
* Output        : None.
* Return value  : 温度值(-55~125) 0.1C精度
*******************************************************************************/
float DS18B20_Get_Temp(uint8_t device)
{
  uint8_t temp;
  uint8_t TL,TH;
  short tem;
  DS18B20_Start(device);                    // ds1820 start convert

  DS18B20_Rst();

  if (tDs18b[device].romAddr[0]==0 || device==SKIP_ROM){
	// 如果ROM无效，或不指定ROM
	DS18B20_Write_Byte(SKIP_ROM);// skip rom
  }else{
	DS18B20_Select(tDs18b[device].romAddr);
  }

  DS18B20_Write_Byte(READ_SCRATCHPAD);// 读暂存存储器
  TL=DS18B20_Read_Byte(); // LSB
  TH=DS18B20_Read_Byte(); // MSB

  if(TH>7)
  {
	TH=~TH;
	TL=~TL;
	temp=0;//温度为负
  }else temp=1;//温度为正
  tem=((short)TH<<8) | TL; //获得高八位

  //tem<<=8;
  //tem+=;//获得底八位
  tDs18b[device].temp =(float)tem*0.0625;

  if(temp) return tDs18b[device].temp; //返回温度值
  else return tDs18b[device].temp =-(float)tem*0.0625;
}


/*******************************************************************************
* Function Name : DS18B20_search
* Description   : 搜索ROM地址
* Input         : None.
* Output        : newAddr 存放器件的ROM地址
* Return value  : 1=找到器件; 0=找不到器件
*******************************************************************************/
uint8_t DS18B20_search(uint8_t *newAddr)
{
  uint8_t id_bit_number;
  uint8_t last_zero, rom_byte_number, search_result;
  uint8_t id_bit, cmp_id_bit;
  uint8_t i =0;
  unsigned char rom_byte_mask, search_direction;

  // initialize for search
  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;

  // if the last call was not the last one
  if (!LastDeviceFlag)
  {
	// 1-Wire reset
	if (DS18B20_Rst())
	{
	  // reset the search
	  LastDiscrepancy = 0;
	  LastDeviceFlag = 0;
	  LastFamilyDiscrepancy = 0;
	  return 0;
	}

	// issue the search command
	DS18B20_Write_Byte(SEARCH_ROM);

	// loop to do the search
	do
	{
	  // read a bit and its complement
	  id_bit = DS18B20_Read_Bit();
	  cmp_id_bit = DS18B20_Read_Bit();

	  // check for no devices on 1-wire
	  if ((id_bit == 1) && (cmp_id_bit == 1))
		break;
	  else
	  {
		// all devices coupled have 0 or 1
		if (id_bit != cmp_id_bit)
		  search_direction = id_bit;  // bit write value for search
		else
		{
		  // if this discrepancy if before the Last Discrepancy
		  // on a previous next then pick the same as last time
		  if (id_bit_number < LastDiscrepancy)
			search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
		  else
			// if equal to last pick 1, if not then pick 0
			search_direction = (id_bit_number == LastDiscrepancy);

		  // if 0 was picked then record its position in LastZero
		  if (search_direction == 0)
		  {
			last_zero = id_bit_number;

			// check for Last discrepancy in family
			if (last_zero < 9)
			  LastFamilyDiscrepancy = last_zero;
		  }
		}

		// set or clear the bit in the ROM byte rom_byte_number
		// with mask rom_byte_mask
		if (search_direction == 1)
		  ROM_NO[rom_byte_number] |= rom_byte_mask;
		else
		  ROM_NO[rom_byte_number] &= ~rom_byte_mask;

		// serial number search direction write bit
		DS18B20_Write_bit(search_direction);

		// increment the byte counter id_bit_number
		// and shift the mask rom_byte_mask
		id_bit_number++;
		rom_byte_mask <<= 1;

		// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
		if (rom_byte_mask == 0)
		{
		  rom_byte_number++;
		  rom_byte_mask = 1;
		}
	  }
	}
	while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

	// if the search was successful then
	if (!(id_bit_number < 65))
	{
	  // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
	  LastDiscrepancy = last_zero;

	  // check for last device
	  if (LastDiscrepancy == 0)
		LastDeviceFlag = 1;

	  search_result = 1;
	}
  }

  // if no device found then reset counters so next 'search' will be like a first
  if (!search_result || !ROM_NO[0])
  {
	LastDiscrepancy = 0;
	LastDeviceFlag = 0;
	LastFamilyDiscrepancy = 0;
	search_result = 0;
  }

  for (i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
  return search_result;
}


/*******************************************************************************
* Function Name : DS18B20_reset_search
* Description   : 重新搜索ROM
* Input         : None.
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_reset_search(void)
{
  uint8_t i =0;
  LastDiscrepancy = 0;
  LastDeviceFlag = 0;
  LastFamilyDiscrepancy = 0;

  for(i = 0;i<8 ; i++)
  {
	ROM_NO[i] = 0;
  }
}


/*******************************************************************************
* Function Name : DS18B20_Select
* Description   : 选择指定ROM
* Input         : rom地址
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_Select( uint8_t *rom)
{
  uint8_t i;
  DS18B20_Write_Byte(MATCH_ROM);           // Choose ROM
  for( i = 0; i < 8; i++) DS18B20_Write_Byte(rom[i]);
}


/*******************************************************************************
* Function Name : DS18B20_skip
* Description   : 跳过ROM选择
* Input         : None.
* Output        : None.
* Return value  :
*******************************************************************************/
void DS18B20_skip(void)
{
  DS18B20_Write_Byte(SKIP_ROM);           // Skip ROM
}


#ifdef _CMSIS_OS_H
void DS18Read_Task(void const * argument)
{
  while(1)
  {
	DS18B20_Get_Temp(0);
	DS18B20_Get_Temp(1);
	printf("DS18a: %3.3f  \t DS18b: %3.3f\r\n",tDs18b[0].temp,tDs18b[1].temp);
	osDelay(DS18_DELAY);
	//osThreadResume(LEDThread2Handle);
  }
}
#endif


