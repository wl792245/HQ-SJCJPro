#ifndef _FAULT_H_
#define _FAULT_H_





typedef enum
{

	FAULT_NONE = 0,		//�޴���
	FAULT_REBOOT,		//������������
	FAULT_PRO,			//Э�����
	FAULT_NODEVICE,		//Ӳ�����ߴ��󣬱���8266����6311�Ӵ�������sim���Ӵ�������Ӳ��ԭ������Ĵ���

} FAULT_TYPE;


typedef enum
{

	NET_FAULT_LEVEL_0 = 0,
	NET_FAULT_LEVEL_1,
	NET_FAULT_LEVEL_2,
	NET_FAULT_LEVEL_3,
	NET_FAULT_LEVEL_4,
	NET_FAULT_LEVEL_5,

} FAULT_LEVEL;


typedef struct
{

	unsigned char net_fault_level;
	
	unsigned char net_fault_level_r;
	
	unsigned char net_fault_count;
	
	_Bool net_fault_flag;
	
	unsigned int net_fault_disc_cnt;

} NET_FAULT_INFO;

extern NET_FAULT_INFO net_fault_info;


void NET_Fault_Process(void);


#endif
