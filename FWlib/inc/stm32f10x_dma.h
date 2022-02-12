/**
  ******************************************************************************
  * @file    stm32f10x_dma.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the DMA firmware 
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
#ifndef __STM32F10x_DMA_H
#define __STM32F10x_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/** @defgroup DMA_Exported_Types
  * @{
  */

/** 
  * @brief  DMA Init structure definition
  */

typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Channelx.定义 DMA外设基地址  */

  uint32_t DMA_MemoryBaseAddr;     /*!< Specifies the memory base address for DMAy Channelx.定义 DMA内存基地址  */

  uint32_t DMA_DIR;                /*!< Specifies if the peripheral is the source or destination
  												DMA_DIR_PeripheralDST 外设作为数据传输的目的地
												DMA_DIR_PeripheralSRC  外设作为数据传输的来源 
                                        This parameter can be a value of @ref DMA_data_transfer_direction */

  uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Channel. 
                                        DMA_BufferSize 用以定义指定 DMA通道的 DMA缓存的大小，单位为数据单位。根据传输方向，数据单
												位等于结构中参数 DMA_PeripheralDataSize 或者参数 DMA_MemoryDataSize 的值
												The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                        or DMA_MemoryDataSize members depending in the transfer direction. */

  uint32_t DMA_PeripheralInc;      /*!< Specifies whether the Peripheral address register is incremented or not.
  											DMA_PeripheralInc_Enable  外设地址寄存器递增 
											DMA_PeripheralInc_Disable  外设地址寄存器不变 
                                        This parameter can be a value of @ref DMA_peripheral_incremented_mode */

  uint32_t DMA_MemoryInc;          /*!< Specifies whether the memory address register is incremented or not.
  										DMA_MemoryInc 用来设定内存地址寄存器递增与否
										DMA_PeripheralInc_Enable  内存地址寄存器递增 
										DMA_PeripheralInc_Disable  内存地址寄存器不变 
                                        This parameter can be a value of @ref DMA_memory_incremented_mode */

  uint32_t DMA_PeripheralDataSize; /*!< Specifies the Peripheral data width.
  										DMA_PeripheralDataSize设定了外设数据宽度
										DMA_PeripheralDataSize_Byte  数据宽度为 8 位 
										DMA_PeripheralDataSize_HalfWord  数据宽度为 16 位 
										DMA_PeripheralDataSize_Word  数据宽度为 32 位 
                                        This parameter can be a value of @ref DMA_peripheral_data_size */

  uint32_t DMA_MemoryDataSize;     /*!< Specifies the Memory data width.
  										DMA_MemoryDataSize 设定了外设数据宽度
										DMA_MemoryDataSize_Byte  数据宽度为 8 位 
										DMA_MemoryDataSize_HalfWord  数据宽度为 16 位 
										DMA_MemoryDataSize_Word  数据宽度为 32 位 
                                        This parameter can be a value of @ref DMA_memory_data_size */

  uint32_t DMA_Mode;               /*!< Specifies the operation mode of the DMAy Channelx.
  										DMA_Mode设置了 CAN的工作模式
										DMA_Mode_Circular  工作在循环缓存模式 
										DMA_Mode_Normal  工作在正常缓存模式
										当指定 DMA通道数据传输配置为内存到内存时，不能使用循环缓存模式
                                        This parameter can be a value of @ref DMA_circular_normal_mode.
                                        @note: The circular buffer mode cannot be used if the memory-to-memory
                                              data transfer is configured on the selected Channel */

  uint32_t DMA_Priority;           /*!< Specifies the software priority for the DMAy Channelx.
  										DMA_Priority设定 DMA通道 x 的软件优先级
										DMA_Priority_VeryHigh  DMA通道 x拥有非常高优先
										DMA_Priority_High  DMA通道 x拥有高优先级 
										DMA_Priority_Medium  DMA通道 x拥有中优先级 
										DMA_Priority_Low  DMA通道 x拥有低优先级 
                                        This parameter can be a value of @ref DMA_priority_level */

  uint32_t DMA_M2M;                /*!< Specifies if the DMAy Channelx will be used in memory-to-memory transfer.
  										DMA_M2M使能 DMA通道的内存到内存传输
										DMA_M2M_Enable  DMA通道 x设置为内存到内存传输 
										DMA_M2M_Disable  DMA通道 x没有设置为内存到内存传输 
                                        This parameter can be a value of @ref DMA_memory_to_memory */
}DMA_InitTypeDef;

