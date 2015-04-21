#include "deviceTask.h"
#include "..\config.h"



//����������
OS_EVENT *g_KeyMsg;
void *KeyMsgArray[2]; // ������������






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
	InitUart0();
	InitUart1();
	uart2Init();
	InitUart3();
    InitBuzzer();
	InitI2C0();
	InitTimer(0,240000);
	
}


void DEV_task(void *pdata)
{	
	Trace("DEV_task\r\n");
	//ϵͳ�����ӿڳ�ʼ��
	SystemInit();
	//�������䡢�ź���	
	CreateMBox();
	
	
	while(1){
		msleep(100);
	}
}



