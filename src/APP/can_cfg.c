/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "includes.h"
#include "can_cfg.h"

/********************************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************************/ 
extern INT8U bmsInfo[BMS_MAX_SIZE];
extern INT8U ccsInfo[CCS_MAX_SIZE];


/*********************************************************************************************************
函数名称: 	DecodeBMSMessage 
功能描述:  PC信息解码

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
INT8U DecodePCMessage(INT8U *pu8DecodeBMSMesg, INT32U ID)
{
	INT8U ret=1;
	
	if(ID== PC_PC_DATE) 					//系统对时报文
	{
		gUserDB.time.year=(pu8DecodeBMSMesg[0]<<8) +pu8DecodeBMSMesg[1];
		gUserDB.time.month= pu8DecodeBMSMesg[2];
		gUserDB.time.day= pu8DecodeBMSMesg[3];
		gUserDB.time.hour= pu8DecodeBMSMesg[4];
		gUserDB.time.min= pu8DecodeBMSMesg[5];
		gUserDB.time.sec= pu8DecodeBMSMesg[6];
	}
	else if(ID== PC_PCU_P_SET)    			//协议版本设置报文
	{
		memcpy(gUserDB.P_version, pu8DecodeBMSMesg,7);
	}
	else if(ID== PC_INQUIRE_ALL)  			//查询命令报文 
	{
		gUserDB.P_inquire_flag1= pu8DecodeBMSMesg[0];
		gUserDB.P_inquire_flag2= pu8DecodeBMSMesg[1];
	}
	
	else if(ID== PC_MCM_STOP_ALL) 			//紧急停机命令 
	{		
			if(pu8DecodeBMSMesg[0]==0x55)//停机
			{
				gUserDB.C_ccsRunCtl=0;	
			}
			
	}
	else if(ID== PC_MCM_START_SINGLE)  	//启动命令 
	{
		    if(pu8DecodeBMSMesg[0]==0x55)//停机
			{
				gUserDB.C_ccsRunCtl=1;				
			}
			
	}
	else if(ID== PC_MCM_STOP_SINGLE)  		//停止命令 
	{
		   if(pu8DecodeBMSMesg[0]==0x55)//停机
			{
				gUserDB.C_ccsRunCtl=0;	
			}
			else
			{
				
			}
	}
	else if(ID== PC_MCM_SET_SINGLE)  		//单体控制参数设置 
	{
		gUserDB.M_singleSetType=pu8DecodeBMSMesg[0];
		gUserDB.M_singleSetStep=pu8DecodeBMSMesg[1];
		gUserDB.M_setSinglePermitChargeVoltH=pu8DecodeBMSMesg[2]+pu8DecodeBMSMesg[3]<<8;		
		gUserDB.M_setSinglePermitChargeTemH=pu8DecodeBMSMesg[4];
		
	}
	else if(ID== PC_MCM_SET_STAGE)  		//阶段控制参数设置
	{
		gUserDB.M_setStageType=pu8DecodeBMSMesg[0];
		gUserDB.M_setStageStep=(pu8DecodeBMSMesg[1]&0xf0)>>4;
		gUserDB.M_setStageMode=pu8DecodeBMSMesg[1]&0x0f;
		gUserDB.M_setOutValue=pu8DecodeBMSMesg[2]+pu8DecodeBMSMesg[3]<<8;		
		gUserDB.M_setLimitValue=pu8DecodeBMSMesg[4]+pu8DecodeBMSMesg[5]<<8;	
		gUserDB.M_setStopValue=pu8DecodeBMSMesg[6]+pu8DecodeBMSMesg[7]<<8;	
	}
	else if(ID== PC_MCM_READ_SET_ALL)     //全部控制参数读取命令 
	{
		if(pu8DecodeBMSMesg[0]==0x55)
			gUserDB.M_readSetAllFlag=1;
	}
	else if(ID== PC_MCM_READ_SET_SPECIAL)  //查询命令报文 
	{
		if(pu8DecodeBMSMesg[0]==0x55)
			gUserDB.M_readStageFlag=1;
	}
	else
	{
		ret=0;
	}	

	gUserDB.canCommAlarmCnt=0;
	gUserDB.pcLinkStatus=NOALARM;
	
  return ret;	
}


/*********************************************************************************************************
函数名称: 	DecodeCCSMessage 
功能描述:  充电机CCS信息组包,也即时PCU信息组包

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void DecodePcuMessage(INT8U *pu8DecodeBMSMesg, INT32U ID)
{
//	INT8U u8AdrCounter = 0;
//	INT8U u8ModbusRegAdrCnt = 0;

    memset(pu8DecodeBMSMesg,0xff,8);

	if(ID== PCU_PCU_LIFE)				//生命报文 
	{
	    if(gUserDB.pcLinkStatus==1)   //与PC连接正常时
		{
			pu8DecodeBMSMesg[0]= gUserDB.P_lifeCnt++;
			pu8DecodeBMSMesg[1]= gUserDB.P_failCode;
	    }
	}
	else if(ID== PCU_BMS_IDENTICATION)								//BMS辨识报文
	{
		pu8DecodeBMSMesg[0]=0;                                   //协议版本
		pu8DecodeBMSMesg[1]=1;
		pu8DecodeBMSMesg[2]=0;
		
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_total_Ah;
		pu8DecodeBMSMesg[4]=(INT32U)gUserDB.B_total_Ah>>8;
		pu8DecodeBMSMesg[5]=(INT32U)gUserDB.B_total_volt;       			//整车电压低字节
		pu8DecodeBMSMesg[6]=(INT32U)gUserDB.B_total_volt>>8;    			//整车电压高字节
	}		
	else if(ID== PCU_BMS_INFORMATION1) 			//BMS信息1报文,PCU每5秒发送一次帧,
	{                                               //当PC收到该帧数据,发现有变化时,PC会触发查询BMS信息命令
		memcpy(pu8DecodeBMSMesg,gUserDB.B_bmsID,8);
	}
	else if(ID== PCU_BMS_INFORMATION2) 			//BMS信息2报文
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_batBoxSum;
		pu8DecodeBMSMesg[1]=gUserDB.B_serialSum;
		pu8DecodeBMSMesg[2]=gUserDB.B_parallelSum;
		pu8DecodeBMSMesg[3]=gUserDB.B_temTestSum;		

		memcpy(&pu8DecodeBMSMesg[5],"V15",3);
	}
	else if(ID== PCU_BMS_INFORMATION3) 			//BMS信息3报文
	{
		pu8DecodeBMSMesg[0]=gUserDB.writeAt;
	   if(gUserDB.writeAt==1)
		{			
			memcpy(&pu8DecodeBMSMesg[1],gUserDB.B_VIN,7);
	   	}
	   else if(gUserDB.writeAt==2)
	   	{	   		
			memcpy(&pu8DecodeBMSMesg[1],&gUserDB.B_VIN[7],7);
	   	}
	   else if(gUserDB.writeAt==3)
	   	{	   		
			memcpy(&pu8DecodeBMSMesg[1],&gUserDB.B_VIN[14],3);
	   	}
	}
	else if(ID== PCU_BMS_INFORMATION4) 			//BMS信息4报文
	{
		pu8DecodeBMSMesg[0]=gUserDB.writeAt;
	   if(gUserDB.writeAt==1)
		{			
			pu8DecodeBMSMesg[1]=(INT32U)(gUserDB.B_permitModuleVoltH);        //PCU中转不对数据进行转换
			pu8DecodeBMSMesg[2]=(INT32U)(gUserDB.B_permitModuleVoltH)>>8;

			pu8DecodeBMSMesg[3]=(INT32U)(gUserDB.B_permitCurrH);
			pu8DecodeBMSMesg[4]=(INT32U)(gUserDB.B_permitCurrH)>>8;
			
			pu8DecodeBMSMesg[5]=gUserDB.B_permitAHHigh;

			pu8DecodeBMSMesg[6]=(INT32U)(gUserDB.B_permitTotalVoltH);
			pu8DecodeBMSMesg[7]=(INT32U)(gUserDB.B_permitTotalVoltH)>>8;
	   	}
	   else if(gUserDB.writeAt==2)
	   	{	   		
			pu8DecodeBMSMesg[1]=gUserDB.B_permitBatTempH;
			
			pu8DecodeBMSMesg[2]=gUserDB.B_batRemainAH;
			pu8DecodeBMSMesg[3]=(gUserDB.B_batRemainAH)>>8;
			
			pu8DecodeBMSMesg[4]=(INT32U)(gUserDB.B_batVoltTotal);			
			pu8DecodeBMSMesg[5]=(INT32U)(gUserDB.B_batVoltTotal)>>8;

			pu8DecodeBMSMesg[6]=gUserDB.B_SOH;			
	   	}
	   
	}
	else if(ID== PCU_BMS_LIFE) 					//BMS生命帧报文 
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_lifeCnt++;	
		if(gUserDB.B_lifeCnt>=15)
			gUserDB.B_lifeCnt=0;
		
		pu8DecodeBMSMesg[1]=gUserDB.B_batStatus;	
		
	}
	else if(ID== PCU_BAT_REQIRE)					//电池充电级别需求报文  
	{
		pu8DecodeBMSMesg[0]=(INT32U)gUserDB.B_batNeedVolt;	
		pu8DecodeBMSMesg[1]=(INT32U)gUserDB.B_batNeedVolt>>8;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_batNeedCurr;
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_batNeedCurr>>8;	
		pu8DecodeBMSMesg[4]=gUserDB.B_batChargeMode;	
		//pu8DecodeBMSMesg[5]=gUserDB.B_batStatus;	
		
	}
	else if(ID== PCU_BAT_STATE) 					//电池充电状态报文
	{
		pu8DecodeBMSMesg[0]=(INT32U)gUserDB.B_batTestVolt;	
		pu8DecodeBMSMesg[1]=(INT32U)gUserDB.B_batTestVolt>>8;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_batTestCurr;
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_batTestCurr>>8;	
		pu8DecodeBMSMesg[4]=gUserDB.B_batTempMin;	
		pu8DecodeBMSMesg[5]=gUserDB.B_batTempMax;
		pu8DecodeBMSMesg[6]=gUserDB.B_SOC;
		pu8DecodeBMSMesg[7]=gUserDB.B_remainTime;	
	}
	else if(ID== PCU_BAT_STATE_LIMIT) 				//电池最高状态报文
	{
		pu8DecodeBMSMesg[0]=(INT32U)gUserDB.B_singleVoltMin;	
		pu8DecodeBMSMesg[1]=(INT32U)gUserDB.B_singleVoltMin>>8;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_singleVoltMax;
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_singleVoltMax>>8;	
		
	}
	else if(ID== PCU_BAT_STOP) 				  //电池充电停止报文
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_bmsStopCause[0];	
		pu8DecodeBMSMesg[1]=gUserDB.B_bmsStopCause[1];	
		pu8DecodeBMSMesg[2]=gUserDB.B_bmsStopCause[2];	
		pu8DecodeBMSMesg[3]=gUserDB.B_bmsStopCause[3];	
	}
	else if(ID== PCU_BAT_COUNT) 				 //电池充电统计报文 
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_beginSoc;	
		pu8DecodeBMSMesg[1]=gUserDB.B_SOC;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_singleVoltMin;		
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_singleVoltMin>>8;	
		pu8DecodeBMSMesg[4]=(INT32U)gUserDB.B_singleVoltMax;
		pu8DecodeBMSMesg[5]=(INT32U)gUserDB.B_singleVoltMax>>8;	
		pu8DecodeBMSMesg[6]=gUserDB.B_batTempMin;	
		pu8DecodeBMSMesg[7]=gUserDB.B_batTempMax;
	}
	else if(ID== PCU_BAT_ERR) 					//BMS错误报文 
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_recCcsIdentifyTimeout;	
		pu8DecodeBMSMesg[1]=gUserDB.B_recCcsMaxOutTimeout+(gUserDB.B_recCcsReadyTimeout<<1);	
		pu8DecodeBMSMesg[2]=gUserDB.B_recCcsStatusTimeout+(gUserDB.B_recCcsStopTimeout<<1)+(gUserDB.B_recCcsCountTimeout<<2);		
		pu8DecodeBMSMesg[3]=gUserDB.B_recCcsAuxVoltStatusTimeout;		
		
	}
	else if(ID== PCU_BAT_FAIL) 				//BMS故障报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_bmsFaultCode,6);	
	}
	else if(ID== PCU_BAT_FAIL2) 				//BMS故障报文2
	{
	}
	else if(ID== PCU_BAT_VOLT_N1) 				//电池单体电压数据报文1
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_singleVolt,6);
	}
	else if(ID== PCU_BAT_VOLT_N2) 				//电池单体电压数据报文2
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[3],6);
	}
	else if(ID== PCU_BAT_VOLT_N3) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[6],6);
	}
	else if(ID== PCU_BAT_VOLT_N4) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[9],6);
	}
	else if(ID== PCU_BAT_VOLT_N5) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[12],6);
	}
	else if(ID== PCU_BAT_VOLT_N6) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[15],6);
	}
	else if(ID== PCU_BAT_VOLT_N7) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[18],6);
	}
	else if(ID== PCU_BAT_VOLT_N8) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[21],6);
	}
	else if(ID== PCU_BAT_VOLT_N9) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[24],6);
	}
	else if(ID== PCU_BAT_VOLT_N10) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[27],6);
	}
	else if(ID== PCU_BAT_VOLT_N11) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[30],6);
	}
	else if(ID== PCU_BAT_VOLT_N12) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[31],6);
	}
	else if(ID== PCU_BAT_VOLT_N13) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[36],6);
	}
	else if(ID== PCU_BAT_VOLT_N14) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[39],6);
	}
	else if(ID== PCU_BAT_VOLT_N15) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[42],6);
	}
	else if(ID== PCU_BAT_VOLT_N16) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[45],6);
	}
	else if(ID== PCU_BAT_VOLT_N17) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[48],6);
	}
	else if(ID== PCU_BAT_VOLT_N18) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[51],6);
	}
	else if(ID== PCU_BAT_VOLT_N19) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[54],6);
	}
	else if(ID== PCU_BAT_VOLT_N20) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleVolt[57],6);
	}
	else if(ID== PCU_BAT_TEMP_N1) 				//电池单体温度数据报文1
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[0],6);
	}
	else if(ID== PCU_BAT_TEMP_N2) 				//电池单体温度数据报文2
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[6],6);
	}
	else if(ID== PCU_BAT_TEMP_N3) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[12],6);
	}
	else if(ID== PCU_BAT_TEMP_N4) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[18],6);
	}
	else if(ID== PCU_BAT_TEMP_N5) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[24],6);
	}
	else if(ID== PCU_BAT_TEMP_N6) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[30],6);
	}
	else if(ID== PCU_BAT_TEMP_N7) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[36],6);
	}
	else if(ID== PCU_BAT_TEMP_N8) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[42],6);
	}
	else if(ID== PCU_BAT_TEMP_N9) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[48],6);
	}
	else if(ID== PCU_BAT_TEMP_N10) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[54],6);
	}
	else if(ID== PCU_BAT_TEMP_N11) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[60],6);
	}
	else if(ID== PCU_BAT_TEMP_N12) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[66],6);
	}
	else if(ID== PCU_BAT_TEMP_N13) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[72],6);
	}
	else if(ID== PCU_BAT_TEMP_N14) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[78],6);
	}
	else if(ID== PCU_BAT_TEMP_N15) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[84],6);
	}
	else if(ID== PCU_BAT_TEMP_N16) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[80],6);
	}
	else if(ID== PCU_BAT_TEMP_N17) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[86],6);
	}
	else if(ID== PCU_BAT_TEMP_N18) 
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[92],6);
	}
	else if(ID== PCU_BAT_TEMP_N19) 
	{
		//memcpy(pu8DecodeBMSMesg,&(INT8U*)gUserDB.B_singleTemp[98],6);
	}
	else if(ID== PCU_BAT_TEMP_N20) 
	{
	}
	else if(ID== PCU_BAT_ID1) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[0],8);	
	}
	else if(ID== PCU_BAT_ID2) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[1],8);	
	}
	else if(ID== PCU_BAT_ID3) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[2],8);	
	}
	else if(ID== PCU_BAT_ID4) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[3],8);	
	}
	else if(ID== PCU_BAT_ID5) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[4],8);	
	}
	else if(ID== PCU_BAT_ID6) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[5],8);	
	}
	else if(ID== PCU_BAT_ID7) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[6],8);	
	}
	else if(ID== PCU_BAT_ID8) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[7],8);	
	}
	else if(ID== PCU_BAT_ID9) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[8],8);	
	}
	else if(ID== PCU_BAT_ID10) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[9],8);	
	}
	else if(ID== PCU_BAT_ID11) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[10],8);	
	}
	else if(ID== PCU_BAT_ID12) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[11],8);	
	}
	else if(ID== PCU_BAT_ID13) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[12],8);	
	}
	else if(ID== PCU_BAT_ID14) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[13],8);	
	}
	else if(ID== PCU_BAT_ID15) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[14],8);	
	}
	else if(ID== PCU_BAT_ID16) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[15],8);	
	}
	else if(ID== PCU_BAT_ID17) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[16],8);	
	}
	else if(ID== PCU_BAT_ID18) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[17],8);	
	}
	else if(ID== PCU_BAT_ID19) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[18],8);	
	}
	else if(ID== PCU_BAT_ID20) 							//电池箱ID报文
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[19],8);	
	}
	
	else if(ID== PCU_MCM_RETURN_SET_SINGLE_ALL) 		//读取所有单体控制参数
	{
		pu8DecodeBMSMesg[0]= (INT32U)gUserDB.M_readSingleTotalSteps;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readSinglePermitChargeTemH;
		
	}
	else if(ID== PCU_MCM_RETURN_SET_STAGE_ALL) 		//读取所有阶段控制参数
	{
		pu8DecodeBMSMesg[0]= 1;
		pu8DecodeBMSMesg[1]= gUserDB.M_readStageType;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readOutValue;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readOutValue>>8;
		pu8DecodeBMSMesg[4]= (INT32U)gUserDB.M_readLimitValue;
		pu8DecodeBMSMesg[5]= (INT32U)gUserDB.M_readLimitValue>>8;
		pu8DecodeBMSMesg[6]= (INT32U)gUserDB.M_readStopValue;
		pu8DecodeBMSMesg[7]= (INT32U)gUserDB.M_readStopValue>>8;
	}
	else if(ID== PCU_MCM_RETURN_SET_SINGLE) 			//读取单体控制参数
	{
		pu8DecodeBMSMesg[0]= gUserDB.M_readSingleTotalSteps;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readSinglePermitChargeTemH;
	}
	else if(ID== PCU_MCM_RETURN_SET_STAGE) 			//读取阶段控制参数
	{
		pu8DecodeBMSMesg[0]= gUserDB.M_readSingleTotalSteps;
		pu8DecodeBMSMesg[1]= gUserDB.M_readStageType;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readOutValue;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readOutValue>>8;
		pu8DecodeBMSMesg[4]= (INT32U)gUserDB.M_readLimitValue;
		pu8DecodeBMSMesg[5]= (INT32U)gUserDB.M_readLimitValue>>8;
		pu8DecodeBMSMesg[6]= (INT32U)gUserDB.M_readStopValue;
		pu8DecodeBMSMesg[7]= (INT32U)gUserDB.M_readStopValue>>8;
	}
	else if(ID== PCU_MCM_STATE0) 						//充电机状态帧1
	{
		pu8DecodeBMSMesg[0]= gUserDB.M_currWorkStep+gUserDB.M_currChargeMode<<4;
		pu8DecodeBMSMesg[1]= 0XFF;
		pu8DecodeBMSMesg[2]= 0XFF;
		pu8DecodeBMSMesg[3]= 0XFF;
		pu8DecodeBMSMesg[4]= 0XFF;
		pu8DecodeBMSMesg[5]= 0XFF;
		pu8DecodeBMSMesg[6]= gUserDB.M_carLinkState;
		pu8DecodeBMSMesg[7]= gUserDB.M_mcmLifeCnt++;
		if(gUserDB.M_mcmLifeCnt>=15)
			gUserDB.M_mcmLifeCnt=0;
	}
	else if(ID== PCU_MCM_STATE1) 						//充电机状态帧2
	{
		pu8DecodeBMSMesg[0]= (INT32U)gUserDB.M_thisPower;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_thisPower>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_thisChargeAH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_thisChargeAH>>8;
		pu8DecodeBMSMesg[4]= (INT32U)gUserDB.M_thisChargePower;
		pu8DecodeBMSMesg[5]= (INT32U)gUserDB.M_thisChargePower>>8;
	}
	else if(ID== PCU_MCM_STATE2) 						//充电机状态帧3
	{
	}
	else if(ID== PCU_MCM_STATE3) 						//充电机状态帧4
	{
	}
	else if(ID== PCU_MCM_PDU_INFO) 					//电表信息帧
	{
		pu8DecodeBMSMesg[0]= gUserDB.writeAt;
		if(gUserDB.writeAt==1)
		{
			memcpy(&pu8DecodeBMSMesg[1], gUserDB.M_pdu_ID,7);			
		}
		else if(gUserDB.writeAt==2)
		{
			pu8DecodeBMSMesg[1]=gUserDB.M_pdu_ID[7];
			pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_degrees;
			pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_degrees>>8;
			pu8DecodeBMSMesg[4]= (INT32U)gUserDB.M_degrees>>16;
			pu8DecodeBMSMesg[5]= (INT32U)gUserDB.M_degrees>>24;
			pu8DecodeBMSMesg[6]= (INT32U)gUserDB.M_pdu_failCode;
			pu8DecodeBMSMesg[7]= (INT32U)gUserDB.M_pdu_failCode>>8;
		}
		else if(gUserDB.writeAt==3)
		{
			pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_pdu_failCode>>16;
			pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_pdu_failCode>>24;
		
		}
	}
	else if(ID== PCU_MCM_DEVICE_ID) 					//充电设备信息帧
	{
		pu8DecodeBMSMesg[0]= gUserDB.M_device_ID[0];
		pu8DecodeBMSMesg[1]= gUserDB.M_device_ID[1];
		pu8DecodeBMSMesg[2]= gUserDB.M_device_ID[2];
		pu8DecodeBMSMesg[3]= gUserDB.M_device_ID[3];
		pu8DecodeBMSMesg[4]= gUserDB.M_device_ID[4];
		pu8DecodeBMSMesg[5]= gUserDB.M_device_ID[5];
		pu8DecodeBMSMesg[6]= gUserDB.M_device_ID[6];
		pu8DecodeBMSMesg[7]= gUserDB.M_device_ID[7];
	}
	else if(ID== PCU_CCS_IDENTIFY) 					//充电辨识报文
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_CCS_Code;
		pu8DecodeBMSMesg[1]= gUserDB.C_chargePlugCode;
		pu8DecodeBMSMesg[2]= gUserDB.C_idenfifyResult;
		pu8DecodeBMSMesg[3]= gUserDB.C_CCS_Area;
		pu8DecodeBMSMesg[4]= gUserDB.C_CCS_AreaCode[0];
		pu8DecodeBMSMesg[5]= gUserDB.C_CCS_AreaCode[1];
		pu8DecodeBMSMesg[6]= gUserDB.C_CCS_AreaCode[2];
		pu8DecodeBMSMesg[7]= gUserDB.C_CCS_AreaCode[3];
	}
	else if(ID== PCU_CCS_MAX_OUTPUT) 					//充电最大输出能力报文
	{
		pu8DecodeBMSMesg[0]= (INT32U)(gUserDB.C_ccsMinChargeVolt);
		pu8DecodeBMSMesg[1]= (INT32U)(gUserDB.C_ccsMinChargeVolt)>>8;
		pu8DecodeBMSMesg[2]= (INT32U)(gUserDB.C_ccsMaxChargeVolt);
		pu8DecodeBMSMesg[3]= (INT32U)(gUserDB.C_ccsMaxChargeVolt)>>8;
		pu8DecodeBMSMesg[4]= (INT32U)(gUserDB.C_ccsMaxChargeCurr);
		pu8DecodeBMSMesg[5]= (INT32U)(gUserDB.C_ccsMaxChargeCurr)>>8;
		pu8DecodeBMSMesg[4]= gUserDB.C_plusCurr;
		pu8DecodeBMSMesg[5]= gUserDB.C_plusCurrLastTime;
	}
	else if(ID== PCU_CCS_STATE) 						//充电状态报文
	{
		pu8DecodeBMSMesg[0]= (INT32U)(gUserDB.C_outVolt);
		pu8DecodeBMSMesg[1]= (INT32U)(gUserDB.C_outVolt)>>8;
		pu8DecodeBMSMesg[2]= (INT32U)(gUserDB.C_outCurr);
		pu8DecodeBMSMesg[3]= (INT32U)(gUserDB.C_outCurr)>>8;
		pu8DecodeBMSMesg[4]= gUserDB.C_chargeTimeTotal;
		pu8DecodeBMSMesg[5]= gUserDB.C_chargeTimeTotal>>8;
		pu8DecodeBMSMesg[6]= gUserDB.C_thisStepTime;
		pu8DecodeBMSMesg[7]= gUserDB.C_thisStepTime>>8;
	}
	else if(ID== PCU_CCS_STOP) 						//充电中止报文
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_CCS_StopCause[0];
		pu8DecodeBMSMesg[1]= gUserDB.C_CCS_StopCause[1];
		pu8DecodeBMSMesg[2]= gUserDB.C_CCS_StopCause[2];
		pu8DecodeBMSMesg[3]= gUserDB.C_CCS_StopCause[3];
	}
	else if(ID== PCU_CCS_COUNT) 						//充电统计报文
	{
		pu8DecodeBMSMesg[0]= (INT32U)(gUserDB.M_thisChargeAH);
		pu8DecodeBMSMesg[1]= (INT32U)(gUserDB.M_thisChargeAH)>>8;
		pu8DecodeBMSMesg[2]= (INT32U)(gUserDB.C_chargeTimeTotal);
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.C_chargeTimeTotal>>8;
		pu8DecodeBMSMesg[4]= (INT32U)(gUserDB.M_thisChargePower);
		pu8DecodeBMSMesg[5]= (INT32U)(gUserDB.M_thisChargePower)>>8;
		pu8DecodeBMSMesg[6]= gUserDB.C_ccs_id;
		
	}
	else if(ID== PCU_CCS_ERR) 							//充电错误报文
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_recBmsIdentifyTimeout+(gUserDB.C_recBatInfoTimeout<<1);
		pu8DecodeBMSMesg[1]= gUserDB.C_recBmsReadyTimeout;
		pu8DecodeBMSMesg[2]= gUserDB.C_recBatchargeLevelTimeout+(gUserDB.C_recBatStatusTimeout<<1)+(gUserDB.C_recBmsStopTimeout<<2);
		pu8DecodeBMSMesg[3]= gUserDB.C_recBmsStatisticsTimeOut;
	
	}
	else if(ID== PCU_CCS_FAULT) 						//充电故障报文
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_CCSFaultCode;
	}

	
}//the end


/*********************************************************************************************************
函数名称: 	can_PgnPackCheck 
功能描述:    验证CAN数据是否为本机合法的PGN

输　入:      adr   目标地址
			    pgn     参数组编号

 输　出:    0		失败
			>0		成功						     
*********************************************************************************************************/
INT8U can_PgnPackCheck(INT8U adr,INT16U pgn)
{
	INT8U ret=0;

	if(adr == CCS_ADR)				// 是否为本机地址?
	{
		switch(pgn)
		{
		case 0x26:                   //DC/DC mes
//		case BMS_BCP1_PGN:			//蓄电池充电参数
//		case BMS_CBP1_PGN:			//整车蓄电池参数1
//		case BMS_CBP2_PGN:			//整车蓄电池参数2
//		case BMS_BCS_PGN:			//蓄电池充电状态
//		case BMS_CTS_PGN:			//时间同步信息
//		case BMS_MVD_PGN:			//蓄电池模块电压数据
//		case BMS_BTD_PGN:			//蓄电池温度数据
//		case BMS_SOC_PGN:			//蓄电池SOC数据
//		case BMS_AVD_PGN:			//蓄电池平均电压数据
//		case BMS_BBI1_PGN:			//蓄电池基本信息1
//		case BMS_BBI2_PGN:			//蓄电池基本信息2
//		case BMS_BBI_PGN:			//蓄电池基本信息
//		case CCS_CLF_PGN:			//充电机生命信息帧
//		case CCS_CCS1_PGN:			//充电机状态1
//		case CCS_CCS2_PGN:			//充电机状态2
//		case CCS_CCP_PGN:			//充电机参数
//		case CCS_EST_PGN:			//充电机紧急停止命令
//		case CCS_CST_PGN:			//充电机启动充电命令
//		case CCS_CSP_PGN:			//充电机停止充电命令		 
		
			ret=1; 					//PGN参数合法标志		
			break;
		default:
			ret=0;					//PGN参数非法标志
			break;
		}
	}
	else if(adr == BROADCAST_ADR)   //是否为广播地址？
	{
		switch(pgn)
		{
		case BMS_BBI1_PGN:			//蓄电池基本信息1
		case BMS_BBI2_PGN:			//蓄电池基本信息2
		case BMS_BBI_PGN:			//蓄电池基本信息
			ret=1;
			break;
		default:
			ret=0; 					//PGN参数非法标志
			break;
		} 		 					
	}
	else
		ret=0;					   //PGN参数非法标志

	return ret;
}






