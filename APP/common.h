/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ϵͳ�������弰ͨ�ú�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef __COMMON_H 
#define __COMMON_H

#define print_log(...)	




//==============================��ʱ�����������ṹ��========================================================

typedef struct _timer_st_{

	volatile unsigned char secTimer;//�����뼶 ��ʱ

//==============================10���뼶��ʱ������=====================================================================
	volatile unsigned short printTimer;
	volatile unsigned short pcm_handle_timeout;
	volatile unsigned short checkDeviceTimeout;
	volatile unsigned short HpHandleTimer;
	volatile unsigned int bill_comunication_timeout;//MDBͨѶ��ʱʱ��
	volatile unsigned short led_paoma_timer;
	volatile unsigned char  user_led_green;

//==============================�뼶��ʱ������=====================================================================
	volatile unsigned short sec_usr_op_timer;//�û�������ʱ	
	volatile unsigned short sec_changer_timer;
	volatile unsigned short sec_hopper_state_timer;//hopper  ��ʱ��λ
	volatile unsigned short sec_pccon_timer;//����PC���ʱ
	volatile unsigned char	PayoutTimer;//���ҵȴ���ʱ��
	volatile unsigned char  sec_coin_reset_timer;


}TIMER_ST;
extern TIMER_ST Timer;





/************************************************************************************
*����ṹ��
***************************************************************************************/
//ֽ��������ͨѶ�ṹ��
typedef struct _task_msg_bill_{

	 unsigned char type;
	 unsigned char status;
	 unsigned int  amount;
	 unsigned int  change;

}TASK_MSG_BILL;

//Ӳ��������ͨѶ�ṹ��
typedef struct _task_msg_coin_{

	 unsigned char type;
	 unsigned char status;
	 unsigned int  amount;
	 unsigned int  change;


}TASK_MSG_COIN;


//hopper ����ͨѶ�ṹ��
typedef struct _task_msg_hopper_{

   	 unsigned char value8;
	 unsigned short value16;
	 unsigned int  value32;

}TASK_MSG_HOPPER;
//�������������豸����֮��ͨ�ŵĽṹ��
typedef struct
{
    unsigned char   type;				//��������
	unsigned char   subType;		   //�Ӽ���������
	TASK_MSG_BILL 	bill;
	TASK_MSG_COIN 	coin;
	TASK_MSG_HOPPER hopper;

	
} MAIN_DEV_TASK_MSG;




//����ͨ������ĶԷ�������
extern OS_EVENT *g_msg_main_to_dev;
extern OS_EVENT *g_msg_dev_to_main;
extern MAIN_DEV_TASK_MSG task_msg_main_to_dev;
extern MAIN_DEV_TASK_MSG task_msg_dev_to_main;


//ֽ��������
extern OS_EVENT *g_billIN;
typedef struct _bill_recv_msg_{
	unsigned char channel;
	unsigned int  value;
	
}BILL_RECV_MSG;
#define G_BILL_IN_SIZE    20  
extern BILL_RECV_MSG bill_recv_msg[G_BILL_IN_SIZE];


//Ӳ����ͨ����Ϣ����
#define G_COIN_IN_SIZE    100
extern OS_EVENT *g_CoinIn;
extern unsigned char  CoinIn[G_COIN_IN_SIZE];


//��ʼ�������ź���
extern OS_EVENT *g_InitSem;


/*---------------------------------------------------------
�����ͨ���豸������ܵĲ���ָ���
-----------------------------------------------------------*/
#define TASK_DISABLE           	1 //�������ͽ��� ����

#define TASK_ENABLE            	3 //��������ʹ�� ����
 	 
#define TASK_HOPPER         	5 //��������hopper ����

#define TASK_INIT		    	7//�������ͳ�ʼ������

#define TASK_CHANGER			9 //��������

#define TASK_OVER				0xAA
#define TASK_NOT_ACK			0x0A
//�������
#define OBJ_BILL					0x0001
#define OBJ_COIN					0x0002
#define OBJ_HOPPER					0x0004

#define OBJ_ALL						0xFFFF



//======================================================================================
unsigned char XorCheck(unsigned char *pstr,unsigned short len);
unsigned short CrcCheck(unsigned char *msg, unsigned short len);
void msleep(unsigned int msec);
unsigned char mbox_post_main_to_dev(unsigned char type);
unsigned char mbox_post_dev_to_main(unsigned char type);
void CreateCommonMBox(void);



#endif
/**************************************End Of File*******************************************************/
