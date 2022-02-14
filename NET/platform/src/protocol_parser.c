/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	protocol_parser.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2019-07-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		协议解析器
	*
	*	修改记录：	MODBUS
	************************************************************
	************************************************************
	************************************************************
**/

//协议
#include "modbuskit.h"

//驱动
#include "usart.h"

//
#include "protocol_parser.h"

//C库
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define PRINT		DebugPrintf


#if(SEND_PASER_EN == 1 || RECV_PASER_EN == 1)

static char print_buf[64];


//==========================================================
//	函数名称：	print_hexstring
//
//	函数功能：	将hex转为字符串
//
//	入口参数：	hex：hex数据
//				len：数据长度
//
//	返回参数：	返回转换后的缓存指针
//
//	说明：		
//==========================================================
static char *print_hexstring(const unsigned char *hex, unsigned char len)
{
	
	char temp_buf[8];

	unsigned char i = 0;
	
	if(hex == (void *)0 || len == 0)									//参数合法性
		return (void *)0;
	
	for(; i < sizeof(print_buf); i++)									//清缓存
		print_buf[i] = 0;
	
	print_buf[0] = '[';
	
	for(i = 0; i < len; i++)
	{
		snprintf(temp_buf, sizeof(temp_buf), "0x%02X", hex[i]);			//格式化
		strcat(print_buf, temp_buf);									//复制到缓存末尾
		
		if(i < len - 1)
			strcat(print_buf, " ");										//添加空格
	}
	
	strcat(print_buf, "]");
	
	return print_buf;

}

//==========================================================
//	函数名称：	print_ascii
//
//	函数功能：	将hex转为ASCII字符
//
//	入口参数：	hex：hex数据
//				len：数据长度
//
//	返回参数：	返回转换后的缓存指针
//
//	说明：		
//==========================================================
static char *print_ascii(const unsigned char *hex, unsigned char len)
{

	char temp_buf[8];

	unsigned char i = 0;
	
	if(hex == (void *)0 || len == 0)									//参数合法性
		return (void *)0;
	
	for(; i < sizeof(print_buf); i++)									//清缓存
		print_buf[i] = 0;
	
	for(i = 0; i < len; i++)
	{
		if(isprint(hex[i]))												//是否为可打印字符
		{
			snprintf(temp_buf, sizeof(temp_buf), "%c", hex[i]);			//格式化
			strcat(print_buf, temp_buf);								//复制到缓存末尾
		}
	}
	
	return print_buf;

}

#endif

#if(SEND_PASER_EN == 1)

//==========================================================
//	函数名称：	check_num
//
//	函数功能：	检查是否为数字
//
//	入口参数：	str：待检测的字符串
//				len：数据长度
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
static _Bool check_num(const unsigned char *str, unsigned char len)
{
	
	if(str == (void *)0 || len == 0)		//参数合法性
		return 1;
	
	while(len--)
	{
		if(*str < '0' || *str > '9')		//判断是否为数字字符
			return 1;
		
		str++;
	}
	
	return 0;

}

//==========================================================
//	函数名称：	check_letter_num
//
//	函数功能：	检查字母和数字
//
//	入口参数：	str：待检测的字符串
//				len：数据长度
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
static _Bool check_letter_num(const unsigned char *str, unsigned char len)
{
	
	if(str == (void *)0 || len == 0)
		return 1;
	
	while(len--)
	{
		if((*str < '0' || *str > '9') && (*str < 'A' || *str > 'Z') && (*str < 'a' || *str > 'z'))
			return 1;
		
		str++;
	}
	
	return 0;

}

#endif

#if(SEND_PASER_EN == 1)

//==========================================================
//	函数名称：	Protocol_Parser_Connect
//
//	函数功能：	连接包协议解析输出打印
//
//	入口参数：	pro_buf：协议包
//
//	返回参数：	0-成功	其他-错误
//
//	说明：		
//==========================================================
static unsigned char Protocol_Parser_Connect(const unsigned char *pro_buf)
{

	unsigned char index = 0;								//index：pro_buf索引
	_Bool flag = 0;											//判断正误标志
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(strcmp((char *)pro_buf, "type") == 0)
		flag = 1;
	else
		flag = 0;
	
	PRINT("第%d~%d字节---%s:[%s]报文字段", index, index + 10, print_hexstring(pro_buf + index, 11), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf, 11));
	if(flag == 0)
		return 1;
	index += 11;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(strcmp((char *)pro_buf + index, "name") == 0)
		flag = 1;
	else
		flag = 0;
	
	PRINT("第%d~%d字节---%s:[%s]报文字段", index, index + 8, print_hexstring(pro_buf + index, 9), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 9));
	if(flag == 0)
		return 2;
	index += 9;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_num(pro_buf + 20, strlen((char *)pro_buf + 20)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("第%d~%d字节---%s:[%s]序列号", index, index + 11, print_hexstring(pro_buf + index, 12), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 12));
	if(flag == 0)
		return 3;
	index += 12;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_num(pro_buf + 32, strlen((char *)pro_buf + 32)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("第%d~%d字节---%s:[%s]密码", index, index + 8, print_hexstring(pro_buf + index, 9), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 9));
	if(flag == 0)
		return 4;
	index += 9;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_letter_num(pro_buf + 41, strlen((char *)pro_buf + 41)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("第%d~%d字节---%s:[%s]产品ID", index, index + 9, print_hexstring(pro_buf + index, 10), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 10));
	if(flag == 0)
		return 5;
	
	return 0;

}

