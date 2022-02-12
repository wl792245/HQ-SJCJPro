/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	protocol_parser.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2019-07-08
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		Э�������
	*
	*	�޸ļ�¼��	MODBUS
	************************************************************
	************************************************************
	************************************************************
**/

//Э��
#include "modbuskit.h"

//����
#include "usart.h"

//
#include "protocol_parser.h"

//C��
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define PRINT		DebugPrintf


#if(SEND_PASER_EN == 1 || RECV_PASER_EN == 1)

static char print_buf[64];


//==========================================================
//	�������ƣ�	print_hexstring
//
//	�������ܣ�	��hexתΪ�ַ���
//
//	��ڲ�����	hex��hex����
//				len�����ݳ���
//
//	���ز�����	����ת����Ļ���ָ��
//
//	˵����		
//==========================================================
static char *print_hexstring(const unsigned char *hex, unsigned char len)
{
	
	char temp_buf[8];

	unsigned char i = 0;
	
	if(hex == (void *)0 || len == 0)									//�����Ϸ���
		return (void *)0;
	
	for(; i < sizeof(print_buf); i++)									//�建��
		print_buf[i] = 0;
	
	print_buf[0] = '[';
	
	for(i = 0; i < len; i++)
	{
		snprintf(temp_buf, sizeof(temp_buf), "0x%02X", hex[i]);			//��ʽ��
		strcat(print_buf, temp_buf);									//���Ƶ�����ĩβ
		
		if(i < len - 1)
			strcat(print_buf, " ");										//��ӿո�
	}
	
	strcat(print_buf, "]");
	
	return print_buf;

}

//==========================================================
//	�������ƣ�	print_ascii
//
//	�������ܣ�	��hexתΪASCII�ַ�
//
//	��ڲ�����	hex��hex����
//				len�����ݳ���
//
//	���ز�����	����ת����Ļ���ָ��
//
//	˵����		
//==========================================================
static char *print_ascii(const unsigned char *hex, unsigned char len)
{

	char temp_buf[8];

	unsigned char i = 0;
	
	if(hex == (void *)0 || len == 0)									//�����Ϸ���
		return (void *)0;
	
	for(; i < sizeof(print_buf); i++)									//�建��
		print_buf[i] = 0;
	
	for(i = 0; i < len; i++)
	{
		if(isprint(hex[i]))												//�Ƿ�Ϊ�ɴ�ӡ�ַ�
		{
			snprintf(temp_buf, sizeof(temp_buf), "%c", hex[i]);			//��ʽ��
			strcat(print_buf, temp_buf);								//���Ƶ�����ĩβ
		}
	}
	
	return print_buf;

}

#endif

#if(SEND_PASER_EN == 1)

//==========================================================
//	�������ƣ�	check_num
//
//	�������ܣ�	����Ƿ�Ϊ����
//
//	��ڲ�����	str���������ַ���
//				len�����ݳ���
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
static _Bool check_num(const unsigned char *str, unsigned char len)
{
	
	if(str == (void *)0 || len == 0)		//�����Ϸ���
		return 1;
	
	while(len--)
	{
		if(*str < '0' || *str > '9')		//�ж��Ƿ�Ϊ�����ַ�
			return 1;
		
		str++;
	}
	
	return 0;

}

//==========================================================
//	�������ƣ�	check_letter_num
//
//	�������ܣ�	�����ĸ������
//
//	��ڲ�����	str���������ַ���
//				len�����ݳ���
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
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
//	�������ƣ�	Protocol_Parser_Connect
//
//	�������ܣ�	���Ӱ�Э����������ӡ
//
//	��ڲ�����	pro_buf��Э���
//
//	���ز�����	0-�ɹ�	����-����
//
//	˵����		
//==========================================================
static unsigned char Protocol_Parser_Connect(const unsigned char *pro_buf)
{

	unsigned char index = 0;								//index��pro_buf����
	_Bool flag = 0;											//�ж������־
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(strcmp((char *)pro_buf, "type") == 0)
		flag = 1;
	else
		flag = 0;
	
	PRINT("��%d~%d�ֽ�---%s:[%s]�����ֶ�", index, index + 10, print_hexstring(pro_buf + index, 11), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf, 11));
	if(flag == 0)
		return 1;
	index += 11;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(strcmp((char *)pro_buf + index, "name") == 0)
		flag = 1;
	else
		flag = 0;
	
	PRINT("��%d~%d�ֽ�---%s:[%s]�����ֶ�", index, index + 8, print_hexstring(pro_buf + index, 9), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 9));
	if(flag == 0)
		return 2;
	index += 9;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_num(pro_buf + 20, strlen((char *)pro_buf + 20)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("��%d~%d�ֽ�---%s:[%s]���к�", index, index + 11, print_hexstring(pro_buf + index, 12), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 12));
	if(flag == 0)
		return 3;
	index += 12;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_num(pro_buf + 32, strlen((char *)pro_buf + 32)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("��%d~%d�ֽ�---%s:[%s]����", index, index + 8, print_hexstring(pro_buf + index, 9), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 9));
	if(flag == 0)
		return 4;
	index += 9;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	if(check_letter_num(pro_buf + 41, strlen((char *)pro_buf + 41)) == 0)
		flag = 1;
	else
		flag = 0;
	PRINT("��%d~%d�ֽ�---%s:[%s]��ƷID", index, index + 9, print_hexstring(pro_buf + index, 10), (flag == 1) ? "OK": "ERR");
	PRINT("---%s\r\n", print_ascii(pro_buf + index, 10));
	if(flag == 0)
		return 5;
	
	return 0;

}

