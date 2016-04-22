/********************************************************************************************************************
 ��˾��������Ԫ����Դϵͳ���޹�˾
 �ļ�: main.c
 �޸�: lin    �汾: V1.0       ����: 2012-05-18
 ����: ������
 ����: 
 ��ʷ:
	    <����>      <����>      <�汾>   <˵��>
        lin         2012-05-18   V1.0     ���� 
*********************************************************************************************************************/ 

/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/

#include "cancom.h" 
#include "ucos_ii.h"
#include <stm32f10x.h>
#include "global.h"
#include "includes.h"
//#include "controller.h"
//#include <time.h>



/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define IWDG_MAXTIME			3000						    //���ÿ��Ź��ĸ�λʱ��3�룬���4.095S*8

#define	OS_TASK_CAN_PROCESS_STACK_SIZE				 256		//��ջ��С
#define	OS_TASK_MODBUS_STACK_SIZE				     256	 	//��ջ��С
#define	OS_TASK_CAN_RECEIVE_STACK_SIZE				 256		//��ջ��С

#define	OS_TASK_PDU_STACK_SIZE	    		         256				//�����ջ��С512���ֽ�	
#define	OS_TASK_ILB_STACK_SIZE				         128				//�����ջ��С512���ֽ�
#define	OS_TASK_SHOW_STACK_SIZE	    		         256				//�����ջ��С1024���ֽ�
#define OS_TASK_POWE_STACK_SIZE                      256 		    //�����ջ��С1024���ֽ�
#define OS_TASK_ACPDU_STACK_SIZE                     256 		    //�����ջ��С1024���ֽ�


#define	OS_TASK_KEY_PRIO								2				//������������ȼ�,����ɨ��
#define	OS_TASK_POWERMANAGE_PRIO					    3				//������������ȼ�
#define	OS_TASK_CAN_PROCESS_PRIO					    1		        //���ȼ�
#define	OS_TASK_CAN_RECEIVE_PRIO					    4		        //���ȼ�
#define	OS_TASK_SHOW_PRIO								5				//������������ȼ�
#define	OS_TASK_MODBUS_PRIO						        6  		        //���ȼ� 
#define	OS_TASK_PDU_PRIO							    7				//������������ȼ���������
#define	OS_TASK_ACPDU_PRIO							    8				//������������ȼ���������
#define	OS_TASK_ILB_PRIO								9				//������������ȼ�,����ɨ��





/********************************************************************************************************************
*                                            ������������������
*********************************************************************************************************************/
OS_STK		TaskStk_canProcess[OS_TASK_CAN_PROCESS_STACK_SIZE];
OS_STK		TaskStk_canReceive[OS_TASK_CAN_RECEIVE_STACK_SIZE];	
OS_STK		TaskStk_powerManage[OS_TASK_POWE_STACK_SIZE];

OS_STK		TaskStkPdu[OS_TASK_PDU_STACK_SIZE];	//��������Ķ�ջ
OS_STK		TaskStkAcPdu[OS_TASK_ACPDU_STACK_SIZE];	//��������Ķ�ջ

OS_STK		TaskStkILB[OS_TASK_ILB_STACK_SIZE];					//��������Ķ�ջ
OS_STK		TaskStkShow[OS_TASK_SHOW_STACK_SIZE];				//��������Ķ�ջ
OS_STK		TaskStkModbus[OS_TASK_MODBUS_STACK_SIZE];			//��������Ķ�ջ   

extern void task_canSend(void *pdata);		
extern void task_modbus(void *pdata);
extern void task_canProcess(void *pdata);
extern void task_canReceive(void *pdata);
extern void task_powerManage(void *pdata);
extern void task_ILBCom(void );
extern void task_Mcp2515Com(void *pdata);

extern INT8U adc_read(INT8U channel, INT16U* value);
void EE_WriteData(INT8U adr,INT8U *buf);
void EE_ReadData(INT8U adr,INT8U *buf);

void sysPareInit(void);	
void ReadSaveData(void);					//��ȡ�ϴα������������
void task_PduCom(void *pdata);		       //PdUͨ������
//void task_key(void *pdata);             //����ɨ������
//void task_show(void *pdata);      		//��ʾ����

void gatherADC(FP32 *outVolt);
void task_otherThing(void *pdata);


extern void Periph_Configuration(void);
extern void ledShow(void);
extern void alarmTest(void);
extern void getDispOutVoltK_value(FP32 volt,FP32 *k);
extern void getDispOutCurrK_value(FP32 curr,FP32 *k);
extern void getDispInVoltK_value(FP32 volt,FP32 *k);
extern void getDispTempK_value(FP32 temp,FP32 *k);
extern void task_AcPduCom(void *pdata);
extern void task_modbus(void *pdata);			        //MODBUS����
extern void modbusReadRegDataDeal(void);
extern void task_moduleComManage(void *pdata);			//ģ���������
extern void TaskIcCard(void *pdata);

