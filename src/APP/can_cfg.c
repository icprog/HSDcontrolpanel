/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "includes.h"
#include "can_cfg.h"

/********************************************************************************************************************
*                                            ������������������
*********************************************************************************************************************/ 
extern INT8U bmsInfo[BMS_MAX_SIZE];
extern INT8U ccsInfo[CCS_MAX_SIZE];


/*********************************************************************************************************
��������: 	DecodeBMSMessage 
��������:  PC��Ϣ����

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
INT8U DecodePCMessage(INT8U *pu8DecodeBMSMesg, INT32U ID)
{
	INT8U ret=1;
	
	if(ID== PC_PC_DATE) 					//ϵͳ��ʱ����
	{
		gUserDB.time.year=(pu8DecodeBMSMesg[0]<<8) +pu8DecodeBMSMesg[1];
		gUserDB.time.month= pu8DecodeBMSMesg[2];
		gUserDB.time.day= pu8DecodeBMSMesg[3];
		gUserDB.time.hour= pu8DecodeBMSMesg[4];
		gUserDB.time.min= pu8DecodeBMSMesg[5];
		gUserDB.time.sec= pu8DecodeBMSMesg[6];
	}
	else if(ID== PC_PCU_P_SET)    			//Э��汾���ñ���
	{
		memcpy(gUserDB.P_version, pu8DecodeBMSMesg,7);
	}
	else if(ID== PC_INQUIRE_ALL)  			//��ѯ����� 
	{
		gUserDB.P_inquire_flag1= pu8DecodeBMSMesg[0];
		gUserDB.P_inquire_flag2= pu8DecodeBMSMesg[1];
	}
	
	else if(ID== PC_MCM_STOP_ALL) 			//����ͣ������ 
	{		
			if(pu8DecodeBMSMesg[0]==0x55)//ͣ��
			{
				gUserDB.C_ccsRunCtl=0;	
			}
			
	}
	else if(ID== PC_MCM_START_SINGLE)  	//�������� 
	{
		    if(pu8DecodeBMSMesg[0]==0x55)//ͣ��
			{
				gUserDB.C_ccsRunCtl=1;				
			}
			
	}
	else if(ID== PC_MCM_STOP_SINGLE)  		//ֹͣ���� 
	{
		   if(pu8DecodeBMSMesg[0]==0x55)//ͣ��
			{
				gUserDB.C_ccsRunCtl=0;	
			}
			else
			{
				
			}
	}
	else if(ID== PC_MCM_SET_SINGLE)  		//������Ʋ������� 
	{
		gUserDB.M_singleSetType=pu8DecodeBMSMesg[0];
		gUserDB.M_singleSetStep=pu8DecodeBMSMesg[1];
		gUserDB.M_setSinglePermitChargeVoltH=pu8DecodeBMSMesg[2]+pu8DecodeBMSMesg[3]<<8;		
		gUserDB.M_setSinglePermitChargeTemH=pu8DecodeBMSMesg[4];
		
	}
	else if(ID== PC_MCM_SET_STAGE)  		//�׶ο��Ʋ�������
	{
		gUserDB.M_setStageType=pu8DecodeBMSMesg[0];
		gUserDB.M_setStageStep=(pu8DecodeBMSMesg[1]&0xf0)>>4;
		gUserDB.M_setStageMode=pu8DecodeBMSMesg[1]&0x0f;
		gUserDB.M_setOutValue=pu8DecodeBMSMesg[2]+pu8DecodeBMSMesg[3]<<8;		
		gUserDB.M_setLimitValue=pu8DecodeBMSMesg[4]+pu8DecodeBMSMesg[5]<<8;	
		gUserDB.M_setStopValue=pu8DecodeBMSMesg[6]+pu8DecodeBMSMesg[7]<<8;	
	}
	else if(ID== PC_MCM_READ_SET_ALL)     //ȫ�����Ʋ�����ȡ���� 
	{
		if(pu8DecodeBMSMesg[0]==0x55)
			gUserDB.M_readSetAllFlag=1;
	}
	else if(ID== PC_MCM_READ_SET_SPECIAL)  //��ѯ����� 
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
��������: 	DecodeCCSMessage 
��������:  ����CCS��Ϣ���,Ҳ��ʱPCU��Ϣ���

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void DecodePcuMessage(INT8U *pu8DecodeBMSMesg, INT32U ID)
{
//	INT8U u8AdrCounter = 0;
//	INT8U u8ModbusRegAdrCnt = 0;

    memset(pu8DecodeBMSMesg,0xff,8);

	if(ID== PCU_PCU_LIFE)				//�������� 
	{
	    if(gUserDB.pcLinkStatus==1)   //��PC��������ʱ
		{
			pu8DecodeBMSMesg[0]= gUserDB.P_lifeCnt++;
			pu8DecodeBMSMesg[1]= gUserDB.P_failCode;
	    }
	}
	else if(ID== PCU_BMS_IDENTICATION)								//BMS��ʶ����
	{
		pu8DecodeBMSMesg[0]=0;                                   //Э��汾
		pu8DecodeBMSMesg[1]=1;
		pu8DecodeBMSMesg[2]=0;
		
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_total_Ah;
		pu8DecodeBMSMesg[4]=(INT32U)gUserDB.B_total_Ah>>8;
		pu8DecodeBMSMesg[5]=(INT32U)gUserDB.B_total_volt;       			//������ѹ���ֽ�
		pu8DecodeBMSMesg[6]=(INT32U)gUserDB.B_total_volt>>8;    			//������ѹ���ֽ�
	}		
	else if(ID== PCU_BMS_INFORMATION1) 			//BMS��Ϣ1����,PCUÿ5�뷢��һ��֡,
	{                                               //��PC�յ���֡����,�����б仯ʱ,PC�ᴥ����ѯBMS��Ϣ����
		memcpy(pu8DecodeBMSMesg,gUserDB.B_bmsID,8);
	}
	else if(ID== PCU_BMS_INFORMATION2) 			//BMS��Ϣ2����
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_batBoxSum;
		pu8DecodeBMSMesg[1]=gUserDB.B_serialSum;
		pu8DecodeBMSMesg[2]=gUserDB.B_parallelSum;
		pu8DecodeBMSMesg[3]=gUserDB.B_temTestSum;		

		memcpy(&pu8DecodeBMSMesg[5],"V15",3);
	}
	else if(ID== PCU_BMS_INFORMATION3) 			//BMS��Ϣ3����
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
	else if(ID== PCU_BMS_INFORMATION4) 			//BMS��Ϣ4����
	{
		pu8DecodeBMSMesg[0]=gUserDB.writeAt;
	   if(gUserDB.writeAt==1)
		{			
			pu8DecodeBMSMesg[1]=(INT32U)(gUserDB.B_permitModuleVoltH);        //PCU��ת�������ݽ���ת��
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
	else if(ID== PCU_BMS_LIFE) 					//BMS����֡���� 
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_lifeCnt++;	
		if(gUserDB.B_lifeCnt>=15)
			gUserDB.B_lifeCnt=0;
		
		pu8DecodeBMSMesg[1]=gUserDB.B_batStatus;	
		
	}
	else if(ID== PCU_BAT_REQIRE)					//��س�缶��������  
	{
		pu8DecodeBMSMesg[0]=(INT32U)gUserDB.B_batNeedVolt;	
		pu8DecodeBMSMesg[1]=(INT32U)gUserDB.B_batNeedVolt>>8;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_batNeedCurr;
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_batNeedCurr>>8;	
		pu8DecodeBMSMesg[4]=gUserDB.B_batChargeMode;	
		//pu8DecodeBMSMesg[5]=gUserDB.B_batStatus;	
		
	}
	else if(ID== PCU_BAT_STATE) 					//��س��״̬����
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
	else if(ID== PCU_BAT_STATE_LIMIT) 				//������״̬����
	{
		pu8DecodeBMSMesg[0]=(INT32U)gUserDB.B_singleVoltMin;	
		pu8DecodeBMSMesg[1]=(INT32U)gUserDB.B_singleVoltMin>>8;	
		pu8DecodeBMSMesg[2]=(INT32U)gUserDB.B_singleVoltMax;
		pu8DecodeBMSMesg[3]=(INT32U)gUserDB.B_singleVoltMax>>8;	
		
	}
	else if(ID== PCU_BAT_STOP) 				  //��س��ֹͣ����
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_bmsStopCause[0];	
		pu8DecodeBMSMesg[1]=gUserDB.B_bmsStopCause[1];	
		pu8DecodeBMSMesg[2]=gUserDB.B_bmsStopCause[2];	
		pu8DecodeBMSMesg[3]=gUserDB.B_bmsStopCause[3];	
	}
	else if(ID== PCU_BAT_COUNT) 				 //��س��ͳ�Ʊ��� 
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
	else if(ID== PCU_BAT_ERR) 					//BMS������ 
	{
		pu8DecodeBMSMesg[0]=gUserDB.B_recCcsIdentifyTimeout;	
		pu8DecodeBMSMesg[1]=gUserDB.B_recCcsMaxOutTimeout+(gUserDB.B_recCcsReadyTimeout<<1);	
		pu8DecodeBMSMesg[2]=gUserDB.B_recCcsStatusTimeout+(gUserDB.B_recCcsStopTimeout<<1)+(gUserDB.B_recCcsCountTimeout<<2);		
		pu8DecodeBMSMesg[3]=gUserDB.B_recCcsAuxVoltStatusTimeout;		
		
	}
	else if(ID== PCU_BAT_FAIL) 				//BMS���ϱ���
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_bmsFaultCode,6);	
	}
	else if(ID== PCU_BAT_FAIL2) 				//BMS���ϱ���2
	{
	}
	else if(ID== PCU_BAT_VOLT_N1) 				//��ص����ѹ���ݱ���1
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_singleVolt,6);
	}
	else if(ID== PCU_BAT_VOLT_N2) 				//��ص����ѹ���ݱ���2
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
	else if(ID== PCU_BAT_TEMP_N1) 				//��ص����¶����ݱ���1
	{
		memcpy(pu8DecodeBMSMesg,(INT8U*)&gUserDB.B_singleTemp[0],6);
	}
	else if(ID== PCU_BAT_TEMP_N2) 				//��ص����¶����ݱ���2
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
	else if(ID== PCU_BAT_ID1) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[0],8);	
	}
	else if(ID== PCU_BAT_ID2) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[1],8);	
	}
	else if(ID== PCU_BAT_ID3) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[2],8);	
	}
	else if(ID== PCU_BAT_ID4) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[3],8);	
	}
	else if(ID== PCU_BAT_ID5) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[4],8);	
	}
	else if(ID== PCU_BAT_ID6) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[5],8);	
	}
	else if(ID== PCU_BAT_ID7) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[6],8);	
	}
	else if(ID== PCU_BAT_ID8) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[7],8);	
	}
	else if(ID== PCU_BAT_ID9) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[8],8);	
	}
	else if(ID== PCU_BAT_ID10) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[9],8);	
	}
	else if(ID== PCU_BAT_ID11) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[10],8);	
	}
	else if(ID== PCU_BAT_ID12) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[11],8);	
	}
	else if(ID== PCU_BAT_ID13) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[12],8);	
	}
	else if(ID== PCU_BAT_ID14) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[13],8);	
	}
	else if(ID== PCU_BAT_ID15) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[14],8);	
	}
	else if(ID== PCU_BAT_ID16) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[15],8);	
	}
	else if(ID== PCU_BAT_ID17) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[16],8);	
	}
	else if(ID== PCU_BAT_ID18) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[17],8);	
	}
	else if(ID== PCU_BAT_ID19) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[18],8);	
	}
	else if(ID== PCU_BAT_ID20) 							//�����ID����
	{
		memcpy(pu8DecodeBMSMesg,gUserDB.B_batID[19],8);	
	}
	
	else if(ID== PCU_MCM_RETURN_SET_SINGLE_ALL) 		//��ȡ���е�����Ʋ���
	{
		pu8DecodeBMSMesg[0]= (INT32U)gUserDB.M_readSingleTotalSteps;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readSinglePermitChargeTemH;
		
	}
	else if(ID== PCU_MCM_RETURN_SET_STAGE_ALL) 		//��ȡ���н׶ο��Ʋ���
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
	else if(ID== PCU_MCM_RETURN_SET_SINGLE) 			//��ȡ������Ʋ���
	{
		pu8DecodeBMSMesg[0]= gUserDB.M_readSingleTotalSteps;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_readSinglePermitChargeVoltH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_readSinglePermitChargeTemH;
	}
	else if(ID== PCU_MCM_RETURN_SET_STAGE) 			//��ȡ�׶ο��Ʋ���
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
	else if(ID== PCU_MCM_STATE0) 						//����״̬֡1
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
	else if(ID== PCU_MCM_STATE1) 						//����״̬֡2
	{
		pu8DecodeBMSMesg[0]= (INT32U)gUserDB.M_thisPower;
		pu8DecodeBMSMesg[1]= (INT32U)gUserDB.M_thisPower>>8;
		pu8DecodeBMSMesg[2]= (INT32U)gUserDB.M_thisChargeAH;
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.M_thisChargeAH>>8;
		pu8DecodeBMSMesg[4]= (INT32U)gUserDB.M_thisChargePower;
		pu8DecodeBMSMesg[5]= (INT32U)gUserDB.M_thisChargePower>>8;
	}
	else if(ID== PCU_MCM_STATE2) 						//����״̬֡3
	{
	}
	else if(ID== PCU_MCM_STATE3) 						//����״̬֡4
	{
	}
	else if(ID== PCU_MCM_PDU_INFO) 					//�����Ϣ֡
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
	else if(ID== PCU_MCM_DEVICE_ID) 					//����豸��Ϣ֡
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
	else if(ID== PCU_CCS_IDENTIFY) 					//����ʶ����
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
	else if(ID== PCU_CCS_MAX_OUTPUT) 					//�����������������
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
	else if(ID== PCU_CCS_STATE) 						//���״̬����
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
	else if(ID== PCU_CCS_STOP) 						//�����ֹ����
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_CCS_StopCause[0];
		pu8DecodeBMSMesg[1]= gUserDB.C_CCS_StopCause[1];
		pu8DecodeBMSMesg[2]= gUserDB.C_CCS_StopCause[2];
		pu8DecodeBMSMesg[3]= gUserDB.C_CCS_StopCause[3];
	}
	else if(ID== PCU_CCS_COUNT) 						//���ͳ�Ʊ���
	{
		pu8DecodeBMSMesg[0]= (INT32U)(gUserDB.M_thisChargeAH);
		pu8DecodeBMSMesg[1]= (INT32U)(gUserDB.M_thisChargeAH)>>8;
		pu8DecodeBMSMesg[2]= (INT32U)(gUserDB.C_chargeTimeTotal);
		pu8DecodeBMSMesg[3]= (INT32U)gUserDB.C_chargeTimeTotal>>8;
		pu8DecodeBMSMesg[4]= (INT32U)(gUserDB.M_thisChargePower);
		pu8DecodeBMSMesg[5]= (INT32U)(gUserDB.M_thisChargePower)>>8;
		pu8DecodeBMSMesg[6]= gUserDB.C_ccs_id;
		
	}
	else if(ID== PCU_CCS_ERR) 							//��������
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_recBmsIdentifyTimeout+(gUserDB.C_recBatInfoTimeout<<1);
		pu8DecodeBMSMesg[1]= gUserDB.C_recBmsReadyTimeout;
		pu8DecodeBMSMesg[2]= gUserDB.C_recBatchargeLevelTimeout+(gUserDB.C_recBatStatusTimeout<<1)+(gUserDB.C_recBmsStopTimeout<<2);
		pu8DecodeBMSMesg[3]= gUserDB.C_recBmsStatisticsTimeOut;
	
	}
	else if(ID== PCU_CCS_FAULT) 						//�����ϱ���
	{
		pu8DecodeBMSMesg[0]= gUserDB.C_CCSFaultCode;
	}

	
}//the end


/*********************************************************************************************************
��������: 	can_PgnPackCheck 
��������:    ��֤CAN�����Ƿ�Ϊ�����Ϸ���PGN

�䡡��:      adr   Ŀ���ַ
			    pgn     ��������

 �䡡��:    0		ʧ��
			>0		�ɹ�						     
*********************************************************************************************************/
INT8U can_PgnPackCheck(INT8U adr,INT16U pgn)
{
	INT8U ret=0;

	if(adr == CCS_ADR)				// �Ƿ�Ϊ������ַ?
	{
		switch(pgn)
		{
		case 0x26:                   //DC/DC mes
//		case BMS_BCP1_PGN:			//���س�����
//		case BMS_CBP1_PGN:			//�������ز���1
//		case BMS_CBP2_PGN:			//�������ز���2
//		case BMS_BCS_PGN:			//���س��״̬
//		case BMS_CTS_PGN:			//ʱ��ͬ����Ϣ
//		case BMS_MVD_PGN:			//����ģ���ѹ����
//		case BMS_BTD_PGN:			//�����¶�����
//		case BMS_SOC_PGN:			//����SOC����
//		case BMS_AVD_PGN:			//����ƽ����ѹ����
//		case BMS_BBI1_PGN:			//���ػ�����Ϣ1
//		case BMS_BBI2_PGN:			//���ػ�����Ϣ2
//		case BMS_BBI_PGN:			//���ػ�����Ϣ
//		case CCS_CLF_PGN:			//����������Ϣ֡
//		case CCS_CCS1_PGN:			//����״̬1
//		case CCS_CCS2_PGN:			//����״̬2
//		case CCS_CCP_PGN:			//��������
//		case CCS_EST_PGN:			//��������ֹͣ����
//		case CCS_CST_PGN:			//���������������
//		case CCS_CSP_PGN:			//����ֹͣ�������		 
		
			ret=1; 					//PGN�����Ϸ���־		
			break;
		default:
			ret=0;					//PGN�����Ƿ���־
			break;
		}
	}
	else if(adr == BROADCAST_ADR)   //�Ƿ�Ϊ�㲥��ַ��
	{
		switch(pgn)
		{
		case BMS_BBI1_PGN:			//���ػ�����Ϣ1
		case BMS_BBI2_PGN:			//���ػ�����Ϣ2
		case BMS_BBI_PGN:			//���ػ�����Ϣ
			ret=1;
			break;
		default:
			ret=0; 					//PGN�����Ƿ���־
			break;
		} 		 					
	}
	else
		ret=0;					   //PGN�����Ƿ���־

	return ret;
}






