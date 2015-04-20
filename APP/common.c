/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ϵͳ�������弰ͨ�ú�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"




/***********************************************************************************************
*  ����ϵͳ����
*  add  by yoc 2014.2.18
************************************************************************************************************/

/***********************************************************************************************
*  ��ʱ��
*  add  by yoc 2014.2.18
************************************************************************************************************/


TIMER_ST Timer;




//����ͨ������ĶԷ�������
OS_EVENT *g_msg_main_to_dev;
OS_EVENT *g_msg_dev_to_main;
MAIN_DEV_TASK_MSG task_msg_main_to_dev;
MAIN_DEV_TASK_MSG task_msg_dev_to_main;



//ֽ����ͨ������
OS_EVENT *g_billIN;
BILL_RECV_MSG bill_recv_msg[G_BILL_IN_SIZE]; 
void *billInNum[G_BILL_IN_SIZE];

//Ӳ����ͨ����Ϣ����
OS_EVENT *g_CoinIn;
unsigned char  CoinIn[G_COIN_IN_SIZE] = {0};
void *coinInNum[G_COIN_IN_SIZE];




//��ʼ�������ź���
OS_EVENT *g_InitSem;



void    TASK_Device(void *pdata); 




/*********************************************************************************************************
** Function name:     	XorCheck
** Descriptions:	    �ۼ�У���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char XorCheck(unsigned char *pstr,unsigned short len)
{
	unsigned char xor_check = 0;
	unsigned short i;
	for(i=0;i<len;i++) 
	{
       	xor_check = xor_check^pstr[i];
   	}
	return xor_check;
}
/*********************************************************************************************************
** Function name:     	CrcCheck
** Descriptions:	    CRCУ���
** input parameters:    msg��Ҫ���������;len���ݳ���
** output parameters:   ��
** Returned value:      CRC������
*********************************************************************************************************/
unsigned short CrcCheck(unsigned char *msg, unsigned short len) 
{
    unsigned short i, j;
    unsigned short crc = 0;
    unsigned short current = 0;
    for(i=0;i<len;i++) 
    {
        current = msg[i] << 8;
        for(j=0;j<8;j++) 
        {
            if((short)(crc^current)<0)
                crc = (crc<<1)^0x1021;
            else 
                crc <<= 1; 
            current <<= 1; 
        }
    }
    return crc;
}






/*********************************************************************************************************
** Function name:     	msleep
** Descriptions:	    ����˯�ߺ���  �Ƚ�
** input parameters:    msg��Ҫ���������;len���ݳ���
** output parameters:   ��
** Returned value:      CRC������
*********************************************************************************************************/

void msleep(unsigned int msec)
{
	unsigned int temp;
	
	temp = (msec > 10) ? msec/5 : 2;
	OSTimeDly(temp);
}



/*********************************************************************************************************
** Function name:       mbox_post_main_to_dev
** Descriptions:        �����������䵽�豸����
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char mbox_post_main_to_dev(unsigned char type)
{
	unsigned char rst;
	
	task_msg_main_to_dev.type = type;
	Trace("task_msg_main_to_dev POST:%dr\n",type);
	rst = OSMboxPost(g_msg_main_to_dev,&task_msg_main_to_dev);
	OSTimeDly(5);
	return (rst == OS_NO_ERR) ? 1 : 0;
}




/*********************************************************************************************************
** Function name:       mbox_post_dev_to_main
** Descriptions:        �豸��������������䷢�ͺ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char mbox_post_dev_to_main(unsigned char type)
{
	unsigned char rst;
	task_msg_dev_to_main.type = type;
	Trace("task_msg_dev_to_main POST:%dr\n",type);
	rst = OSMboxPost(g_msg_dev_to_main,&task_msg_dev_to_main);
	OSTimeDly(5);
	return (rst == OS_NO_ERR) ? 1 : 0;
}

/*********************************************************************************************************
** Function name:       CreateCommonMBox
** Descriptions:        ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/

void CreateCommonMBox(void)
{
	//�����������豸���������
	g_msg_main_to_dev = OSMboxCreate(NULL);
	//�����豸���񷵻������������
	g_msg_dev_to_main = OSMboxCreate(NULL);
	
	g_billIN = OSQCreate(&billInNum[0],G_BILL_IN_SIZE);
	g_CoinIn = OSQCreate(&coinInNum[0],G_COIN_IN_SIZE);
	//������ʼ�������ź���
	g_InitSem = OSSemCreate(0);
}


/**************************************End Of File*******************************************************/
