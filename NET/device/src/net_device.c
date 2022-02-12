/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	net_device.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2019-12-20
	*
	*	�汾�� 		V1.6
	*
	*	˵���� 		�����豸Ӧ�ò�
	*
	*	�޸ļ�¼��	V1.1��1.ƽ̨IP��PORTͨ����������ķ�ʽȷ��������˲�ͬЭ�������豸������ͨ�õ����⡣
	*					  2.���ӻ�վ��λ���ܣ���net_device.h��ͨ����M631X_LOCATION��ȷʵ�Ƿ�ʹ�á�
	*					  3.NET_DEVICE_SendCmd����������mode���������Ƿ������������ķ���ֵ��
	*				V1.2��1.ȡ��V1.1�еĹ���3��
	*					  2.���������ݻ�ȡ�ӿڣ�����rb���ơ�
	*					  3.ȡ����͸��ģʽ
	*				V1.3��1.���ӡ�æ����־�жϡ�
	*					  2.����ģ�鷢��ʱ����������ģ�����ܶ�����
	*					  3.�޸��� NET_DEVICE_Check �������ơ�
	*				V1.4��1.����������ѡ������ͷ���β��
	*				V1.5��1.�޸� NET_DEVICE_AddDataSendList �������ӵ�����ͷ���Ұ�ڴ����⡣
	*				V1.6��1.�޸� NET_DEVICE_Connect ��������֤���ӵ��ȶ��ԡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//��Ƭ��������
#include "mcu_gpio.h"

//�����豸
#include "net_device.h"
#include "net_io.h"

//Ӳ������
#include "delay.h"
#include "usart.h"

#if(NET_TIME_EN == 1)
#include <time.h>
#endif

//C��
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static _Bool nd_busy[2] = {0, 0};


NET_DEVICE_INFO net_device_info = {NULL, NULL, NULL, NULL, NULL,
									0, 0, 0, 0, 100,
									0, 0, 0, 0, 0, 1, 0};


GPS_INFO gps;


#if(GPS_EN == 1)
GPS_EX_INFO gps_ex_info;
#endif


#if(LBS_EN == 1)
LBS_INFO lbs_info;
#endif


//Ϊ��ͨ���ԣ�gpio�豸�б����name�̶���ôд
const static GPIO_LIST net_device_gpio_list[] = {
													{GPIOC, GPIO_Pin_7, "nd_rst"},
													{GPIOC, GPIO_Pin_6, "nd_pwrk"},
												};


static _Bool cmd_resp_err_flag = 0;				//1-��������м�������ERROR��


/*
************************************************************
*	�������ƣ�	NET_DEVCIE_IsBusReady
*
*	�������ܣ�	��ѯ�����Ƿ����
*
*	��ڲ�����	type��1-���ݷ���		0-�����
*
*	���ز�����	0-����	1-δ����
*
*	˵����		
************************************************************
*/
__inline static _Bool NET_DEVCIE_IsBusReady(_Bool type)
{
	
	_Bool result = 1;
	
	if(nd_busy[type] == 0)
	{
		nd_busy[type] = 1;
		
		result = 0;
	}
	
	return result;

}

/*
************************************************************
*	�������ƣ�	NET_DEVCIE_FreeBus
*
*	�������ܣ�	�ͷ�����
*
*	��ڲ�����	type��1-���ݷ���		0-�����
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
__inline static void NET_DEVCIE_FreeBus(_Bool type)
{

	nd_busy[type] = 0;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Delay
*
*	�������ܣ�	��ʱ
*
*	��ڲ�����	time����ʱʱ��
*
*	���ز�����	��
*
*	˵����		���ڵ�ǰ��ʱʱ��
************************************************************
*/
__inline static void NET_DEVICE_Delay(unsigned int time)
{

	DelayXms(time);

}

//==========================================================
//	�������ƣ�	NET_DEVICE_PowerCtl
//
//	�������ܣ�	�����豸��Դ����
//
//	��ڲ�����	flag��1-�򿪵�Դ		0-�ر�
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void NET_DEVICE_PowerCtl(_Bool flag)
{

	if(flag == 1)
	{
		MCU_GPIO_Output_Ctl("nd_pwrk", 1);
		
		NET_DEVICE_Delay(1100);
		
		MCU_GPIO_Output_Ctl("nd_pwrk", 0);
	}
	else
	{
		MCU_GPIO_Output_Ctl("nd_pwrk", 1);
		
		NET_DEVICE_Delay(1100);
		
		MCU_GPIO_Output_Ctl("nd_pwrk", 0);
		
		NET_DEVICE_Delay(2000);
	}

}