//==========================================================
//	�������ƣ�	Protocol_Parser_Send
//
//	�������ܣ�	�������ݰ�Э����������ӡ
//
//	��ڲ�����	pro_buf��Э���
//
//	���ز�����	0-�ɹ�	����-����
//
//	˵����		
//==========================================================
static unsigned char Protocol_Parser_Send(const unsigned char *pro_buf)
{

	unsigned char index = 0;						//index��pro_buf����
	unsigned char len = 0;							//���ݳ���(�ֽ�)
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d�ֽ�---[0x%02X]:[OK]�ӻ���ַ\r\n", index, pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d�ֽ�---[0x%02X]:[%s]������\r\n", index, pro_buf[index], (pro_buf[index] >= 1 && pro_buf[index] <= 4) ? "OK": "ERR");
	if(pro_buf[index] < 1 || pro_buf[index] > 4)
		return 1;
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	len = pro_buf[index];
	PRINT("��%d�ֽ�---[0x%02X]:[OK]���ݳ���---%d�ֽ�\r\n", index, pro_buf[index], pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d~%d�ֽ�---%s:[OK]����\r\n", index, index + len - 1, print_hexstring(pro_buf + index, len));
	index += len;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d~%d�ֽ�---[0x%02X 0x%02X]:[OK]У����\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	
	return 0;

}

#endif

#if(RECV_PASER_EN == 1)

//==========================================================
//	�������ƣ�	Protocol_Parser_Recv
//
//	�������ܣ�	�������ݰ�Э����������ӡ
//
//	��ڲ�����	pro_buf��Э���
//
//	���ز�����	0-�ɹ�	����-����
//
//	˵����		
//==========================================================
static unsigned char Protocol_Parser_Recv(const unsigned char *pro_buf)
{

	unsigned char index = 0;						//index��pro_buf����
	unsigned char len = 0;							//���ݳ���(�ֽ�)
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d�ֽ�---[0x%02X]:[OK]�ӻ���ַ\r\n", index, pro_buf[index]);
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d�ֽ�---[0x%02X]:[%s]������\r\n", index, pro_buf[index], (pro_buf[index] >= 1 && pro_buf[index] <= 4) ? "OK": "ERR");
	if(pro_buf[index] < 1 || pro_buf[index] > 4)
		return 1;
	index++;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d~%d�ֽ�---[0x%02X 0x%02X]:[OK]�Ĵ�����ַ\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	index += 2;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	len = pro_buf[index] << 8 | pro_buf[index + 1];
	PRINT("��%d~%d�ֽ�---[0x%02X 0x%02X]:[OK]���ݳ���---%d�ֽ�\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1], len);
	index += 2;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
	PRINT("��%d~%d�ֽ�---[0x%02X 0x%02X]:[OK]У����\r\n", index, index + 1, pro_buf[index], pro_buf[index + 1]);
	
	return 0;

}

#endif

//==========================================================
//	�������ƣ�	Protocol_Parser_Print
//
//	�������ܣ�	Э����������ӡ
//
//	��ڲ�����	pro_buf��Э���
//				flag��0-���Ͱ�	1-���հ�
//
//	���ز�����	
//
//	˵����		
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
			PRINT("\r\n��¼���Ĵ���:%d\r\n", result);
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
			PRINT("\r\nModBus������:%d\r\n", result);
		PRINT("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\r\n");
		PRINT("----------------------------------------------------------------------\r\n\r\n");
#endif
	}
	
	return 0;
}
