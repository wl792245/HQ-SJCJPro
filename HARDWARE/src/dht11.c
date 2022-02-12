//单片机头文件
#include "stm32f10x.h"
#include "sys.h"
//单片机相关组件
#include "mcu_gpio.h"

//硬件驱动
#include "dht11.h"
#include "delay.h"




DHT11_Info dht11_info = {1,0.0,0.0,DHT11_Exist,DHT11_Get_TemperAndHumi};

const static GPIO_LIST dht11_gpio_list[1]={
											{GPIOA, GPIO_Pin_8, (void *)0},
											};


#define DHT11_IO_IN()  {dht11_gpio_list[0].gpio_group->CRH&=0XFFFFFFF0;dht11_gpio_list[0].gpio_group->CRH |= 8<<0;}
#define DHT11_IO_OUT() {dht11_gpio_list[0].gpio_group->CRH&=0XFFFFFFF0;dht11_gpio_list[0].gpio_group->CRH|=3<<0;}//输出，50MHz	
#define	DHT11_DQ_OUT 		PAout(8) //数据端口	PA8 
#define	DHT11_DQ_IN  		PAin(8)  //数据端口	PA8 

static _Bool dht11_busy[1] = {DHT11_OK};

/*
************************************************************
*	函数名称：	DHT11_DelayMs
*
*	函数功能：	延时毫秒
*
*	入口参数：	time：延时时间
*
*	返回参数：	无
*
*	说明：		基于当前延时时基
************************************************************
*/
__inline static void DHT11_DelayMs(unsigned int time)
{

	DelayXms(time);
}
/*
************************************************************
*	函数名称：	DHT11_DelayUs
*
*	函数功能：	延时微秒
*
*	入口参数：	time：延时时间
*
*	返回参数：	无
*
*	说明：		基于当前延时时基
************************************************************
*/
__inline static void DHT11_DelayUs(unsigned int time)
{

	DelayUs(time);
}
/*
************************************************************
*	函数名称：	DHT11_IsBusReady
*
*	函数功能：	查询总线是否就绪
*
*	入口参数：	无
*
*	返回参数：	0-就绪	1-未就绪
*
*	说明：		
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
*	函数名称：	DHT11_FreeBus
*
*	函数功能：	释放总线
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void DHT11_FreeBus(void)
{
	dht11_busy[0] = DHT11_OK;
}

/*
************************************************************
*	函数名称：	DHT11_Reset
*
*	函数功能：	DHT11设备复位
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
__inline static void DHT11_Reset(void)
{
	if(DHT11_IsBusReady() == DHT11_OK)
	{
		DHT11_IO_OUT(); //SET OUTPUT
		DHT11_DQ_OUT=0; 	//拉低DQ
		DHT11_DelayMs(20);    	//拉低至少18ms
		DHT11_DQ_OUT=1; 	//DQ=1 
		DHT11_DelayUs(30);    //主机拉高20~40us
		DHT11_FreeBus();
	}
	
}
/*
************************************************************
*	函数名称：	DHT11_Check
*
*	函数功能：	DHT11检测
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
__inline static _Bool DHT11_Check(void)
{
		u8 retry=0;
		DHT11_IO_IN();//SET INPUT	 
		while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
		{
			retry++;
			DHT11_DelayUs(1);
		}	 
		if(retry>=100)return 1;
		else retry=0;
		while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
		{
			retry++;
			DHT11_DelayUs(1);
		}
		if(retry>=100)return 1;	 //返回1:未检测到DHT11的存在
		return 0;                //返回0:存在

}
/*
************************************************************
*	函数名称：	DHT11_Exist
*
*	函数功能：	DHT11设备在线检查
*
*	入口参数：	无
*
*	返回参数：	0-未检测到	1-检测到
*
*	说明：		
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
*	函数名称：	DHT11_Read_Bit
*
*	函数功能：	从DHT11读取一个位
*
*	入口参数：	无
*
*	返回参数：返回Bit值
*
*	说明：		
************************************************************
*/
__inline static _Bool DHT11_Read_Bit(void)     
{
	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		DHT11_DelayUs(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		DHT11_DelayUs(1);
	}
	DHT11_DelayUs(40);//等待40us
	if(DHT11_DQ_IN)return 1; //返回值：1
	else return 0;		     //返回值：0
}
/*
************************************************************
*	函数名称：	DHT11_Read_Byte
*
*	函数功能：	从DHT11读取一个字节
*
*	入口参数：	无
*
*	返回参数：返回Byte
*
*	说明：		
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
  return dat;//返回值：读到的数据
}
/*
************************************************************
*	函数名称：	DHT11_Get_TemperAndHumi
*
*	函数功能：	获取4字节数据
*
*	入口参数：	buf：目标字节
*
*	返回参数：无
*
*	说明：		
************************************************************
*/
_Bool DHT11_Get_TemperAndHumi(void)
{
	u8 buf[5];
	u8 i;
	DHT11_Reset();//复位DHT11总线完成
	if(DHT11_Check()==0)
	{
		//检测到DHT11响应
		for(i=0;i<5;i++)//读取40位数据
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
