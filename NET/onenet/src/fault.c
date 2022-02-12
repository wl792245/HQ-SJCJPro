/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	fault.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		���������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//Э��
#include "onenet.h"
#include "fault.h"

//�����豸
#include "net_device.h"

//Ӳ������
#include "usart.h"


NET_FAULT_INFO net_fault_info;


//==========================================================
//	�������ƣ�	Fault_Process
//
//	�������ܣ�	������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void NET_Fault_Process(void)
{
	
	if(net_fault_info.net_fault_flag == 1)
	{
		net_fault_info.net_fault_flag = 0;
		
		net_fault_info.net_fault_disc_cnt++;
	}

	switch(net_fault_info.net_fault_level)
	{
		case NET_FAULT_LEVEL_0:
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_0\r\n");
		
		break;
		
		case NET_FAULT_LEVEL_1:											//����ȼ�1-��������IP
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_1\r\n");
			
			net_device_info.send_count = 0;
			
			onenet_info.connect_ip = 0;
			onenet_info.net_work = 0;
			
			NET_DEVICE_Close();
			
			if(++net_fault_info.net_fault_count >= 5)
			{
				net_fault_info.net_fault_count = 0;
				net_fault_info.net_fault_level = NET_FAULT_LEVEL_3;		//����ȼ�3
			}
			else
				net_fault_info.net_fault_level = NET_FAULT_LEVEL_0;		//������ȼ��������
		
		break;
		
		case NET_FAULT_LEVEL_2:											//����ȼ�2-��������ע������
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_2\r\n");
			
			net_device_info.device_ok = 0;
			net_device_info.net_work = 0;
			NET_DEVICE_ReConfig(0);
			
			onenet_info.connect_ip = 0;
			onenet_info.net_work = 0;
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_0;			//������ȼ��������
		
		break;
		
		case NET_FAULT_LEVEL_3:											//����ȼ�3-����λ�����豸
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_3\r\n");
			
			NET_DEVICE_Reset();
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_2;			//������ȼ�����
		
		break;
		
		case NET_FAULT_LEVEL_4:											//����ȼ�4-����������豸�����ϵ�
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_4\r\n");
			
			NET_DEVICE_PowerCtl(0);
			
			NET_DEVICE_PowerCtl(1);
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_3;			//������ȼ�����
		
		break;
		
		case NET_FAULT_LEVEL_5:
		
			UsartPrintf(USART_DEBUG, "WARN:	NET_FAULT_LEVEL_5\r\n");
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_0;
		
		break;
		
		default:
		break;
	}

}
