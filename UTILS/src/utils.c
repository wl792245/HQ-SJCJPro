/******************** (C) COPYRIGHT 2021 江苏恒沁科技有限公司 ********************
 * 文件名   ：utils.c
 * 项目描述 ：字符处理定义
 * 开发平台 ：基于STM32F103ZET6芯片开发
 * 作  者   ：Terry  QQ: 792245610
 * 时  间   ：2021-1-11
**********************************************************************************/
#include "utils.h"



//C库
#include "math.h" 
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

u8 commandStringBuffer[USART_RXBUFF_SIZE] = {0};//用于暂时缓存串口中的命令数据
u8 commandStringLength = 0;						//记录串口中的命令的长度

/*******************************串口命令预处理函数定义********************************/
//将串口缓冲区数据拿出来以备以后处理
void Command_Copy(void)
{
	u8 i = 0;
	memset(commandStringBuffer, 0, sizeof(commandStringBuffer));						//清空buffer
	commandStringLength = (strlen((char *)alter_info.alter_buf)&0xff);
	for(i = 0; i < commandStringLength; i++)
	{
		commandStringBuffer[i] = alter_info.alter_buf[i];
	}
	//清除串口缓冲区标志
	//serial_Buffer_Length = 0;
}

//检测串口接收到数据是否全部是空格,不是则返回1,是返回0
u8 Command_Is_Vailed(void)
{
	u8 i = 0;
	int length = strlen((char *)alter_info.alter_buf);
	for(i = 0; i < (length&0xff);i++)
	{
		if(alter_info.alter_buf[i] == ' ')continue;
		else return 1;
	}
	return 0;
}
//去除命令字符串的前面的空格字符
void Command_Remove_Space_Head(void)
{
	u8 index = 0;
	u8 i = 0;
	for(index = 0; index < commandStringLength; index++)
	{
		if(commandStringBuffer[index] == ' ')continue;
		else break;
	}
	if(index == 0)//前面没有空格
	{
		return;
	}
	else
	{
		//删除空格
		for(i = 0; i < (commandStringLength-index);i++)
		{
			commandStringBuffer[i] = commandStringBuffer[index+i];
		}
		commandStringLength -= index;
	}
}
//去除命令字符串后面的空格
void Command_Remove_Space_End(void)
{
	u8 i = 0;
	//寻找字符串最尾巴上空格的位置
	for(i = commandStringLength; i > 0; i--)
	{
		if(commandStringBuffer[i-1] == ' ')continue;//如果这个是空格,继续下一次寻找
		else break;//不是空格,到此为止
	}
	if(i == commandStringLength)//尾上没有空格
	{
		return;
	}
	else		//尾上有空格
	{
		commandStringBuffer[i] = '\0';
		commandStringLength = i;
		return;
	}
	
}
//去除命令字符串中间的空格,将连续两个的空格合并成一个
void Command_Remove_Space_Inner(void)
{
	u8 spaceCount;
	u8 i = 0;
	u8 j = 0;
	for(i = 0; i < commandStringLength; i++)
	{
		//此时检测到一个空格
		if(commandStringBuffer[i] == ' ')
		{
			//立刻查看下一个是不是空格
			if(commandStringBuffer[i+1] == ' ')
			{
				spaceCount = 2;
				//下一个也是空格,此时说明已经有了两个连续的空格了必须立刻查找到结束的空格在哪
				for(j = i+2; j < commandStringLength; j++)
				{
					//当不是空格的时候跳出来,是空格就一直加
					if(commandStringBuffer[j] == ' ')spaceCount++;
					else break;
				}
				//跳出来根据space的值来移动数组,同时减小长度
				//i是第一个空格,i+1是第二个空格,最后一个空格是spaceCount-2
				for(j = i+1;j < commandStringLength-spaceCount+1;j++)
				{
					//要跳过spacecount-1的数量,来拷贝有效字符
					commandStringBuffer[j] = commandStringBuffer[j+spaceCount-1];
				}
				//最后修改长度,长度缩减量是空格数-1,因为保留了一个空格
				commandStringLength -= (spaceCount-1);
			}
			else
			{
				//下一个不是空格,说明是只有一个空格的环境,不用操心,进行下一次循环
				continue;
			}
		}
		
	}
}
/***********************************************************************************************/