/**
  * @}
  */

/** @defgroup DMA_Exported_Constants
  * @{
  */

#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA1_Channel1) || \
                                   ((PERIPH) == DMA1_Channel2) || \
                                   ((PERIPH) == DMA1_Channel3) || \
                                   ((PERIPH) == DMA1_Channel4) || \
                                   ((PERIPH) == DMA1_Channel5) || \
                                   ((PERIPH) == DMA1_Channel6) || \
                                   ((PERIPH) == DMA1_Channel7) || \
                                   ((PERIPH) == DMA2_Channel1) || \
                                   ((PERIPH) == DMA2_Channel2) || \
                                   ((PERIPH) == DMA2_Channel3) || \
                                   ((PERIPH) == DMA2_Channel4) || \
                                   ((PERIPH) == DMA2_Channel5))

/** @defgroup DMA_data_transfer_direction 
  * @{
  */

#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))
/**
  * @}
  */

/** @defgroup DMA_peripheral_incremented_mode 
  * @{
  */

#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_memory_incremented_mode 
  * @{
  */

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_peripheral_data_size 
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_memory_data_size 
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_circular_normal_mode 
  * @{
  */

#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))
/**
  * @}
  */

/** @defgroup DMA_priority_level 
  * @{
  */

#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))
/**
  * @}
  */

/** @defgroup DMA_memory_to_memory 
  * @{
  */

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))

/**
  * @}
  */

/** @defgroup DMA_interrupts_definition 
  * @{
  */

#define DMA_IT_TC                          ((uint32_t)0x00000002)
#define DMA_IT_HT                          ((uint32_t)0x00000004)
#define DMA_IT_TE                          ((uint32_t)0x00000008)
#define IS_DMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFFF1) == 0x00) && ((IT) != 0x00))

#define DMA1_IT_GL1                        ((uint32_t)0x00000001)
#define DMA1_IT_TC1                        ((uint32_t)0x00000002)
#define DMA1_IT_HT1                        ((uint32_t)0x00000004)
#define DMA1_IT_TE1                        ((uint32_t)0x00000008)
#define DMA1_IT_GL2                        ((uint32_t)0x00000010)
#define DMA1_IT_TC2                        ((uint32_t)0x00000020)
#define DMA1_IT_HT2                        ((uint32_t)0x00000040)
#define DMA1_IT_TE2                        ((uint32_t)0x00000080)
#define DMA1_IT_GL3                        ((uint32_t)0x00000100)
#define DMA1_IT_TC3                        ((uint32_t)0x00000200)
#define DMA1_IT_HT3                        ((uint32_t)0x00000400)
#define DMA1_IT_TE3                        ((uint32_t)0x00000800)
#define DMA1_IT_GL4                        ((uint32_t)0x00001000)
#define DMA1_IT_TC4                        ((uint32_t)0x00002000)
#define DMA1_IT_HT4                        ((uint32_t)0x00004000)
#define DMA1_IT_TE4                        ((uint32_t)0x00008000)
#define DMA1_IT_GL5                        ((uint32_t)0x00010000)
#define DMA1_IT_TC5                        ((uint32_t)0x00020000)
#define DMA1_IT_HT5                        ((uint32_t)0x00040000)
#define DMA1_IT_TE5                        ((uint32_t)0x00080000)
#define DMA1_IT_GL6                        ((uint32_t)0x00100000)
#define DMA1_IT_TC6                        ((uint32_t)0x00200000)
#define DMA1_IT_HT6                        ((uint32_t)0x00400000)
#define DMA1_IT_TE6                        ((uint32_t)0x00800000)
#define DMA1_IT_GL7                        ((uint32_t)0x01000000)
#define DMA1_IT_TC7                        ((uint32_t)0x02000000)
#define DMA1_IT_HT7                        ((uint32_t)0x04000000)
#define DMA1_IT_TE7                        ((uint32_t)0x08000000)

