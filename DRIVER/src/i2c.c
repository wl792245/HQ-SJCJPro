/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	i2c.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-08-16
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		IIC��������
	*
	*	�޸ļ�¼��	V1.1�����IIC����˫���߻���
	*					  ����Ӳ��IIC����
	************************************************************
	************************************************************
	************************************************************
**/

//����
#include "i2c.h"
#include "usart.h"
#include "delay.h"

#if(HW_I2C == 1)
#include "mcu_i2c.h"
#include "mcu_nvic.h"
#endif


static _Bool i2c_busy[2] = {IIC_OK, IIC_OK};


IIC_INFO iic_info;


#if(HW_I2C == 0)
const GPIO_LIST i2c_gpio_list[4] = {
										{GPIOB, GPIO_Pin_6, "iic1_scl"},
										{GPIOB, GPIO_Pin_7, "iic1_sda"},
										
										{GPIOB, GPIO_Pin_10, "iic_scl"},
										{GPIOB, GPIO_Pin_11, "iic_sda"},
									};
#endif

/*
************************************************************
*	�������ƣ�	IIC_SpeedCtl
*
*	�������ܣ�	IIC�ٶȿ���
*
*	��ڲ�����	speed����ʱ����
*
*	���ز�����	��
*
*	˵����		��λ��΢��
************************************************************
*/
void IIC_SpeedCtl(unsigned short speed)
{

	iic_info.speed = speed;

}

/*
************************************************************
*	�������ƣ�	IIC_IsBusReady
*
*	�������ܣ�	��ѯ�����Ƿ����
*
*	��ڲ�����	i2c_x��I2C1 �� I2C2
*
*	���ز�����	0-����	1-δ����
*
*	˵����		
************************************************************
*/
_Bool IIC_IsBusReady(I2C_TypeDef *i2c_x)
{
	
	_Bool result = IIC_Err;
#ifdef OS	
	RTOS_ENTER_CRITICAL();

#endif
	if(i2c_busy[i2c_x == I2C1 ? 0 : 1] == IIC_OK
#if(HW_I2C == 1)
		//&& I2C_GetFlagStatus(i2c_x, I2C_FLAG_BUSY) == RESET
#endif
	)
	{
		i2c_busy[i2c_x == I2C1 ? 0 : 1] = IIC_Err;
		
		result = IIC_OK;
	}
#ifdef OS	
	RTOS_EXIT_CRITICAL();
#endif
	return result;

}

/*
************************************************************
*	�������ƣ�	IIC_FreeBus
*
*	�������ܣ�	�ͷ�����
*
*	��ڲ�����	i2c_x��I2C1 �� I2C2
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_FreeBus(I2C_TypeDef *i2c_x)
{
#ifdef OS	
	RTOS_ENTER_CRITICAL();
#endif	
	i2c_busy[i2c_x == I2C1 ? 0 : 1] = IIC_OK;
#ifdef OS		
	RTOS_EXIT_CRITICAL();
#endif
}

/*
************************************************************
*	�������ƣ�	IIC_Init
*
*	�������ܣ�	���IIC����IO��ʼ��
*
*	��ڲ�����	i2c_x��I2C1 �� I2C2
*
*	���ز�����	0-�ɹ�	1-ʧ��
*
*	˵����		ʹ�ÿ�©��ʽ���������Բ����л�IO�ڵ������������
************************************************************
*/
_Bool IIC_Init(I2C_TypeDef *i2c_x)
{
	
	_Bool result = 1;
	
#if(HW_I2C == 1)
	MCU_I2C_Init(i2c_x, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit, 350000, I2C_DutyCycle_2, I2C_Mode_I2C, 0);
	I2C_Cmd(i2c_x, ENABLE);
	
	I2C_ITConfig(i2c_x, I2C_IT_ERR, ENABLE);				//ʹ�ܴ����ж�
	
	if(i2c_x == I2C1)
		MCU_NVIC_Init(I2C1_ER_IRQn, ENABLE, 1, 0);
	else if(i2c_x == I2C2)
		MCU_NVIC_Init(I2C2_ER_IRQn, ENABLE, 1, 0);
	
	if(IIC_IsBusReady(i2c_x) == IIC_OK)
	{
		result = 0;
		
		IIC_FreeBus(i2c_x);
		
		UsartPrintf(USART_DEBUG, "Tips:	I2C%d is Ready\r\n", i2c_x == I2C1 ? 1 : 2);
	}
	else
		UsartPrintf(USART_DEBUG, "Tips:	I2C%d is Err\r\n", i2c_x == I2C1 ? 1 : 2);
#else
	if(i2c_x == I2C1)
	{
		result = 0;
		
		MCU_GPIO_Init(i2c_gpio_list[0].gpio_group, i2c_gpio_list[0].gpio_pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, i2c_gpio_list[0].gpio_name);
		MCU_GPIO_Init(i2c_gpio_list[1].gpio_group, i2c_gpio_list[1].gpio_pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, i2c_gpio_list[1].gpio_name);
	}
	else if(i2c_x == I2C2)
	{
		result = 0;
		
		MCU_GPIO_Init(i2c_gpio_list[2].gpio_group, i2c_gpio_list[2].gpio_pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, i2c_gpio_list[2].gpio_name);
		MCU_GPIO_Init(i2c_gpio_list[3].gpio_group, i2c_gpio_list[3].gpio_pin, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, i2c_gpio_list[3].gpio_name);
	}
	
	iic_info.i2c_x = i2c_x;
	
	IIC_SpeedCtl(5);
	
	SDA_H;													//����SDA�ߣ����ڿ���״̬
	SCL_H;													//����SCL�ߣ����ڿ���״̬
#endif
	
	return result;

}

