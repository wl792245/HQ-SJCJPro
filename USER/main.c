
//单片机头文件
#include "stm32f10x.h"

////框架
//#include "framework.h"

////网络协议层
//#include "onenet.h"
//#include "fault.h"

////网络设备
//#include "net_device.h"

////网络任务
//#include "net_task.h"

////驱动
#include "delay.h"
#include "usart.h"
//#include "hwtimer.h"
//#include "i2c.h"
//#include "iwdg.h"
//#include "rtc.h"

////硬件
#include "led.h"
#include "dht11.h"
//#include "key.h"
//#include "adxl362.h"
//#include "sht20.h"
//#include "at24c02.h"
//#include "beep.h"

//TLSF动态内存管理算法
#include "tlsf.h"

//C库
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//分配内存空间
#define TLSF_MEM_POOL_SIZE		1024 * 15   //15K

static unsigned char tlsf_mem_pool[TLSF_MEM_POOL_SIZE];
/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断控制器分组设置
	Delay_Init();																			//systick初始化
	Usart1_Init(115200); 															//初始化串口   115200bps
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alter_info.alter_buf, sizeof(alter_info.alter_buf), USART_RX_TYPE);
#endif
	
	//init_memory_pool(TLSF_MEM_POOL_SIZE, tlsf_mem_pool);						//内存池初始化
	
	LED_Init();																	//LED初始化
//	
//	KEY_Init();																	//按键初始化
//	
//	BEEP_Init();																//蜂鸣器初始化
//	
//	IIC_Init(I2C2);																//IIC总线初始化
//	
//	RTC_Init();																	//初始化RTC
//	
	UsartPrintf(USART_DEBUG, "DHT11: %s\r\n", dht11_info.dht11_exit() ? "Error" : "Ok");	//EEPROM检测
//	
//	UsartPrintf(USART_DEBUG, "SHT20: %s\r\n", SHT20_Exist() ? "Ok" : "Err");	//SHT20检测
//	
//	UsartPrintf(USART_DEBUG, "ADXL362: %s\r\n", ADXL362_Init() ? "Ok" : "Err");	//ADXL362检测

//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//如果是看门狗复位则提示
//	{
//		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
//		
//		RCC_ClearFlag();														//清除看门狗复位标志位
//		
//		net_fault_info.net_fault_level = net_fault_info.net_fault_level_r
//														= NET_FAULT_LEVEL_5;	//错误等级5
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
//	//Iwdg_Init(4, 1250); 														//64分频，每秒625次，重载1250次，2s
//	
//	Timer_X_Init(TIM6, 49, 35999, 1, 0);										//72MHz，36000分频-500us，50重载值。则中断周期为500us * 50 = 25ms
	
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");							//提示初始化完成

}


/*
************************************************************
*	函数名称：	SENSOR_Task
*
*	函数功能：	传感器数据采集、显示
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		传感器数据采集任务。进行外接传感器的数据采集、读取、显示
************************************************************
*/
void SENSOR_Task(void)
{
	
	static unsigned char count = 0;

	if(dht11_info.device_ok == DHT11_OK) 											//只有设备存在时，才会读取值和显示
	{
		if(dht11_info.dht11_data()==0)												//采集传感器数据
			UsartPrintf(USART_DEBUG, "\"temperature\":\"%.1f\",\"humidity\":\"%.0f\",\n",dht11_info.tempreture,dht11_info.humidity);
	}
	
//	if(sht20_info.device_ok) 											//只有设备存在时，才会读取值和显示
//	{
//		SHT20_GetValue();												//采集传感器数据
//	}
			
//	if(++count >= 10)										//每隔一段时间发送一次红外数据
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
	Hardware_Init();									//硬件初始化
	while(1)
	{
		DelayXms(1000);
		DelayXms(1000);
		SENSOR_Task();
		
	}
}