#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG			USART1	//���Դ�ӡ��ʹ�õĴ�����

#define USART_TXBUFF_SIZE   250   		   		//���崮��2 ���ͻ�������С 1024�ֽ�
#define USART_RXBUFF_SIZE   250              //���崮��2 ���ջ�������С 1024�ֽ�

#define USART_DMA_TX_EN		1		//1-ʹ��DMA����		0-ʧ��DMA����

#define USART_DMA_RX_EN		1		//1-ʹ��DMA����		0-ʧ��DMA����


#define USART_TX_TYPE		0

#define USART_RX_TYPE		1


#define USART_OK			0

#define USART_Err			1


typedef struct
{

	u8 alter_buf[USART_RXBUFF_SIZE];
	u16 alter_count;
	_Bool rev_idle;

} ALTER_INFO;


extern ALTER_INFO alter_info;


_Bool USART_IsBusReady(USART_TypeDef *usart_x);

void USART_FreeBus(USART_TypeDef *usart_x);

void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);

void USARTx_ResetMemoryBaseAddr(USART_TypeDef *usart_x, unsigned int mem_addr, unsigned short num, _Bool type);

void Usart_SendString(USART_TypeDef *usart_x, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *usart_x, char *fmt,...);


#endif