/*
************************************************************
*	�������ƣ�	IIC_Start
*
*	�������ܣ�	���IIC��ʼ�ź�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_Start(void)
{
	
#if(HW_I2C == 1)
	I2C_GenerateSTART(iic_info.i2c_x, ENABLE);
	while(I2C_CheckEvent(iic_info.i2c_x, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);	//�ȴ�EV5
#else
	SDA_H;						//����SDA��
	SCL_H;						//����SCL��
	DelayUs(iic_info.speed);	//��ʱ���ٶȿ���
	
	SDA_L;						//��SCL��Ϊ��ʱ��SDA��һ���½��ش���ʼ�ź�
	DelayUs(iic_info.speed);	//��ʱ���ٶȿ���
	SCL_L;						//ǯסSCL�ߣ��Ա㷢������
#endif

}

/*
************************************************************
*	�������ƣ�	IIC_Stop
*
*	�������ܣ�	���IICֹͣ�ź�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_Stop(void)
{

#if(HW_I2C == 1)
	I2C_GenerateSTOP(iic_info.i2c_x, ENABLE);
#else
	SDA_L;										//����SDA��
	SCL_L;										//����SCL��
	DelayUs(iic_info.speed);					//��ʱ���ٶȿ���
	
	SCL_H;										//����SCL��
	SDA_H;										//����SDA�ߣ���SCL��Ϊ��ʱ��SDA��һ�������ش���ֹͣ�ź�
	DelayUs(iic_info.speed);
#endif

}

/*
************************************************************
*	�������ƣ�	IIC_WaitAck
*
*	�������ܣ�	���IIC�ȴ�Ӧ��
*
*	��ڲ�����	time_out����ʱʱ��
*
*	���ز�����	��
*
*	˵����		��λ��΢��
************************************************************
*/
_Bool IIC_WaitAck(unsigned int time_out)
{
	
	
#if(HW_I2C == 1)
	
#else
	SDA_H;DelayUs(iic_info.speed);			//����SDA��
	SCL_H;DelayUs(iic_info.speed);			//����SCL��
	
	while(SDA_R)							//�������SDA��Ϊ1����ȴ���Ӧ���ź�Ӧ��0
	{
		if(--time_out == 0)
		{
			UsartPrintf(USART1, "WaitAck TimeOut\r\n");

			IIC_Stop();						//��ʱδ�յ�Ӧ����ֹͣ����
			
			return IIC_Err;					//����ʧ��
		}
		
		DelayUs(iic_info.speed);
	}
	
	SCL_L;									//����SCL�ߣ��Ա�����շ�����
#endif
	
	return IIC_OK;							//���سɹ�
	
}

/*
************************************************************
*	�������ƣ�	IIC_Ack
*
*	�������ܣ�	���IIC����һ��Ӧ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��SDA��Ϊ��ʱ��SCL��һ�������������һ��Ӧ���ź�
************************************************************
*/
void IIC_Ack(void)
{
	
#if(HW_I2C == 1)
	I2C_AcknowledgeConfig(iic_info.i2c_x, ENABLE);		//�����Զ�Ӧ��
#else
	SCL_L;												//����SCL��
	SDA_L;												//����SDA��
	DelayUs(iic_info.speed);
	SCL_H;												//����SCL��
	DelayUs(iic_info.speed);
	SCL_L;												//����SCL��
#endif
	
}

