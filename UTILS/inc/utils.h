/******************** (C) COPYRIGHT 2021 ���պ��߿Ƽ����޹�˾ ********************
 * �ļ���   ��utils.h
 * ��Ŀ���� ���ַ��������
 * ����ƽ̨ ������STM32F103ZET6оƬ����
 * ��  ��   ��Terry  QQ: 792245610
 * ʱ  ��   ��2021-1-11
**********************************************************************************/
#ifndef __UTILS_H_
#define __UTILS_H_

#include "stm32f10x.h"

//����
#include "usart.h"

extern u8 commandStringBuffer[USART_RXBUFF_SIZE];
extern u8 commandStringLength;

//�����ڻ����������ó����Ա��Ժ���
void Command_Copy(void);
//��⴮�ڽ��յ������Ƿ�ȫ���ǿո�,�����򷵻�1,�Ƿ���0
u8 Command_Is_Vailed(void);
//ȥ�������ַ�����ǰ��Ŀո��ַ�
void Command_Remove_Space_Head(void);
//ȥ�������ַ�������Ŀո�
void Command_Remove_Space_End(void);
//ȥ�������ַ����м�Ŀո�,�����������Ŀո�ϲ���һ��
void Command_Remove_Space_Inner(void);
//��ȡ�ַ�������
u8 StringGetLength(u8* string);
//����0��ʶ�Ҳ����ո�,���кŴ�1��ʼ
u8 Command_Find_Space_Postion(u8 index);


void Byte_Split(unsigned char *pDest, unsigned char *pSrc, int len);
void HexToChar(unsigned char *pDest, unsigned char *pSrc, int len);
void StrToHex( unsigned char *pbDest, unsigned char *pbSrc, int nLen);
void  StrToDec(unsigned char *pbDest, char *pbSrc);
int StringToInt(char* pbSrc);
#endif

/******************** (C) COPYRIGHT 2021 ���պ��߿Ƽ����޹�˾ ********************/
