/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-27
	*
	*	版本： 		V1.0
	*
	*	说明： 		OneNET平台应用示例
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "net_device.h"

//协议文件
#include "onenet.h"
#include "fault.h"
#include "modbuskit.h"

//协议分析
#include "protocol_parser.h"

//硬件驱动
#include "usart.h"
#include "delay.h"

//C库
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MODBUS_SLAVE_ADDR		1


ONETNET_INFO onenet_info = {"1234567", "1234567", "113237",
							"183.230.40.42", "2987",
							6, NULL,
							0, 0, 0, 0, 0, NULL, NULL, 0,
							0, 0, 1, 0, 0, 0, 0};


//==========================================================
//	函数名称：	OneNET_GetLinkIP
//
//	函数功能：	获取使用协议的登录IP和PORT
//
//	入口参数：	protocol：协议号
//				ip：保存返回IP的缓存区
//				port：保存返回port的缓存区
//
//	返回参数：	0-成功		1-失败
//
//	说明：		1-edp	2-nwx	3-jtext		4-Hiscmd
//				5-jt808			6-modbus	7-mqtt
//				8-gr20			9-reg		10-HTTP(自定义)
//				获取IP本身不支持HTTP协议，这里自定义一个标志
//==========================================================
_Bool OneNET_GetLinkIP(unsigned char protocol, char *ip, char *port)
{
	
	_Bool result = 1;
	char *data_ptr = NULL;
	char send_buf[128];
	unsigned char time_out = 200;
	
	if(!net_device_info.net_work)
		return result;
	
	if(onenet_info.get_ip)
		return !result;
	
	if(protocol == 10)													//如果是HTTP协议
	{
		strcpy(ip, "183.230.40.33");
		strcpy(port, "80");
		
		onenet_info.get_ip = 1;
		
		return !result;
	}
	
	if(NET_DEVICE_Connect("TCP", "183.230.40.33", "80") == 0)
	{
		memset(send_buf, 0, sizeof(send_buf));
		snprintf(send_buf, sizeof(send_buf), "GET http://api.heclouds.com/s?t=%d&v=public HTTP/1.1\r\n"
												"api-key:=sUT=jsLGXkQcUz3Z9EaiNQ80U0=\r\n"
												"Host:api.heclouds.com\r\n\r\n",
												protocol);
		
		net_device_info.cmd_ipd = NULL;
		
		NET_DEVICE_SendData((unsigned char *)send_buf, strlen(send_buf));
		
		while(--time_out)
		{
			if(net_device_info.cmd_ipd != NULL)
				break;
			
			DelayXms(10);
		}
		
		if(time_out)
		{
			data_ptr = strstr(net_device_info.cmd_ipd, "no-cache");				//找到最后的关键词
		}
		
		if(data_ptr != NULL)
		{
			if(strstr(data_ptr, "unsupportted") != NULL)						//不支持的协议类型
			{
				UsartPrintf(USART_DEBUG, "不支持该协议类型\r\n");
				
				onenet_info.get_ip = 1;
			}
			else if(strstr(data_ptr, "can't find a available") != NULL)			//不支持的协议类型
			{
				UsartPrintf(USART_DEBUG, "can't find a available IP\r\n");
				
				onenet_info.get_ip = 1;
			}
			else
			{
				if(sscanf(data_ptr, "no-cache\r\n%[^:]:%s", ip, port) == 2)
				{
					onenet_info.get_ip = 1;
					result = 0;
					
					UsartPrintf(USART_DEBUG, "Get ip: %s, port: %s\r\n", ip, port);
				}
			}
		}
	}
	
	return result;

}

//==========================================================
//	函数名称：	OneNET_ConnectIP
//
//	函数功能：	连接IP
//
//	入口参数：	ip：IP地址缓存指针
//				port：端口缓存指针
//
//	返回参数：	返回初始化结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool OneNET_ConnectIP(char *ip, char *port)
{

	_Bool result = 1;
	
	if(!net_device_info.net_work)									//网络模组具备网络连接能力
		return result;
	
	if(onenet_info.connect_ip)										//已经连接了IP
		return !result;
	
	if(!NET_DEVICE_Connect("TCP", ip, port))
	{
		result = 0;
		net_fault_info.net_fault_count = 0;
		onenet_info.connect_ip = 1;
	}
	else
	{
		DelayXms(500);
		
		if(++onenet_info.err_count >= 5)
		{
			onenet_info.err_count = 0;
			UsartPrintf(USART_DEBUG, "请检查IP地址和PORT是否正确\r\n");
		}
	}
	
	return result;

}