//==========================================================
//	函数名称：	Protocol_Parser_Send
//
//	函数功能：	发送数据包协议解析输出打印
//
//	入口参数：	pro_buf：协议包
//
//	返回参数：	0-成功	其他-错误
//
//	说明：		
//==========================================================
static unsigned char Protocol_Parser_Send(const unsigned char *pro_buf)
{

	unsigned char index = 0;						//index：pro_buf索引
	unsigned char len = 0;							//数据长度(字节)
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d字节---[0x%02X]:[OK]从机地址\r\n", index, pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d字节---[0x%02X]:[%s]命令码\r\n", index, pro_buf[index], (pro_buf[index] >= 1 && pro_buf[index] <= 4) ? "OK": "ERR");
	if(pro_buf[index] < 1 || pro_buf[index] > 4)
		return 1;
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	len = pro_buf[index];
	PRINT("第%d字节---[0x%02X]:[OK]数据长度---%d字节\r\n", index, pro_buf[index], pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d~%d字节---%s:[OK]数据\r\n", index, index + len - 1, print_hexstring(pro_buf + index, len));
	index += len;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d~%d字节---[0x%02X 0x%02X]:[OK]校验码\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	
	return 0;

}

#endif

#if(RECV_PASER_EN == 1)

//==========================================================
//	函数名称：	Protocol_Parser_Recv
//
//	函数功能：	接收数据包协议解析输出打印
//
//	入口参数：	pro_buf：协议包
//
//	返回参数：	0-成功	其他-错误
//
//	说明：		
//==========================================================
static unsigned char Protocol_Parser_Recv(const unsigned char *pro_buf)
{

	unsigned char index = 0;						//index：pro_buf索引
	unsigned char len = 0;							//数据长度(字节)
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d字节---[0x%02X]:[OK]从机地址\r\n", index, pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d字节---[0x%02X]:[%s]命令码\r\n", index, pro_buf[index], (pro_buf[index] >= 1 && pro_buf[index] <= 4) ? "OK": "ERR");
	if(pro_buf[index] < 1 || pro_buf[index] > 4)
		return 1;
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d~%d字节---[0x%02X 0x%02X]:[OK]寄存器地址\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	index += 2;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	len = pro_buf[index] << 8 | pro_buf[index + 1];
	PRINT("第%d~%d字节---[0x%02X 0x%02X]:[OK]数据长度---%d字节\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1], len);
	index += 2;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("第%d~%d字节---[0x%02X 0x%02X]:[OK]校验码\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	
	return 0;

}

#endif

//==========================================================
//	函数名称：	Protocol_Parser_Print
//
//	函数功能：	协议解析输出打印
//
//	入口参数：	pro_buf：协议包
//				flag：0-发送包	1-接收包
//
//	返回参数：	
//
//	说明：		
//==========================================================
unsigned char Protocol_Parser_Print(const unsigned char *pro_buf, _Bool flag)
{
	
#if(SEND_PASER_EN == 1 || RECV_PASER_EN == 1)
	unsigned char result = 0;
#endif
	
	if(pro_buf[0] == 't' && pro_buf[1] == 'y')
	{
#if(SEND_PASER_EN == 1)
		PRINT("\r\n----------------------------------------------------------------------\r\n");
		PRINT("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\r\n");
		result = Protocol_Parser_Connect(pro_buf);
		if(result)
			PRINT("\r\n登录报文错误:%d\r\n", result);
		PRINT("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\r\n");
		PRINT("----------------------------------------------------------------------\r\n\r\n");
#endif
	}
	else
	{
#if(SEND_PASER_EN == 1 || RECV_PASER_EN == 1)
		PRINT("\r\n----------------------------------------------------------------------\r\n");
		PRINT("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\r\n");
#endif
		if(flag == 0)
		{
#if(SEND_PASER_EN == 1)
			result = Protocol_Parser_Send(pro_buf);
#endif
		}
		else
		{
#if(RECV_PASER_EN == 1)
			result = Protocol_Parser_Recv(pro_buf);
#endif
		}
		
#if(SEND_PASER_EN == 1 || RECV_PASER_EN == 1)
		if(result)
			PRINT("\r\nModBus包错误:%d\r\n", result);
		PRINT("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\r\n");
		PRINT("----------------------------------------------------------------------\r\n\r\n");
#endif
	}
	
	return 0;
}
