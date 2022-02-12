#ifndef __SYS_H_
#define __SYS_H_
#include "stm32f10x.h"

//位段模式操作IO口定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)   

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  


//系统中断优先级分组 2位抢占 2位子优先级
//系统默认中断分组
#define BOARD_NVIC_GROUP	NVIC_PriorityGroup_2	//2BIT 2BIT

//NVIC中断分组以及系统中中断优先级的定义
typedef enum PREEMPTPRIORITY
{
	PreemptPriority0 = 0,
	PreemptPriority1 = 1,
	PreemptPriority2 = 2,
	PreemptPriority3 = 3
}PREEMPTPRIORITY;

typedef enum SUBPRIORITY
{
	SubPriority0 = 0,
	SubPriority1 = 1,
	SubPriority2 = 2,
	SubPriority3 = 3
}SUBPRIORITY;

//串口一中断优先级,优先级最高,防止命令丢失
#define UART1_PreemptionPriority		PreemptPriority0
#define UART1_SubPriority				SubPriority0

//实时时钟中断,优先级最低,防止命令丢失
#define RTC_PreemptionPriority			PreemptPriority3
#define RTC_SubPriority					SubPriority3

//窗口看门狗中断,优先级最低,防止命令丢失
#define WWDG_PreemptionPriority			PreemptPriority2
#define WWDG_SubPriority				SubPriority3

//按键中断优先级,可以低一些
#define KEYUP_PreemptionPriority		PreemptPriority3
#define KEYUP_SubPriority				SubPriority3
//按键中断优先级
#define KEYDOWN_PreemptionPriority		PreemptPriority3
#define KEYDOWN_SubPriority				PreemptPriority3
//按键中断优先级
#define KEYRIGHT_PreemptionPriority		PreemptPriority3
#define KEYRIGHT_SubPriority			PreemptPriority3
//按键中断优先级
#define KEYLEFT_PreemptionPriority		PreemptPriority3
#define KEYLEFT_SubPriority				PreemptPriority3
//外部捕获红外中断优先级
#define REMOTE_PreemptionPriority		PreemptPriority1
#define REMOTE_SubPriority				PreemptPriority1
//485通讯中断优先级
#define RS485_PreemptionPriority		PreemptPriority3
#define RS485_SubPriority				PreemptPriority3



//USB唤醒中断优先级 ,居中的优先级
#define USB_WAKE_UP_PreemptionPriority	PreemptPriority2	//usb唤醒中断
#define USB_WAKE_UP_SubPriority			SubPriority2
//USB低优先级中断的中断优先级,仅仅低于串口
#define USB_LP_PreemptionPriority		PreemptPriority1	//usb低优先级中断(对应usb高优先级中断)
#define USB_LP_SubPriority				PreemptPriority1
//USB高优先级的中断优先级
#define USB_HP_PreemptionPriority		PreemptPriority1	//usb高优先级中断(对应usb高优先级中断)
#define USB_HP_SubPriority				PreemptPriority1

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址
void RCC_Configuration(void);


#endif



