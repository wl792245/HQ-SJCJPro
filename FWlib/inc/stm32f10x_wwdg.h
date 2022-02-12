/**
  ******************************************************************************
  * @file    stm32f10x_wwdg.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the WWDG firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_WWDG_H
#define __STM32F10x_WWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup WWDG
  * @{
  */ 

/** @defgroup WWDG_Exported_Types
  * @{
  */ 
  
/**
  * @}
  */ 

/** @defgroup WWDG_Exported_Constants
  * @{
  */ 
  
/** @defgroup WWDG_Prescaler 
  * @{
  */ 
  
#define WWDG_Prescaler_1    ((uint32_t)0x00000000)
#define WWDG_Prescaler_2    ((uint32_t)0x00000080)
#define WWDG_Prescaler_4    ((uint32_t)0x00000100)
#define WWDG_Prescaler_8    ((uint32_t)0x00000180)
#define IS_WWDG_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Prescaler_1) || \
                                      ((PRESCALER) == WWDG_Prescaler_2) || \
                                      ((PRESCALER) == WWDG_Prescaler_4) || \
                                      ((PRESCALER) == WWDG_Prescaler_8))
#define IS_WWDG_WINDOW_VALUE(VALUE) ((VALUE) <= 0x7F)
#define IS_WWDG_COUNTER(COUNTER) (((COUNTER) >= 0x40) && ((COUNTER) <= 0x7F))

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup WWDG_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup WWDG_Exported_Functions
  * @{
  */ 
//将外设 WWDG寄存器重设为缺省值   
void WWDG_DeInit(void);
//设置 WWDG 预分频值 
/*WWDG_Prescaler_1  WWDG计数器时钟为（PCLK1/4096）/1 
 *WWDG_Prescaler_2  WWDG计数器时钟为（PCLK1/4096）/2 
 *WWDG_Prescaler_4  WWDG计数器时钟为（PCLK1/4096）/4 
 *WWDG_Prescaler_8  WWDG计数器时钟为（PCLK1/4096）/8 */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler);
//设置 WWDG 窗口值 
//WindowValue r：指定的窗口值。该参数取值必须在 0x40 与 0x7F之间。
void WWDG_SetWindowValue(uint8_t WindowValue);
//使能 WWDG 早期唤醒中断（EWI）
void WWDG_EnableIT(void);
//设置 WWDG 计数器值 0x40 与0x7F
void WWDG_SetCounter(uint8_t Counter);
//使能 WWDG 并装入计数器值
void WWDG_Enable(uint8_t Counter);
//检查 WWDG 早期唤醒中断标志位被设置与否 
FlagStatus WWDG_GetFlagStatus(void);
//清除早期唤醒中断标志位 
void WWDG_ClearFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_WWDG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