//==========================================================
//	函数名称：	OneNET_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	serial：序列号
//				pswd：密码
//				devid：创建设备的devid或产品ID
//
//	返回参数：	无
//
//	说明：		与onenet平台建立连接，成功或会标记oneNetInfo.netWork网络状态标志
//==========================================================
void OneNET_DevLink(const char *serial, const char *pswd, const char *proid)
{
	
	if(onenet_info.connect_ip == 1)
	{
		MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};				//协议包
		
		UsartPrintf(USART_DEBUG, "OneNET_DevLink\r\nPROID: %s,	SERIAL: %s,	PSWD: %s\r\n"
									, proid, serial, pswd);
		
		if(MODBUS_Connect(serial, pswd, proid, &modbusPacket) == 0)
		{
			Protocol_Parser_Print(modbusPacket._data, 0);
			
			NET_DEVICE_SendData(modbusPacket._data, modbusPacket._len);		//上传平台
			//NET_DEVICE_AddDataSendList(edpPacket._data, edpPacket._len, 1);
			
			MODBUS_DeleteBuffer(&modbusPacket);								//删包
			
			onenet_info.s_addr = MODBUS_SLAVE_ADDR;
		}
	}
	
}

//==========================================================
//	函数名称：	OneNET_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	无
//
//	返回参数：	SEND_TYPE_OK-发送成功	SEND_TYPE_DATA-需要重送
//
//	说明：		
//==========================================================
unsigned char OneNET_SendData(unsigned short *value_table, unsigned short value_table_cnt)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};						//协议包
	
	if(MODBUS_PacketCmd(onenet_info.s_addr, onenet_info.m_cmd, value_table, value_table_cnt, &modbusPacket) == 0)
	{
		UsartPrintf(USART_DEBUG, "Tips:	ModBus Send %d Bytes\r\n", modbusPacket._len);
		
		Protocol_Parser_Print(modbusPacket._data, 0);
		
		NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len, 1);	//加入链表
		
		MODBUS_DeleteBuffer(&modbusPacket);										//删包
		
		return SEND_TYPE_OK;
	}
	else
		return SEND_TYPE_DATA;
	
}

//==========================================================
//	函数名称：	OneNet_HeartBeat
//
//	函数功能：	发送心跳请求
//
//	入口参数：	无
//
//	返回参数：	SEND_TYPE_OK-发送成功	SEND_TYPE_HEART-需要重送
//
//	说明：		
//==========================================================
unsigned char OneNET_SendData_Heart(void)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};					//协议包
	
	if(!onenet_info.net_work)												//如果网络为连接 或 不为数据收发模式
		return SEND_TYPE_HEART;
	
	if(MODBUS_PacketPing(&modbusPacket))
		return SEND_TYPE_HEART;
	
	onenet_info.heart_beat = 1;
	
	//NET_DEVICE_SendData(edpPacket._data, edpPacket._len);					//向平台上传心跳请求
	NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len, 0);	//加入链表
	
	MODBUS_DeleteBuffer(&modbusPacket);										//删包
	
	UsartPrintf(USART_DEBUG, "Tips:	HeartBeat Ok\r\n");
	
	return SEND_TYPE_OK;
	
}

//==========================================================
//	函数名称：	OneNET_CmdHandle
//
//	函数功能：	读取平台rb中的数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET_CmdHandle(void)
{
	
	unsigned char *dataPtr = NULL, *ipdPtr = NULL;					//数据指针

	dataPtr = NET_DEVICE_Read();									//等待数据

	if(dataPtr != NULL)												//数据有效
	{
		ipdPtr = NET_DEVICE_GetIPD(dataPtr);						//检查是否是平台数据
		if(ipdPtr != NULL)
		{
			net_device_info.send_count = 0;
			
			if(onenet_info.connect_ip)
				onenet_info.cmd_ptr = ipdPtr;			//集中处理
			
			net_device_info.cmd_ipd = (char *)ipdPtr;
		}
		else
		{
			if(strstr((char *)dataPtr, "SEND OK") != NULL)
			{
				net_device_info.send_count = 0;
				
				UsartPrintf(USART_DEBUG, "Tips:	Send Ok\r\n");
			}
			else if(strstr((char *)dataPtr, "CLOSE") != NULL && onenet_info.net_work)
			{
				UsartPrintf(USART_DEBUG, "WARN:	连接断开，准备重连\r\n");
				
				onenet_info.err_check = 1;
			}
			else
				NET_DEVICE_CmdHandle((char *)dataPtr);
		} 
	}

}