/********************************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************************/

//INT8U  gCommStation = 0x00;									//��¼����ͨѶ�׶�
//INT16U ModbusREGDat[MODBUSBUFMAX];

 FP32 gOutVolt,gOutCurr,gInVolt,gTermperature,gVref;
 INT16U 	lVolValue,lCurValue,lExtValue;					//��ѹ��������ֵ 
 
 USER_DB gUserDB;

///*********************************************************************************************************************
//��������: 	peripConfigInit          
//��������: 	��Χ��ʼ������
//�䡡��:   	��
//
//�䡡��:   	��                 
//*********************************************************************************************************************/
//void peripConfigInit(void)
//{
//	IWDG_Configuration();	   		//���Ź���ʼ��
//	CAN_Initial(CAN_REC_BMS);  		//���ܽ���BMS���ݣ���Ϊ��ʼ�ͽ�������
//}
/*********************************************************************************************************************
��������: 	main          
��������: 	main������
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
int main(void)
{
	SystemInit();                   //STM32ϵͳ��ʼ��
	Periph_Configuration();	 		//CPU��Χ��ʼ��
	sysPareInit();	  			    //������ʼ��
	
  	OSInit();				
  	SysTick_Config(72000000/OS_TICKS_PER_SEC);//OS_TICKS_PER_SEC=100  10ms
	
	OSTaskCreate(task_PduCom, (void *)0, (OS_STK *)&TaskStkPdu[OS_TASK_PDU_STACK_SIZE - 1],OS_TASK_PDU_PRIO);	                             //ֱ��������Ե����ͨ�������߳�
	OSTaskCreate(TaskIcCard, (void *)0, (OS_STK *)&TaskStkAcPdu[OS_TASK_ACPDU_STACK_SIZE - 1],OS_TASK_ACPDU_PRIO);	                     //������������߳�
			
	OSTaskCreate(task_otherThing, (void *)0, (OS_STK *)&TaskStkShow[OS_TASK_SHOW_STACK_SIZE - 1],OS_TASK_SHOW_PRIO);                         //���������߳�																				
	OSTaskCreate(task_modbus, (void *)0, (OS_STK *)&TaskStkModbus[OS_TASK_MODBUS_STACK_SIZE - 1],OS_TASK_MODBUS_PRIO);	                     //��ͨ�������߳�	

	//OSTaskCreate(task_canProcess, (void *)0, (OS_STK *)&TaskStk_canProcess[OS_TASK_CAN_PROCESS_STACK_SIZE - 1],OS_TASK_CAN_PROCESS_PRIO);	 //CAN��ʱ�����봦���߳�
//	OSTaskCreate(task_canReceive, (void *)0, (OS_STK *)&TaskStk_canReceive[OS_TASK_CAN_RECEIVE_STACK_SIZE - 1],OS_TASK_CAN_RECEIVE_PRIO);    //CAN�����߳�								
	OSTaskCreate(task_powerManage, (void *)0, (OS_STK *)&TaskStk_powerManage[OS_TASK_POWE_STACK_SIZE - 1],OS_TASK_POWERMANAGE_PRIO);	     //�������߳�	
                            //��Ե���ͨ���߳�							
	


	OSStart();			
  	return(0);
}



/**********************************************************
*	�����˲���ѹ�������ⲿ������ѹ
***********************************************************/
#define GLIDELENGTH	100										//�����˲��ĳ���
INT16U GlideData(INT16U *ptrArrValue)
{
	INT8U  tmp,tmp2;												//������ʱ����
	INT16U tmp16;											//������ʱ����
	INT32U tmp32;											//������ʱ����
	INT16U tmpArr[GLIDELENGTH];								//Ϊ������ʹ�õ���ʱ����	
	for(tmp=0, tmp32=0; tmp<GLIDELENGTH; tmp++)
	{
		tmpArr[tmp] =ptrArrValue[tmp];
	}
	for(tmp=0; tmp<GLIDELENGTH-1; tmp++)				//�����������ݴӴ�С����
	{
		for(tmp2=0; tmp2<GLIDELENGTH-1; tmp2++)
		{
			if(tmpArr[tmp2] < tmpArr[tmp2+1])
			{
				tmp16= tmpArr[tmp2+1];
				tmpArr[tmp2+1] = tmpArr[tmp2];
				tmpArr[tmp2] = tmp16;
			}
		}
	}
	tmp32 = (tmpArr[GLIDELENGTH/2-5] + tmpArr[GLIDELENGTH/2-4]
					+tmpArr[GLIDELENGTH/2-3] + tmpArr[GLIDELENGTH/2-2]
					+tmpArr[GLIDELENGTH/2-1] + tmpArr[GLIDELENGTH/2]
					+tmpArr[GLIDELENGTH/2+1] + tmpArr[GLIDELENGTH/2+2]
					+tmpArr[GLIDELENGTH/2+3] + tmpArr[GLIDELENGTH/2+4]

					+tmpArr[GLIDELENGTH/2+5] + tmpArr[GLIDELENGTH/2+6]
					+tmpArr[GLIDELENGTH/2+7] + tmpArr[GLIDELENGTH/2+8]
					+tmpArr[GLIDELENGTH/2+9] + tmpArr[GLIDELENGTH/2-6]
					+tmpArr[GLIDELENGTH/2-7] + tmpArr[GLIDELENGTH/2-8]
					+tmpArr[GLIDELENGTH/2-9] + tmpArr[GLIDELENGTH/2-10]
					)/20;


	return 	(INT16U)tmp32;

} 

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************************************************************************************
��������: 	gatherADC          
��������:   AD���ݲɼ�
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
void gatherADC(FP32 *outVolt)
{
			 

}

