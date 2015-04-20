/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           pcoin.c
** Last modified Date:  2013-01-12
** Last Version:         
** Descriptions:        ����Ӳ�����ӿ�(���С�����)                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-12
** Version:             V0.1
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define	PARALLCOIN_CTL		(1ul<<29)				//P1.20��������Ӳ����ʹ����,�͵�ƽʹ�ܡ��ߵ�ƽ����
#define SERIALCOIN_CTL 		(1ul<<30)				//P1.21��������Ӳ����ʹ����,�͵�ƽʹ�ܡ��ߵ�ƽ����

#define PARALLCHL_IN		0x0000000000070103		//����Ӳ����ͨ��
#define GETPPCOIN_CHL1()	((FIO1PIN >> 10)& 0x01)	//ͨ��1
#define GETPPCOIN_CHL2()	((FIO1PIN >> 9) & 0x01)	//ͨ��2
#define GETPPCOIN_CHL3()	((FIO1PIN >> 8) & 0x01)	//ͨ��3
#define GETPPCOIN_CHL4()	((FIO1PIN >> 4) & 0x01)	//ͨ��4
#define GETPPCOIN_CHL5()	((FIO1PIN >> 1) & 0x01)	//ͨ��5
#define GETPPCOIN_CHL6()	((FIO1PIN >> 0) & 0x01)	//ͨ��6

#define SERIALCOIN_IN		(1ul<<11)				//����Ӳ����ͨ��P2.11
#define GETSERIALCON_CHL()	((FIO2PIN >>11) & 0x01)	//����ͨ��

#define PARALLELCOIN_CHANNEL_QUEUE_SIZE 8
#define SERIALCOIN_CHANNEL_QUEUE_SIZE 	20

void *PCoinChannel[PARALLELCOIN_CHANNEL_QUEUE_SIZE];






OS_EVENT *QPChannel;
volatile unsigned char PostParallelCoinChannel;


OS_EVENT *QSChannel;//���崮������Ӳ������Ϣ�¼�
void *SCoinChannel[SERIALCOIN_CHANNEL_QUEUE_SIZE];
volatile unsigned char PostSerialCoinChannel[SERIALCOIN_CHANNEL_QUEUE_SIZE];
static unsigned char  serialCoinIndex = 0; 



volatile unsigned char PARALLELPULSECOINACCEPTORDEVICESTATUS = 0x00;//Ĭ�Ϲرղ�������Ӳ����
volatile unsigned char SERIALPULSECOINACCEPTORDEVICESTATUS   = 0x00;//Ĭ�Ϲرմ���������Ӳ����

/*****************************************����Ӳ����API**************************************************/

