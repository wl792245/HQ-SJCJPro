/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	framework.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-12-11
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		�����ܲ�
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//���
#include "framework.h"

//Ӳ������
#include "hwtimer.h"

//C��
#include <stdlib.h>


FW_TASK_INFO fw_task_info;


/*
************************************************************
*	�������ƣ�	FW_Init
*
*	�������ܣ�	��ܳ�ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void FW_Init(void)
{

	Timer_X_Init(TIM7, (10000 / FW_RUN_HZ) - 1, 7199, 1, 0);		//72MHz��7200��Ƶ-100us
	
}

/*
************************************************************
*	�������ƣ�	FW_CreateTask
*
*	�������ܣ�	���񴴽�
*
*	��ڲ�����	task������ָ��
*				ticks��������������
*
*	���ز�����	0-�ɹ�	1-ʧ��
*
*	˵����		�ȴ������������ȼ���
************************************************************
*/
unsigned char FW_CreateTask(task_fun task, unsigned short ticks)
{

	struct FW_TASK_TCB *current = (struct FW_TASK_TCB *)malloc(sizeof(struct FW_TASK_TCB));
																//�����ڴ�
	
	if(current == NULL)
		return 1;
	
	if(++fw_task_info.task_cnt > FW_MAX_TASKS)					//���������
	{
		fw_task_info.task_cnt--;
		free(current);
		
		return 2;
	}
	
	if(task == FW_NULL)
	{
		free(current);
		
		return 3;
	}
	
	if(fw_task_info.fw_task_ctb_h == NULL)						//���headΪNULL
		fw_task_info.fw_task_ctb_h = current;					//headָ��ǰ������ڴ���
	else														//���head��ΪNULL
		fw_task_info.fw_task_ctb_e->next = current;				//��endָ��ǰ������ڴ���
	
	current->task = task;
	current->ticks = ticks;
	current->ticks_count = fw_task_info.fw_ticks;
	current->status = FW_READY;
	current->next = NULL;										//��һ��ΪNULL
	
	fw_task_info.fw_task_ctb_e = current;						//endָ��ǰ������ڴ���
	
	return 0;

}

/*
************************************************************
*	�������ƣ�	FW_TickHandler
*
*	�������ܣ�	
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void FW_TickHandler(void)
{
	
	unsigned char i = 0;
	
	struct FW_TASK_TCB *current = fw_task_info.fw_task_ctb_h;

	fw_task_info.fw_ticks++;
	
	for(i = 0; i < fw_task_info.task_cnt; i++)
	{
		if(fw_task_info.fw_ticks >= current->ticks + current->ticks_count)
		{
			current->status = FW_READY;
		}
		
		if(current->next == FW_NULL)
			break;
		else
			current = current->next;
	}

}

/*
************************************************************
*	�������ƣ�	FW_StartSchedule
*
*	�������ܣ�	���������ĵ���
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��������������ȼ�������
************************************************************
*/
void FW_StartSchedule(void)
{

	unsigned char i = 0;
	
	struct FW_TASK_TCB *current = fw_task_info.fw_task_ctb_h;
	
	while(1)
	{
		for(i = 0; i < fw_task_info.task_cnt; i++)
		{
			if(current->status == FW_READY)
			{
				if(current->task != FW_NULL)
				{
					current->status = FW_RUNNING;
					
					current->task();
					
					current->status = FW_SUSPEND;
					current->ticks_count = fw_task_info.fw_ticks;
					
					current = fw_task_info.fw_task_ctb_h;
					
					break;
				}
			}
			else
			{
				if(fw_task_info.fw_ticks >= current->ticks + current->ticks_count)
				{
					current->status = FW_READY;
				}
				
				if(current->next == FW_NULL)
					current = fw_task_info.fw_task_ctb_h;
				else
					current = current->next;
			}
		}
	}

}

/*
************************************************************
*	�������ƣ�	FW_GetTicks
*
*	�������ܣ�	��ȡ��ǰticks
*
*	��ڲ�����	��
*
*	���ز�����	ticks
*
*	˵����		��������������ȼ�������
************************************************************
*/
unsigned int FW_GetTicks(void)
{

	return fw_task_info.fw_ticks;

}

/*
************************************************************
*	�������ƣ�	TIM7_IRQHandler
*
*	�������ܣ�	Timer7�����жϷ�����
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void TIM7_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		
		TIM7->SR &= 0xFFFE;
		
		FW_TickHandler();
	}

}