#define DMA2_IT_GL1                        ((uint32_t)0x10000001)
#define DMA2_IT_TC1                        ((uint32_t)0x10000002)
#define DMA2_IT_HT1                        ((uint32_t)0x10000004)
#define DMA2_IT_TE1                        ((uint32_t)0x10000008)
#define DMA2_IT_GL2                        ((uint32_t)0x10000010)
#define DMA2_IT_TC2                        ((uint32_t)0x10000020)
#define DMA2_IT_HT2                        ((uint32_t)0x10000040)
#define DMA2_IT_TE2                        ((uint32_t)0x10000080)
#define DMA2_IT_GL3                        ((uint32_t)0x10000100)
#define DMA2_IT_TC3                        ((uint32_t)0x10000200)
#define DMA2_IT_HT3                        ((uint32_t)0x10000400)
#define DMA2_IT_TE3                        ((uint32_t)0x10000800)
#define DMA2_IT_GL4                        ((uint32_t)0x10001000)
#define DMA2_IT_TC4                        ((uint32_t)0x10002000)
#define DMA2_IT_HT4                        ((uint32_t)0x10004000)
#define DMA2_IT_TE4                        ((uint32_t)0x10008000)
#define DMA2_IT_GL5                        ((uint32_t)0x10010000)
#define DMA2_IT_TC5                        ((uint32_t)0x10020000)
#define DMA2_IT_HT5                        ((uint32_t)0x10040000)
#define DMA2_IT_TE5                        ((uint32_t)0x10080000)

#define IS_DMA_CLEAR_IT(IT) (((((IT) & 0xF0000000) == 0x00) || (((IT) & 0xEFF00000) == 0x00)) && ((IT) != 0x00))

#define IS_DMA_GET_IT(IT) (((IT) == DMA1_IT_GL1) || ((IT) == DMA1_IT_TC1) || \
                           ((IT) == DMA1_IT_HT1) || ((IT) == DMA1_IT_TE1) || \
                           ((IT) == DMA1_IT_GL2) || ((IT) == DMA1_IT_TC2) || \
                           ((IT) == DMA1_IT_HT2) || ((IT) == DMA1_IT_TE2) || \
                           ((IT) == DMA1_IT_GL3) || ((IT) == DMA1_IT_TC3) || \
                           ((IT) == DMA1_IT_HT3) || ((IT) == DMA1_IT_TE3) || \
                           ((IT) == DMA1_IT_GL4) || ((IT) == DMA1_IT_TC4) || \
                           ((IT) == DMA1_IT_HT4) || ((IT) == DMA1_IT_TE4) || \
                           ((IT) == DMA1_IT_GL5) || ((IT) == DMA1_IT_TC5) || \
                           ((IT) == DMA1_IT_HT5) || ((IT) == DMA1_IT_TE5) || \
                           ((IT) == DMA1_IT_GL6) || ((IT) == DMA1_IT_TC6) || \
                           ((IT) == DMA1_IT_HT6) || ((IT) == DMA1_IT_TE6) || \
                           ((IT) == DMA1_IT_GL7) || ((IT) == DMA1_IT_TC7) || \
                           ((IT) == DMA1_IT_HT7) || ((IT) == DMA1_IT_TE7) || \
                           ((IT) == DMA2_IT_GL1) || ((IT) == DMA2_IT_TC1) || \
                           ((IT) == DMA2_IT_HT1) || ((IT) == DMA2_IT_TE1) || \
                           ((IT) == DMA2_IT_GL2) || ((IT) == DMA2_IT_TC2) || \
                           ((IT) == DMA2_IT_HT2) || ((IT) == DMA2_IT_TE2) || \
                           ((IT) == DMA2_IT_GL3) || ((IT) == DMA2_IT_TC3) || \
                           ((IT) == DMA2_IT_HT3) || ((IT) == DMA2_IT_TE3) || \
                           ((IT) == DMA2_IT_GL4) || ((IT) == DMA2_IT_TC4) || \
                           ((IT) == DMA2_IT_HT4) || ((IT) == DMA2_IT_TE4) || \
                           ((IT) == DMA2_IT_GL5) || ((IT) == DMA2_IT_TC5) || \
                           ((IT) == DMA2_IT_HT5) || ((IT) == DMA2_IT_TE5))

/**
  * @}
  */

/** @defgroup DMA_flags_definition 
  * @{
  */