/*
************************************************************
*	�������ƣ�	IIC_NAck
*
*	�������ܣ�	���IIC����һ�Ǹ�Ӧ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��SDA��Ϊ��ʱ��SCL��һ�������������һ����Ӧ���ź�
************************************************************
*/
void IIC_NAck(void)
{
	
#if(HW_I2C == 1)
	I2C_AcknowledgeConfig(iic_info.i2c_x, DISABLE);		//�ر��Զ�Ӧ��
#else
	SCL_L;												//����SCL��
	SDA_H;												//����SDA��
	DelayUs(iic_info.speed);
	SCL_H;												//����SCL��
	DelayUs(iic_info.speed);
	SCL_L;												//����SCL��
#endif
	
}

/*
************************************************************
*	�������ƣ�	IIC_SendByte
*
*	�������ܣ�	���IIC����һ���ֽ�
*
*	��ڲ�����	byte����Ҫ���͵��ֽ�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
_Bool IIC_SendByte(unsigned char byte)
{

#if(HW_I2C == 1)
	I2C_SendData(iic_info.i2c_x, byte);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);	//�ȴ�EV8_2
#else
	unsigned char count = 0;
	
    SCL_L;												//����ʱ�ӿ�ʼ���ݴ���
	
    for(; count < 8; count++)							//ѭ��8�Σ�ÿ�η���һ��bit
    {
		if(byte & 0x80)									//�������λ
			SDA_H;
		else
			SDA_L;
		
		byte <<= 1;										//byte����1λ
		
		DelayUs(iic_info.speed);
		SCL_H;
		DelayUs(iic_info.speed);
		SCL_L;
    }
#endif
	
	return IIC_OK;

}

/*
************************************************************
*	�������ƣ�	IIC_RecvByte
*
*	�������ܣ�	���IIC����һ���ֽ�
*
*	��ڲ�����	��
*
*	���ز�����	���յ����ֽ�����
*
*	˵����		
************************************************************
*/
unsigned char IIC_RecvByte(void)
{
	
#if(HW_I2C == 1)
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_RECEIVED);	//�ȴ�EV7
	return I2C_ReceiveData(iic_info.i2c_x);
#else
	unsigned char count = 0, receive = 0;
	
	SDA_H;												//����SDA�ߣ���©״̬�£����������Ա��ȡ����
	
    for(; count < 8; count++ )							//ѭ��8�Σ�ÿ�η���һ��bit
	{
		SCL_L;
		DelayUs(iic_info.speed);
		SCL_H;
		
        receive <<= 1;									//����һλ
		
        if(SDA_R)										//���SDA��Ϊ1����receive����������ÿ���������Ƕ�bit0��+1��Ȼ����һ��ѭ����������һ��
			receive++;
		
		DelayUs(iic_info.speed);
    }
	
    return receive;
#endif
	
}

/*
************************************************************
*	�������ƣ�	I2C_WriteByte
*
*	�������ܣ�	���IICдһ������
*
*	��ڲ�����	slave_addr���ӻ���ַ
*				reg_addr���Ĵ�����ַ
*				byte����Ҫд�������
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*byte�ǻ���д�����ݵı����ĵ�ַ����Ϊ��Щ�Ĵ���ֻ��Ҫ�����¼Ĵ�����������Ҫд��ֵ
************************************************************
*/
_Bool I2C_WriteByte(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *byte)
{
	
	unsigned char addr = 0;

	addr = slave_addr << 1;			//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	iic_info.i2c_x = i2c_x;

	IIC_Start();					//��ʼ�ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Transmitter);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//�ȴ�EV6
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);					//�ȴ�EV8
#else
	IIC_SendByte(addr);				//�����豸��ַ(д)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	IIC_SendByte(reg_addr);			//���ͼĴ�����ַ
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
	
	if(byte)
	{
		IIC_SendByte(*byte);		//��������
		if(IIC_WaitAck(5000))		//�ȴ�Ӧ��
			return IIC_Err;
	}
	
	IIC_Stop();						//ֹͣ�ź�
	
	return IIC_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_ReadByte
*
*	�������ܣ�	���IIC��ȡһ���ֽ�
*
*	��ڲ�����	i2c_x��I2C��
*				slave_addr���ӻ���ַ
*				reg_addr���Ĵ�����ַ
*				val����Ҫ��ȡ�����ݻ���
*
*	���ز�����	0-�ɹ�		1-ʧ��
*
*	˵����		val��һ����������ĵ�ַ
************************************************************
*/
_Bool I2C_ReadByte(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *val)
{
	
	unsigned char addr = 0;

    addr = slave_addr << 1;			//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	iic_info.i2c_x = i2c_x;

	IIC_Start();					//��ʼ�ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Transmitter);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//�ȴ�EV6
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);					//�ȴ�EV8
#else
	IIC_SendByte(addr);				//�����豸��ַ(д)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	IIC_SendByte(reg_addr);			//���ͼĴ�����ַ
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
	
	IIC_Start();					//�����ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Receiver);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);				//�ȴ�EV6
