/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		OneNETƽ̨Ӧ��ʾ��
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "net_device.h"

//Э���ļ�
#include "onenet.h"
#include "fault.h"
#include "modbuskit.h"

//Э�����
#include "protocol_parser.h"

//Ӳ������
#include "usart.h"
#include "delay.h"

//C��
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
//	�������ƣ�	OneNET_GetLinkIP
//
//	�������ܣ�	��ȡʹ��Э��ĵ�¼IP��PORT
//
//	��ڲ�����	protocol��Э���
//				ip�����淵��IP�Ļ�����
//				port�����淵��port�Ļ�����
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		1-edp	2-nwx	3-jtext		4-Hiscmd
//				5-jt808			6-modbus	7-mqtt
//				8-gr20			9-reg		10-HTTP(�Զ���)
//				��ȡIP����֧��HTTPЭ�飬�����Զ���һ����־
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
	
	if(protocol == 10)													//�����HTTPЭ��
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
			data_ptr = strstr(net_device_info.cmd_ipd, "no-cache");				//�ҵ����Ĺؼ���
		}
		
		if(data_ptr != NULL)
		{
			if(strstr(data_ptr, "unsupportted") != NULL)						//��֧�ֵ�Э������
			{
				UsartPrintf(USART_DEBUG, "��֧�ָ�Э������\r\n");
				
				onenet_info.get_ip = 1;
			}
			else if(strstr(data_ptr, "can't find a available") != NULL)			//��֧�ֵ�Э������
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
//	�������ƣ�	OneNET_ConnectIP
//
//	�������ܣ�	����IP
//
//	��ڲ�����	ip��IP��ַ����ָ��
//				port���˿ڻ���ָ��
//
//	���ز�����	���س�ʼ�����
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool OneNET_ConnectIP(char *ip, char *port)
{

	_Bool result = 1;
	
	if(!net_device_info.net_work)									//����ģ��߱�������������
		return result;
	
	if(onenet_info.connect_ip)										//�Ѿ�������IP
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
			UsartPrintf(USART_DEBUG, "����IP��ַ��PORT�Ƿ���ȷ\r\n");
		}
	}
	
	return result;

}

//==========================================================
//	�������ƣ�	OneNET_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	serial�����к�
//				pswd������
//				devid�������豸��devid���ƷID
//
//	���ز�����	��
//
//	˵����		��onenetƽ̨�������ӣ��ɹ������oneNetInfo.netWork����״̬��־
//==========================================================
void OneNET_DevLink(const char *serial, const char *pswd, const char *proid)
{
	
	if(onenet_info.connect_ip == 1)
	{
		MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};				//Э���
		
		UsartPrintf(USART_DEBUG, "OneNET_DevLink\r\nPROID: %s,	SERIAL: %s,	PSWD: %s\r\n"
									, proid, serial, pswd);
		
		if(MODBUS_Connect(serial, pswd, proid, &modbusPacket) == 0)
		{
			Protocol_Parser_Print(modbusPacket._data, 0);
			
			NET_DEVICE_SendData(modbusPacket._data, modbusPacket._len);		//�ϴ�ƽ̨
			//NET_DEVICE_AddDataSendList(edpPacket._data, edpPacket._len, 1);
			
			MODBUS_DeleteBuffer(&modbusPacket);								//ɾ��
			
			onenet_info.s_addr = MODBUS_SLAVE_ADDR;
		}
	}
	
}

//==========================================================
//	�������ƣ�	OneNET_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	��
//
//	���ز�����	SEND_TYPE_OK-���ͳɹ�	SEND_TYPE_DATA-��Ҫ����
//
//	˵����		
//==========================================================
unsigned char OneNET_SendData(unsigned short *value_table, unsigned short value_table_cnt)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};						//Э���
	
	if(MODBUS_PacketCmd(onenet_info.s_addr, onenet_info.m_cmd, value_table, value_table_cnt, &modbusPacket) == 0)
	{
		UsartPrintf(USART_DEBUG, "Tips:	ModBus Send %d Bytes\r\n", modbusPacket._len);
		
		Protocol_Parser_Print(modbusPacket._data, 0);
		
		NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len, 1);	//��������
		
		MODBUS_DeleteBuffer(&modbusPacket);										//ɾ��
		
		return SEND_TYPE_OK;
	}
	else
		return SEND_TYPE_DATA;
	
}

//==========================================================
//	�������ƣ�	OneNet_HeartBeat
//
//	�������ܣ�	������������
//
//	��ڲ�����	��
//
//	���ز�����	SEND_TYPE_OK-���ͳɹ�	SEND_TYPE_HEART-��Ҫ����
//
//	˵����		
//==========================================================
unsigned char OneNET_SendData_Heart(void)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};					//Э���
	
	if(!onenet_info.net_work)												//�������Ϊ���� �� ��Ϊ�����շ�ģʽ
		return SEND_TYPE_HEART;
	
	if(MODBUS_PacketPing(&modbusPacket))
		return SEND_TYPE_HEART;
	
	onenet_info.heart_beat = 1;
	
	//NET_DEVICE_SendData(edpPacket._data, edpPacket._len);					//��ƽ̨�ϴ���������
	NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len, 0);	//��������
	
	MODBUS_DeleteBuffer(&modbusPacket);										//ɾ��
	
	UsartPrintf(USART_DEBUG, "Tips:	HeartBeat Ok\r\n");
	
	return SEND_TYPE_OK;
	
}

