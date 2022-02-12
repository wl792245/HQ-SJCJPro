/**
  ******************************************************************************
  * @file    stm32f10x_bkp.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the BKP firmware 
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
#ifndef __STM32F10x_BKP_H
#define __STM32F10x_BKP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup BKP
  * @{
  */

/** @defgroup BKP_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Constants
  * @{
  */

/** @defgroup Tamper_Pin_active_level 
  * @{
  */

#define BKP_TamperPinLevel_High           ((uint16_t)0x0000)
#define BKP_TamperPinLevel_Low            ((uint16_t)0x0001)
#define IS_BKP_TAMPER_PIN_LEVEL(LEVEL) (((LEVEL) == BKP_TamperPinLevel_High) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Low))
/**
  * @}
  */

/** @defgroup RTC_output_source_to_output_on_the_Tamper_pin 
  * @{
  */

#define BKP_RTCOutputSource_None          ((uint16_t)0x0000)
#define BKP_RTCOutputSource_CalibClock    ((uint16_t)0x0080)
#define BKP_RTCOutputSource_Alarm         ((uint16_t)0x0100)
#define BKP_RTCOutputSource_Second        ((uint16_t)0x0300)
#define IS_BKP_RTC_OUTPUT_SOURCE(SOURCE) (((SOURCE) == BKP_RTCOutputSource_None) || \
                                          ((SOURCE) == BKP_RTCOutputSource_CalibClock) || \
                                          ((SOURCE) == BKP_RTCOutputSource_Alarm) || \
                                          ((SOURCE) == BKP_RTCOutputSource_Second))
/**
  * @}
  */

/** @defgroup Data_Backup_Register 
  * @{
  */

#define BKP_DR1                           ((uint16_t)0x0004)
#define BKP_DR2                           ((uint16_t)0x0008)
#define BKP_DR3                           ((uint16_t)0x000C)
#define BKP_DR4                           ((uint16_t)0x0010)
#define BKP_DR5                           ((uint16_t)0x0014)
#define BKP_DR6                           ((uint16_t)0x0018)
#define BKP_DR7                           ((uint16_t)0x001C)
#define BKP_DR8                           ((uint16_t)0x0020)
#define BKP_DR9                           ((uint16_t)0x0024)
#define BKP_DR10                          ((uint16_t)0x0028)
#define BKP_DR11                          ((uint16_t)0x0040)
#define BKP_DR12                          ((uint16_t)0x0044)
#define BKP_DR13                          ((uint16_t)0x0048)
#define BKP_DR14                          ((uint16_t)0x004C)
#define BKP_DR15                          ((uint16_t)0x0050)
#define BKP_DR16                          ((uint16_t)0x0054)
#define BKP_DR17                          ((uint16_t)0x0058)
#define BKP_DR18                          ((uint16_t)0x005C)
#define BKP_DR19                          ((uint16_t)0x0060)
#define BKP_DR20                          ((uint16_t)0x0064)
#define BKP_DR21                          ((uint16_t)0x0068)
#define BKP_DR22                          ((uint16_t)0x006C)
#define BKP_DR23                          ((uint16_t)0x0070)
#define BKP_DR24                          ((uint16_t)0x0074)
#define BKP_DR25                          ((uint16_t)0x0078)
#define BKP_DR26                          ((uint16_t)0x007C)
#define BKP_DR27                          ((uint16_t)0x0080)
#define BKP_DR28                          ((uint16_t)0x0084)
#define BKP_DR29                          ((uint16_t)0x0088)
#define BKP_DR30                          ((uint16_t)0x008C)
#define BKP_DR31                          ((uint16_t)0x0090)
#define BKP_DR32                          ((uint16_t)0x0094)
#define BKP_DR33                          ((uint16_t)0x0098)
#define BKP_DR34                          ((uint16_t)0x009C)
#define BKP_DR35                          ((uint16_t)0x00A0)
#define BKP_DR36                          ((uint16_t)0x00A4)
#define BKP_DR37                          ((uint16_t)0x00A8)
#define BKP_DR38                          ((uint16_t)0x00AC)
#define BKP_DR39                          ((uint16_t)0x00B0)
#define BKP_DR40                          ((uint16_t)0x00B4)
#define BKP_DR41                          ((uint16_t)0x00B8)
#define BKP_DR42                          ((uint16_t)0x00BC)

