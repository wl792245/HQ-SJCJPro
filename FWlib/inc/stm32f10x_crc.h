/**
  ******************************************************************************
  * @file    stm32f10x_crc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the CRC firmware 
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
#ifndef __STM32F10x_CRC_H
#define __STM32F10x_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */

/** @defgroup CRC_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Functions
  * @{
  */
//crc校验单元复位
void CRC_ResetDR(void);
//进行单个crc校验
uint32_t CRC_CalcCRC(uint32_t Data);
//多字节循环校验，因为在校验的时候cpu会停止写入，要给入校验长度
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
//获得crc校验结果
uint32_t CRC_GetCRC(void);
//向crc独立寄存器写入数据
void CRC_SetIDRegister(uint8_t IDValue);
//获得crc独立寄存器写入的数据
uint8_t CRC_GetIDRegister(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_CRC_H */
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
