/********************************************************************************************************************
 公司：深圳市元正能源系统有限公司
 文件: main.c
 修改: lin    版本: V1.0       日期: 2012-05-18
 描述: 主程序
 其它: 
 历史:
	    <作者>      <日期>      <版本>   <说明>
        lin         2012-05-18   V1.0     初版 
*********************************************************************************************************************/ 

/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/

#include "cancom.h" 
#include "ucos_ii.h"
#include <stm32f10x.h>
#include "global.h"
#include "includes.h"
//#include "controller.h"
//#include <time.h>



/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define IWDG_MAXTIME			3000						    //设置看门狗的复位时间3秒，最大4.095S*8

#define	OS_TASK_CAN_PROCESS_STACK_SIZE				 256		//堆栈大小
#define	OS_TASK_MODBUS_STACK_SIZE				     256	 	//堆栈大小
#define	OS_TASK_CAN_RECEIVE_STACK_SIZE				 256		//堆栈大小

#define	OS_TASK_PDU_STACK_SIZE	    		         256				//定义堆栈大小512个字节	
#define	OS_TASK_ILB_STACK_SIZE				         128				//定义堆栈大小512个字节
#define	OS_TASK_SHOW_STACK_SIZE	    		         256				//定义堆栈大小1024个字节
#define OS_TASK_POWE_STACK_SIZE                      256 		    //定义堆栈大小1024个字节
#define OS_TASK_ACPDU_STACK_SIZE                     256 		    //定义堆栈大小1024个字节


#define	OS_TASK_KEY_PRIO								2				//定义任务的优先级,按键扫描
#define	OS_TASK_POWERMANAGE_PRIO					    3				//定义任务的优先级
#define	OS_TASK_CAN_PROCESS_PRIO					    1		        //优先级
#define	OS_TASK_CAN_RECEIVE_PRIO					    4		        //优先级
#define	OS_TASK_SHOW_PRIO								5				//定义任务的优先级
#define	OS_TASK_MODBUS_PRIO						        6  		        //优先级 
#define	OS_TASK_PDU_PRIO							    7				//定义任务的优先级，空任务
#define	OS_TASK_ACPDU_PRIO							    8				//定义任务的优先级，空任务
#define	OS_TASK_ILB_PRIO								9				//定义任务的优先级,按键扫描





/********************************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************************/
OS_STK		TaskStk_canProcess[OS_TASK_CAN_PROCESS_STACK_SIZE];
OS_STK		TaskStk_canReceive[OS_TASK_CAN_RECEIVE_STACK_SIZE];	
OS_STK		TaskStk_powerManage[OS_TASK_POWE_STACK_SIZE];

OS_STK		TaskStkPdu[OS_TASK_PDU_STACK_SIZE];	//定义任务的堆栈
OS_STK		TaskStkAcPdu[OS_TASK_ACPDU_STACK_SIZE];	//定义任务的堆栈

OS_STK		TaskStkILB[OS_TASK_ILB_STACK_SIZE];					//定义任务的堆栈
OS_STK		TaskStkShow[OS_TASK_SHOW_STACK_SIZE];				//定义任务的堆栈
OS_STK		TaskStkModbus[OS_TASK_MODBUS_STACK_SIZE];			//定义任务的堆栈   

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
void ReadSaveData(void);					//读取上次保存的所有数据
void task_PduCom(void *pdata);		       //PdU通信任务
//void task_key(void *pdata);             //按键扫描任务
//void task_show(void *pdata);      		//显示任务

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
extern void task_modbus(void *pdata);			        //MODBUS任务
extern void modbusReadRegDataDeal(void);
extern void task_moduleComManage(void *pdata);			//模块管理任务
extern void TaskIcCard(void *pdata);

/********************************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************************/

//INT8U  gCommStation = 0x00;									//记录各个通讯阶段
//INT16U ModbusREGDat[MODBUSBUFMAX];

 FP32 gOutVolt,gOutCurr,gInVolt,gTermperature,gVref;
 INT16U 	lVolValue,lCurValue,lExtValue;					//电压电流采样值 
 
 USER_DB gUserDB;

