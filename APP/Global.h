#ifndef _GLOBAL_H
#define _GLOBAL_H


#define VMC_DISP_OFF					0
#define VMC_DISP_LED					1
#define VMC_DISP_LCD					2


#define VMC_DISP_SELECT					VMC_DISP_LED  //lcd 1   0 led


#define VMC_TYPE_CHANGER               (0x51) //��������ͨ�û���
#define VMC_TYPE_TRADE                (0x81) //�����ۻ���ͨ�û���
#define VMC_TYPE					   VMC_TYPE_CHANGER  //ʵ�ֻ���ת��



//=============================================================================
//��������
//Ŀǰ�ƻ����ۻ����Ͷ�������ϳ�һ������Զ�ʶ��
//=============================================================================

#define PINTU							(3)

#define VMC_TRACE_ON					1
#define VMC_TRACE_OFF					0
#define VMC_TRACE_FLAG					1 //���Կ���



#define VMC_OPEN_COINFG					0 //�Ƿ��������ļ�1����  0�ر�



//���������
//Ӳ������������
#define  VMC_COIN_SERIAL	1
#define  VMC_COIN_MDB		2
#define  VMC_COIN_PARALLEL  3
#define  VMC_COIN_NO		0//��Ӳ����
#define  VMC_COIN_TYPE     VMC_COIN_MDB


//������ͨ����ֵ
#define VMC_COIN_CHANNEL_1	100
#define VMC_COIN_CHANNEL_2	0
#define VMC_COIN_CHANNEL_3	0
#define VMC_COIN_CHANNEL_4	0
#define VMC_COIN_CHANNEL_5	0
#define VMC_COIN_CHANNEL_6	0
#define VMC_COIN_CHANNEL_7	0
#define VMC_COIN_CHANNEL_8	0

//ֽ����MDB

#define VMC_BILL_NO			0//��ֽ����
#define VMC_BILL_RS232		1 //GBA
#define VMC_BILL_MDB		2//MDB
#define VMC_BILL_MDB_ITL	3// MDB ITL

#define VMC_BILL_TYPE     VMC_BILL_NO

//������

#define VMC_CHANGER_HOPPER		1
#define VMC_CHANGER_NO			0//��������
#define VMC_CHANGER_TYPE     VMC_CHANGER_NO

//����hopper���� ����
#define HOPPER_LEVEL_RATIO			 {2,1,0} //��hopper��Ҫ��Ӧ����hopper��ֵ�� �Ӵ�С 
#define HOPPER_BASE_CHANGED			  500   //��������Է�Ϊ��λ

//������ͨ����ֵ
#define VMC_CHANGER_CHANNEL_1	100
#define VMC_CHANGER_CHANNEL_2	200
#define VMC_CHANGER_CHANNEL_3	0




//С����λ��
#define VMC_PIONT_0					0
#define VMC_PIONT_1					1
#define VMC_PIONT_2					2


#define VMC_PIONT_TYPE      VMC_PIONT_1

//������������ļ�
#define VMC_HUODAO_OPEN_CONFIG     1 //�Ƿ��������ļ�����
#define VMC_HUODAO_LEVEL_OPEN     {1,0} //����㿪��

//�����������
#define VMC_HUODAO_COLUMN_OPEN    {{1,0,0,1,1,1,0,0},   \
								   {0,0,0,0,0,0,0,0} }



//======================�������ý���=======================================================


//=============================================================================
//�꺯�������ʽ
//=============================================================================

#define HUINT16(n)				(unsigned char)(((n) >> 8) & 0xff)//16λ��ֵ��8λ
#define LUINT16(n)				(unsigned char)((n) & 0xff)//16λ��ֵ��8λ	
#define H0UINT32(n)				(unsigned char)(((n) >> 24) & 0xff)//32λ��ֵ��һ����8λ
#define H1UINT32(n)				(unsigned char)(((n) >> 16) & 0xff)//32λ��ֵ�ڶ�����8λ
#define L0UINT32(n)				(unsigned char)(((n) >> 8) & 0xff)//32λ��ֵ��һ����8λ
#define L1UINT32(n)				(unsigned char)((n) & 0xff)//32λ��ֵ�ڶ�����8λ
#define INTEG16(h,l)			(unsigned short)(((unsigned short)(h) << 8) | ((l) & 0xff))
	//�ĸ�unsigned char �������ϳ�unsigned int������
#define INTEG32(h0,l0,h1,l1)	(unsigned int)(((unsigned int)(h0) << 24) | \
									((unsigned int)(l0) << 16) | \
									((unsigned int)(h1) << 8) | \
									((l1) & 0xff))



//�Ƿ���Hopper���㹦��
#define HOPPER_OPEN						(3)
//�Ƿ���ֽ�����ձҹ���
#define BILL_OPEN						(4)


extern OS_EVENT *g_KeyMsg;









#endif
