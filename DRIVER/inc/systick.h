#ifndef _systick_H
#define _systick_H

#include "stm32f10x.h"

void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void CPU_Reset(void);
void PC_Delay(void);
void Heart_Delay(void);
#endif
