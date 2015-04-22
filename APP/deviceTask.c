#include "deviceTask.h"
#include "..\config.h"

//#define DEV_DEBUG
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



//���� 
static void DEV_mdbSwitch(ST_MDB *mdb)
{
	uint8 res;
	res = BT_open(1,mdb->column);
	if(res == 1){
		MDB_setRequest(MDB_COL_SUCCESS);
	}
	else{
		MDB_setRequest(MDB_COL_FAILED);
	}
	
}

static void DEV_mdbCtrl(ST_MDB *mdb)
{
	uint8 res;
	res = EV_bento_light(1,mdb->lightCtrl);
	if(res == 1){
		MDB_setRequest(MDB_COL_IDLE);
	}
	else{
		MDB_setRequest(MDB_COL_ERROR);
	}
	
}


static void DEV_mdbReset(ST_MDB *mdb)
{
	uint8 res;
	print_dev("DEV_mdbReset\r\n");
	res = EV_bento_check(1,mdb->bin);
	if(res == 1){
		MDB_setRequest(MDB_COL_JUSTRESET);
	}
	else{
		MDB_setRequest(MDB_COL_ERROR);
	}
	print_dev("MDB_getRequest() = %d\r\n",MDB_getRequest());
	
}

void DEV_taskPoll(void)
{
	ST_MDB *mdb = &stMdb;
	if(MDB_getRequest() == MDB_COL_BUSY){
		print_dev("MDB-Request:type= %d\r\n",mdb->type);
		switch(mdb->type){
			case G_MDB_RESET:
				DEV_mdbReset(mdb);
				break;
			case G_MDB_SWITCH:
				DEV_mdbSwitch(mdb);
				break;
			case G_MDB_CTRL:
				DEV_mdbCtrl(mdb);
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
		msleep(20);
	}
}



