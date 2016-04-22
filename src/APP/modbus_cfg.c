 /*********************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************/
#include "Global.h"

/*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************/


/********************************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************************/

extern void RestoreData(void);
extern void EE_WriteData(INT8U adr,INT8U *buf);
void modbusReadRegDataDeal(void);

 /*********************************************************************************************************************
��������: 	modbusWriteCallBack          
��������: 	modbusд�ص�����,�Ĵ�����ַ����̬��һ��
�䡡��:   	��

�䡡��:   	��                 
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
   
    case 1: //ϵͳ���� 	    
		    gUserDB.systemCtl=ModbusREGDat[1]; 		
		break; 
	case 5: //���ʱ��    
		    gUserDB.CCS_charge_time=ModbusREGDat[5]; 		
		break;
	case 6: //ϵͳ���� 	    
		   // gUserDB.systemCtl=ModbusREGDat[1]; 		
		break;	   
  	case 7: //�ֶ���������
		    gUserDB.manualSetCurr=ModbusREGDat[7]/10.0f; 
		//	EE_WriteData(ADDR_MANUAL_SET_VOLT,TmpArr);			//����������� 

		break;
	case 8: //�ֶ���ѹ����
		    gUserDB.manualSetVolt=ModbusREGDat[8]/10.0f; 
		//	EE_WriteData(ADDR_MANUAL_SET_VOLT,TmpArr);			//���������ѹ 
		break; 	
	case 9:  //�ֶ�����	     
	    if(	ModbusREGDat[9]==0x01)
	    {
			gUserDB.manualRunCtlFlag =RUN; 	   //�ֶ����� 
			gUserDB.autoCtlFlag=MANUAL_CTL;			
		}
		else if(	ModbusREGDat[9]==0x00)
		 {   gUserDB.manualRunCtlFlag =STOP; 	   //�ֶ�����
		 }
		break;
	case 10:  //�Զ�����
	     
	    if(	ModbusREGDat[10]==0x01)
	    {	
			gUserDB.autoRunCtlFlag =RUN; 	   //�Զ�����
			gUserDB.autoCtlFlag=AUTO_CTL;
		}
		else if(ModbusREGDat[10]==0x00)
		 {  
		  gUserDB.autoRunCtlFlag =STOP; 	   //�Զ�����
		 }
		break;
	case 140:  //��߳���ѹ	     
	    gUserDB.CCS_outVoltMax =ModbusREGDat[140]/10.0f; 	   //
		break;
	case 141:  //��߳�����
	     gUserDB.CCS_outCurrMax =ModbusREGDat[141]/10.0f; 	   //
	    
		break;
	case 142:  //B_�����ѹ������
	     gUserDB.cellOverProtect =ModbusREGDat[142]/100.0f; 	   //
	    
		break;
	case 143:  //B_����Ƿѹ������
	     //gUserDB.CCS_outCurrMax =ModbusREGDat[143]/100; 	   // 	    
		break;
	case 144:  //B_���±�����
	   //  gUserDB.BatTempProtect =ModbusREGDat[144]; 	   //	    
		break;
	case 145:  //������ַ
	     gUserDB.CCS_adr =ModbusREGDat[145]; 	   //
	    
		break;
	case 146:  //������
	     gUserDB.CCS_charge_power =ModbusREGDat[146]; 	   //
	    
		break;
	case 147:  //�պ��̵ܼ�������
//	     if(	ModbusREGDat[147]==0x01)
//	    	gUserDB.sumContactor_ctl=1;
//		else
//		    gUserDB.sumContactor_ctl=0;
           if (MODBUS_DEVICEID ==2 )
		   {
		     gUserDB.ledCtl  =	ModbusREGDat[147] ;
		   }
		   
		break;
	case 148 :  //ϵͳģʽ����
	    //	if (MODBUS_DEVICEID ==1 )
		    {
			   gUserDB.sysCtlMode  =	ModbusREGDat[148] ; 
		    }
		   
		break;
	case 149 :  //����2����
	       if (MODBUS_DEVICEID ==1 )
		   {
		      gUserDB.Machine2RunFlag  =	ModbusREGDat[149] ; 
		   }
		   
		     
		break;
	case 150 :  //�պ��̵ܼ�������
	    
		break;

	default:
	  break;
	
	}

	gUserDB.m485comLinkStatus=NOALARM;	
    gUserDB.m485CommAlarmCnt=0;	
}

 /*********************************************************************************************************************
��������: 	modbusReadCallBack          
��������: 	modbus���ص�����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/

void modbusReadCallBack(void)
{    
 // ModbusREGDat[0] =(gUserDB.BMS_canCommAlarm<<1);	                 //byte0:pc����״̬byte1 ���״̬ byte8~byte15PCU��ַ
 // ModbusREGDat[PDU_VOLT_ADR] =(INT32U)(gUserDB.C_outVolt*10);        	 							//�����ѹ
 // ModbusREGDat[PDU_CUR_ADR] =(INT32U)(gUserDB.C_outCurr*10);        	 							//�������
 // ModbusREGDat[PDU_POWER_L_ADR] =(INT32U)gUserDB.M_degrees;        	 							//����1��λ
 // ModbusREGDat[PDU_POWER_H_ADR] =(INT32U)gUserDB.M_degrees>>16;        						    //����2��λ


 // modbusReadRegDataDeal();            //����ͨ������ת��  
 

  gUserDB.m485comLinkStatus=NOALARM;	
  gUserDB.m485CommAlarmCnt=0; 
  
  

}

 /*********************************************************************************************************************
��������: 	modbusReadRegDataDeal          
��������: 	modbus�Ĵ���ֵ��ʱת��������
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/

void modbusReadRegDataDeal(void)
{ 
   INT32U tmp;
      
  ModbusREGDat[1] =gUserDB.ccu1_CommAlarm +(gUserDB.CCS_chargeRunStatus<<1)+(gUserDB.bms_readyStatus<<2)
	                 +(gUserDB.CCS_chargeEnable<<3);                					//����״̬

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


//  ModbusREGDat[6] = gModuleInfo[0].comAlarm;        	 											//ģ������״̬
//  ModbusREGDat[5] = gModuleInfo[0].runStatus ;        	 										//ģ������״̬
//  OSTimeDlyHMSM(0,0,0,10);

  ModbusREGDat[17] =(INT32U)(gModuleInfo[0].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[18] =(INT32U)(gModuleInfo[1].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[19] =(INT32U)(gModuleInfo[2].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[20] =(INT32U)(gModuleInfo[3].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[21] =(INT32U)(gModuleInfo[4].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[22] =(INT32U)(gModuleInfo[5].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[23] =(INT32U)(gModuleInfo[6].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[24] =(INT32U)(gModuleInfo[7].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[25] =(INT32U)(gModuleInfo[8].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[26] =(INT32U)(gModuleInfo[9].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[27] =(INT32U)(gModuleInfo[10].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[28] =(INT32U)(gModuleInfo[11].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[29] =(INT32U)(gModuleInfo[12].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[30] =(INT32U)(gModuleInfo[13].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[31] =(INT32U)(gModuleInfo[14].setCurr*10);        						            //ģ��2�����ѹ	
  ModbusREGDat[32] =(INT32U)(gModuleInfo[15].setCurr*10);        						            //ģ��2�����ѹ

  ModbusREGDat[136] =(INT32U)(gModuleInfo[16].setCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[137] =(INT32U)(gModuleInfo[17].setCurr*10);        						            //ģ��2�����ѹ
	
  ModbusREGDat[138] =(INT32U)(pdu_info.power_total*100)>>16;        						            //��������λ
  ModbusREGDat[139] =(INT32U)(pdu_info.power_total*100);        						               //��������λ

 // OSTimeDlyHMSM(0,0,0,10); 	

  ModbusREGDat[33] =(INT32U)(gModuleInfo[0].outVolt*10);        	 							    //ģ��1�����ѹ
  ModbusREGDat[34] =(INT32U)(gModuleInfo[1].outVolt*10);        						            //ģ��2�����ѹ	
  ModbusREGDat[35] =(INT32U)(gModuleInfo[2].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[36] =(INT32U)(gModuleInfo[3].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[37] =(INT32U)(gModuleInfo[4].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[38] =(INT32U)(gModuleInfo[5].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[39] =(INT32U)(gModuleInfo[6].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[40] =(INT32U)(gModuleInfo[7].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[41] =(INT32U)(gModuleInfo[8].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[42] =(INT32U)(gModuleInfo[9].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[43] =(INT32U)(gModuleInfo[10].outVolt*10);        	 							    //ģ��1�����ѹ
  ModbusREGDat[44] =(INT32U)(gModuleInfo[11].outVolt*10);        						            //ģ��2�����ѹ	
  ModbusREGDat[45] =(INT32U)(gModuleInfo[12].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[46] =(INT32U)(gModuleInfo[13].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[47] =(INT32U)(gModuleInfo[14].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[48] =(INT32U)(gModuleInfo[15].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[49] =(INT32U)(gModuleInfo[16].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[50] =(INT32U)(gModuleInfo[17].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[51] =(INT32U)(gModuleInfo[18].outVolt*10);        						            //ģ��2�����ѹ
  ModbusREGDat[52] =(INT32U)(gModuleInfo[19].outVolt*10);        						            //ģ��2�����ѹ

 // OSTimeDlyHMSM(0,0,0,10);
  ModbusREGDat[53] =(INT32U)(gModuleInfo[0].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[54] =(INT32U)(gModuleInfo[1].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[55] =(INT32U)(gModuleInfo[2].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[56] =(INT32U)(gModuleInfo[3].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[57] =(INT32U)(gModuleInfo[4].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[58] =(INT32U)(gModuleInfo[5].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[59] =(INT32U)(gModuleInfo[6].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[60] =(INT32U)(gModuleInfo[7].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[61] =(INT32U)(gModuleInfo[8].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[62] =(INT32U)(gModuleInfo[9].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[63] =(INT32U)(gModuleInfo[10].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[64] =(INT32U)(gModuleInfo[11].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[65] =(INT32U)(gModuleInfo[12].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[66] =(INT32U)(gModuleInfo[13].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[67] =(INT32U)(gModuleInfo[14].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[68] =(INT32U)(gModuleInfo[15].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[69] =(INT32U)(gModuleInfo[16].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[70] =(INT32U)(gModuleInfo[17].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[71] =(INT32U)(gModuleInfo[18].outCurr*10);        						            //ģ��2�����ѹ
  ModbusREGDat[72] =(INT32U)(gModuleInfo[19].outCurr*10);        						            //ģ��2�����ѹ
//  
  ModbusREGDat[12] =gUserDB.startModuleSum;        						            //�趨����ģ����
  ModbusREGDat[11] =gUserDB.C_CCSFaultCode;        						            //�趨����ģ����

  ModbusREGDat[14] =gUserDB.CCS_setOutVolt*10;        						            //���õ�ѹ
  ModbusREGDat[15] =gUserDB.CCS_setOutCurr*10;        						            //���õ���
	
//	ModbusREGDat[147] =(INT32U)(pdu_info.A_curr);        						            //ģ��2�����ѹ
//	ModbusREGDat[148] =(INT32U)(pdu_info.B_curr);        						            //ģ��2�����ѹ
//	ModbusREGDat[149] =(INT32U)(pdu_info.C_curr);        						            //ģ��2�����ѹ
//	ModbusREGDat[150] =(INT32U)(pdu_info.A_volt);        						            //ģ��2�����ѹ
//	ModbusREGDat[151] =(INT32U)(pdu_info.B_volt);        						            //ģ��2�����ѹ
//	ModbusREGDat[152] =(INT32U)(pdu_info.C_volt);        						            //ģ��2�����ѹ

//  OSTimeDlyHMSM(0,0,0,10); 	
  

 

}


