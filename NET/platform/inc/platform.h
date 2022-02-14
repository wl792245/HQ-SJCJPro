#ifndef _PLATFORM_H_
#define _PLATFORM_H_


typedef struct ONETNET_INFO ONETNET_INFO;
struct ONETNET_INFO
{
	char dev_id[16];
  char api_key[32];
	
	char pro_id[10];
	char auif[50];
	
	char reg_code[24];
	
	char ip[16];
	char port[8];
	
	char *file_bin_name;
	const unsigned char *file_bin;
	unsigned int file_bin_size;
	
	const unsigned char protocol;	//协议类型号		1-edp	2-nwx	3-jtext		4-Hiscmd
									//				5-jt808			6-modbus	7-mqtt
									//				8-gr20			9-reg		10-HTTP(自定义)
	
	unsigned char *cmd_ptr;
	
	unsigned char send_data;
	
	unsigned short net_work : 1;	//1-OneNET接入成功		0-OneNET接入失败
	unsigned short err_count : 3;	//错误计数
	unsigned short heart_beat : 1;	//心跳
	unsigned short get_ip : 1;		//获取到最优登录IP
	unsigned short lbs : 1;			//1-已获取到了位置坐标
	unsigned short lbs_count : 3;	//获取计数
	unsigned short connect_ip : 1;	//连接了IP
	unsigned short err_check : 1;	//错误检测
	unsigned short reverse : 4;
};

#endif