/*********************************************************************************************************************
��������: 	task_PduCom          
��������:  PCU���ͨ���߳�,ѭ����ȡ����ѹ�����������ʵ�
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
void task_Mcp2515Com(void *pdata)								//����1
{ 	
	 Mcp2515_init();
} 

/*********************************************************************************************************************
��������: 	task_PduCom          
��������:  PCU���ͨ���߳�,ѭ����ȡ����ѹ�����������ʵ�
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
void task_PduCom(void *pdata)								//����1
{
	
	INT8U cmd=0,ret;
	static INT8U cnt=0;

	pdata = pdata;											//��������
//	OSSchedLock();											//��������
//	OSSchedUnlock();										//�����������
	
  pdu_Manage(PDU_READ_ADDR);                              //��ȡPDUͨ�ŵ�ַ

  while(1)													//�����������
 {
	OSTimeDlyHMSM(0,0,0,200); 	
	   
	ret=pdu_Manage(cmd);                                   //PDUͨ�ţ�ѭ����ȡ��ѹ�����������ʵ�
	if(ret==1)
	{
		switch(cmd)
		{
		case PDU_READ_ADDR:  //ַַ
			memcpy(gUserDB.M_pdu_ID,pdu_info.addr,6);
			break;
		case PDU_READ_A_VOLT:  //��ѹ
			gUserDB.C_outVolt=pdu_info.A_volt;
			break;
		case PDU_READ_A_CURR:  //����
			gUserDB.C_outCurr=pdu_info.A_curr;
			if(	gUserDB.C_outCurr<1)
				gUserDB.C_outCurr=0;
			break;
		case PDU_READ_POWER:  //�����
			gUserDB.M_degrees=pdu_info.power_total;
			break;
		}

		cnt=0;
		gUserDB.DcPduComAlarm=NOALARM;  

		cmd++;	   //�·���һ������
		if(cmd>PDU_READ_POWER)
			cmd=1;
	}
	else  //ͨ���쳣���»�ȡ����ַ
	{	   
	   cnt++; 	   	
	   if(cnt >=100)
	   	{
		    cmd=0;    //���¶�����ַ����
	   	  gUserDB.DcPduComAlarm=ALARM;  
	   	}
	}
	
	
//	task_ILBCom();	          //���Ե����ͨ��
	
  }
} 

/*********************************************************************************************************************
��������: 	PareInit          
��������: 	������ʼ��
�䡡��:   	u8Select 1-��ʼ��Ϊ������������	    2-��ʼΪ�������������ӣ������ѳ�����BMS����Ҫ���

�䡡��:   	0   -������	  >0  -��������Ϣ�ĸ���	                 
*********************************************************************************************************************/
void sysPareInit(void)
{	
 	INT8U i=0;

  gUserDB.moduleCurrMax  = 11;	   //ģ������������,500V����������25A
	gUserDB.BatTempProtect=65;
	gUserDB.cellOverProtect=3.8;
	gUserDB.CCS_outCurrMax=200;
	gUserDB.CCS_outVoltMax=620;
	gUserDB.CCS_adr=1;
	gUserDB.moduleNornalSum=0;
	gUserDB.dev_adr=1;
	gUserDB.M_carLinkState=OK ;
	gUserDB.user_indicating=1;
	
	
	pdu_info.addr[0]=0x00;
	pdu_info.addr[1]=0x00;
	pdu_info.addr[2]=0x00;
	pdu_info.addr[3]=0x00;
	pdu_info.addr[4]=0x00;
	pdu_info.addr[5]=0x00;
	BEEP_ON;
	gUserDB.cardBeepEn=0;
    BEEP_OFF;	

	for(i=0;i<20;i++)
	{
		gModuleInfo[i].comAlarm=1;
		gModuleInfo[i].runStatus=1;	   //��ʼΪ������
		
	}
	//gUserDB.BatTempProtect =65;
   
   //ReadSaveData();                         //��ȡ�������������	   
	
} 

