#ifndef _ONENET_H_
#define _ONENET_H_





struct MODBUS_LIST
{

	unsigned short *buf;		//ModBus---传感器值缓存表
	unsigned short dataLen;		//ModBus---传感器值的个数
	
	struct MODBUS_LIST *next;	//下一个

};


typedef struct
{

    char serial[12];
    char pswd[12];
	char pro_id[12];
	
	char ip[16];
	char port[8];
	
	const unsigned char protocol;	//协议类型号		1-edp	2-nwx	3-jtext		4-Hiscmd
									//				5-jt808			6-modbus	7-mqtt
									//				8-gr20			9-reg		10-HTTP(自定义)
	
	unsigned char *cmd_ptr;
	
	unsigned char s_addr;			//ModBus---本机地址
	unsigned char m_cmd;			//ModBus---收到的命令码
	unsigned short r_addr;			//ModBus---寄存器地址
	unsigned short r_len;			//ModBus---寄存器读取长度
	unsigned char rev_cmd_cnt;		//收到的命令个数
/*************************发送队列*************************/
	struct MODBUS_LIST *head, *end;
	
	unsigned char send_data;
	
	unsigned short net_work : 1;	//1-OneNET接入成功		0-OneNET接入失败
	unsigned short err_count : 3;	//错误计数
	unsigned short heart_beat : 1;	//心跳
	unsigned short get_ip : 1;		//获取到最优登录IP
	unsigned short connect_ip : 1;	//连接了IP
	unsigned short err_check : 1;	//错误检测
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