//==========================================================
//	函数名称：	OneNET_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		平台主动下发查询命令
//==========================================================
void OneNET_RevPro(unsigned char *cmd, unsigned short len)
{
	
	unsigned char s_addr = 0;
	
	Protocol_Parser_Print(cmd, 1);
	
	//平台手动命令下发格式：010100010001AC0A
	//注：命令体只能为数字字符，且个数必须是偶数，否则最后一个数据将被忽略而不被下发
	//命令无需专门回复，就像正常上传数据那样即可
	
	if(MODBUS_UnPacketCmd(&s_addr, &onenet_info.m_cmd, &onenet_info.r_addr, &onenet_info.r_len, cmd, len) == 0)
	{
		UsartPrintf(USART_DEBUG, "从机地址: %X, 命令: %X, 寄存器地址: %X, 长度: %x\r\n",
									s_addr, onenet_info.m_cmd, onenet_info.r_addr, onenet_info.r_len);
		
		if(s_addr == onenet_info.s_addr)
		{
			if(++onenet_info.rev_cmd_cnt >= 250)
				onenet_info.rev_cmd_cnt = 0;
		}
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MODBUS_UnPacketCmd Err\r\n");

}


/******************************************************************************************
										消息队列
******************************************************************************************/

//==========================================================
//	函数名称：	OneNET_CheckListHead
//
//	函数功能：	检查消息链表头是否为空
//
//	入口参数：	无
//
//	返回参数：	0-空	1-不为空
//
//	说明：		
//==========================================================
_Bool OneNET_CheckListHead(void)
{

	if(onenet_info.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	函数名称：	OneNET_GetListHeadBuf
//
//	函数功能：	获取链表里需要发送的数据指针
//
//	入口参数：	无
//
//	返回参数：	获取链表里需要发送的数据指针
//
//	说明：		
//==========================================================
unsigned short *OneNET_GetListHeadBuf(void)
{

	return onenet_info.head->buf;

}

//==========================================================
//	函数名称：	OneNET_GetListHeadLen
//
//	函数功能：	获取链表里需要发送的数据长度
//
//	入口参数：	无
//
//	返回参数：	获取链表里需要发送的数据长度
//
//	说明：		
//==========================================================
unsigned short OneNET_GetListHeadLen(void)
{

	return onenet_info.head->dataLen;

}

//==========================================================
//	函数名称：	OneNET_AddDataSendList
//
//	函数功能：	在消息链表尾新增一个消息链表
//
//	入口参数：	buf：需要发送的数据
//				dataLen：数据长度(半字个数)
//
//	返回参数：	0-成功	其他-失败
//
//	说明：		异步发送方式
//==========================================================
unsigned char OneNET_AddDataSendList(unsigned short *buf ,unsigned short dataLen)
{
	
	struct MODBUS_LIST *current = (struct MODBUS_LIST *)NET_MallocBuffer(sizeof(struct MODBUS_LIST));
																	//分配内存
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned short *)NET_MallocBuffer(dataLen << 1);//分配内存
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);									//失败则释放
		return 2;
	}
	
	if(onenet_info.head == NULL)										//如果head为NULL
		onenet_info.head = current;									//head指向当前分配的内存区
	else															//如果head不为NULL
		onenet_info.end->next = current;								//则end指向当前分配的内存区
	
	memcpy(current->buf, buf, dataLen << 1);						//复制数据
	current->dataLen = dataLen;
	current->next = NULL;											//下一段为NULL
	
	onenet_info.end = current;										//end指向当前分配的内存区
	
	return 0;

}

//==========================================================
//	函数名称：	OneNET_DeleteDataSendList
//
//	函数功能：	从链表头删除一个链表
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
_Bool OneNET_DeleteDataSendList(void)
{
	
	struct MODBUS_LIST *next = onenet_info.head->next;		//保存链表头的下一段数据地址
	
	onenet_info.head->dataLen = 0;
	onenet_info.head->next = NULL;
	NET_FreeBuffer(onenet_info.head->buf);					//释放内存
	NET_FreeBuffer(onenet_info.head);						//释放内存
	
	onenet_info.head = next;									//链表头指向下一段数据
	
	return 0;

}