/***********************************串口命令解析函数*******************************************/
//获取字符串长度
u8 StringGetLength(u8* string)
{
	u8 i = 0; 
	while(*(string+i) != '\0')
	{
		i++;
	}
	return i;
}
//寻找命令字符串中的空格位置,也就是空格的下标
//返回0标识找不到空格,序列号从1开始
u8 Command_Find_Space_Postion(u8 index)
{
	//这个函数针对的是给定的字符串,不是任意字符串
	u8 i = 0;
	u8 spaceCount = 0;
	for(i = 0; i < commandStringLength; i++)
	{
		if(commandStringBuffer[i] == ' ')
		{
			spaceCount++;
			if(spaceCount == index)
				return i;
		}
		else 
			continue;
	}
	return 0;
}
/*******************************************************************************
* 函 数 名         : Byte_Split
* 函数功能		     : 字节拆解
* 输    入         : pDest：目标数组， pSrc：源数组；len：拆分长度
* 输    出         : 无
*******************************************************************************/
void Byte_Split(unsigned char *pDest, unsigned char *pSrc, int len)
{
	unsigned char i;
	for(i = 0; i < len/2; i++)
	{
		pDest[2*i] = pSrc[i]>>4; //高4位
		pDest[2*i+1] = pSrc[i]&0xf;    //低4位
	}
	pDest[len] = '\0';
}
/*******************************************************************************
* 函 数 名         : HexToChar
* 函数功能		     : 16进制转字符
* 输    入         : pDest：目标数组， pSrc：源数组；len：数据长度
* 输    出         : 无
*******************************************************************************/
void HexToChar(unsigned char *pDest, unsigned char *pSrc, int len)
{
	unsigned char i;
	for (i=0; i<len;i++)
	{
		if(pSrc[i] <= 0x09)
		{
			pDest[i] = pSrc[i]+'0'; 
		}
		else if ((pSrc[i] <= 0x0F) && (pSrc[i] >= 0x0A))
		{
			pDest[i] = pSrc[i] - 0X0A +'A'; 
		}
		else
		{
			pDest[i] = '0';
		}
	}
	pDest[i] = '\0';
}
/*******************************************************************************
* 函 数 名         : StrToHex
* 函数功能		     : 字符转16进制
* 输    入         : pDest：目标数组， pSrc：源数组；len：数据长度
* 输    出         : 无
*******************************************************************************/
void StrToHex( unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
	char h1,h2;
	uint8_t s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
		s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
		s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}
/*******************************************************************************
* 函 数 名         : StrToDec
* 函数功能		     : 字符转10进制
* 输    入         : pbSrc:源数组
* 输    出         : 无
*******************************************************************************/
void  StrToDec(unsigned char *pbDest, char *pbSrc)
{
	char h1,h2;
	uint8_t s1,s2;

	h1 = pbSrc[0];
	h2 = pbSrc[1];
	if ((h1==0x20)&&(h2==0x20))
		*pbDest = 0;
	if ((h1==0x20)&& (h2!=0x20))
	{
		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
		s2 -= 7;
		*pbDest = s2;
	}
	if ((h2==0x20)&& (h1!=0x20))
	{
		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
		s1 -= 7;
		*pbDest = s1;
	}
	if ((h2!=0x20)&& (h1!=0x20))
	{
		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
		s1 -= 7;
		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
		s2 -= 7;
		*pbDest = s1*10 + s2;
	}
}

int StringToInt(char* pbSrc)
{
  const char* ptr = pbSrc;
  int priceNum = 0;
  while (*ptr != '\0')
  {
    priceNum *= 16;
    if((*ptr >= '0') && (*ptr <= '9'))
      priceNum += *ptr - '0';
    if ((*ptr >= 'A') && (*ptr <= 'Z'))
      priceNum += *ptr - 'A';
    if ((*ptr >= 'a') && (*ptr <= 'z'))
      priceNum += *ptr - 'a'+ 10;
    ptr++;
  }
	return priceNum;
}
/******************** (C) COPYRIGHT 2021 江苏恒沁科技有限公司 ********************/
