#include "my_pin_drive.h"


static rt_uint32_t MyTestThreadInit_stack = 1024;
/*
 * �����嵥������һ�� PIN �豸ʹ������
 * ���̵����� pin_beep_sample ��������ն�
 * ������ø�ʽ��pin_beep_sample
 * �����ܣ�ͨ���������Ʒ�������Ӧ���ŵĵ�ƽ״̬���Ʒ�����
*/


/* �жϻص����� ---------------------------------------------------------------------------- */
static void hdr_callback(void *args)
{
	char *str = args;
	rt_kprintf("KEY0 pressed. %s\n", str);
}


/* MyTestThreadInitThread�߳���ں��� -------------------------------------------------------------------------*/
void MyTestThreadInitThread(void *parameter)
{
    /* ����0����Ϊ����ģʽ */
    rt_pin_mode(KEY0_PIN_NUM, PIN_MODE_INPUT_PULLUP);
    /* ���жϣ��½���ģʽ���ص�������Ϊbeep_on */
    rt_pin_attach_irq(KEY0_PIN_NUM, PIN_IRQ_MODE_FALLING, hdr_callback, "----fyztest");
    /* ʹ���ж� */
    rt_pin_irq_enable(KEY0_PIN_NUM, PIN_IRQ_ENABLE);
	while(1)
	{
		rt_thread_mdelay(500);
	}
}


void MyTestThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
	    /* �����߳̾�� */
    rt_thread_t tid;
 
    /* ������̬pin�߳� �����ȼ� 25 ��ʱ��Ƭ 5��ϵͳ�δ��߳�ջ512�ֽ� */
    tid = rt_thread_create("irq_thread",
                  MyTestThreadInitThread,
                  RT_NULL,
                  MyTestThreadInit_stack,
                  ThreadPriority,
                  ThreadTick);
 
    /* �����ɹ���������̬�߳� */
    if (tid != RT_NULL)
    {
		rt_kprintf("thread start success!!\n");
    	rt_thread_startup(tid);
	}

}
/* ������ msh �����б��� */
//MSH_CMD_EXPORT(pin_beep_sample, pin beep sample);