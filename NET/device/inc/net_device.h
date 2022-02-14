#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_


//=============================配置==============================
//===========可以提供RTOS的内存管理方案，也可以使用C库的=========
#include "tlsf.h"

#define NET_MallocBuffer	tlsf_malloc

#define NET_FreeBuffer		tlsf_free
//==========================================================


#define GPS_EN					0			//1-使能GPS			0-不使能


#define LBS_EN					1			//1-获取基站信息		0-不获取


#define NET_TIME_EN				0			//1-获取网络时间		0-不获取

#define NET_DEVICE_OK			0

#define NET_DEVICE_Err			1

typedef struct NET_SEND_LIST NET_SEND_LIST;

struct NET_SEND_LIST
{

	unsigned short dataLen;					//数据长度
	unsigned char *buf;							//数据指针
	
	struct NET_SEND_LIST *next;				//下一个

};

typedef struct NET_DEVICE_INFO NET_DEVICE_INFO;

struct NET_DEVICE_INFO
{
	
	char *cmd_resp;										//命令回复指针。比如获取命令返回的数据，可以提取此指针内的数据
	char *cmd_ipd;										//在接入onenet前通过网络获取的数据，比如网络时间、获取接入ip等
	char *cmd_hdl;										//命令处理。在发送命令后，会在返回内容里搜索用户指定的返回数据
	
/*************************发送队列*************************/
	struct NET_SEND_LIST *head, *end;
	
	unsigned int net_time;						//网络时间
	
	int data_bytes;										//接收到的数据长度
	
	signed char signal;								//信号值
	
	unsigned short network_delay_time;	//网络延迟时间(毫秒)
	
	unsigned short send_time;					//模组最短数据稳定发送时间间隔发送
	
	unsigned short err : 4; 					//错误类型
	unsigned short init_step : 4;			//初始化步骤
	unsigned short reboot : 1;				//死机重启标志
	unsigned short net_work : 1;			//网络访问OK
	unsigned short device_ok : 1;			//设备检测
	unsigned short send_count : 3;		//网络设备层的发送成功
	unsigned short reverse : 2;				//保留

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

	float altitude;							//海拔
	float speed;							//速度
	int satellite;							//卫星数

} GPS_EX_INFO;

extern GPS_EX_INFO gps_ex_info;
#endif


#if(LBS_EN == 1)
typedef struct  LBS_INFO LBS_INFO;
struct LBS_INFO
{

	//移动网号、移动国家号码默认为0和460
	char cell_id[16];						//基站码
	char lac[16];							//地区区域码
	
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
	unsigned char network_type;				//网络制式
	
	unsigned char flag;						//10-十进制		16-十六进制
	
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

#define NET_DEVICE_CONNECTED	0			//已连接
#define NET_DEVICE_CONNECTING	1			//连接中
#define NET_DEVICE_CLOSED			2			//已断开
#define NET_DEVICE_GOT_IP			3			//已获取到IP
#define NET_DEVICE_NO_DEVICE	4			//无设备
#define NET_DEVICE_INITIAL		5			//初始化状态
#define NET_DEVICE_NO_CARD		6			//没有sim卡
#define NET_DEVICE_BUSY			254			//忙
#define NET_DEVICE_NO_ERR		255 		//无错误


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
