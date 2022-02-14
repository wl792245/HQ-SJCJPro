#ifndef _BSP_PLATFORM_H_
#define _BSP_PLATFORM_H_

/*�ײ�ƽ̨��*/


struct MODBUS_LIST
{
	unsigned short *buf;			//ModBus---ˮ������ֵ�����
	unsigned short dataLen;		//ModBus---ˮ������ֵ�ĸ���
	struct MODBUS_LIST *next;	//��һ��
};

typedef struct BSP_PLATFORM_INFO BSP_PLATFORM_INFO;
struct BSP_PLATFORM_INFO
{

	unsigned char *cmd_ptr;
	
	unsigned char s_addr;					//ModBus---������ַ
	unsigned char m_cmd;					//ModBus---�յ���������
	unsigned short r_addr;				//ModBus---�Ĵ�����ַ
	unsigned short r_len;					//ModBus---�Ĵ�����ȡ����
	unsigned char rev_cmd_cnt;		//�յ����������
/*************************���Ͷ���*************************/
	struct MODBUS_LIST *head, *end;
	
	unsigned char send_data;
	
//unsigned short net_work : 1;	//1-OneNET����ɹ�		0-OneNET����ʧ��
//unsigned short err_count : 3;	//�������
//unsigned short heart_beat : 1;	//����
//unsigned short get_ip : 1;		//��ȡ�����ŵ�¼IP
//unsigned short connect_ip : 1;	//������IP
//unsigned short err_check : 1;	//������
//unsigned short reverse : 6;

};
#endif
