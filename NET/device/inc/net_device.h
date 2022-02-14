#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_


//=============================����==============================
//===========�����ṩRTOS���ڴ��������Ҳ����ʹ��C���=========
#include "tlsf.h"

#define NET_MallocBuffer	tlsf_malloc

#define NET_FreeBuffer		tlsf_free
//==========================================================


#define GPS_EN					0			//1-ʹ��GPS			0-��ʹ��


#define LBS_EN					1			//1-��ȡ��վ��Ϣ		0-����ȡ


#define NET_TIME_EN				0			//1-��ȡ����ʱ��		0-����ȡ

#define NET_DEVICE_OK			0

#define NET_DEVICE_Err			1

typedef struct NET_SEND_LIST NET_SEND_LIST;

struct NET_SEND_LIST
{

	unsigned short dataLen;					//���ݳ���
	unsigned char *buf;							//����ָ��
	
	struct NET_SEND_LIST *next;				//��һ��

};

typedef struct NET_DEVICE_INFO NET_DEVICE_INFO;

struct NET_DEVICE_INFO
{
	
	char *cmd_resp;										//����ظ�ָ�롣�����ȡ����ص����ݣ�������ȡ��ָ���ڵ�����
	char *cmd_ipd;										//�ڽ���onenetǰͨ�������ȡ�����ݣ���������ʱ�䡢��ȡ����ip��
	char *cmd_hdl;										//������ڷ�������󣬻��ڷ��������������û�ָ���ķ�������
	
/*************************���Ͷ���*************************/
	struct NET_SEND_LIST *head, *end;
	
	unsigned int net_time;						//����ʱ��
	
	int data_bytes;										//���յ������ݳ���
	
	signed char signal;								//�ź�ֵ
	
	unsigned short network_delay_time;	//�����ӳ�ʱ��(����)
	
	unsigned short send_time;					//ģ����������ȶ�����ʱ��������
	
	unsigned short err : 4; 					//��������
	unsigned short init_step : 4;			//��ʼ������
	unsigned short reboot : 1;				//����������־
	unsigned short net_work : 1;			//�������OK
	unsigned short device_ok : 1;			//�豸���
	unsigned short send_count : 3;		//�����豸��ķ��ͳɹ�
	unsigned short reverse : 2;				//����

};

extern NET_DEVICE_INFO net_device_info;

typedef struct GPS_INFO GPS_INFO;

struct GPS_INFO
{

	char lon[24];
	char lat[24];
	
	_Bool flag;

};

extern GPS_INFO gps;

#if(GPS_EN == 1)
typedef struct
{

	float altitude;							//����
	float speed;							//�ٶ�
	int satellite;							//������

} GPS_EX_INFO;

extern GPS_EX_INFO gps_ex_info;
#endif


#if(LBS_EN == 1)
typedef struct  LBS_INFO LBS_INFO;
struct LBS_INFO
{

	//�ƶ����š��ƶ����Һ���Ĭ��Ϊ0��460
	char cell_id[16];						//��վ��
	char lac[16];							//����������
	
	/*
		0 GSM 
		1 GSM Compact 
		2 UTRAN 
		3 GSM w/EGPRS 
		4 UTRAN w/HSDPA 
		5 UTRAN w/HSUPA 
		6 UTRAN w/HSDPA and HSUPA (
		7 E-UTRAN
		8 EC-GSM-IoT 
		9 E-UTRAN 
		10 E-UTRA connected to a 5G CN 
		11 NR connected to a 5G CN 
		12 NR connected to an EPS core 
		13 NG-RAN 
		14 E-UTRA-NR dual connectivity
	*/
	unsigned char network_type;				//������ʽ
	
	unsigned char flag;						//10-ʮ����		16-ʮ������
	
	_Bool lbs_ok;

};

extern LBS_INFO lbs_info;
#endif

typedef enum NET_DEVICE_SELECT NET_DEVICE_SELECT;
enum NET_DEVICE_SELECT
{
	M6312 = 0,
	M5310A,
	
};

#define NET_DEVICE_CONNECTED	0			//������
#define NET_DEVICE_CONNECTING	1			//������
#define NET_DEVICE_CLOSED			2			//�ѶϿ�
#define NET_DEVICE_GOT_IP			3			//�ѻ�ȡ��IP
#define NET_DEVICE_NO_DEVICE	4			//���豸
#define NET_DEVICE_INITIAL		5			//��ʼ��״̬
#define NET_DEVICE_NO_CARD		6			//û��sim��
#define NET_DEVICE_BUSY			254			//æ
#define NET_DEVICE_NO_ERR		255 		//�޴���


void NET_DEVICE_IO_Init(void);

void NET_DEVICE_PowerCtl(_Bool flag);

_Bool NET_DEVICE_GetSerial(char **serial);

signed char NET_DEVICE_GetSignal(void);

_Bool NET_DEVICE_GetGps(char *lon, char *lat, float *altitude, float *speed, int *satellite, unsigned char gps_mode);

_Bool NET_DEVICE_Exist(void);

//_Bool NET_DEVICE_Init(void);
_Bool NET_DEVICE_Init(NET_DEVICE_SELECT net_device_select);

void NET_DEVICE_Reset(void);

_Bool NET_DEVICE_Close(void);

_Bool NET_DEVICE_Connect(char *type, char *ip, char *port);

void NET_DEVICE_CmdHandle(char *cmd);

_Bool NET_DEVICE_SendCmd(char *cmd, char *res, unsigned short time_out);

_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len);

unsigned char *NET_DEVICE_Read(void);

unsigned char *NET_DEVICE_GetIPD(unsigned char *data_ptr);

unsigned char NET_DEVICE_Check(void);

void NET_DEVICE_ReConfig(unsigned char step);

_Bool NET_DEVICE_CheckListHead(void);

unsigned char *NET_DEVICE_GetListHeadBuf(void);

unsigned short NET_DEVICE_GetListHeadLen(void);

unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short data_len, _Bool mode);

_Bool NET_DEVICE_DeleteDataSendList(void);

#endif
