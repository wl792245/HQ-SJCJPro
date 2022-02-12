/******************** (C) COPYRIGHT 2021 ���պ��߿Ƽ����޹�˾ ********************
 * �ļ���   ��utils.c
 * ��Ŀ���� ���ַ�������
 * ����ƽ̨ ������STM32F103ZET6оƬ����
 * ��  ��   ��Terry  QQ: 792245610
 * ʱ  ��   ��2021-1-11
**********************************************************************************/
#include "utils.h"



//C��
#include "math.h" 
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

u8 commandStringBuffer[USART_RXBUFF_SIZE] = {0};//������ʱ���洮���е���������
u8 commandStringLength = 0;						//��¼�����е�����ĳ���

/*******************************��������Ԥ����������********************************/
//�����ڻ����������ó����Ա��Ժ���
void Command_Copy(void)
{
	u8 i = 0;
	memset(commandStringBuffer, 0, sizeof(commandStringBuffer));						//���buffer
	commandStringLength = (strlen((char *)alter_info.alter_buf)&0xff);
	for(i = 0; i < commandStringLength; i++)
	{
		commandStringBuffer[i] = alter_info.alter_buf[i];
	}
	//������ڻ�������־
	//serial_Buffer_Length = 0;
}

//��⴮�ڽ��յ������Ƿ�ȫ���ǿո�,�����򷵻�1,�Ƿ���0
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
//ȥ�������ַ�����ǰ��Ŀո��ַ�
void Command_Remove_Space_Head(void)
{
	u8 index = 0;
	u8 i = 0;
	for(index = 0; index < commandStringLength; index++)
	{
		if(commandStringBuffer[index] == ' ')continue;
		else break;
	}
	if(index == 0)//ǰ��û�пո�
	{
		return;
	}
	else
	{
		//ɾ���ո�
		for(i = 0; i < (commandStringLength-index);i++)
		{
			commandStringBuffer[i] = commandStringBuffer[index+i];
		}
		commandStringLength -= index;
	}
}
//ȥ�������ַ�������Ŀո�
void Command_Remove_Space_End(void)
{
	u8 i = 0;
	//Ѱ���ַ�����β���Ͽո��λ��
	for(i = commandStringLength; i > 0; i--)
	{
		if(commandStringBuffer[i-1] == ' ')continue;//�������ǿո�,������һ��Ѱ��
		else break;//���ǿո�,����Ϊֹ
	}
	if(i == commandStringLength)//β��û�пո�
	{
		return;
	}
	else		//β���пո�
	{
		commandStringBuffer[i] = '\0';
		commandStringLength = i;
		return;
	}
	
}
//ȥ�������ַ����м�Ŀո�,�����������Ŀո�ϲ���һ��
void Command_Remove_Space_Inner(void)
{
	u8 spaceCount;
	u8 i = 0;
	u8 j = 0;
	for(i = 0; i < commandStringLength; i++)
	{
		//��ʱ��⵽һ���ո�
		if(commandStringBuffer[i] == ' ')
		{
			//���̲鿴��һ���ǲ��ǿո�
			if(commandStringBuffer[i+1] == ' ')
			{
				spaceCount = 2;
				//��һ��Ҳ�ǿո�,��ʱ˵���Ѿ��������������Ŀո��˱������̲��ҵ������Ŀո�����
				for(j = i+2; j < commandStringLength; j++)
				{
					//�����ǿո��ʱ��������,�ǿո��һֱ��
					if(commandStringBuffer[j] == ' ')spaceCount++;
					else break;
				}
				//����������space��ֵ���ƶ�����,ͬʱ��С����
				//i�ǵ�һ���ո�,i+1�ǵڶ����ո�,���һ���ո���spaceCount-2
				for(j = i+1;j < commandStringLength-spaceCount+1;j++)
				{
					//Ҫ����spacecount-1������,��������Ч�ַ�
					commandStringBuffer[j] = commandStringBuffer[j+spaceCount-1];
				}
				//����޸ĳ���,�����������ǿո���-1,��Ϊ������һ���ո�
				commandStringLength -= (spaceCount-1);
			}
			else
			{
				//��һ�����ǿո�,˵����ֻ��һ���ո�Ļ���,���ò���,������һ��ѭ��
				continue;
			}
		}
		
	}
}
/***********************************************************************************************/

/***********************************���������������*******************************************/
//��ȡ�ַ�������
u8 StringGetLength(u8* string)
{
	u8 i = 0; 
	while(*(string+i) != '\0')
	{
		i++;
	}
	return i;
}
//Ѱ�������ַ����еĿո�λ��,Ҳ���ǿո���±�
//����0��ʶ�Ҳ����ո�,���кŴ�1��ʼ
u8 Command_Find_Space_Postion(u8 index)
{
	//���������Ե��Ǹ������ַ���,���������ַ���
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
* �� �� ��         : Byte_Split
* ��������		     : �ֽڲ��
* ��    ��         : pDest��Ŀ�����飬 pSrc��Դ���飻len����ֳ���
* ��    ��         : ��
*******************************************************************************/
void Byte_Split(unsigned char *pDest, unsigned char *pSrc, int len)
{
	unsigned char i;
	for(i = 0; i < len/2; i++)
	{
		pDest[2*i] = pSrc[i]>>4; //��4λ
		pDest[2*i+1] = pSrc[i]&0xf;    //��4λ
	}
	pDest[len] = '\0';
}
/*******************************************************************************
* �� �� ��         : HexToChar
* ��������		     : 16����ת�ַ�
* ��    ��         : pDest��Ŀ�����飬 pSrc��Դ���飻len�����ݳ���
* ��    ��         : ��
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
* �� �� ��         : StrToHex
* ��������		     : �ַ�ת16����
* ��    ��         : pDest��Ŀ�����飬 pSrc��Դ���飻len�����ݳ���
* ��    ��         : ��
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
* �� �� ��         : StrToDec
* ��������		     : �ַ�ת10����
* ��    ��         : pbSrc:Դ����
* ��    ��         : ��
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
/******************** (C) COPYRIGHT 2021 ���պ��߿Ƽ����޹�˾ ********************/
