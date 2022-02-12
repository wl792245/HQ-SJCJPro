#ifndef _I2C_H_
#define _I2C_H_


//单片机头文件
#include "stm32f10x.h"


#define HW_I2C		1				//1-硬件I2C		0-软件I2C


#define IIC_OK		0

#define IIC_Err		1


#if(HW_I2C == 1)
#define IIC_CHECK_EVENT(event)		{																				\
										unsigned short time_out = 5000;												\
										while((I2C_CheckEvent(iic_info.i2c_x, event) == ERROR) && --time_out)		\
											DelayUs(1);																\
										if(!time_out)																\
											return IIC_Err;															\
									}
#else
//单片机相关组件
#include "mcu_gpio.h"

//I2C1	SDA		PB7
//I2C1	SCL		PB6
//I2C2	SDA		PB11
//I2C2	SCL		PB10
#define SDA_H	iic_info.i2c_x == I2C1 ? (i2c_gpio_list[1].gpio_group->BSRR = i2c_gpio_list[1].gpio_pin) : (i2c_gpio_list[3].gpio_group->BSRR = i2c_gpio_list[3].gpio_pin)
#define SDA_L	iic_info.i2c_x == I2C1 ? (i2c_gpio_list[1].gpio_group->BRR = i2c_gpio_list[1].gpio_pin) : (i2c_gpio_list[3].gpio_group->BRR = i2c_gpio_list[3].gpio_pin)
#define SDA_R	MCU_GPIO_Input_Read(iic_info.i2c_x == I2C1 ? (i2c_gpio_list[1].gpio_name) : (i2c_gpio_list[3].gpio_name))

#define SCL_H	iic_info.i2c_x == I2C1 ? (i2c_gpio_list[0].gpio_group->BSRR = i2c_gpio_list[0].gpio_pin) : (i2c_gpio_list[2].gpio_group->BSRR = i2c_gpio_list[2].gpio_pin)
#define SCL_L	iic_info.i2c_x == I2C1 ? (i2c_gpio_list[0].gpio_group->BRR = i2c_gpio_list[0].gpio_pin) : (i2c_gpio_list[2].gpio_group->BRR = i2c_gpio_list[2].gpio_pin)


extern const GPIO_LIST i2c_gpio_list[4];
#endif


typedef struct
{
	
	I2C_TypeDef *i2c_x;
	
	unsigned short speed;

} IIC_INFO;

extern IIC_INFO iic_info;


_Bool IIC_Init(I2C_TypeDef *i2c_x);

void IIC_SpeedCtl(unsigned short speed);

_Bool IIC_IsBusReady(I2C_TypeDef *i2c_x);

void IIC_FreeBus(I2C_TypeDef *i2c_x);

_Bool I2C_WriteByte(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *byte);

_Bool I2C_ReadByte(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *val);

_Bool I2C_WriteBytes(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *buf, unsigned char num);

_Bool I2C_ReadBytes(I2C_TypeDef *i2c_x, unsigned char slave_addr, unsigned char reg_addr, unsigned char *buf, unsigned char num);

void IIC_Start(void);

void IIC_Stop(void);

_Bool IIC_WaitAck(unsigned int time_out);

void IIC_Ack(void);

void IIC_NAck(void);

_Bool IIC_SendByte(unsigned char byte);

unsigned char IIC_RecvByte(void);


#endif
