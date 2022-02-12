#ifndef _ONENET_H_
#define _ONENET_H_





struct MODBUS_LIST
{

	unsigned short *buf;		//ModBus---������ֵ�����
	unsigned short dataLen;		//ModBus---������ֵ�ĸ���
	
	struct MODBUS_LIST *next;	//��һ��

};


typedef struct
{

    char serial[12];
    char pswd[12];
	char pro_id[12];
	
	char ip[16];
	char port[8];
	
	const unsigned char protocol;	//Э�����ͺ�		1-edp	2-nwx	3-jtext		4-Hiscmd
									//				5-jt808			6-modbus	7-mqtt
									//				8-gr20			9-reg		10-HTTP(�Զ���)
	
	unsigned char *cmd_ptr;
	
	unsigned char s_addr;			//ModBus---������ַ
	unsigned char m_cmd;			//ModBus---�յ���������
	unsigned short r_addr;			//ModBus---�Ĵ�����ַ
	unsigned short r_len;			//ModBus---�Ĵ�����ȡ����
	unsigned char rev_cmd_cnt;		//�յ����������
/*************************���Ͷ���*************************/
	struct MODBUS_LIST *head, *end;
	
	unsigned char send_data;
	
	unsigned short net_work : 1;	//1-OneNET����ɹ�		0-OneNET����ʧ��
	unsigned short err_count : 3;	//�������
	unsigned short heart_beat : 1;	//����
	unsigned short get_ip : 1;		//��ȡ�����ŵ�¼IP
	unsigned short connect_ip : 1;	//������IP
	unsigned short err_check : 1;	//������
	unsigned short reverse : 6;

} ONETNET_INFO;

extern ONETNET_INFO onenet_info;


#define SEND_TYPE_OK			(1 << 0)	//
#define SEND_TYPE_DATA			(1 << 1)	//
#define SEND_TYPE_HEART			(1 << 2)	//


_Bool OneNET_GetLinkIP(unsigned char protocol, char *ip, char *port);

_Bool OneNET_ConnectIP(char *ip, char *port);

void OneNET_DevLink(const char *serial, const char *pswd, const char *devid);

unsigned char OneNET_SendData(unsigned short *value_table, unsigned short value_table_cnt);

unsigned char OneNET_SendData_Heart(void);

void OneNET_CmdHandle(void);

void OneNET_RevPro(unsigned char *cmd, unsigned short len);

_Bool OneNET_CheckListHead(void);

unsigned short *OneNET_GetListHeadBuf(void);

unsigned short OneNET_GetListHeadLen(void);

unsigned char OneNET_AddDataSendList(unsigned short *buf ,unsigned short dataLen);

_Bool OneNET_DeleteDataSendList(void);

#endif
