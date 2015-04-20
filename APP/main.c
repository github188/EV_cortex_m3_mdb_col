/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �������ļ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"
#include "task_device.h"
#include "Global.h"



/*********************************************************************************************************
  ����ȫ�ֱ���
*********************************************************************************************************/


//MAIN
#define MAIN_START						(1)//���׿�ʼ
#define MAIN_FREE						(2)//���׿���
#define MAIN_CHECK_GOODS				(4)//����ǰ���
#define MAIN_OUTGOODS					(5)//����
#define MAIN_OVER						(6)//���׽���
#define MAIN_WEIHU						(7)//ά��ģʽ
#define MAIN_FAULT						(0x16)//����ģʽ
#define MAIN_PAYOUT						(9)//����


//����������
OS_EVENT *g_KeyMsg;






void *KeyMsgArray[2]; // ������������



/*********************************************************************************************************
  ������ ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/
#define TASK_Trade_ID                          7                             /*   ����������ID                       */
#define TASK_Trade_PRIO                        TASK_Trade_ID                      /* �������ȼ�                   */
#define TASK_Trade_STACK_SIZE                  1024                           /* �����û���ջ����             */
OS_STK  TASK_Trade_STACK[TASK_Trade_STACK_SIZE];                                  /* ��������B��ջ                */
extern void    TASK_Trade(void *pdata);                                      /* ��������B                    */


/*********************************************************************************************************
  �豸 ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/   
#define TASK_Device_ID                          5                             /* ����ID                       */
#define TASK_Device_PRIO                        TASK_Device_ID                      /* �������ȼ�                   */
#define TASK_Device_STACK_SIZE                  512                           /* �����û���ջ����             */
OS_STK  TASK_Device_STACK[TASK_Device_STACK_SIZE];                                  /* ��������C ��ջ               */



/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        Ϊ����֮��ͨ�Ŵ���������ź���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CreateMBox(void)
{
	//������������
	g_KeyMsg = OSQCreate(&KeyMsgArray[0],2);
	CreateCommonMBox();
}

/*********************************************************************************************************
** Function name:       SystemInit
** Descriptions:        ϵͳ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemInit()
{
	//ע�⸴λ��Uart0ʹ�ܣ�Uart2/3������
	InitUart0();
	Trace("InitUart0 init over.....\r\n");
	InitUart1();
	InitUart2();
	InitUart3();
    InitBuzzer();
	InitI2C0();
	InitTimer(0,240000);
	
}


/*********************************************************************************************************
** Function name:       TASK_Trade
** Descriptions:       ����������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void TASK_Trade(void *pvData)
{

	pvData = pvData;
	//ϵͳ�����ӿڳ�ʼ��
	SystemInit();
	//�������䡢�ź���	
	CreateMBox();
	//�����豸ά������
	OSTaskCreateExt(TASK_Device, 
				(void *)0,
				&TASK_Device_STACK[TASK_Device_STACK_SIZE-1],
				TASK_Device_PRIO, 
				TASK_Device_ID, 
				&TASK_Device_STACK[0], 
				TASK_Device_STACK_SIZE, 
				(void *)0,
				OS_TASK_OPT_STK_CHK);
	while(1)
	{
		OSTimeDly(200);
	}

}






/*********************************************************************************************************
** Function name:       main
** Descriptions:          �û�������ں���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main(void)
{
    targetInit();																//��ʼ��Ŀ���
    pinInit();      															//IO��ʼ��                                                                     
    OSInit();                                                                                                       
	//���� ����������	
	OSTaskCreateExt(TASK_Trade, 
					(void *)0,
					&TASK_Trade_STACK[TASK_Trade_STACK_SIZE-1],
					TASK_Trade_PRIO, 
					TASK_Trade_ID, 
					&TASK_Trade_STACK[0], 
					TASK_Trade_STACK_SIZE, 
					(void *)0,
					OS_TASK_OPT_STK_CHK);

	OSStart();																	//����������
}
/**************************************End Of File*******************************************************/
