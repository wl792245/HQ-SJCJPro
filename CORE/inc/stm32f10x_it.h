#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H


#include "stm32f10x.h"


void NMI_Handler(void);

void HardFault_Handler(void);

void MemManage_Handler(void);

void BusFault_Handler(void);

void UsageFault_Handler(void);

void SVC_Handler(void);

void DebugMon_Handler(void);

//void PendSV_Handler(void); //使用操作系统,注释掉这里

void SysTick_Handler(void);


#ifndef STM32F10X_CL
void USB_LP_CAN1_RX0_IRQHandler(void);
#endif /* STM32F10X_CL */

#ifdef STM32F10X_CL
void OTG_FS_IRQHandler(void);
#endif /* STM32F10X_CL */





#endif 