///*********************************************************************************************************************
//函数名称: 	peripConfigInit          
//功能描述: 	外围初始化函数
//输　入:   	无
//
//输　出:   	无                 
//*********************************************************************************************************************/
//void peripConfigInit(void)
//{
//	IWDG_Configuration();	   		//看门狗初始化
//	CAN_Initial(CAN_REC_BMS);  		//仅能接收BMS数据，因为开始就进行连接
//}
/*********************************************************************************************************************
函数名称: 	main          
功能描述: 	main主函数
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
int main(void)
{
	SystemInit();                   //STM32系统初始化
	Periph_Configuration();	 		//CPU外围初始化
	sysPareInit();	  			    //参数初始化
	
  	OSInit();				
  	SysTick_Config(72000000/OS_TICKS_PER_SEC);//OS_TICKS_PER_SEC=100  10ms
	
	OSTaskCreate(task_PduCom, (void *)0, (OS_STK *)&TaskStkPdu[OS_TASK_PDU_STACK_SIZE - 1],OS_TASK_PDU_PRIO);	                             //直流电表与绝缘检测板通信任务线程
	OSTaskCreate(TaskIcCard, (void *)0, (OS_STK *)&TaskStkAcPdu[OS_TASK_ACPDU_STACK_SIZE - 1],OS_TASK_ACPDU_PRIO);	                     //交流电表任务线程
			
	OSTaskCreate(task_otherThing, (void *)0, (OS_STK *)&TaskStkShow[OS_TASK_SHOW_STACK_SIZE - 1],OS_TASK_SHOW_PRIO);                         //其它任务线程																				
	OSTaskCreate(task_modbus, (void *)0, (OS_STK *)&TaskStkModbus[OS_TASK_MODBUS_STACK_SIZE - 1],OS_TASK_MODBUS_PRIO);	                     //屏通信任务线程	

	//OSTaskCreate(task_canProcess, (void *)0, (OS_STK *)&TaskStk_canProcess[OS_TASK_CAN_PROCESS_STACK_SIZE - 1],OS_TASK_CAN_PROCESS_PRIO);	 //CAN定时发送与处理线程
//	OSTaskCreate(task_canReceive, (void *)0, (OS_STK *)&TaskStk_canReceive[OS_TASK_CAN_RECEIVE_STACK_SIZE - 1],OS_TASK_CAN_RECEIVE_PRIO);    //CAN接收线程								
	OSTaskCreate(task_powerManage, (void *)0, (OS_STK *)&TaskStk_powerManage[OS_TASK_POWE_STACK_SIZE - 1],OS_TASK_POWERMANAGE_PRIO);	     //充电管理线程	
                            //绝缘检测通信线程							
	


	OSStart();			
  	return(0);
}



/**********************************************************
*	滑动滤波电压电流，外部给定电压
***********************************************************/
#define GLIDELENGTH	100										//滑动滤波的长度
INT16U GlideData(INT16U *ptrArrValue)
{
	INT8U  tmp,tmp2;												//定义临时变量
	INT16U tmp16;											//定义临时变量
	INT32U tmp32;											//定义临时变量
	INT16U tmpArr[GLIDELENGTH];								//为了排序使用的临时变量	
	for(tmp=0, tmp32=0; tmp<GLIDELENGTH; tmp++)
	{
		tmpArr[tmp] =ptrArrValue[tmp];
	}
	for(tmp=0; tmp<GLIDELENGTH-1; tmp++)				//将采样的数据从大到小排序
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
函数名称: 	gatherADC          
功能描述:   AD数据采集
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
void gatherADC(FP32 *outVolt)
{
			 

}

/*********************************************************************************************************************
函数名称: 	task_PduCom          
功能描述:  PCU电表通信线程,循环读取电表电压、电流、功率等
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
void task_Mcp2515Com(void *pdata)								//任务1
{ 	
	 Mcp2515_init();
} 

/*********************************************************************************************************************
函数名称: 	task_PduCom          
功能描述:  PCU电表通信线程,循环读取电表电压、电流、功率等
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
void task_PduCom(void *pdata)								//任务1
{
	
	INT8U cmd=0,ret;
	static INT8U cnt=0;

	pdata = pdata;											//消除警告
//	OSSchedLock();											//锁定任务
//	OSSchedUnlock();										//解除任务锁定
	
  pdu_Manage(PDU_READ_ADDR);                              //读取PDU通信地址

  while(1)													//进入程序主体
 {
	OSTimeDlyHMSM(0,0,0,200); 	
	   
	ret=pdu_Manage(cmd);                                   //PDU通信，循环读取电压、电流、功率等
	if(ret==1)
	{
		switch(cmd)
		{
		case PDU_READ_ADDR:  //址址
			memcpy(gUserDB.M_pdu_ID,pdu_info.addr,6);
			break;
		case PDU_READ_A_VOLT:  //电压
			gUserDB.C_outVolt=pdu_info.A_volt;
			break;
		case PDU_READ_A_CURR:  //电流
			gUserDB.C_outCurr=pdu_info.A_curr;
			if(	gUserDB.C_outCurr<1)
				gUserDB.C_outCurr=0;
			break;
		case PDU_READ_POWER:  //电度数
			gUserDB.M_degrees=pdu_info.power_total;
			break;
		}

		cnt=0;
		gUserDB.DcPduComAlarm=NOALARM;  

		cmd++;	   //下发下一条命令
		if(cmd>PDU_READ_POWER)
			cmd=1;
	}
	else  //通信异常重新获取电表地址
	{	   
	   cnt++; 	   	
	   if(cnt >=100)
	   	{
		    cmd=0;    //重新读电表地址命令
	   	  gUserDB.DcPduComAlarm=ALARM;  
	   	}
	}
	
	
//	task_ILBCom();	          //与绝缘检测板通信
	
  }
} 

/*********************************************************************************************************************
函数名称: 	PareInit          
功能描述: 	参数初始化
输　入:   	u8Select 1-初始化为可以重新连接	    2-初始为不可以重新连接，比如已充满或BMS不需要充电

输　出:   	0   -无数据	  >0  -缓冲区消息的个数	                 
*********************************************************************************************************************/
void sysPareInit(void)
{	
 	INT8U i=0;

  gUserDB.moduleCurrMax  = 11;	   //模块最大输出电流,500V以下最大电流25A
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
		gModuleInfo[i].runStatus=1;	   //初始为非运行
		
	}
	//gUserDB.BatTempProtect =65;
   
   //ReadSaveData();                         //读取保存的设置数据	   
	
} 

