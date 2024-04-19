#include "eeprom.h"


#define EEPROM_I2C_BUS_NAME "i2c1"
struct rt_i2c_bus_device *EepromI2CBus;

/**
 * @name: EepromHwInit
 * @msg:  EEPROM��ʼ��
 * @param [in]
 *         ��
 * @param [out]
 *         �� 
 * @return 1:��ʼ��ʧ�ܣ�0����ʼ���ɹ�
 * @note:  ��
 *         
 */
rt_uint8_t EepromHwInit(void)
{
    EepromI2CBus = (struct rt_i2c_bus_device *)rt_device_find(EEPROM_I2C_BUS_NAME);

    if (RT_NULL == EepromI2CBus)
    {
        rt_kprintf("can't find EEPROM %s device!\n", EEPROM_I2C_BUS_NAME);
        return RT_ERROR;
    }
    else
    {
		rt_kprintf("find EEPROM %s device!\n", EEPROM_I2C_BUS_NAME);
        return RT_EOK;
    } 
}
// INIT_DEVICE_EXPORT(EepromHwInit);

/**
 * @name: ReadEeprom
 * @msg:  ��EEPROM
 * @param [in]
 *         ReadAddr��EEPROM���Ŀ�ʼ��ַ
 *         len����ȡ�����ݳ���
 * @param [out]
 *         buf�������ݻ�����ָ��
 * @return -1:��ʧ�ܣ�0����ȡ�ɹ�
 * @note:  1)������ǰ��Ҫ���ȷ��Ͷ��ĵ�ַ
 *         2)EEPROM�ĵļĴ�����ַΪ˫�ֽ�,���ȷ��ͼĴ�����ַ�ĸ�8λ�ֽڵ�ַ,Ȼ���ͼĴ�����ַ�ĵ�8λ�ֽڵ�ַ
 *         
 */
rt_err_t ReadEeprom(rt_uint16_t ReadAddr, rt_uint8_t *buf, rt_uint16_t len)
{
    struct rt_i2c_msg msgs[2] = {0};
    rt_uint8_t TempBuf[2] = {0};

    TempBuf[0] = (ReadAddr >> 8);
    TempBuf[1] = ReadAddr;

    //д��ȡ�ĵ�ַ
    msgs[0].addr  = EEPROM_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = TempBuf;
    msgs[0].len   = 2;

    msgs[1].addr  = EEPROM_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (2 == rt_i2c_transfer(EepromI2CBus, msgs, 2))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to read EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: WriteEeprom
 * @msg:  дEEPROM
 * @param [in]
 *         WriteAddr��EEPROM��ʼд�ĵ�ַ
 *         data��д���ݻ�����ָ��
 *         len�� д���ݳ���
 * @param [out]
 *         ��
 * @return -1:��ʧ�ܣ�0����ȡ�ɹ�
 * @note:  1)д����ǰ��Ҫ���ȷ���д�ĵ�ַ
 *         2)EEPROM�ĵļĴ�����ַΪ˫�ֽ�,���ȷ��ͼĴ�����ַ�ĸ�8λ�ֽڵ�ַ,Ȼ���ͼĴ�����ַ�ĵ�8λ�ֽڵ�ַ
 *         
 */
rt_err_t WriteEeprom(rt_uint16_t WriteAddr, rt_uint8_t *data, rt_uint16_t len)
{
    struct rt_i2c_msg msgs = {0};
    rt_uint8_t buf[len+2];

    //д��ַ
    buf[0] = (WriteAddr >> 8);
    buf[1] = WriteAddr;
    rt_memcpy(buf+2, data, len);

    msgs.addr  = EEPROM_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = (len + 2);

    if (1 == rt_i2c_transfer(EepromI2CBus, &msgs, 1))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write EEPROM!\n");
        return -RT_ERROR;
    }
}

void eeprom_write_test(void)
{
	rt_uint8_t testbuff[4] = {0x12,0x34,0x56,0x78};
	if(WriteEeprom(TEST_WRITE_ADDR,testbuff,4) == RT_EOK)
	{
		 rt_kprintf("success to write EEPROM!\n");
	}
}

void eeprom_read_test(void)
{
	rt_uint8_t testbuff[4] = {0};
	rt_uint8_t i =0;
    if (RT_EOK == ReadEeprom(TEST_WRITE_ADDR, testbuff, 4))
    {
        //�ж�ָ����EEPROM�Ƿ�Ϊ�հ���
        for (i = 0; i < 4; i++)
        {
            if (0xFF != testbuff[i])
            {
                break;
            }
        }
		if(i != 4)
		{
				rt_kprintf("From EEPROM data is %02x,%02x,%02x,%02x\n",testbuff[0],testbuff[1],testbuff[2],testbuff[3]);
		}
    }
}

MSH_CMD_EXPORT(eeprom_write_test, eeprom write test);
MSH_CMD_EXPORT(eeprom_read_test, eeprom read test);