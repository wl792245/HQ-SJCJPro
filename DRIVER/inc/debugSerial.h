#ifndef __DEBUGSERIAL_H_
#define __DEBUGSERIAL_H_

#include "sys.h"
#include "stdio.h"

//串口中断数据缓冲区
//串口接受缓冲区最大长度,不要超过255
#define SERIAL_MAX_LENGTH	250	


//串口接收数据缓冲区,大小根据宏来定
extern u8 serial_Buffer[SERIAL_MAX_LENGTH];


//串口接收数据的状态数据
//0-7位标识接收数据长度,最大为SERIAL_MAX_LENGTH
//14位标识接收到'\r'
//15位标识接收到'\n',也就是接收完成
extern u16 serial_Buffer_Length;

extern u8 receiveMode;
extern u8 receiveExpectCount;//串口期望接收长度



void Debug_Serial_Init(u32 baud);

void Debug_Serial_Send_Char(u8 val);

void Debug_Serial_Send_Buffer(u8* buffer,u8 length);



#endif