/*********************************************************************************************************************
��������: 	car_link_detect          
��������: 	
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void car_link_detect(void )
{
	FP32  tmp;

//	gatherADC( &tmp);	                                         //��ȡ���ǹ���Ӽ���ѹ

//	tmp= tmp* 7.33333;                                           //�Ŵ�7.3333��	
//	gUserDB.car_link_volt = gUserDB.car_link_volt*0.7+tmp*0.3;	 //

}

/*********************************************************************************************************************
��������: 	RestoreData          
��������:   
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
void RestoreData(void)
{
//	INT8U i=0; 	
	INT8U TmpArr[32];										//��ȡ��д�����ݵ���ʱ�Ĵ���
	INT8U FirstFlag=0;										//��ȡ�Ƿ�Ϊ��һ�����г���	  	

//	gUserDB.outVolt_k1_pwm=PWM_PERIOD*10/30;     		//10V

  


	FirstFlag=0XA5;
	EE_WriteData(ADDR_FIRSTFLAG,&FirstFlag);			//����ȱ�����Ա����־λ 	


	
}

/*******************************************************************************
* 	��ȡϵͳ�������ϴ����õĲ���
*******************************************************************************/
void ReadSaveData()
{	
	INT8U TmpArr[32];										//��ȡ��д�����ݵ���ʱ�Ĵ���
	INT8U FirstFlag=0;										//��ȡ�Ƿ�Ϊ��һ�����г���
	
	EE_ReadData(ADDR_FIRSTFLAG,&FirstFlag);	  //������ȱ�����Ա����־λ   	
	if(FirstFlag != 0xA5)									//��δд���򱣴�ȱʡ����
	{
		RestoreData(); //������λ	   		 	
	}
	else
	{
		EE_ReadData(ADDR_AUTO_FLAG,TmpArr);				//���ֶ��Զ�ģʽ
		EE_ReadData(ADDR_AVER_FLAG,TmpArr);				//��������ģʽ
		EE_ReadData(ADDR_MANUAL_AVER_VOLT,TmpArr);		//���ֶ������ѹ
		EE_ReadData(ADDR_MANUAL_FLOAT_VOLT,TmpArr);		//���ֶ������ѹ 
		EE_ReadData(ADDR_MANUAL_SET_VOLT,TmpArr);		//�������ѹ 
		EE_ReadData(ADDR_MANUAL_SET_CURR,TmpArr);		//��������� 
		EE_ReadData(ADDR_MAX_VOLT,TmpArr);				//���������ѹ
		EE_ReadData(ADDR_MAX_CURR,TmpArr);				//����������� 
		EE_ReadData(SINGLE_VOLT,TmpArr);				//��������ѹֵ 
		
	} 
}

/*********************************************************************************************************************
��������: 	systemAlarmCodeDeal          
��������: 	
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void systemAlarmCodeDeal(void )
{

	     //ϵͳ״̬���� 
//		if(gUserDB.insulationAlarm==ALARM)	 //0--��Ե������
//		{
//			gUserDB.systemAlarmCode |=	 (1<<0);
//		}
//		else
//		{
//			gUserDB.systemAlarmCode &=	 (0<<0)	;
//		}

	
}
/*********************************************************************************************************************
��������: 	task_otherThing          
��������: 	
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void task_otherThing(void *pdata)
{
	static INT8U i=0;
	 OUT4_OFF;            //ģ���ϵ��

	
	
	 while(1)
	 {
	    if(WATCHDOG_EN)
	         IWDG_ReloadCounter();								//clear watch dog

		//i++;
		//if( i %50 ==1)	                        //400MSת��һ������
	 	{   
			ledShow();  						//LED��ʾ	         
		} 		 
    
		if(gUserDB.charge_temeEn==1)
		{
			gUserDB.charge_timeCnt++;
  	}
		else
		{
			gUserDB.charge_timeCnt=0;
		}

		OSTimeDlyHMSM(0,0,1,0); 
//		modbusReadRegDataDeal();            //����ͨ������ת��
	 }
}