/*********************************************************************************************************
** Function name:       InitParallelPluseCoinAcceptor
** Descriptions:        ��ʼ������Ӳ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitParallelPluseCoinAcceptor(void)
{
	QPChannel = OSQCreate(&PCoinChannel[0],PARALLELCOIN_CHANNEL_QUEUE_SIZE);
	FIO1DIR &= (~PARALLCHL_IN);				//����ͨ��Ϊ����ģʽ
	//EnableParallelPluseCoinAcceptor();		//ʹ��Ӳ����
	InitTimer(1,480000);					//20msɨ��һ�β���Ӳ����			
}
/*********************************************************************************************************
** Function name:       EnableParallelPluseCoinAcceptor
** Descriptions:        ʹ�ܲ���Ӳ����
** input parameters:    ����ȫ�ֱ���PARALLELPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void EnableParallelPluseCoinAcceptor(void)
{
	PARALLELPULSECOINACCEPTORDEVICESTATUS =0x01;
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	//Trace("ParalleEnable+FIO0.30--%x %x\r\n",FIO0PIN>>30,FIO0PIN>>29);
}
/*********************************************************************************************************
** Function name:       DisableParallelPluseCoinAcceptor
** Descriptions:        ���ܲ���Ӳ����
** input parameters:    ��
** output parameters:   ����ȫ�ֱ���PARALLELPULSECOINACCEPTORDEVICESTATUS
** Returned value:      ��
*********************************************************************************************************/
void DisableParallelPluseCoinAcceptor(void)
{
	PARALLELPULSECOINACCEPTORDEVICESTATUS =0x00;
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	//Trace("ParalleDisable+FIO0.30--%x %x\r\n",FIO0PIN>>30,FIO0PIN>>29);
	//FIO0DIR &= ~(SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	//FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
}
/*********************************************************************************************************
** Function name:       GetParallelCoinAcceptorStatus
** Descriptions:        ��ȡ����Ӳ����״̬�����ܻ�ʹ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      Ӳ����״̬��0x00-�رգ�0x01-����
*********************************************************************************************************/
unsigned char GetParallelCoinAcceptorStatus(void)
{
	return PARALLELPULSECOINACCEPTORDEVICESTATUS;	
}
/*********************************************************************************************************
** Function name:       ReadParallelCoinAcceptor
** Descriptions:        ��ȡ����Ӳ����ͨ��ֵ
** input parameters:    ��
** output parameters:   ��
** Returned value:      Ӳ����ͨ����0x00-δͶ�ң�0x01~0x06��Ӧ��ͨ����Ͷ��
*********************************************************************************************************/
unsigned char ReadParallelCoinAcceptor(void)
{
	unsigned char err;
	unsigned char *Pchannel;
	Pchannel = (unsigned char *)OSQPend(QPChannel,5,&err);
	if(err == OS_NO_ERR)
	{
		return *Pchannel;	
	}
	else
		return 0x00;	
}
/*********************************************************************************************************
** Function name:       ScanPPCoinChannel
** Descriptions:        ɨ�貢��Ӳ����ͨ����ע��ֻ����Time1��ʱ�ж���ɨ��
** input parameters:    ��
** output parameters:   ��ɨ�赽��ֵ���벢������Ӳ����ר����Ϣ����
** Returned value:      ��
*********************************************************************************************************/
void ScanPPCoinChannel(void)
{
	unsigned int Rchannel;
	unsigned char CurChannel,Temp;
	static unsigned char PreChannel;
	static unsigned char PreStatus ;
	Rchannel  = GETPPCOIN_CHL1()<<0;
	Rchannel |= GETPPCOIN_CHL2()<<1;
	Rchannel |= GETPPCOIN_CHL3()<<2;
	Rchannel |= GETPPCOIN_CHL4()<<3;
	Rchannel |= GETPPCOIN_CHL5()<<4;
	Rchannel |= GETPPCOIN_CHL6()<<5;
	Temp = (unsigned char)(~Rchannel);
	Temp &= 0x3F;
	switch(Temp)
	{
		case 0x00 : if(PreStatus == 0x01)		
					{
						PreStatus = 0x00;
						PostParallelCoinChannel = PreChannel;
						OSQPost(QPChannel,(void *)&PostParallelCoinChannel);
					}
					PreChannel = 0x00;
					CurChannel = 0x00;
					break;
		case 0x01 : CurChannel = 0x06;
					break;
		case 0x02 : CurChannel = 0x05;
					break;
		case 0x04 : CurChannel = 0x04;
					break;
		case 0x08 : CurChannel = 0x03;
					break;
		case 0x10 : CurChannel = 0x02;
					break;
		case 0x20 : CurChannel = 0x01;
					break;
		default  :  CurChannel = 0x00;
					break;					
	}
	if(PreStatus == 0x00)
	{
		if((PreChannel > 0x00)&(PreChannel == CurChannel))
			PreStatus = 0x01;
		else
			PreChannel = CurChannel;
	}
}

/*****************************************����Ӳ����API**************************************************/