//==========================================================
//	�������ƣ�	NET_DEVICE_IO_Init
//
//	�������ܣ�	��ʼ�������豸IO��
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		��ʼ�������豸�Ŀ������š������շ����ܵ�
//==========================================================
void NET_DEVICE_IO_Init(void)
{
	
	MCU_GPIO_Init(net_device_gpio_list[0].gpio_group, net_device_gpio_list[0].gpio_pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, net_device_gpio_list[0].gpio_name);
	
	MCU_GPIO_Init(net_device_gpio_list[1].gpio_group, net_device_gpio_list[1].gpio_pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, net_device_gpio_list[1].gpio_name);
	
	NET_IO_Init();									//�����豸����IO���ʼ��
	
	if(net_device_info.reboot == 0)
	{
		NET_DEVICE_PowerCtl(1);
	}
	
	net_device_info.reboot = 0;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetSerial
//
//	�������ܣ�	��ȡһ��Ψһ����
//
//	��ڲ�����	serial��ָ���ַ
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		Ψһ���ţ�GSMģ�������IMEI��ΨһID��WIFIģ����Ի�ȡMAC��ַ��
//==========================================================
_Bool NET_DEVICE_GetSerial(char **serial)
{
	
	_Bool result = 1;
	char *data_ptr = NULL, *data_ptr_t = NULL;
	unsigned char len = 0;
	
	if(!NET_DEVICE_SendCmd("AT+GSN=0\r\n", "GSN=0", 200))
	{
		//AT+GSN=0\r\n\r\n22273P00235041004524\r\n\r\nOK
		data_ptr = net_device_info.cmd_resp;
		
		//�ҵ�serial��ͷ
		data_ptr = strchr(net_device_info.cmd_resp, '\n');
		
		data_ptr += 3;
		
		data_ptr_t = data_ptr;
		UsartPrintf(USART_DEBUG, "Serial: %s\r\n", data_ptr_t);
		//����serial����
		while(*data_ptr_t != '\r')
		{
			len++;
		}
		
		*serial = (char *)NET_MallocBuffer(len + 1);
		if(*serial == NULL)
			return result;
		
		memset(*serial, 0, len + 1);
		
		//��������
		memcpy(*serial, data_ptr, len);
		
		UsartPrintf(USART_DEBUG, "Serial: %s\r\n", *serial);
		
		result = 0;
	}
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetSignal
//
//	�������ܣ�	��ȡ�ź�ֵ
//
//	��ڲ�����	��
//
//	���ز�����	�����ź�ֵ
//
//	˵����		
//==========================================================
signed char NET_DEVICE_GetSignal(void)
{
	
	char *data_ptr = NULL;
	char num_buf[4] = {0, 0, 0, 0};
	unsigned char i = 0;
	
	if(!net_device_info.net_work)
		return 0;
	
	//+CSQ: 31,0\r\n\r\nOK
	if(NET_DEVICE_SendCmd("AT+CSQ\r\n", "OK", 200) == 0)
	{
		data_ptr = net_device_info.cmd_resp;
		
		while(*data_ptr < '0' || *data_ptr > '9')
			data_ptr++;
		
		while(*data_ptr >= '0' && *data_ptr <= '9')
			num_buf[i++] = *data_ptr++;
		
		net_device_info.signal = (signed char)atoi(num_buf);
	}
	else
		net_device_info.signal = 0;
	
	return net_device_info.signal;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetGps
//
//	�������ܣ�	��ȡGPS
//
//	��ڲ�����	lon������
//				lat��γ��
//				gps_mode��GPSģʽ
//
//	���ز�����	�����ź�ֵ
//
//	˵����		1-GPS			2-BDS			3-GLONSS
//				4-GPS+BDS		5-GPS+GLONSS	6-BDS+GLONSS
//==========================================================
_Bool NET_DEVICE_GetGps(char *lon, char *lat, float *altitude, float *speed, int *satellite, unsigned char gps_mode)
{

	_Bool result = 1;
	
#if(GPS_EN == 1)
	char cmd_buf[16];
	char *data_ptr = NULL;
	
	if(gps_mode ==0 || gps_mode > 6)
		return result;
	
	if(NET_DEVICE_SendCmd("AT+MGNSS=1\r\n", "OK", 200))
		return result;
	
	snprintf(cmd_buf, sizeof(cmd_buf), "AT+MGNSSTYPE=%d\r\n", gps_mode);
	if(NET_DEVICE_SendCmd(cmd_buf, "OK", 200))
		return result;
	
	if(!NET_DEVICE_SendCmd("AT+MGNSSINFO\r\n", "+MGNSSINFO:", 200))
	{
		//+MGNSSINFO: GNSS NO SINGAL
		//+MGNSSINFO: E106.4936916,N29.6183583,271.1,0.879,4\r\n\r\nOK
		
		data_ptr = net_device_info.cmd_resp;
		
		if(strstr(data_ptr, "NO SINGAL"))
		{
			UsartPrintf(USART_DEBUG, "Tips:	GPS No Signal\r\n");
		}
		else
		{
			unsigned char i = 0;
			char temp_str[8];
			
			//��ȡ����=========================================================
			data_ptr = strchr(data_ptr, 'E');
			data_ptr++;
			while(*data_ptr != ',')
				lon[i++] = *data_ptr++;
			lon[i] = 0;
			
			//��ȡγ��=========================================================
			i = 0;
			data_ptr += 2;
			while(*data_ptr != ',')
				lat[i++] = *data_ptr++;
			lat[i] = 0;
			
			//��ȡ����=========================================================
			i = 0;
			data_ptr++;
			while(*data_ptr != ',')
				temp_str[i++] = *data_ptr++;
			temp_str[i] = 0;
			*altitude = atof(temp_str);
			
			//��ȡ�ٶ�=========================================================
			i = 0;
			data_ptr++;
			while(*data_ptr != ',')
				temp_str[i++] = *data_ptr++;
			temp_str[i] = 0;
			*speed = atof(temp_str);
			
			//��ȡ����=========================================================
			i = 0;
			data_ptr++;
			while(*data_ptr != '\r')
				temp_str[i++] = *data_ptr++;
			temp_str[i] = 0;
			*satellite = atoi(temp_str);
			
			UsartPrintf(USART_DEBUG, "����:%s,γ��:%s,����:%f,�ٶ�:%f,������:%d\r\n", lon, lat, *altitude, *speed, *satellite);
				
			result = 0;
		}
	}
#endif
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetTime
//
//	�������ܣ�	��ȡ����ʱ��
//
//	��ڲ�����	��
//
//	���ز�����	UTC��ֵ
//
//	˵����		������NTPЭ���������
//				NTP��������UDPЭ�飬�˿�123
//
//				�Ѳ��Կ��õ�NTP������-2017-11-07
//				1.cn.pool.ntp.org		��Դ�����ϳ��ģ���֪��������(ע�⡰1.��������ţ���������һ����)
//				cn.ntp.org.cn			��Դ���й�
//				edu.ntp.org.cn			��Դ���й�������
//				183.230.40.42			��Դ��OneNET
//				tw.ntp.org.cn			��Դ���й�̨��
//				us.ntp.org.cn			��Դ������
//				sgp.ntp.org.cn			��Դ���¼���
//				kr.ntp.org.cn			��Դ������
//				de.ntp.org.cn			��Դ���¹�
//				jp.ntp.org.cn			��Դ���ձ�
//==========================================================
unsigned int NET_DEVICE_GetTime(void)
{
	
	unsigned int second = 0;
	
#if(NET_TIME_EN == 1)
	struct tm *local_time;
	unsigned char time_out = 200;
	char *data_ptr = NULL;
	unsigned char times = 0;
	char *ntp_server[] = {"183.230.40.42", "cn.ntp.org.cn", "edu.ntp.org.cn"};
	
	unsigned char time_buffer[48];

	NET_DEVICE_Close();
	
	while(times < sizeof(ntp_server) / sizeof(ntp_server[0]))
	{
		if(NET_DEVICE_Connect("UDP", ntp_server[times], "123") == 0)
		{
			memset(time_buffer, 0, sizeof(time_buffer));
			
			time_buffer[0] = 0xE3;							//LI, Version, Mode
			time_buffer[1] = 0;								//��ʾ����ʱ�ӵĲ��ˮƽ
			time_buffer[2] = 6;								//��λsigned integer����ʾ������Ϣ֮��������
			time_buffer[3] = 0xEB;							//��ʾ����ʱ�Ӿ��ȣ���ȷ�����ƽ����
			
			NET_DEVICE_SendData(time_buffer, sizeof(time_buffer));
			net_device_info.cmd_ipd = NULL;
			
			while(--time_out)
			{
				if(net_device_info.cmd_ipd != NULL)
					break;
				
				NET_DEVICE_Delay(10);
			}
			
			NET_DEVICE_Close();
			
			if(net_device_info.cmd_ipd)
			{
				data_ptr = net_device_info.cmd_ipd;
				
				if(((*data_ptr >> 6) & 0x03) == 3)			//bit6��bit7ͬΪ1��ʾ��ǰ���ɶ�ʱ����������������״̬��
					break;
				
				second = *(data_ptr + 40) << 24 | *(data_ptr + 41) << 16 | *(data_ptr + 42) << 8 | *(data_ptr + 43);
				second -= 2208960000UL;						//ʱ������
				
				local_time = localtime(&second);
				
				UsartPrintf(USART_DEBUG, "UTC Time: %d-%d-%d %d:%d:%d\r\n",
										local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
										local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
				
				break;
			}
		}
		
		++times;
		NET_DEVICE_Delay(500);
	}
#endif
	
	return second;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Exist
//
//	�������ܣ�	�����豸���ڼ��
//
//	��ڲ�����	��
//
//	���ز�����	���ؽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Exist(void)
{

	return NET_DEVICE_SendCmd("AT\r\n", "OK", 200);

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Init
//
//	�������ܣ�	�����豸��ʼ��
//
//	��ڲ�����	��
//
//	���ز�����	���س�ʼ�����
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Init(void)
{
	
	_Bool status = 1;
	
	net_device_info.net_work = 0;
	
	NET_DEVICE_Delay(1000);
	
	switch(net_device_info.init_step)
	{
		case 0:
			UsartPrintf(USART_DEBUG, "Tips:	ATE0\r\n");						//�رջ���
			if(!NET_DEVICE_SendCmd("ATE0\r\n", "OK", 400))
				net_device_info.init_step++;
		break;
		
		case 1:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CPIN?\r\n");
			if(!NET_DEVICE_SendCmd("AT+CPIN?\r\n", "READY", 400))			//ȷ��SIM��PIN�����������READY����ʾ�����ɹ�
				net_device_info.init_step++;
		break;
		
		case 2:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CFUN=1\r\n");
			if(!NET_DEVICE_SendCmd("AT+CFUN=1\r\n", "OK", 400))				//��Э��ջ
				net_device_info.init_step++;
		break;
		
		case 3:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CREG=1\r\n");
			if(!NET_DEVICE_SendCmd("AT+CREG=1\r\n", "OK", 400))				//������ע��
				net_device_info.init_step++;
		break;
		
		case 4:																//�Զ��жϿ�����
		{
			UsartPrintf(USART_DEBUG, "Tips:	AT+CREG?\r\n");
			
			if(NET_DEVICE_SendCmd("AT+CREG?\r\n", "1,1", 400) == 0)
			{
				net_device_info.init_step++;
			}
			else
			{
				if(strstr(net_device_info.cmd_resp, "1,5"))
					net_device_info.init_step++;
			}
		}
		break;
			
		case 5:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CEREG=2\r\n");				//ʹ����ע��״̬��λ����Ϣ������������Ч
			if(!NET_DEVICE_SendCmd("AT+CEREG=2\r\n", "OK", 400))
				net_device_info.init_step++;
		break;
			
		case 6:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CEREG?\r\n");				//�������ע��״̬
			if(!NET_DEVICE_SendCmd("AT+CEREG?\r\n", "+CEREG:", 400))
			{
				//+CEREG: 2,1,"xxx","xxxxxxxx",7									//sta,tac,ci,act
				
#if(LBS_EN == 1)
				char *data_ptr = strchr(net_device_info.cmd_resp, '"');
				if(data_ptr)
				{
					int nt = 5, act = 0;
					
					if(sscanf(data_ptr, "\"%[^\"]\",\"%[^\"]\",%d", lbs_info.lac, lbs_info.cell_id, &act) == 3)
					{
						lbs_info.flag = 16;									//����Ϊhex
						lbs_info.lbs_ok = 1;
						lbs_info.network_type = nt;
						
						UsartPrintf(USART_DEBUG, "cid: %s, lac: %s, flag: %d\r\n", lbs_info.cell_id, lbs_info.lac,
																							lbs_info.flag);
					}
				}
#endif
				NET_DEVICE_SendCmd("AT+CGREG=0\r\n", NULL, 400);			//ȡ���Զ�����
				NET_DEVICE_Delay(100);
				net_device_info.init_step++;
			}
		break;
		
		case 7:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
			NET_DEVICE_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK", 400);		//����APN
			net_device_info.init_step++;
		break;
		
		case 8:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CGACT=1,1\r\n");
			if(!NET_DEVICE_SendCmd("AT+CGACT=1,1\r\n", "OK", 400)) 			//����
				net_device_info.init_step++;
		break;
		
		case 9:
			UsartPrintf(USART_DEBUG, "Tips:	AT+CGATT=1\r\n");				//����GPRSҵ��
			if(!NET_DEVICE_SendCmd("AT+CGATT=1\r\n", "OK", 400))
				net_device_info.init_step++;
		break;
			
		case 10:
		
#if(NET_TIME_EN == 1)
			if(!net_device_info.net_time)
				net_device_info.net_time = NET_DEVICE_GetTime();
#endif
			net_device_info.init_step++;
		
		break;
			
		default:
			net_device_info.send_count = 0;
			net_device_info.net_work = 1;
			status = 0;
		break;
	}

	return status;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Reset
//
//	�������ܣ�	�����豸��λ
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void NET_DEVICE_Reset(void)
{
	
	UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE_Reset\r\n");

	MCU_GPIO_Output_Ctl("nd_rst", 1);		//��λ
	NET_DEVICE_Delay(250);
	
	MCU_GPIO_Output_Ctl("nd_rst", 0);		//������λ
	NET_DEVICE_Delay(1000);

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Close
//
//	�������ܣ�	�ر���������
//
//	��ڲ�����	��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_Close(void)
{
	
	_Bool result = 1;
	
	UsartPrintf(USART_DEBUG, "Tips:	CLOSE\r\n");

	result = NET_DEVICE_SendCmd("AT+MIPCLOSE=0\r\n", "OK", 400);
	
	NET_DEVICE_Delay(150);
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Connect
//
//	�������ܣ�	����ƽ̨
//
//	��ڲ�����	type��TCP �� UDP
//				ip��IP��ַ����ָ��
//				port���˿ڻ���ָ��
//
//	���ز�����	�������ӽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Connect(char *type, char *ip, char *port)
{
	
	_Bool status = 1;
	char cmd_buf[48];
	unsigned char err_count = 5;
	
	snprintf(cmd_buf, sizeof(cmd_buf), "AT+MIPOPEN=0,\"%s\",\"%s\",%s\r\n", type, ip, port);
	
	UsartPrintf(USART_DEBUG, "Tips:	%s", cmd_buf);
	
	do
	{
		
		NET_DEVICE_Close();
		
		status = NET_DEVICE_SendCmd(cmd_buf, "CONNECT OK", 2000);
		
		if(status == 0)
		{
			err_count = 0;
		}
		else
		{
			unsigned char connect_status = NET_DEVICE_Check();
			
			if(connect_status == NET_DEVICE_CONNECTING)
			{
				unsigned char connect_count = 20;
				unsigned char connect_status_t = NET_DEVICE_NO_DEVICE;
				
				do
				{
					connect_status_t = NET_DEVICE_Check();
					
					if(connect_status_t == NET_DEVICE_CONNECTED)
					{
						status = 0;
						err_count = 0;
						
						break;
					}
					else if(connect_status_t == NET_DEVICE_CLOSED)
					{
						err_count--;
						
						break;
					}
					
					connect_count--;
					if(connect_count == 0)
					{
						err_count--;
						
						if(err_count == 0)
						{
							NET_DEVICE_Reset();
							NET_DEVICE_ReConfig(0);
						}
					}
					
					NET_DEVICE_Delay(1000);
				} while(connect_count);
			}
			else if(connect_status == NET_DEVICE_CONNECTED)
			{
				status = 0;
				err_count = 0;
			}
			else
			{
				err_count--;
				
				NET_DEVICE_Delay(500);
			}
		}
		
	} while(err_count);
	
	return status;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_CmdHandle
//
//	�������ܣ�	���������Ƿ���ȷ
//
//	��ڲ�����	cmd����Ҫ���͵�����
//
//	���ز�����	��
//
//	˵����		������ɹ���ָ����NULL
//==========================================================
void NET_DEVICE_CmdHandle(char *cmd)
{
	
	if(strstr(cmd, net_device_info.cmd_hdl) != NULL)
		net_device_info.cmd_hdl = NULL;
	else if(strstr(cmd, "ERROR") != NULL)
	{
		cmd_resp_err_flag = 1;
		net_device_info.cmd_hdl = NULL;
	}
	
	net_device_info.cmd_resp = cmd;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_SendCmd
//
//	�������ܣ�	�������豸����һ��������ȴ���ȷ����Ӧ
//
//	��ڲ�����	cmd����Ҫ���͵�����
//				res����Ҫ��������Ӧ
//				time_out���ȴ�ʱ��(��ϵͳʱ��Ϊ׼)
//
//	���ز�����	�������ӽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_SendCmd(char *cmd, char *res, unsigned short time_out)
{
	
	_Bool result = 0;
	
	if(NET_DEVCIE_IsBusReady(0) == 1)
		return 1;
	
	NET_IO_Send((unsigned char *)cmd, strlen(cmd));			//д��������豸
	
	if(res == NULL)											//���Ϊ�գ���ֻ�Ƿ���
	{
		NET_DEVCIE_FreeBus(0);
		
		return 0;
	}
	
	net_device_info.cmd_hdl = res;							//��Ҫ���ѵĹؼ���
	
	while((net_device_info.cmd_hdl != NULL) && --time_out)	//�ȴ�
		NET_DEVICE_Delay(10);
	
	if((time_out == 0) || (cmd_resp_err_flag == 1))
	{
		cmd_resp_err_flag = 0;
		result = 1;
	}
	
	NET_DEVCIE_FreeBus(0);
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_SendData
//
//	�������ܣ�	ʹ�����豸�������ݵ�ƽ̨
//
//	��ڲ�����	data����Ҫ���͵�����
//				len�����ݳ���
//
//	���ز�����	0-�������	1-����ʧ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{
	
	_Bool result = 1;
	char cmd_buf[40];
	
	if(NET_DEVCIE_IsBusReady(1) == 1)
		return 1;
	
	net_device_info.send_count++;
	
	NET_DEVICE_Delay(5);

	sprintf(cmd_buf, "AT+MIPSEND=0,%d\r\n", len);			//��������
	if(!NET_DEVICE_SendCmd(cmd_buf, ">", 400))				//�յ���>��ʱ���Է�������
	{
		NET_IO_Send(data, len);  							//�����豸������������
		
		result = 0;
	}
	
	NET_DEVICE_Delay(50);
	
	NET_DEVCIE_FreeBus(1);
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Read
//
//	�������ܣ�	��ȡһ֡����
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
unsigned char *NET_DEVICE_Read(void)
{

	return NET_IO_Read();

}

//==========================================================
//	�������ƣ�	NET_DEVICE_ReConfig
//
//	�������ܣ�	�豸�����豸��ʼ���Ĳ���
//
//	��ڲ�����	����ֵ
//
//	���ز�����	��
//
//	˵����		�ú������õĲ����������豸��ʼ������õ�
//==========================================================
void NET_DEVICE_ReConfig(unsigned char step)
{

	net_device_info.init_step = step;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	timeOut�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��M6312�ķ��ظ�ʽΪ	"<IPDATA: x>\r\nyyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *NET_DEVICE_GetIPD(unsigned char *data_ptr)
{
	
	char *data_ptr_t = (char *)data_ptr;
	
	if(data_ptr_t != NULL)
	{
		data_ptr_t = strstr(data_ptr_t, "+MIPURC:");				//������+MIPURC��ͷ
		if(data_ptr_t == NULL)										//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
		{
			//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
		}
		else
		{
			if(sscanf(data_ptr_t, "+MIPURC: \"recv\",0,%d", &net_device_info.data_bytes) == 1)
			{
				data_ptr_t = strchr(data_ptr_t, '\n');
				
				if(data_ptr_t != NULL)
				{
					data_ptr_t++;
					return (unsigned char *)(data_ptr_t);
				}
			}
		}
	}

	return NULL;													//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Check
//
//	�������ܣ�	��������豸����״̬
//
//	��ڲ�����	��
//
//	���ز�����	����״̬
//
//	˵����		
//==========================================================
unsigned char NET_DEVICE_Check(void)
{
	
	unsigned char status = NET_DEVICE_NO_DEVICE, err_count = 3;
	
	while(--err_count)
	{
		if(NET_DEVICE_SendCmd("AT+MIPSTATE\r\n", "+MIPSTATE:0", 400) == 0)		//����״̬���
		{
			if(strstr(net_device_info.cmd_resp, "CONNECT OK"))					//TCP������
			{
				UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE TCP CONNECT OK\r\n");
				status = NET_DEVICE_CONNECTED;
				
				break;
			}
			else if(strstr(net_device_info.cmd_resp, "CONNECTING"))				//������...
			{
				UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICECONNECTING...\r\n");
				status = NET_DEVICE_CONNECTING;
				
				break;
			}
			else if(strstr(net_device_info.cmd_resp, "CLOSE"))					//�ѹر�
			{
				UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICECLOSED\r\n");
				status = NET_DEVICE_CLOSED;
				
				break;
			}
			else if(strstr(net_device_info.cmd_resp, "INITIAL"))				//�ѹر�
			{
				UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE INITIA\r\n");		//��ʼ̬
				status = NET_DEVICE_INITIAL;
				
				break;
			}
		}
		
		NET_DEVICE_Delay(100);
	}
	
	return status;

}

/******************************************************************************************
										��Ϣ����
******************************************************************************************/

//==========================================================
//	�������ƣ�	NET_DEVICE_CheckListHead
//
//	�������ܣ�	��鷢������ͷ�Ƿ�Ϊ��
//
//	��ڲ�����	��
//
//	���ز�����	0-��	1-��Ϊ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_CheckListHead(void)
{

	if(net_device_info.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetListHeadBuf
//
//	�������ܣ�	��ȡ��������Ҫ���͵�����ָ��
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵�����ָ��
//
//	˵����		
//==========================================================
unsigned char *NET_DEVICE_GetListHeadBuf(void)
{

	return net_device_info.head->buf;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetListHeadLen
//
//	�������ܣ�	��ȡ��������Ҫ���͵����ݳ���
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵����ݳ���
//
//	˵����		
//==========================================================
unsigned short NET_DEVICE_GetListHeadLen(void)
{

	return net_device_info.head->dataLen;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_AddDataSendList
//
//	�������ܣ�	�ڷ�������β����һ����������
//
//	��ڲ�����	buf����Ҫ���͵�����
//				data_len�����ݳ���
//				mode��0-���ӵ�ͷ��	1-���ӵ�β��
//
//	���ز�����	0-�ɹ�	����-ʧ��
//
//	˵����		�첽���ͷ�ʽ
//==========================================================
unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short data_len, _Bool mode)
{
	
	struct NET_SEND_LIST *current = (struct NET_SEND_LIST *)NET_MallocBuffer(sizeof(struct NET_SEND_LIST));
																//�����ڴ�
	
	_Bool head_insert_flag = 0;
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned char *)NET_MallocBuffer(data_len);	//�����ڴ�
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);								//ʧ�����ͷ�
		return 2;
	}
	
	if(net_device_info.head == NULL)							//���headΪNULL
		net_device_info.head = current;							//headָ��ǰ������ڴ���
	else														//���head��ΪNULL
	{
		if(mode)
			net_device_info.end->next = current;				//��endָ��ǰ������ڴ���
		else
		{
			struct NET_SEND_LIST *head_t = NULL;
			
			head_t = net_device_info.head;
			net_device_info.head = current;
			current->next = head_t;
			
			head_insert_flag = 1;
		}
	}
	
	memcpy(current->buf, buf, data_len);						//��������
	current->dataLen = data_len;
	if(head_insert_flag == 0)
	{
		current->next = NULL;									//��һ��ΪNULL
		net_device_info.end = current;							//endָ��ǰ������ڴ���
	}
	
	return 0;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_DeleteDataSendList
//
//	�������ܣ�	������ͷɾ��һ������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_DeleteDataSendList(void)
{
	
	struct NET_SEND_LIST *next = net_device_info.head->next;
	
	net_device_info.head->dataLen = 0;
	net_device_info.head->next = NULL;
	NET_FreeBuffer(net_device_info.head->buf);
	NET_FreeBuffer(net_device_info.head);
	
	net_device_info.head = next;
	
	return 0;

}