/*********************************************************************************************************************
函数名称: 	car_link_detect          
功能描述: 	
输　入:   	无

输　出:     无                 
*********************************************************************************************************************/
void car_link_detect(void )
{
	FP32  tmp;

//	gatherADC( &tmp);	                                         //获取充电枪连接检测电压

//	tmp= tmp* 7.33333;                                           //放大7.3333倍	
//	gUserDB.car_link_volt = gUserDB.car_link_volt*0.7+tmp*0.3;	 //

}

/*********************************************************************************************************************
函数名称: 	RestoreData          
功能描述:   
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
void RestoreData(void)
{
//	INT8U i=0; 	
	INT8U TmpArr[32];										//读取或写入数据的临时寄存器
	INT8U FirstFlag=0;										//读取是否为第一次运行程序	  	

//	gUserDB.outVolt_k1_pwm=PWM_PERIOD*10/30;     		//10V

  


	FirstFlag=0XA5;
	EE_WriteData(ADDR_FIRSTFLAG,&FirstFlag);			//保存缺参数以保存标志位 	


	
}

/*******************************************************************************
* 	读取系统参数和上次设置的参数
*******************************************************************************/
void ReadSaveData()
{	
	INT8U TmpArr[32];										//读取或写入数据的临时寄存器
	INT8U FirstFlag=0;										//读取是否为第一次运行程序
	
	EE_ReadData(ADDR_FIRSTFLAG,&FirstFlag);	  //读保存缺参数以保存标志位   	
	if(FirstFlag != 0xA5)									//若未写入则保存缺省参数
	{
		RestoreData(); //参数复位	   		 	
	}
	else
	{
		EE_ReadData(ADDR_AUTO_FLAG,TmpArr);				//读手动自动模式
		EE_ReadData(ADDR_AVER_FLAG,TmpArr);				//读均浮充模式
		EE_ReadData(ADDR_MANUAL_AVER_VOLT,TmpArr);		//读手动均充电压
		EE_ReadData(ADDR_MANUAL_FLOAT_VOLT,TmpArr);		//读手动浮充电压 
		EE_ReadData(ADDR_MANUAL_SET_VOLT,TmpArr);		//读输出电压 
		EE_ReadData(ADDR_MANUAL_SET_CURR,TmpArr);		//读输出电流 
		EE_ReadData(ADDR_MAX_VOLT,TmpArr);				//读输出最大电压
		EE_ReadData(ADDR_MAX_CURR,TmpArr);				//读输出最大电流 
		EE_ReadData(SINGLE_VOLT,TmpArr);				//读单体限压值 
		
	} 
}

/*********************************************************************************************************************
函数名称: 	systemAlarmCodeDeal          
功能描述: 	
输　入:   	无

输　出:     无                 
*********************************************************************************************************************/
void systemAlarmCodeDeal(void )
{

	     //系统状态代码 
//		if(gUserDB.insulationAlarm==ALARM)	 //0--绝缘检测故障
//		{
//			gUserDB.systemAlarmCode |=	 (1<<0);
//		}
//		else
//		{
//			gUserDB.systemAlarmCode &=	 (0<<0)	;
//		}

	
}
/*********************************************************************************************************************
函数名称: 	task_otherThing          
功能描述: 	
输　入:   	无

输　出:     无                 
*********************************************************************************************************************/
void task_otherThing(void *pdata)
{
	static INT8U i=0;
	 OUT4_OFF;            //模块上电灯

	
	
	 while(1)
	 {
	    if(WATCHDOG_EN)
	         IWDG_ReloadCounter();								//clear watch dog

		//i++;
		//if( i %50 ==1)	                        //400MS转换一次数据
	 	{   
			ledShow();  						//LED显示	         
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
//		modbusReadRegDataDeal();            //与屏通信数据转换
	 }
}