//==========================================================
//	�������ƣ�	OneNET_CmdHandle
//
//	�������ܣ�	��ȡƽ̨rb�е�����
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNET_CmdHandle(void)
{
	
	unsigned char *dataPtr = NULL, *ipdPtr = NULL;					//����ָ��

	dataPtr = NET_DEVICE_Read();									//�ȴ�����

	if(dataPtr != NULL)												//������Ч
	{
		ipdPtr = NET_DEVICE_GetIPD(dataPtr);						//����Ƿ���ƽ̨����
		if(ipdPtr != NULL)
		{
			net_device_info.send_count = 0;
			
			if(onenet_info.connect_ip)
				onenet_info.cmd_ptr = ipdPtr;			//���д���
			
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
				UsartPrintf(USART_DEBUG, "WARN:	���ӶϿ���׼������\r\n");
				
				onenet_info.err_check = 1;
			}
			else
				NET_DEVICE_CmdHandle((char *)dataPtr);
		} 
	}

}

//==========================================================
//	�������ƣ�	OneNET_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		ƽ̨�����·���ѯ����
//==========================================================
void OneNET_RevPro(unsigned char *cmd, unsigned short len)
{
	
	unsigned char s_addr = 0;
	
	Protocol_Parser_Print(cmd, 1);
	
	//ƽ̨�ֶ������·���ʽ��010100010001AC0A
	//ע��������ֻ��Ϊ�����ַ����Ҹ���������ż�����������һ�����ݽ������Զ������·�
	//��������ר�Żظ������������ϴ�������������
	
	if(MODBUS_UnPacketCmd(&s_addr, &onenet_info.m_cmd, &onenet_info.r_addr, &onenet_info.r_len, cmd, len) == 0)
	{
		UsartPrintf(USART_DEBUG, "�ӻ���ַ: %X, ����: %X, �Ĵ�����ַ: %X, ����: %x\r\n",
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
										��Ϣ����
******************************************************************************************/

//==========================================================
//	�������ƣ�	OneNET_CheckListHead
//
//	�������ܣ�	�����Ϣ����ͷ�Ƿ�Ϊ��
//
//	��ڲ�����	��
//
//	���ز�����	0-��	1-��Ϊ��
//
//	˵����		
//==========================================================
_Bool OneNET_CheckListHead(void)
{

	if(onenet_info.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	OneNET_GetListHeadBuf
//
//	�������ܣ�	��ȡ��������Ҫ���͵�����ָ��
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵�����ָ��
//
//	˵����		
//==========================================================
unsigned short *OneNET_GetListHeadBuf(void)
{

	return onenet_info.head->buf;

}

//==========================================================
//	�������ƣ�	OneNET_GetListHeadLen
//
//	�������ܣ�	��ȡ��������Ҫ���͵����ݳ���
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵����ݳ���
//
//	˵����		
//==========================================================
unsigned short OneNET_GetListHeadLen(void)
{

	return onenet_info.head->dataLen;

}

//==========================================================
//	�������ƣ�	OneNET_AddDataSendList
//
//	�������ܣ�	����Ϣ����β����һ����Ϣ����
//
//	��ڲ�����	buf����Ҫ���͵�����
//				dataLen�����ݳ���(���ָ���)
//
//	���ز�����	0-�ɹ�	����-ʧ��
//
//	˵����		�첽���ͷ�ʽ
//==========================================================
unsigned char OneNET_AddDataSendList(unsigned short *buf ,unsigned short dataLen)
{
	
	struct MODBUS_LIST *current = (struct MODBUS_LIST *)NET_MallocBuffer(sizeof(struct MODBUS_LIST));
																	//�����ڴ�
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned short *)NET_MallocBuffer(dataLen << 1);//�����ڴ�
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);									//ʧ�����ͷ�
		return 2;
	}
	
	if(onenet_info.head == NULL)										//���headΪNULL
		onenet_info.head = current;									//headָ��ǰ������ڴ���
	else															//���head��ΪNULL
		onenet_info.end->next = current;								//��endָ��ǰ������ڴ���
	
	memcpy(current->buf, buf, dataLen << 1);						//��������
	current->dataLen = dataLen;
	current->next = NULL;											//��һ��ΪNULL
	
	onenet_info.end = current;										//endָ��ǰ������ڴ���
	
	return 0;

}

//==========================================================
//	�������ƣ�	OneNET_DeleteDataSendList
//
//	�������ܣ�	������ͷɾ��һ������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
_Bool OneNET_DeleteDataSendList(void)
{
	
	struct MODBUS_LIST *next = onenet_info.head->next;		//��������ͷ����һ�����ݵ�ַ
	
	onenet_info.head->dataLen = 0;
	onenet_info.head->next = NULL;
	NET_FreeBuffer(onenet_info.head->buf);					//�ͷ��ڴ�
	NET_FreeBuffer(onenet_info.head);						//�ͷ��ڴ�
	
	onenet_info.head = next;									//����ͷָ����һ������
	
	return 0;

}