#define DMA1_FLAG_GL1                      ((uint32_t)0x00000001)
#define DMA1_FLAG_TC1                      ((uint32_t)0x00000002)
#define DMA1_FLAG_HT1                      ((uint32_t)0x00000004)
#define DMA1_FLAG_TE1                      ((uint32_t)0x00000008)
#define DMA1_FLAG_GL2                      ((uint32_t)0x00000010)
#define DMA1_FLAG_TC2                      ((uint32_t)0x00000020)
#define DMA1_FLAG_HT2                      ((uint32_t)0x00000040)
#define DMA1_FLAG_TE2                      ((uint32_t)0x00000080)
#define DMA1_FLAG_GL3                      ((uint32_t)0x00000100)
#define DMA1_FLAG_TC3                      ((uint32_t)0x00000200)
#define DMA1_FLAG_HT3                      ((uint32_t)0x00000400)
#define DMA1_FLAG_TE3                      ((uint32_t)0x00000800)
#define DMA1_FLAG_GL4                      ((uint32_t)0x00001000)
#define DMA1_FLAG_TC4                      ((uint32_t)0x00002000)
#define DMA1_FLAG_HT4                      ((uint32_t)0x00004000)
#define DMA1_FLAG_TE4                      ((uint32_t)0x00008000)
#define DMA1_FLAG_GL5                      ((uint32_t)0x00010000)
#define DMA1_FLAG_TC5                      ((uint32_t)0x00020000)
#define DMA1_FLAG_HT5                      ((uint32_t)0x00040000)
#define DMA1_FLAG_TE5                      ((uint32_t)0x00080000)
#define DMA1_FLAG_GL6                      ((uint32_t)0x00100000)
#define DMA1_FLAG_TC6                      ((uint32_t)0x00200000)
#define DMA1_FLAG_HT6                      ((uint32_t)0x00400000)
#define DMA1_FLAG_TE6                      ((uint32_t)0x00800000)
#define DMA1_FLAG_GL7                      ((uint32_t)0x01000000)
#define DMA1_FLAG_TC7                      ((uint32_t)0x02000000)
#define DMA1_FLAG_HT7                      ((uint32_t)0x04000000)
#define DMA1_FLAG_TE7                      ((uint32_t)0x08000000)

#define DMA2_FLAG_GL1                      ((uint32_t)0x10000001)
#define DMA2_FLAG_TC1                      ((uint32_t)0x10000002)
#define DMA2_FLAG_HT1                      ((uint32_t)0x10000004)
#define DMA2_FLAG_TE1                      ((uint32_t)0x10000008)
#define DMA2_FLAG_GL2                      ((uint32_t)0x10000010)
#define DMA2_FLAG_TC2                      ((uint32_t)0x10000020)
#define DMA2_FLAG_HT2                      ((uint32_t)0x10000040)
#define DMA2_FLAG_TE2                      ((uint32_t)0x10000080)
#define DMA2_FLAG_GL3                      ((uint32_t)0x10000100)
#define DMA2_FLAG_TC3                      ((uint32_t)0x10000200)
#define DMA2_FLAG_HT3                      ((uint32_t)0x10000400)
#define DMA2_FLAG_TE3                      ((uint32_t)0x10000800)
#define DMA2_FLAG_GL4                      ((uint32_t)0x10001000)
#define DMA2_FLAG_TC4                      ((uint32_t)0x10002000)
#define DMA2_FLAG_HT4                      ((uint32_t)0x10004000)
#define DMA2_FLAG_TE4                      ((uint32_t)0x10008000)
#define DMA2_FLAG_GL5                      ((uint32_t)0x10010000)
#define DMA2_FLAG_TC5                      ((uint32_t)0x10020000)
#define DMA2_FLAG_HT5                      ((uint32_t)0x10040000)
#define DMA2_FLAG_TE5                      ((uint32_t)0x10080000)

