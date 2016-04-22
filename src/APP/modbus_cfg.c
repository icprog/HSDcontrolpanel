 /*********************************************************************************************************
*                                            头文件包含
*********************************************************************************************************/
#include "Global.h"

/*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************/


/********************************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************************/

extern void RestoreData(void);
extern void EE_WriteData(INT8U adr,INT8U *buf);
void modbusReadRegDataDeal(void);

 /*********************************************************************************************************************
函数名称: 	modbusWriteCallBack          
功能描述: 	modbus写回调函数,寄存器地址与组态屏一致
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/

void modbusWriteCallBack(INT16U adr,INT8U len)
{ 
	//static INT8U buf[4];
	//FP32 tmp;
	//FP32 k=0;
	//INT8U i=0;
	INT32U tmp;
	INT8U  TmpArr[10];
	 

   switch(adr)
   { 
   
    case 1: //系统控制 	    
		    gUserDB.systemCtl=ModbusREGDat[1]; 		
		break; 
	case 5: //充电时间    
		    gUserDB.CCS_charge_time=ModbusREGDat[5]; 		
		break;
	case 6: //系统控制 	    
		   // gUserDB.systemCtl=ModbusREGDat[1]; 		
		break;	   
  	case 7: //手动电流设置
		    gUserDB.manualSetCurr=ModbusREGDat[7]/10.0f; 
		//	EE_WriteData(ADDR_MANUAL_SET_VOLT,TmpArr);			//设置输出电流 

		break;
	case 8: //手动电压设置
		    gUserDB.manualSetVolt=ModbusREGDat[8]/10.0f; 
		//	EE_WriteData(ADDR_MANUAL_SET_VOLT,TmpArr);			//设置输出电压 
		break; 	
	case 9:  //手动开关	     
	    if(	ModbusREGDat[9]==0x01)
	    {
			gUserDB.manualRunCtlFlag =RUN; 	   //手动开关 
			gUserDB.autoCtlFlag=MANUAL_CTL;			
		}
		else if(	ModbusREGDat[9]==0x00)
		 {   gUserDB.manualRunCtlFlag =STOP; 	   //手动开关
		 }
		break;
	case 10:  //自动开关
	     
	    if(	ModbusREGDat[10]==0x01)
	    {	
			gUserDB.autoRunCtlFlag =RUN; 	   //自动开关
			gUserDB.autoCtlFlag=AUTO_CTL;
		}
		else if(ModbusREGDat[10]==0x00)
		 {  
		  gUserDB.autoRunCtlFlag =STOP; 	   //自动开关
		 }
		break;
	case 140:  //最高充电电压	     
	    gUserDB.CCS_outVoltMax =ModbusREGDat[140]/10.0f; 	   //
		break;
	case 141:  //最高充电电流
	     gUserDB.CCS_outCurrMax =ModbusREGDat[141]/10.0f; 	   //
	    
		break;
	case 142:  //B_单体过压保护点
	     gUserDB.cellOverProtect =ModbusREGDat[142]/100.0f; 	   //
	    
		break;
	case 143:  //B_单体欠压保护点
	     //gUserDB.CCS_outCurrMax =ModbusREGDat[143]/100; 	   // 	    
		break;
	case 144:  //B_过温保护点
	   //  gUserDB.BatTempProtect =ModbusREGDat[144]; 	   //	    
		break;
	case 145:  //充电机地址
	     gUserDB.CCS_adr =ModbusREGDat[145]; 	   //
	    
		break;
	case 146:  //充电电量
	     gUserDB.CCS_charge_power =ModbusREGDat[146]; 	   //
	    
		break;
	case 147:  //闭合总继电器控制
//	     if(	ModbusREGDat[147]==0x01)
//	    	gUserDB.sumContactor_ctl=1;
//		else
//		    gUserDB.sumContactor_ctl=0;
           if (MODBUS_DEVICEID ==2 )
		   {
		     gUserDB.ledCtl  =	ModbusREGDat[147] ;
		   }
		   
		break;
	case 148 :  //系统模式控制
	    //	if (MODBUS_DEVICEID ==1 )
		    {
			   gUserDB.sysCtlMode  =	ModbusREGDat[148] ; 
		    }
		   
		break;
	case 149 :  //充电机2控制
	       if (MODBUS_DEVICEID ==1 )
		   {
		      gUserDB.Machine2RunFlag  =	ModbusREGDat[149] ; 
		   }
		   
		     
		break;
	case 150 :  //闭合总继电器控制
	    
		break;

	default:
	  break;
	
	}

	gUserDB.m485comLinkStatus=NOALARM;	
    gUserDB.m485CommAlarmCnt=0;	
}

 /*********************************************************************************************************************
函数名称: 	modbusReadCallBack          
功能描述: 	modbus读回调函数
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/

void modbusReadCallBack(void)
{    
 // ModbusREGDat[0] =(gUserDB.BMS_canCommAlarm<<1);	                 //byte0:pc连接状态byte1 电表状态 byte8~byte15PCU地址
 // ModbusREGDat[PDU_VOLT_ADR] =(INT32U)(gUserDB.C_outVolt*10);        	 							//输出电压
 // ModbusREGDat[PDU_CUR_ADR] =(INT32U)(gUserDB.C_outCurr*10);        	 							//输出电流
 // ModbusREGDat[PDU_POWER_L_ADR] =(INT32U)gUserDB.M_degrees;        	 							//电能1低位
 // ModbusREGDat[PDU_POWER_H_ADR] =(INT32U)gUserDB.M_degrees>>16;        						    //电能2低位


 // modbusReadRegDataDeal();            //与屏通信数据转换  
 

  gUserDB.m485comLinkStatus=NOALARM;	
  gUserDB.m485CommAlarmCnt=0; 
  
  

}

 /*********************************************************************************************************************
函数名称: 	modbusReadRegDataDeal          
功能描述: 	modbus寄存器值定时转换处理函数
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/

void modbusReadRegDataDeal(void)
{ 
   INT32U tmp;
      
  ModbusREGDat[1] =gUserDB.ccu1_CommAlarm +(gUserDB.CCS_chargeRunStatus<<1)+(gUserDB.bms_readyStatus<<2)
	                 +(gUserDB.CCS_chargeEnable<<3);                					//充电机状态

  ModbusREGDat[2] =gModuleInfo[0].comAlarm +(gModuleInfo[1].comAlarm<<1)+(gModuleInfo[2].comAlarm<<2)+
   (gModuleInfo[3].comAlarm<<3)+(gModuleInfo[4].comAlarm<<4)+(gModuleInfo[5].comAlarm<<5)+
   (gModuleInfo[6].comAlarm<<6)+(gModuleInfo[7].comAlarm<<7)+(gModuleInfo[8].comAlarm<<8)+
   (gModuleInfo[9].comAlarm<<9)+(gModuleInfo[10].comAlarm<<10)+(gModuleInfo[11].comAlarm<<11)+
   (gModuleInfo[12].comAlarm<<12)+ (gModuleInfo[13].comAlarm<<13)+(gModuleInfo[14].comAlarm<<14)+
   (gModuleInfo[15].comAlarm<<15);
   
   ModbusREGDat[3]=(gModuleInfo[16].comAlarm<<0)+(gModuleInfo[17].comAlarm<<1)+(gModuleInfo[18].comAlarm<<2)+
    (gModuleInfo[19].comAlarm<<3)+(gModuleInfo[0].runStatus<<4) +
	(gModuleInfo[1].runStatus<<5)+(gModuleInfo[2].runStatus<<6)+(gModuleInfo[3].runStatus<<7)+
	(gModuleInfo[4].runStatus<<8)+(gModuleInfo[5].runStatus<<9)+(gModuleInfo[6].runStatus<<10)+
	(gModuleInfo[7].runStatus<<11)+(gModuleInfo[8].runStatus<<12)+(gModuleInfo[9].runStatus<<13)+
	(gModuleInfo[10].runStatus<<14)+ (gModuleInfo[11].runStatus<<15);

	ModbusREGDat[4]=(gModuleInfo[12].runStatus<<0)+(gModuleInfo[13].runStatus<<1)+(gModuleInfo[14].runStatus<<2)+
	(gModuleInfo[15].runStatus<<3)+(gModuleInfo[16].runStatus<<4)+(gModuleInfo[17].runStatus<<5)+
	(gModuleInfo[18].runStatus<<6)+(gModuleInfo[19].runStatus<<7)+(gUserDB.DcPduComAlarm<<8);


//  ModbusREGDat[6] = gModuleInfo[0].comAlarm;        	 											//模块连接状态
//  ModbusREGDat[5] = gModuleInfo[0].runStatus ;        	 										//模块启动状态
//  OSTimeDlyHMSM(0,0,0,10);

  ModbusREGDat[17] =(INT32U)(gModuleInfo[0].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[18] =(INT32U)(gModuleInfo[1].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[19] =(INT32U)(gModuleInfo[2].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[20] =(INT32U)(gModuleInfo[3].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[21] =(INT32U)(gModuleInfo[4].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[22] =(INT32U)(gModuleInfo[5].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[23] =(INT32U)(gModuleInfo[6].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[24] =(INT32U)(gModuleInfo[7].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[25] =(INT32U)(gModuleInfo[8].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[26] =(INT32U)(gModuleInfo[9].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[27] =(INT32U)(gModuleInfo[10].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[28] =(INT32U)(gModuleInfo[11].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[29] =(INT32U)(gModuleInfo[12].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[30] =(INT32U)(gModuleInfo[13].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[31] =(INT32U)(gModuleInfo[14].setCurr*10);        						            //模块2输出电压	
  ModbusREGDat[32] =(INT32U)(gModuleInfo[15].setCurr*10);        						            //模块2输出电压

  ModbusREGDat[136] =(INT32U)(gModuleInfo[16].setCurr*10);        						            //模块2输出电压
  ModbusREGDat[137] =(INT32U)(gModuleInfo[17].setCurr*10);        						            //模块2输出电压
	
  ModbusREGDat[138] =(INT32U)(pdu_info.power_total*100)>>16;        						            //电表读数高位
  ModbusREGDat[139] =(INT32U)(pdu_info.power_total*100);        						               //电表读数低位

 // OSTimeDlyHMSM(0,0,0,10); 	

  ModbusREGDat[33] =(INT32U)(gModuleInfo[0].outVolt*10);        	 							    //模块1输出电压
  ModbusREGDat[34] =(INT32U)(gModuleInfo[1].outVolt*10);        						            //模块2输出电压	
  ModbusREGDat[35] =(INT32U)(gModuleInfo[2].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[36] =(INT32U)(gModuleInfo[3].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[37] =(INT32U)(gModuleInfo[4].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[38] =(INT32U)(gModuleInfo[5].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[39] =(INT32U)(gModuleInfo[6].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[40] =(INT32U)(gModuleInfo[7].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[41] =(INT32U)(gModuleInfo[8].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[42] =(INT32U)(gModuleInfo[9].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[43] =(INT32U)(gModuleInfo[10].outVolt*10);        	 							    //模块1输出电压
  ModbusREGDat[44] =(INT32U)(gModuleInfo[11].outVolt*10);        						            //模块2输出电压	
  ModbusREGDat[45] =(INT32U)(gModuleInfo[12].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[46] =(INT32U)(gModuleInfo[13].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[47] =(INT32U)(gModuleInfo[14].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[48] =(INT32U)(gModuleInfo[15].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[49] =(INT32U)(gModuleInfo[16].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[50] =(INT32U)(gModuleInfo[17].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[51] =(INT32U)(gModuleInfo[18].outVolt*10);        						            //模块2输出电压
  ModbusREGDat[52] =(INT32U)(gModuleInfo[19].outVolt*10);        						            //模块2输出电压

 // OSTimeDlyHMSM(0,0,0,10);
  ModbusREGDat[53] =(INT32U)(gModuleInfo[0].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[54] =(INT32U)(gModuleInfo[1].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[55] =(INT32U)(gModuleInfo[2].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[56] =(INT32U)(gModuleInfo[3].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[57] =(INT32U)(gModuleInfo[4].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[58] =(INT32U)(gModuleInfo[5].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[59] =(INT32U)(gModuleInfo[6].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[60] =(INT32U)(gModuleInfo[7].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[61] =(INT32U)(gModuleInfo[8].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[62] =(INT32U)(gModuleInfo[9].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[63] =(INT32U)(gModuleInfo[10].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[64] =(INT32U)(gModuleInfo[11].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[65] =(INT32U)(gModuleInfo[12].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[66] =(INT32U)(gModuleInfo[13].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[67] =(INT32U)(gModuleInfo[14].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[68] =(INT32U)(gModuleInfo[15].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[69] =(INT32U)(gModuleInfo[16].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[70] =(INT32U)(gModuleInfo[17].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[71] =(INT32U)(gModuleInfo[18].outCurr*10);        						            //模块2输出电压
  ModbusREGDat[72] =(INT32U)(gModuleInfo[19].outCurr*10);        						            //模块2输出电压
//  
  ModbusREGDat[12] =gUserDB.startModuleSum;        						            //设定启动模块数
  ModbusREGDat[11] =gUserDB.C_CCSFaultCode;        						            //设定启动模块数

  ModbusREGDat[14] =gUserDB.CCS_setOutVolt*10;        						            //设置电压
  ModbusREGDat[15] =gUserDB.CCS_setOutCurr*10;        						            //设置电流
	
//	ModbusREGDat[147] =(INT32U)(pdu_info.A_curr);        						            //模块2输出电压
//	ModbusREGDat[148] =(INT32U)(pdu_info.B_curr);        						            //模块2输出电压
//	ModbusREGDat[149] =(INT32U)(pdu_info.C_curr);        						            //模块2输出电压
//	ModbusREGDat[150] =(INT32U)(pdu_info.A_volt);        						            //模块2输出电压
//	ModbusREGDat[151] =(INT32U)(pdu_info.B_volt);        						            //模块2输出电压
//	ModbusREGDat[152] =(INT32U)(pdu_info.C_volt);        						            //模块2输出电压

//  OSTimeDlyHMSM(0,0,0,10); 	
  

 

}


