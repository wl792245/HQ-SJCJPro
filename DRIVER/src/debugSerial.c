#include "debugSerial.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}


void Debug_Serial_Init(u32 baud)
{
	u32 NVICtemp    = 0;       			 	//用于NVIC控制器的变量

	GPIO_InitTypeDef GPIO_InitStructure;	//gpio初始化变量

	USART_InitTypeDef USART_InitStructure;	//串口初始化结构体

	//下面这些都是一样的配置，所以放在判断之外
	USART_InitStructure.USART_BaudRate = baud;//波特率;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	 //8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //奇偶校验失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   //发送与接收都使能

	//使能串口所有GPIO模块时钟,uart模块时钟,并使能AFIO模块时钟 ,uart1位于apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//io模块配置pa9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//设置TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//pa10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//设置RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//io配置完成

	//中断优先级变量解码,在sys文件中设置的宏定义
	NVICtemp = NVIC_EncodePriority(BOARD_NVIC_GROUP , UART1_PreemptionPriority , UART1_SubPriority);    
	NVIC_SetPriority(USART1_IRQn,NVICtemp);          //设置中断优先级


	USART_Init(USART1, &USART_InitStructure);		//初始化USART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	 //接收中断使能
	NVIC_EnableIRQ(USART1_IRQn);                     //使能对应中断 
	USART_Cmd(USART1, ENABLE);		//使能串口模块	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

void Debug_Serial_Send_Char(u8 val)
{
	/* 向相应串口发送数据*/
	USART_SendData(USART1, val);
	/* 等待数据发送完成 ，传输完成标志置位*/
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void Debug_Serial_Send_Buffer(u8* buffer,u8 length)
{
	u8 i = 0;
	for(i = 0; i < length; i++)
	{
		Debug_Serial_Send_Char(buffer[i]);
	}
	printf("\r\n");
}



//串口接收缓冲区
u8 serial_Buffer[SERIAL_MAX_LENGTH] = {0};
//串口接收数据长度
u16 serial_Buffer_Length = 0;

u8 receiveMode = 0;//接收参数的中断处理模型,为0的时候是命令模式,为1的时候为下载模式
u8 receiveExpectCount = 0;//串口期望接收长度


//串口中断处理
static void SerialRecv(u8 ch)
{
	if(receiveMode == 0)
	{
		if((serial_Buffer_Length&0x8000) == 0x8000)//已经接收完成,系统还没处理
		{
			serial_Buffer_Length |= 0x8000;//退出
		}
		else if((serial_Buffer_Length&0x4000) == 0x4000)//接收到回车还没接收到换行
		{
			if(ch == '\n')serial_Buffer_Length |= 0x8000;
			else 
			{
				//一帧接受失败
				serial_Buffer_Length = 0;
			}
		}
		else
		{
			if((serial_Buffer_Length&0xff) < SERIAL_MAX_LENGTH)
			{
				if(ch == '\r')serial_Buffer_Length |= 0x4000;
				else 
				{
					serial_Buffer[(serial_Buffer_Length&0xff)] = ch;
					serial_Buffer_Length++;
				}
			}
			else
			{
				//一帧接受失败
				serial_Buffer_Length = 0;
			}
		}
	}
	else
	{
		//下载模式,只控制字符串的量,数据的第一位是该数据包的长度,接收到这么多长度,接收完成位置一
		//注意,在这种模式下,清除serial_Buffer_Length之前应当清除receiveExpectCount的值
		if(receiveExpectCount == 0)//期望下载为0,第一个数就是期望下载数
		{
			receiveExpectCount = ch;
		}
		else
		{
			if((serial_Buffer_Length&0x8000) == 0x8000)//已经接收完成,系统还没处理,此时不接收数据
			{
				serial_Buffer_Length |= 0x8000;//退出
			}
			else
			{
				serial_Buffer[(serial_Buffer_Length&0xff)] = ch;//接收数据并保存
				serial_Buffer_Length++;
				if((serial_Buffer_Length&0xff) == receiveExpectCount)//接收到了期望长度的数据
				{
					serial_Buffer_Length |= 0x8000;//一包接收完成标志
				}
			}
		}
		
	}
}


void USART1_IRQHandler(void)
{
	u8 ch = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//检查中断发生
	{
		ch = (u8)USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//清除中断
		//Debug_Serial_Send_Char(ch);				//将收到的数据发送出去
		SerialRecv(ch);							//处理中断数据
	}
}