#define IS_DMA_CLEAR_FLAG(FLAG) (((((FLAG) & 0xF0000000) == 0x00) || (((FLAG) & 0xEFF00000) == 0x00)) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA1_FLAG_GL1) || ((FLAG) == DMA1_FLAG_TC1) || \
                               ((FLAG) == DMA1_FLAG_HT1) || ((FLAG) == DMA1_FLAG_TE1) || \
                               ((FLAG) == DMA1_FLAG_GL2) || ((FLAG) == DMA1_FLAG_TC2) || \
                               ((FLAG) == DMA1_FLAG_HT2) || ((FLAG) == DMA1_FLAG_TE2) || \
                               ((FLAG) == DMA1_FLAG_GL3) || ((FLAG) == DMA1_FLAG_TC3) || \
                               ((FLAG) == DMA1_FLAG_HT3) || ((FLAG) == DMA1_FLAG_TE3) || \
                               ((FLAG) == DMA1_FLAG_GL4) || ((FLAG) == DMA1_FLAG_TC4) || \
                               ((FLAG) == DMA1_FLAG_HT4) || ((FLAG) == DMA1_FLAG_TE4) || \
                               ((FLAG) == DMA1_FLAG_GL5) || ((FLAG) == DMA1_FLAG_TC5) || \
                               ((FLAG) == DMA1_FLAG_HT5) || ((FLAG) == DMA1_FLAG_TE5) || \
                               ((FLAG) == DMA1_FLAG_GL6) || ((FLAG) == DMA1_FLAG_TC6) || \
                               ((FLAG) == DMA1_FLAG_HT6) || ((FLAG) == DMA1_FLAG_TE6) || \
                               ((FLAG) == DMA1_FLAG_GL7) || ((FLAG) == DMA1_FLAG_TC7) || \
                               ((FLAG) == DMA1_FLAG_HT7) || ((FLAG) == DMA1_FLAG_TE7) || \
                               ((FLAG) == DMA2_FLAG_GL1) || ((FLAG) == DMA2_FLAG_TC1) || \
                               ((FLAG) == DMA2_FLAG_HT1) || ((FLAG) == DMA2_FLAG_TE1) || \
                               ((FLAG) == DMA2_FLAG_GL2) || ((FLAG) == DMA2_FLAG_TC2) || \
                               ((FLAG) == DMA2_FLAG_HT2) || ((FLAG) == DMA2_FLAG_TE2) || \
                               ((FLAG) == DMA2_FLAG_GL3) || ((FLAG) == DMA2_FLAG_TC3) || \
                               ((FLAG) == DMA2_FLAG_HT3) || ((FLAG) == DMA2_FLAG_TE3) || \
                               ((FLAG) == DMA2_FLAG_GL4) || ((FLAG) == DMA2_FLAG_TC4) || \
                               ((FLAG) == DMA2_FLAG_HT4) || ((FLAG) == DMA2_FLAG_TE4) || \
                               ((FLAG) == DMA2_FLAG_GL5) || ((FLAG) == DMA2_FLAG_TC5) || \
                               ((FLAG) == DMA2_FLAG_HT5) || ((FLAG) == DMA2_FLAG_TE5))
/**
  * @}
  */

/** @defgroup DMA_Buffer_Size 
  * @{
  */

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions
  * @{
  */
//将 DMA的通道 x 寄存器重设为缺省值 
//DMA Channelx：x 可以是1，2…，或者 7 来选择DMA通道 x 
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);
//根据 DMA_InitStruct 中指定的参数初始化 DMA的通道 x 寄存器 
//DMA Channelx：x 可以是1，2…，或者 7 来选择DMA通道 x 
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
//把 DMA_InitStruct 中的每一个参数按缺省值填入 
/*DMA_PeripheralBaseAddr  0 
 *DMA_MemoryBaseAddr  0 
 *DMA_DIR  DMA_DIR_Peripheral
 *DMA_BufferSize  0 
 *DMA_PeripheralInc  DMA_PeripheralInc_D
 *DMA_MemoryInc  DMA_MemoryInc_Dis
 *DMA_PeripheralDataSize  DMA_PeripheralDataS
 *DMA_MemoryDataSize  DMA_MemoryDataSiz
 *DMA_Mode  DMA_Mode_Normal 
 *DMA_Priority  DMA_Priority_Low 
 *DMA_M2M  DMA_M2M_Disable */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
//使能或者失能指定的通道 x 
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);
//使能或者失能指定的通道 x 中断 
/*DMA_IT_TC  传输完成中断屏蔽 
 *DMA_IT_HT  传输过半中断屏蔽 
 *DMA_IT_TE  传输错误中断屏蔽 */
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState);

