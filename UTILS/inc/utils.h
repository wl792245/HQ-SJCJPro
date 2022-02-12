/******************** (C) COPYRIGHT 2021 江苏恒沁科技有限公司 ********************
 * 文件名   ：utils.h
 * 项目描述 ：字符处理调用
 * 开发平台 ：基于STM32F103ZET6芯片开发
 * 作  者   ：Terry  QQ: 792245610
 * 时  间   ：2021-1-11
**********************************************************************************/
#ifndef __UTILS_H_
#define __UTILS_H_

#include "stm32f10x.h"

//驱动
#include "usart.h"

extern u8 commandStringBuffer[USART_RXBUFF_SIZE];
extern u8 commandStringLength;

//将串口缓冲区数据拿出来以备以后处理
void Command_Copy(void);
//检测串口接收到数据是否全部是空格,不是则返回1,是返回0
u8 Command_Is_Vailed(void);
//去除命令字符串的前面的空格字符
void Command_Remove_Space_Head(void);
//去除命令字符串后面的空格
void Command_Remove_Space_End(void);
//去除命令字符串中间的空格,将连续两个的空格合并成一个
void Command_Remove_Space_Inner(void);
//获取字符串长度
u8 StringGetLength(u8* string);
//返回0标识找不到空格,序列号从1开始
u8 Command_Find_Space_Postion(u8 index);


void Byte_Split(unsigned char *pDest, unsigned char *pSrc, int len);
void HexToChar(unsigned char *pDest, unsigned char *pSrc, int len);
void StrToHex( unsigned char *pbDest, unsigned char *pbSrc, int nLen);
void  StrToDec(unsigned char *pbDest, char *pbSrc);
int StringToInt(char* pbSrc);
#endif

/******************** (C) COPYRIGHT 2021 江苏恒沁科技有限公司 ********************/
