#include "systick.h"


static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

/*******************************************************************************
* 函 数 名         : delay_init
* 函数功能		     : 延时初始化
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void delay_init(void)
{

	#ifdef USE_OS 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
		u32 reload;
	#endif
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
		fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  
		 
	#ifdef USE_OS 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
		reload=SystemCoreClock/8000000;		//每秒钟的计数次数 单位为K	   
		reload*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
								//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
		fac_ms=1000/OS_TICKS_PER_SEC;//代表ucos可以延时的最少单位	   
		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
		SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
	#else
		fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   
	#endif
}						
/*******************************************************************************
* 函 数 名         : delay_us
* 函数功能		     : 延时函数，延时us
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达			//bit16置一说明时间到，跳出循环   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//void delay_us(u32 nus)  
//{  
//    uint8_t i;  
//      
//    while(nus--) for(i = 0; i < 8; i ++);  
//}  
/*******************************************************************************
* 函 数 名         : delay_ms
* 函数功能		     : 延时函数，延时ms
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void delay_ms(u16 nms)
{	 		 
	u32 temp;	
	//IWDG_Feed();	//4s喂一次看门狗
	
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
//void delay_ms(u16 nms)
//{
//	while(nms--) delay_us(1000);  
//}

/*******************************************************************************
* 函 数 名         : CPU_Reset
* 函数功能		     : CPU复位
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void	CPU_Reset(void)
{
	 //向AT24C02写入错误次数
	  //DeviceInfo.TerminalInfo.Fault
		__set_FAULTMASK(1);      // 关闭系统中断  
		/* system soft reset */
		NVIC_SystemReset();		// 函数在core_m3.c中
}