/*********************************************************************************************************
** Function name:       InitSerialPluseCoinAcdeptor
** Descriptions:        ��ʼ��������Ӳ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitSerialPluseCoinAcdeptor(void)
{
	QSChannel = OSQCreate(&SCoinChannel[0],SERIALCOIN_CHANNEL_QUEUE_SIZE);
	FIO2DIR &= (~SERIALCOIN_IN);			//����ͨ��Ϊ����ģʽ
	//EnableSerialPluseCoinAcceptor();		//ʹ��Ӳ����
	InitTimer(1,480000);					//20msɨ��һ�β���Ӳ����
}
/*********************************************************************************************************
** Function name:       EnableSerialPluseCoinAcceptor
** Descriptions:        ʹ�ܴ���Ӳ����
** input parameters:    ����ȫ�ֱ���SERIALPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void EnableSerialPluseCoinAcceptor(void)
{
	SERIALPULSECOINACCEPTORDEVICESTATUS =0x01;
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	//FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	OSTimeDly(5);	
	//Trace("SerialDisable+FIO0.30--%x %x\r\n",FIO0PIN>>30,FIO0PIN>>29);
}
/*********************************************************************************************************
** Function name:       DisableSerialPluseCoinAcceptor
** Descriptions:        ʹ�ܴ���Ӳ����
** input parameters:    ����ȫ�ֱ���SERIALPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DisableSerialPluseCoinAcceptor(void)
{
	SERIALPULSECOINACCEPTORDEVICESTATUS =0x00;
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	//FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	OSTimeDly(5);
	//Trace("SerialDisable+FIO0.30--%x %x\r\n",FIO0PIN>>30,FIO0PIN>>29);
}
/*********************************************************************************************************
** Function name:       GetSerialCoinAcceptorStatus
** Descriptions:        ��ȡ����Ӳ����״̬
** input parameters:    ����ȫ�ֱ���SERIALPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char GetSerialCoinAcceptorStatus(void)
{
	return SERIALPULSECOINACCEPTORDEVICESTATUS;
}
/*********************************************************************************************************
** Function name:       ReadSerialPluseCoinAcceptor
** Descriptions:        ��ȡ����Ӳ����ͨ��ֵ
** input parameters:    ��
** output parameters:   ��
** Returned value:      Ӳ����ͨ����0x00-δͶ�ң�0x01�յ�һ�����壬
*********************************************************************************************************/
unsigned char ReadSerialPluseCoinAcceptor(void)
{
	unsigned char err;
	unsigned char *Schannel;
	Schannel = (unsigned char *)OSQPend(QSChannel,5,&err);
	if(err == OS_NO_ERR)
	{
		//Trace("\r\nSchannel...%d\r\n",*Schannel);
		return *Schannel;	
	}
	else
		return 0x00;
}
/*********************************************************************************************************
** Function name:       ScanSPCoinChannel
** Descriptions:        ɨ�贮��Ӳ����ͨ����ע��ֻ����Time1��ʱ�ж���ɨ��
** input parameters:    ��
** output parameters:   ��ɨ�赽��ֵ���봮������Ӳ����ר����Ϣ����
** Returned value:      ��
*********************************************************************************************************/
void ScanSPCoinChannel(void)
{
	unsigned char CurStatus;
	static unsigned char PreStatus ;

	FIO2DIR &= (~SERIALCOIN_IN);
	CurStatus  = (unsigned char)GETSERIALCON_CHL();
	switch(CurStatus)
	{					
		case 0x00 :	if(PreStatus == 0x01)
					{
						PostSerialCoinChannel[serialCoinIndex] = 0x01;
						OSQPost(QSChannel,(void *)&PostSerialCoinChannel[serialCoinIndex]);
						serialCoinIndex = (++serialCoinIndex) % SERIALCOIN_CHANNEL_QUEUE_SIZE;
					}
					PreStatus = CurStatus;
					break;
		case 0x01 : PreStatus = CurStatus;
					break;	
		default   : break;
	}
}

/**************************************End Of File*******************************************************/
