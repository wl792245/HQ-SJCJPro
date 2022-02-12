#ifndef _HWTIMER_H_
#define _HWTIMER_H_


#include "stm32f10x.h"


void Timer1_8_PWM_Init(TIM_TypeDef *TIMx, GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr);

void TIM3_PWM_Init(GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr);

void Timer6_7_Init(TIM_TypeDef *TIMx, unsigned short psc, unsigned short arr);


#endif
