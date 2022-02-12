
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

////���
//#include "framework.h"

////����Э���
//#include "onenet.h"
//#include "fault.h"

////�����豸
//#include "net_device.h"

////��������
//#include "net_task.h"

////����
#include "delay.h"
#include "usart.h"
//#include "hwtimer.h"
//#include "i2c.h"
//#include "iwdg.h"
//#include "rtc.h"

////Ӳ��
#include "led.h"
#include "dht11.h"
//#include "key.h"
//#include "adxl362.h"
//#include "sht20.h"
//#include "at24c02.h"
//#include "beep.h"

//TLSF��̬�ڴ�����㷨
#include "tlsf.h"

//C��
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//�����ڴ�ռ�
#define TLSF_MEM_POOL_SIZE		1024 * 15   //15K

static unsigned char tlsf_mem_pool[TLSF_MEM_POOL_SIZE];
/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�жϿ�������������
	Delay_Init();																			//systick��ʼ��
	Usart1_Init(115200); 															//��ʼ������   115200bps
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alter_info.alter_buf, sizeof(alter_info.alter_buf), USART_RX_TYPE);
#endif
	
	//init_memory_pool(TLSF_MEM_POOL_SIZE, tlsf_mem_pool);						//�ڴ�س�ʼ��
	
	LED_Init();																	//LED��ʼ��
//	
//	KEY_Init();																	//������ʼ��
//	
//	BEEP_Init();																//��������ʼ��
//	
//	IIC_Init(I2C2);																//IIC���߳�ʼ��
//	
//	RTC_Init();																	//��ʼ��RTC
//	
	UsartPrintf(USART_DEBUG, "DHT11: %s\r\n", dht11_info.dht11_exit() ? "Error" : "Ok");	//EEPROM���
//	
//	UsartPrintf(USART_DEBUG, "SHT20: %s\r\n", SHT20_Exist() ? "Ok" : "Err");	//SHT20���
//	
//	UsartPrintf(USART_DEBUG, "ADXL362: %s\r\n", ADXL362_Init() ? "Ok" : "Err");	//ADXL362���

//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//����ǿ��Ź���λ����ʾ
//	{
//		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
//		
//		RCC_ClearFlag();														//������Ź���λ��־λ
//		
//		net_fault_info.net_fault_level = net_fault_info.net_fault_level_r
//														= NET_FAULT_LEVEL_5;	//����ȼ�5
//		
//		net_device_info.reboot = 1;
//	}
//	else
//	{
//		UsartPrintf(USART_DEBUG, "PROID:%s,	SERIAL: %s,	PSWD: %s\r\n"
//							, onenet_info.pro_id, onenet_info.serial, onenet_info.pswd);
//		
//		net_device_info.reboot = 0;
//	}
//	
//	//Iwdg_Init(4, 1250); 														//64��Ƶ��ÿ��625�Σ�����1250�Σ�2s
//	
//	Timer_X_Init(TIM6, 49, 35999, 1, 0);										//72MHz��36000��Ƶ-500us��50����ֵ�����ж�����Ϊ500us * 50 = 25ms
	
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");							//��ʾ��ʼ�����

}


/*
************************************************************
*	�������ƣ�	SENSOR_Task
*
*	�������ܣ�	���������ݲɼ�����ʾ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		���������ݲɼ����񡣽�����Ӵ����������ݲɼ�����ȡ����ʾ
************************************************************
*/
void SENSOR_Task(void)
{
	
	static unsigned char count = 0;

	if(dht11_info.device_ok == DHT11_OK) 											//ֻ���豸����ʱ���Ż��ȡֵ����ʾ
	{
		if(dht11_info.dht11_data()==0)												//�ɼ�����������
			UsartPrintf(USART_DEBUG, "\"temperature\":\"%.1f\",\"humidity\":\"%.0f\",\n",dht11_info.tempreture,dht11_info.humidity);
	}
	
//	if(sht20_info.device_ok) 											//ֻ���豸����ʱ���Ż��ȡֵ����ʾ
//	{
//		SHT20_GetValue();												//�ɼ�����������
//	}
			
//	if(++count >= 10)										//ÿ��һ��ʱ�䷢��һ�κ�������
//	{
//		count = 0;
//		
//		NET_DEVICE_GetSignal();
//		
//		//NEC_SendData(0, send_data++);
//	}

}
int main()
{
	Hardware_Init();									//Ӳ����ʼ��
	while(1)
	{
		DelayXms(1000);
		DelayXms(1000);
		SENSOR_Task();
		
	}
}