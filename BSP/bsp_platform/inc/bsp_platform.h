#ifndef _BSP_PLATFORM_H_
#define _BSP_PLATFORM_H_

/*底层平台层*/


struct MODBUS_LIST
{
	unsigned short *buf;			//ModBus---水表传感器值缓存表
	unsigned short dataLen;		//ModBus---水表传感器值的个数
	struct MODBUS_LIST *next;	//下一个
};

typedef struct BSP_PLATFORM_INFO BSP_PLATFORM_INFO;
struct BSP_PLATFORM_INFO
{

	unsigned char *cmd_ptr;
	
	unsigned char s_addr;					//ModBus---本机地址
	unsigned char m_cmd;					//ModBus---收到的命令码
	unsigned short r_addr;				//ModBus---寄存器地址
	unsigned short r_len;					//ModBus---寄存器读取长度
	unsigned char rev_cmd_cnt;		//收到的命令个数
/*************************发送队列*************************/
	struct MODBUS_LIST *head, *end;
	
	unsigned char send_data;
	
//unsigned short net_work : 1;	//1-OneNET接入成功		0-OneNET接入失败
//unsigned short err_count : 3;	//错误计数
//unsigned short heart_beat : 1;	//心跳
//unsigned short get_ip : 1;		//获取到最优登录IP
//unsigned short connect_ip : 1;	//连接了IP
//unsigned short err_check : 1;	//错误检测
//unsigned short reverse : 6;

};
#endif
