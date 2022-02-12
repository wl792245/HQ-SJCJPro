#include "debugSerial.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}


void Debug_Serial_Init(u32 baud)
{
	u32 NVICtemp    = 0;       			 	//����NVIC�������ı���

	GPIO_InitTypeDef GPIO_InitStructure;	//gpio��ʼ������

	USART_InitTypeDef USART_InitStructure;	//���ڳ�ʼ���ṹ��

	//������Щ����һ�������ã����Է����ж�֮��
	USART_InitStructure.USART_BaudRate = baud;//������;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	 //8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //��żУ��ʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   //��������ն�ʹ��

	//ʹ�ܴ�������GPIOģ��ʱ��,uartģ��ʱ��,��ʹ��AFIOģ��ʱ�� ,uart1λ��apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ioģ������pa9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//����TX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//pa10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//����RX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//io�������

	//�ж����ȼ���������,��sys�ļ������õĺ궨��
	NVICtemp = NVIC_EncodePriority(BOARD_NVIC_GROUP , UART1_PreemptionPriority , UART1_SubPriority);    
	NVIC_SetPriority(USART1_IRQn,NVICtemp);          //�����ж����ȼ�


	USART_Init(USART1, &USART_InitStructure);		//��ʼ��USART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	 //�����ж�ʹ��
	NVIC_EnableIRQ(USART1_IRQn);                     //ʹ�ܶ�Ӧ�ж� 
	USART_Cmd(USART1, ENABLE);		//ʹ�ܴ���ģ��	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

void Debug_Serial_Send_Char(u8 val)
{
	/* ����Ӧ���ڷ�������*/
	USART_SendData(USART1, val);
	/* �ȴ����ݷ������ ��������ɱ�־��λ*/
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



//���ڽ��ջ�����
u8 serial_Buffer[SERIAL_MAX_LENGTH] = {0};
//���ڽ������ݳ���
u16 serial_Buffer_Length = 0;

u8 receiveMode = 0;//���ղ������жϴ���ģ��,Ϊ0��ʱ��������ģʽ,Ϊ1��ʱ��Ϊ����ģʽ
u8 receiveExpectCount = 0;//�����������ճ���


//�����жϴ���
static void SerialRecv(u8 ch)
{
	if(receiveMode == 0)
	{
		if((serial_Buffer_Length&0x8000) == 0x8000)//�Ѿ��������,ϵͳ��û����
		{
			serial_Buffer_Length |= 0x8000;//�˳�
		}
		else if((serial_Buffer_Length&0x4000) == 0x4000)//���յ��س���û���յ�����
		{
			if(ch == '\n')serial_Buffer_Length |= 0x8000;
			else 
			{
				//һ֡����ʧ��
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
				//һ֡����ʧ��
				serial_Buffer_Length = 0;
			}
		}
	}
	else
	{
		//����ģʽ,ֻ�����ַ�������,���ݵĵ�һλ�Ǹ����ݰ��ĳ���,���յ���ô�೤��,�������λ��һ
		//ע��,������ģʽ��,���serial_Buffer_Length֮ǰӦ�����receiveExpectCount��ֵ
		if(receiveExpectCount == 0)//��������Ϊ0,��һ������������������
		{
			receiveExpectCount = ch;
		}
		else
		{
			if((serial_Buffer_Length&0x8000) == 0x8000)//�Ѿ��������,ϵͳ��û����,��ʱ����������
			{
				serial_Buffer_Length |= 0x8000;//�˳�
			}
			else
			{
				serial_Buffer[(serial_Buffer_Length&0xff)] = ch;//�������ݲ�����
				serial_Buffer_Length++;
				if((serial_Buffer_Length&0xff) == receiveExpectCount)//���յ����������ȵ�����
				{
					serial_Buffer_Length |= 0x8000;//һ��������ɱ�־
				}
			}
		}
		
	}
}


void USART1_IRQHandler(void)
{
	u8 ch = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//����жϷ���
	{
		ch = (u8)USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//����ж�
		//Debug_Serial_Send_Char(ch);				//���յ������ݷ��ͳ�ȥ
		SerialRecv(ch);							//�����ж�����
	}
}



