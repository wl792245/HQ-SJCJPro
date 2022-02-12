/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	hwtimer.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-27
	*
	*	版本： 		V1.0
	*
	*	说明： 		单片机定时器初始化
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机相关组件
#include "mcu_timer.h"
#include "mcu_nvic.h"
#include "mcu_gpio.h"

//硬件驱动
#include "hwtimer.h"


/*
************************************************************
*	函数名称：	Timer1_8_PWM_Init
*
*	函数功能：	Timer1或8的PWM配置
*
*	入口参数：	TIMx：TIM1 或者 TIM8
*				psc：分频值
*				arr：重载值
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Timer1_8_PWM_Init(TIM_TypeDef *TIMx, GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr)
{
	
	MCU_GPIO_Init(gpio_group, gpio_pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, (void *)0);
	
	MCU_TIMER_Base_Init(TIMx, TIM_CKD_DIV1, TIM_CounterMode_Up, arr, psc, 0);
	
	MCU_Timer_PWM_Init(TIMx, TPC_4, TIM_OCMode_PWM1, TIM_OutputState_Enable, 0, 0, TIM_OCPolarity_Low, 0, 0, 0);
	
	TIM_Cmd(TIMx, ENABLE);										//使能TIMx

}

/*
************************************************************
*	函数名称：	TIM3_PWM_Init
*
*	函数功能：	Timer3_PWM配置
*
*	入口参数：	psc：分频值
*				arr：重载值
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void TIM3_PWM_Init(GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr)
{
	
	MCU_GPIO_Init(gpio_group, gpio_pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, (void *)0);
	
	MCU_TIMER_Base_Init(TIM3, TIM_CKD_DIV1, TIM_CounterMode_Up, arr, psc, 0);
	
	MCU_Timer_PWM_Init(TIM3, TPC_4, TIM_OCMode_PWM2, TIM_OutputState_Enable, 0, 0, TIM_OCPolarity_Low, 0, 0, 0);
	
	TIM_Cmd(TIM3, ENABLE);										//使能TIM3

}

/*
************************************************************
*	函数名称：	Timer6_7_Init
*
*	函数功能：	Timer6或7的定时配置
*
*	入口参数：	TIMx：TIM6 或者 TIM7
*				psc分频值
*				arr：重载值
*
*	返回参数：	无
*
*	说明：		timer6和timer7只具有更新中断功能
************************************************************
*/
#ifdef STM32F10X_HD
void Timer6_7_Init(TIM_TypeDef *TIMx, unsigned short psc, unsigned short arr)
{
	
	MCU_TIMER_Base_Init(TIMx, 0, TIM_CounterMode_Up, arr, psc, 0);
	
	if(TIMx == TIM6)
		MCU_NVIC_Init(TIM6_IRQn, ENABLE, 1, 1);
	else
		MCU_NVIC_Init(TIM7_IRQn, ENABLE, 1, 1);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);					//使能更新中断
	
	TIM_Cmd(TIMx, ENABLE); 										//使能定时器

}
#endif
/*
************************************************************
*	函数名称：	TIM6_IRQHandler
*
*	函数功能：	RTOS的心跳定时中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
#ifdef STM32F10X_HD
void TIM6_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}

}
#endif
/*
************************************************************
*	函数名称：	TIM7_IRQHandler
*
*	函数功能：	Timer7更新中断服务函数
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
#ifdef STM32F10X_HD
void TIM7_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}

}
#endif