#else
	IIC_SendByte(addr + 1);			//�����豸��ַ(��)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	*val = IIC_RecvByte();			//����
	IIC_NAck();						//����һ����Ӧ���źţ������ȡ����
	
	IIC_Stop();						//ֹͣ�ź�
	
	return IIC_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_WriteBytes
*
*	�������ܣ�	���IICд�������
*
*	��ڲ�����	slave_addr���ӻ���ַ
*				reg_addr���Ĵ�����ַ
*				buf����Ҫд������ݻ���
*				num�����ݳ���
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*buf��һ�������ָ��һ����������ָ��
************************************************************
*/
_Bool I2C_WriteBytes(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *buf, unsigned char num)
{

	unsigned char addr = 0;

	addr = slave_addr << 1;			//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	iic_info.i2c_x = i2c_x;
	
	IIC_Start();					//��ʼ�ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Transmitter);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//�ȴ�EV6
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);					//�ȴ�EV8
#else
	IIC_SendByte(addr);				//�����豸��ַ(д)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	IIC_SendByte(reg_addr);			//���ͼĴ�����ַ
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
	
	while(num--)					//ѭ��д������
	{
		IIC_SendByte(*buf);			//��������
		if(IIC_WaitAck(5000))		//�ȴ�Ӧ��
			return IIC_Err;
		
		buf++;						//����ָ��ƫ�Ƶ���һ��
		
		DelayUs(10);
	}
	
	IIC_Stop();						//ֹͣ�ź�
	
	return IIC_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_ReadBytes
*
*	�������ܣ�	���IIC���������
*
*	��ڲ�����	slave_addr���ӻ���ַ
*				reg_addr���Ĵ�����ַ
*				buf����Ҫ��ȡ�����ݻ���
*				num�����ݳ���
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*buf��һ�������ָ��һ����������ָ��
************************************************************
*/
_Bool I2C_ReadBytes(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *buf, unsigned char num)
{
	
	unsigned short addr = 0;

    addr = slave_addr << 1;			//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	iic_info.i2c_x = i2c_x;

	IIC_Start();					//��ʼ�ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Transmitter);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//�ȴ�EV6
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);					//�ȴ�EV8
#else
	IIC_SendByte(addr);				//�����豸��ַ(д)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	IIC_SendByte(reg_addr);			//���ͼĴ�����ַ
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
	
	IIC_Start();					//�����ź�
	
#if(HW_I2C == 1)
	I2C_Send7bitAddress(iic_info.i2c_x, addr, I2C_Direction_Receiver);
	IIC_CHECK_EVENT(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);				//�ȴ�EV6
	
	IIC_Ack();																//�����Զ�Ӧ��
#else
	IIC_SendByte(addr + 1);			//�����豸��ַ(��)
	if(IIC_WaitAck(5000))			//�ȴ�Ӧ��
		return IIC_Err;
#endif
	
	while(num--)
	{
		*buf = IIC_RecvByte();
		buf++;						//ƫ�Ƶ���һ�����ݴ洢��ַ
		
		if(num == 0)
			IIC_NAck();				//���һ��������Ҫ��NOACK
#if(HW_I2C == 0)
        else
			IIC_Ack();				//��ӦACK
#endif
	}
	
	IIC_Stop();
	
	return IIC_OK;

}

