/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	led.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		LED��ʼ��������LED
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//��Ƭ��������
#include "mcu_gpio.h"

//����
#include "delay.h"
#include "led.h"


LED_STATUS led_status;

//Ī��
const static GPIO_LIST led_gpio_list[LED_NUM] = {
													{GPIOB, GPIO_Pin_5, "led1"},
													{GPIOE, GPIO_Pin_5, "led2"}
												};
/*
************************************************************
*	�������ƣ�	LED_Init
*
*	�������ܣ�	LED��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		�ߵ�ƽ�ص�		�͵�ƽ����
************************************************************
*/
void LED_Init(void)
{
	
	unsigned char i = 0;
	
	for(; i < LED_NUM; i++)
		MCU_GPIO_Init(led_gpio_list[i].gpio_group, led_gpio_list[i].gpio_pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, led_gpio_list[i].gpio_name);
	
	LED_Ctl(0, LED_ON);
	LED_Ctl(1, LED_ON);
}
/*
************************************************************
*	�������ƣ�	LED_Ctl
*
*	�������ܣ�	LED����
*
*	��ڲ�����	num��LED���
*				status������״̬
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void LED_Ctl(unsigned char num, LED_ENUM status)
{
	
	unsigned char i = 0;
	#ifdef USE_OS
	RTOS_TaskSuspendAll();
	#endif
	if(num < LED_NUM)
	{
		MCU_GPIO_Output_Ctl(led_gpio_list[num].gpio_name, !status);
		
		led_status.led_status[num] = status;
	}
	else if(num == LED_ALL)
	{
		for(; i < LED_NUM; i++)
		{
			MCU_GPIO_Output_Ctl(led_gpio_list[i].gpio_name, !status);
			
			led_status.led_status[i] = status;
		}
	}
	#ifdef USE_OS
	RTOS_TaskResumeAll();
	#endif
}