void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t DataNumber); 
//返回当前 DMA通道 x剩余的待传输数据数目
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);
//检查指定的 DMA通道 x标志位设置与否 
/*DMA_FLAG_GL1  通道 1 全局标志位 
 *DMA_FLAG_TC1  通道 1 传输完成标志位 
 *DMA_FLAG_HT1  通道 1 传输过半标志位 
 *DMA_FLAG_TE1  通道 1 传输错误标志位 
 *DMA_FLAG_GL2  通道 2 全局标志位 
 *DMA_FLAG_TC2  通道 2 传输完成标志位 
 *DMA_FLAG_HT2  通道 2 传输过半标志位 
 *DMA_FLAG_TE2  通道 2 传输错误标志位 
 *DMA_FLAG_GL3  通道 3 全局标志位 
 *DMA_FLAG_TC3  通道 3 传输完成标志位 
 *DMA_FLAG_HT3  通道 3 传输过半标志位 
 *DMA_FLAG_TE3  通道 3 传输错误标志位 
 *DMA_FLAG_GL4  通道 4 全局标志位 
 *DMA_FLAG_TC4  通道 4 传输完成标志位 
 *DMA_FLAG_HT4  通道 4 传输过半标志位 
 *DMA_FLAG_TE4  通道 4 传输错误标志位 
 *DMA_FLAG_GL5  通道 5 全局标志位 
 *DMA_FLAG_TC5  通道 5 传输完成标志位 
 *DMA_FLAG_HT5  通道 5 传输过半标志位 
 *DMA_FLAG_TE5  通道 5 传输错误标志位 
 *DMA_FLAG_GL6  通道 6 全局标志位 
 *DMA_FLAG_TC6  通道 6 传输完成标志位 
 *DMA_FLAG_HT6  通道 6 传输过半标志位 
 *DMA_FLAG_TE6  通道 6 传输错误标志位 
 *DMA_FLAG_GL7  通道 7 全局标志位 
 *DMA_FLAG_TC7  通道 7 传输完成标志位 
 *DMA_FLAG_HT7  通道 7 传输过半标志位 
 *DMA_FLAG_TE7  通道 7 传输错误标志位 */
FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG);
//清除 DMA通道 x 待处理标志位 ，参数就是上一个函数的参数
void DMA_ClearFlag(uint32_t DMAy_FLAG);
//检查指定的 DMA通道 x中断发生与否 
/*DMA_IT_GL1  通道 1 全局中断 
 *DMA_IT_TC1  通道 1 传输完成中断 
 *DMA_IT_HT1  通道 1 传输过半中断 
 *DMA_IT_TE1  通道 1 传输错误中断 
 *DMA_IT_GL2  通道 2 全局中断 
 *DMA_IT_TC2  通道 2 传输完成中断 
 *DMA_IT_HT2  通道 2 传输过半中断 
 *DMA_IT_TE2  通道 2 传输错误中断 
 *DMA_IT_GL3  通道 3 全局中断 
 *DMA_IT_TC3  通道 3 传输完成中断 
 *DMA_IT_HT3  通道 3 传输过半中断 
 *DMA_IT_TE3  通道 3 传输错误中断 
 *DMA_IT_GL4  通道 4 全局中断 
 *DMA_IT_TC4  通道 4 传输完成中断 
 *DMA_IT_HT4  通道 4 传输过半中断 
 *DMA_IT_TE4  通道 4 传输错误中断 
 *DMA_IT_GL5  通道 5 全局中断 
 *DMA_IT_TC5  通道 5 传输完成中断 
 *DMA_IT_HT5  通道 5 传输过半中断 
 *DMA_IT_TE5  通道 5 传输错误中断 
 *DMA_IT_GL6  通道 6 全局中断 
 *DMA_IT_TC6  通道 6 传输完成中断 
 *DMA_IT_HT6  通道 6 传输过半中断 
 *DMA_IT_TE6  通道 6 传输错误中断 
 *DMA_IT_GL7  通道 7 全局中断 
 *DMA_IT_TC7  通道 7 传输完成中断 
 *DMA_IT_HT7  通道 7 传输过半中断 
 *DMA_IT_TE7  通道 7 传输错误中断 */
ITStatus DMA_GetITStatus(uint32_t DMAy_IT);
//清除 DMA通道 x 中断待处理标志位 参数就是上一个函数的参数
void DMA_ClearITPendingBit(uint32_t DMAy_IT);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_DMA_H */
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