#define IS_BKP_DR(DR) (((DR) == BKP_DR1)  || ((DR) == BKP_DR2)  || ((DR) == BKP_DR3)  || \
                       ((DR) == BKP_DR4)  || ((DR) == BKP_DR5)  || ((DR) == BKP_DR6)  || \
                       ((DR) == BKP_DR7)  || ((DR) == BKP_DR8)  || ((DR) == BKP_DR9)  || \
                       ((DR) == BKP_DR10) || ((DR) == BKP_DR11) || ((DR) == BKP_DR12) || \
                       ((DR) == BKP_DR13) || ((DR) == BKP_DR14) || ((DR) == BKP_DR15) || \
                       ((DR) == BKP_DR16) || ((DR) == BKP_DR17) || ((DR) == BKP_DR18) || \
                       ((DR) == BKP_DR19) || ((DR) == BKP_DR20) || ((DR) == BKP_DR21) || \
                       ((DR) == BKP_DR22) || ((DR) == BKP_DR23) || ((DR) == BKP_DR24) || \
                       ((DR) == BKP_DR25) || ((DR) == BKP_DR26) || ((DR) == BKP_DR27) || \
                       ((DR) == BKP_DR28) || ((DR) == BKP_DR29) || ((DR) == BKP_DR30) || \
                       ((DR) == BKP_DR31) || ((DR) == BKP_DR32) || ((DR) == BKP_DR33) || \
                       ((DR) == BKP_DR34) || ((DR) == BKP_DR35) || ((DR) == BKP_DR36) || \
                       ((DR) == BKP_DR37) || ((DR) == BKP_DR38) || ((DR) == BKP_DR39) || \
                       ((DR) == BKP_DR40) || ((DR) == BKP_DR41) || ((DR) == BKP_DR42))

#define IS_BKP_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x7F)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Functions
  * @{
  */
//将外设 BKP的全部寄存器重设为缺省值 
void BKP_DeInit(void);
//设置侵入检测管脚的有效电平 
/*BKP_TamperPinLevel_High  侵入检测管脚高电平有效 
 *BKP_TamperPinLevel_Low  侵入检测管脚低电平有效 */
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel);
//使能或者失能管脚的侵入检测功能
void BKP_TamperPinCmd(FunctionalState NewState);
//使能或者失能侵入检测中断 
void BKP_ITConfig(FunctionalState NewState);
//选择在侵入检测管脚上输出的 RTC时钟源 
/*BKP_RTCOutputSource_None  侵入检测管脚上无 RTC输出 
 *BKP_RTCOutputSource_CalibClock  侵入检测管脚上输出，其时钟频率为 RTC 时钟除以 64
 *BKP_RTCOutputSource_Alarm  侵入检测管脚上输出 RTC 闹钟脉冲 
 *BKP_RTCOutputSource_Second  侵入检测管脚上输出 RTC 秒脉冲 */
void BKP_RTCOutputConfig(uint16_t BKP_RTCOutputSource);
//设置 RTC 时钟校准值 
//CalibrationValue：RTC 时钟校准值 该参数允许取值范围为 0 到0x7F 
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue);
//向指定的后备寄存器中写入用户程序数据 	data写入的值
/*BKP_DR1  选中数据寄存器 1 
 *BKP_DR2  选中数据寄存器 2 
 *BKP_DR3  选中数据寄存器 3 
 *BKP_DR4  选中数据寄存器 4 
 *BKP_DR5  选中数据寄存器 5 
 *BKP_DR6  选中数据寄存器 6 
 *BKP_DR7  选中数据寄存器 7 
 *BKP_DR8  选中数据寄存器 8 
 *BKP_DR9  选中数据寄存器 9 
 *BKP_DR10  选中数据寄存器 10 */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);
//从指定的后备寄存器中读出数据 ，还是上面那些寄存器
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);
//检查侵入检测管脚事件的标志位被设置与否
FlagStatus BKP_GetFlagStatus(void);
//清除侵入检测管脚事件的待处理标志位 
void BKP_ClearFlag(void);
//检查侵入检测中断发生与否 
ITStatus BKP_GetITStatus(void);
//清除侵侵入检测中断的待处理位 
void BKP_ClearITPendingBit(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_BKP_H */
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
