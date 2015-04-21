#include "deviceTask.h"
#include "..\config.h"

#define DEV_DEBUG
#ifdef DEV_DEBUG
#define print_dev(...)	Trace(__VA_ARGS__)
#else
#define print_dev(...)
#endif

//����������
OS_EVENT *g_KeyMsg;
void *KeyMsgArray[2]; // ������������

extern OS_EVENT *g_mdb_event; //����MDb�¼�




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
	MDB_createMbox();

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




static void DEV_mdbSwitch(G_MDB_ST *g_mdb_st)
{
	//���� 
	uint8 res;
	res = BT_open(1,g_mdb_st->column);
	if(res == 1){
		MDB_setColStatus(MDB_COL_SUCCESS);
	}
	else{
		MDB_setColStatus(MDB_COL_FAILED);
	}
	
	
}

static void DEV_mdbCtrl(G_MDB_ST *g_mdb_st)
{
	uint8 res;
	
	res = EV_bento_light(1,g_mdb_st->lightCtrl);
	if(res == 1){
		MDB_setColStatus(MDB_COL_IDLE);	
	}
	else{
		MDB_setColStatus(MDB_COL_ERROR);	
	}
	
}


static void DEV_mdbReset(G_MDB_ST *g_mdb_st)
{
	uint8 res;
	res = EV_bento_check(1,g_mdb_st->bin);
	if(res == 1){
		MDB_setColStatus(MDB_COL_JUSTRESET);
	}
	else{
		MDB_setColStatus(MDB_COL_ERROR);
	}
	
}

void DEV_taskPoll(void)
{
	G_MDB_ST *g_mdb_st;
	INT8U err;
	g_mdb_st = OSQPend (g_mdb_event, 20, &err);
	if(err == OS_NO_ERR){
		print_dev("OSQPend:type= %d\r\n",g_mdb_st->type);
		switch(g_mdb_st->type){
			case G_MDB_RESET:
				DEV_mdbReset(g_mdb_st);
				break;
			case G_MDB_SWITCH:
				DEV_mdbSwitch(g_mdb_st);
				break;
			case G_MDB_CTRL:
				DEV_mdbCtrl(g_mdb_st);
				break;
			default:break;
		}
	}

}


void DEV_task(void *pdata)
{	
	//ϵͳ�����ӿڳ�ʼ��
	SystemInit();
	//�������䡢�ź���	
	CreateMBox();
	while(1){
		DEV_taskPoll();
		msleep(10);
	}
}