#if(HW_I2C == 1)
/*
************************************************************
*	�������ƣ�	I2C1_ER_IRQHandler
*
*	�������ܣ�	I2C1�����ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void I2C1_ER_IRQHandler(void)
{

	if(I2C_GetITStatus(I2C1, I2C_IT_BERR) == SET)			//���ߴ���
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
		
		UsartPrintf(USART_DEBUG, "I2C1 Bus Error\r\n");
		
		IIC_Stop();											//����ģʽ����£�Ӳ�����ͷ����ߣ�ͬʱ��Ӱ�쵱ǰ�Ĵ���״̬����ʱ����������Ƿ�Ҫ��ֹ��ǰ�Ĵ���
	}
	
	if(I2C_GetITStatus(I2C1, I2C_IT_ARLO) == SET)			//�ٲö�ʧ����
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_ARLO);
		
		UsartPrintf(USART_DEBUG, "I2C1 Arbitration lost Error\r\n");
		
		//I2C�ӿ��Զ��ص���ģʽ(M/SLλ�����).��I2C�ӿڶ�ʧ���ٲ�,�����޷���ͬһ����������Ӧ���Ĵӵ�ַ,����������Ӯ�����ߵ����豸��������ʼ����֮����Ӧ
		//Ӳ���ͷ�����
	}
	
	if(I2C_GetITStatus(I2C1, I2C_IT_AF) == SET)				//Ӧ�����
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
		
		UsartPrintf(USART_DEBUG, "I2C1 Acknowledge Error\r\n");
		
		IIC_Stop();											//����Ǵ�����ģʽ,�����������һ��ֹͣ����
	}
	
	if(I2C_GetITStatus(I2C1, I2C_IT_OVR) == SET)			//����/Ƿ�ش���
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_OVR);
		
		UsartPrintf(USART_DEBUG, "I2C1 Overrun/Underrun Error\r\n");
		
		I2C_ClearFlag(I2C2, I2C_FLAG_RXNE);					//�ڹ��ش���ʱ,���Ӧ���RxNEλ,������Ӧ�����·������һ�η��͵��ֽ�
	}
	
	if(I2C_GetITStatus(I2C1, I2C_IT_PECERR) == SET)			//PECУ�����
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_PECERR);
		
		UsartPrintf(USART_DEBUG, "I2C1 PEC Error\r\n");
		
		//У�����Ӱ��ͨ�Ź���
	}
	
	if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT) == SET)		//���߳�ʱ����
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_TIMEOUT);
		
		UsartPrintf(USART_DEBUG, "I2C1 TimeOut Error\r\n");
		
		IIC_Stop();											//�����ֹͣ����
	}

}

/*
************************************************************
*	�������ƣ�	I2C2_ER_IRQHandler
*
*	�������ܣ�	I2C2�����ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void I2C2_ER_IRQHandler(void)
{

	if(I2C_GetITStatus(I2C2, I2C_IT_BERR) == SET)			//���ߴ���
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_BERR);
		
		UsartPrintf(USART_DEBUG, "I2C2 Bus Error\r\n");
		
		IIC_Stop();											//����ģʽ�����,Ӳ�����ͷ�����,ͬʱ��Ӱ�쵱ǰ�Ĵ���״̬.��ʱ����������Ƿ�Ҫ��ֹ��ǰ�Ĵ���
	}
	
	if(I2C_GetITStatus(I2C2, I2C_IT_ARLO) == SET)			//�ٲö�ʧ����
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_ARLO);
		
		UsartPrintf(USART_DEBUG, "I2C2 Arbitration lost Error\r\n");
		
		//I2C�ӿ��Զ��ص���ģʽ(M/SLλ�����).��I2C�ӿڶ�ʧ���ٲ�,�����޷���ͬһ����������Ӧ���Ĵӵ�ַ,����������Ӯ�����ߵ����豸��������ʼ����֮����Ӧ
		//Ӳ���ͷ�����
	}
	
	if(I2C_GetITStatus(I2C2, I2C_IT_AF) == SET)				//Ӧ�����
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
		
		UsartPrintf(USART_DEBUG, "I2C2 Acknowledge Error\r\n");
		
		IIC_Stop();											//����Ǵ�����ģʽ,�����������һ��ֹͣ����
	}
	
	if(I2C_GetITStatus(I2C2, I2C_IT_OVR) == SET)			//����/Ƿ�ش���
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_OVR);
		
		UsartPrintf(USART_DEBUG, "I2C2 Overrun/Underrun Error\r\n");
		
		I2C_ClearFlag(I2C2, I2C_FLAG_RXNE);					//�ڹ��ش���ʱ,���Ӧ���RxNEλ,������Ӧ�����·������һ�η��͵��ֽ�
	}
	
	if(I2C_GetITStatus(I2C2, I2C_IT_PECERR) == SET)			//PECУ�����
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_PECERR);
		
		UsartPrintf(USART_DEBUG, "I2C2 PEC Error\r\n");
		
		//У�����Ӱ��ͨ�Ź���
	}
	
	if(I2C_GetITStatus(I2C2, I2C_IT_TIMEOUT) == SET)		//���߳�ʱ����
	{
		I2C_ClearITPendingBit(I2C2, I2C_IT_TIMEOUT);
		
		UsartPrintf(USART_DEBUG, "I2C2 TimeOut Error\r\n");
		
		IIC_Stop();											//�����ֹͣ����
	}

}
#endif
