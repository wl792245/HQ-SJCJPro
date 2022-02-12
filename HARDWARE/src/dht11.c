//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"
#include "sys.h"
//��Ƭ��������
#include "mcu_gpio.h"

//Ӳ������
#include "dht11.h"
#include "delay.h"




DHT11_Info dht11_info = {1,0.0,0.0,DHT11_Exist,DHT11_Get_TemperAndHumi};

const static GPIO_LIST dht11_gpio_list[1]={
											{GPIOA, GPIO_Pin_8, (void *)0},
											};


#define DHT11_IO_IN()  {dht11_gpio_list[0].gpio_group->CRH&=0XFFFFFFF0;dht11_gpio_list[0].gpio_group->CRH |= 8<<0;}
#define DHT11_IO_OUT() {dht11_gpio_list[0].gpio_group->CRH&=0XFFFFFFF0;dht11_gpio_list[0].gpio_group->CRH|=3<<0;}//�����50MHz	
#define	DHT11_DQ_OUT 		PAout(8) //���ݶ˿�	PA8 
#define	DHT11_DQ_IN  		PAin(8)  //���ݶ˿�	PA8 

static _Bool dht11_busy[1] = {DHT11_OK};

/*
************************************************************
*	�������ƣ�	DHT11_DelayMs
*
*	�������ܣ�	��ʱ����
*
*	��ڲ�����	time����ʱʱ��
*
*	���ز�����	��
*
*	˵����		���ڵ�ǰ��ʱʱ��
************************************************************
*/
__inline static void DHT11_DelayMs(unsigned int time)
{

	DelayXms(time);
}
/*
************************************************************
*	�������ƣ�	DHT11_DelayUs
*
*	�������ܣ�	��ʱ΢��
*
*	��ڲ�����	time����ʱʱ��
*
*	���ز�����	��
*
*	˵����		���ڵ�ǰ��ʱʱ��
************************************************************
*/
__inline static void DHT11_DelayUs(unsigned int time)
{

	DelayUs(time);
}
/*
************************************************************
*	�������ƣ�	DHT11_IsBusReady
*
*	�������ܣ�	��ѯ�����Ƿ����
*
*	��ڲ�����	��
*
*	���ز�����	0-����	1-δ����
*
*	˵����		
************************************************************
*/
_Bool DHT11_IsBusReady(void)
{
	_Bool result = DHT11_Err;
	if(dht11_busy[0] == DHT11_OK)
	{
		dht11_busy[0] = DHT11_Err;
		
		result = DHT11_OK;
	}
	return result;
}

/*
************************************************************
*	�������ƣ�	DHT11_FreeBus
*
*	�������ܣ�	�ͷ�����
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void DHT11_FreeBus(void)
{
	dht11_busy[0] = DHT11_OK;
}

/*
************************************************************
*	�������ƣ�	DHT11_Reset
*
*	�������ܣ�	DHT11�豸��λ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
__inline static void DHT11_Reset(void)
{
	if(DHT11_IsBusReady() == DHT11_OK)
	{
		DHT11_IO_OUT(); //SET OUTPUT
		DHT11_DQ_OUT=0; 	//����DQ
		DHT11_DelayMs(20);    	//��������18ms
		DHT11_DQ_OUT=1; 	//DQ=1 
		DHT11_DelayUs(30);    //��������20~40us
		DHT11_FreeBus();
	}
	
}
/*
************************************************************
*	�������ƣ�	DHT11_Check
*
*	�������ܣ�	DHT11���
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
__inline static _Bool DHT11_Check(void)
{
		u8 retry=0;
		DHT11_IO_IN();//SET INPUT	 
		while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
		{
			retry++;
			DHT11_DelayUs(1);
		}	 
		if(retry>=100)return 1;
		else retry=0;
		while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
		{
			retry++;
			DHT11_DelayUs(1);
		}
		if(retry>=100)return 1;	 //����1:δ��⵽DHT11�Ĵ���
		return 0;                //����0:����

}
/*
************************************************************
*	�������ƣ�	DHT11_Exist
*
*	�������ܣ�	DHT11�豸���߼��
*
*	��ڲ�����	��
*
*	���ز�����	0-δ��⵽	1-��⵽
*
*	˵����		
************************************************************
*/
_Bool DHT11_Exist(void)
{
	MCU_GPIO_Init(dht11_gpio_list[0].gpio_group,dht11_gpio_list[0].gpio_pin,GPIO_Mode_Out_PP, GPIO_Speed_50MHz, dht11_gpio_list[0].gpio_name);
	DHT11_Reset();
	dht11_info.device_ok = DHT11_Check();
	return dht11_info.device_ok;
}
/*
************************************************************
*	�������ƣ�	DHT11_Read_Bit
*
*	�������ܣ�	��DHT11��ȡһ��λ
*
*	��ڲ�����	��
*
*	���ز���������Bitֵ
*
*	˵����		
************************************************************
*/
__inline static _Bool DHT11_Read_Bit(void)     
{
	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		DHT11_DelayUs(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		DHT11_DelayUs(1);
	}
	DHT11_DelayUs(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1; //����ֵ��1
	else return 0;		     //����ֵ��0
}
/*
************************************************************
*	�������ƣ�	DHT11_Read_Byte
*
*	�������ܣ�	��DHT11��ȡһ���ֽ�
*
*	��ڲ�����	��
*
*	���ز���������Byte
*
*	˵����		
************************************************************
*/
__inline static u8 DHT11_Read_Byte(void)
{
	u8 i,dat;
  dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
  }						    
  return dat;//����ֵ������������
}
/*
************************************************************
*	�������ƣ�	DHT11_Get_TemperAndHumi
*
*	�������ܣ�	��ȡ4�ֽ�����
*
*	��ڲ�����	buf��Ŀ���ֽ�
*
*	���ز�������
*
*	˵����		
************************************************************
*/
_Bool DHT11_Get_TemperAndHumi(void)
{
	u8 buf[5];
	u8 i;
	DHT11_Reset();//��λDHT11�������
	if(DHT11_Check()==0)
	{
		//��⵽DHT11��Ӧ
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			dht11_info.humidity=buf[0]+buf[1]/10.0;
			dht11_info.tempreture = buf[2]+buf[3]/10.0;
		}
		else
			return 1;
	}
	else
		return 1;
	return 0;
}
