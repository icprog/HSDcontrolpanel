/*********************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************/
#include "Global.h"
#include "../mod/moduleCom/moduleCom.h"

/*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************/
#define  MINITUE_3_TICK                 120

/*********************************************************************************************************
*                                            ������������������
*********************************************************************************************************/	


/*********************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************/
extern USER_DB gUserDB;
void alarmTest( void );
void pwmVoltSetEnable(void);
void pwmCurrSetEnable(void);
void ledShow(void);
void chargeManage(void);

INT16U gSetVoltage=0;										//���õ�ѹֵ
INT16U gSetCurrent=0;										//���õ���ֵ
extern void EE_WriteData(INT8U adr,INT8U *buf);
void moduleComManage(void);
extern void BMS_Power_ctl(INT8U onoff);
extern void BMS_relay_ctl(INT8U onoff);
void setCcsVoltCurr(void);
extern void fans_ctl(INT8U onoff);
extern ReadCardDataStr ReadCardData;
CHARGE_LOG_INFO currChargeLog;
void beep_alarm_on(void );
/*********************************************************************************************************************
��������: 	bmsDataInfoDeal          
��������: 	BMS��Ϣ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void bmsDataInfoDeal(void)
{
	

	

}

/*********************************************************************************************************************
��������: 	setCcsVoltCurr          
��������: 	���ó�����ѹ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void setCcsVoltCurr(void)
{
    
}

/*********************************************************************************************************************
��������: 	moduleCurrReset          
��������: 	ģ��������·���������ģʽ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void moduleVoltCurrSet(FP32 setOutCurr)
{
   
}

/*********************************************************************************************************************
��������: 	chargeFullStrege          
��������: 	���������
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void chargeFullStrege(void)
{
//	gUserDB.CCS_fullFlag=1;                           //������־
	gUserDB.manualRunCtlFlag=STOP;	                  //����ʱ�����ֶ�������־
//	gUserDB.MCT_switch =STOP;		                      //����ʱ�����Զ�������־
//	gUserDB.BMS_setStartFlag =1;	        //������BMS���ӱ�־	
}

/*********************************************************************************************************************
��������: 	chargeManage          
��������: 	��������
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void chargeManage(void)
{	
	static INT16U 	delayCnt=0 ;
	
	
	  gUserDB.C_outVolt=pdu_info.A_volt;
	  gUserDB.C_outCurr=pdu_info.A_curr;
	  gUserDB.M_meterDegree=pdu_info.power_total;
	
	if(gUserDB.CCS_runCtlFlag==RUN && gUserDB.chargeFull ==0)	                           //����
	  {
			
			if(gUserDB.CCS_chargeRunStatus !=RUN)
		  {
		     gUserDB.charge_temeEn=1;
		     gUserDB.CCS_chargeRunStatus =RUN;                  //����״̬ 
             OUT1_ON ;	
             OUT2_ON ;	
		   }

		   if(  gUserDB.M_thisChargeTime  <10)
		  {
		     gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //���γ��ʱ��
		     gUserDB.M_thisChargeDegree=0;                 //���γ�����
             
             gUserDB.M_thisChargeMoney=0;                   //���γ����	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //���γ�����
		   }
		   else
		   {
		    gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //���γ��ʱ��
            gUserDB.M_thisChargeDegree=(gUserDB.M_meterDegree-gUserDB.M_startDegree);                 //���γ�����                                                           
            gUserDB.M_thisChargeMoney=(gUserDB.M_meterDegree-gUserDB.M_startDegree)*1;                //���γ����
		  }
		if( gUserDB.C_outCurr < 1 &&  gUserDB.M_thisChargeTime  >180)	
		{
		    delayCnt++;
			if ( delayCnt  >10 )
			{
			  delayCnt =10; 
			  gUserDB.chargeFull =1; 
			  gUserDB.CCS_runCtlFlag=STOP ;
		    }	
		 }
		 else
		 {
		    
		    delayCnt =0;	
		  }
       		
	  }
	  else                                                     //ͣ��״̬
	  {
			gUserDB.charge_temeEn=0;
			 gUserDB.CCS_chargeRunStatus =STOP;                  //����״̬ 
			 OUT1_OFF ;
			 OUT2_OFF ;
			 //OUT2_OFF ;	     
	  }


	  if ( gUserDB.m485comLinkStatus==ALARM || gUserDB.DcPduComAlarm == ALARM )
	  {
		 OUT3_ON ;
		 beep_alarm_on();
		 gUserDB.CCS_runCtlFlag =STOP;
		 
	  }
	  else
	  {
	    if ( gUserDB.cardBeepEn==0 )
		 {
		   BEEP_OFF;
		 }
		 OUT3_OFF;
	  }
}
/*********************************************************************************************************************
��������: 	alrm_Log_Test          
��������: 	�澯��¼���
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void beep_open(void )
{
//	gUserDB.card_beep_en=1;
   gUserDB.cardBeepEn=1;
  	BEEP_ON;
	OSTimeDlyHMSM(0,0,0,300);  
	 BEEP_OFF;	
    gUserDB.cardBeepEn=0;
}

/*********************************************************************************************************************
��������: 	alrm_Log_Test          
��������: 	�澯��¼���
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void beep_alarm_on(void )
{
  static INT16U	cnt=0 ;

	 cnt++;
	if (cnt<2 )
  	   BEEP_ON;
	else if (cnt<6 ) 
	   BEEP_OFF;
	else
	   cnt=0 ;

//	gUserDB.card_beep_en=0;
}

/*********************************************************************************************************************
��������: 	userIndicating_execute          
��������: 	�û�����ִ�� , gUserDB.user_indicating ���� 
            ���ֽڣ�0 ��ʾ�����·�     1��ʾ��������ִ��  2��ʾ����ִ�н���
            ���ֽڣ���ʾ��������

�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void charge_searchCard(void)
{
	
			  switch(gUserDB.cardReader_indicating) //������״̬
			  {
				case CARD_CMD_S_SEARCH_END: //Ѱ������
					{		
            gUserDB.userCardState=0; 
						
						if(ReadCardData.CardStatus.Bit.AdvanceHandle==0 
							&& ReadCardData.CardStatus.Bit.SystemCard==0 
						&& ReadCardData.CardStatus.Bit.SelectCard==0
						&& ReadCardData.CardStatus.Bit.ResetUserCard==0
						&& ReadCardData.CardStatus.Bit.ResetPSAM==0) //Ԥ����ɹ�,��ϵͳ�п�������Ϊϵͳ�� 
					  {
						
							memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //����							  
							currChargeLog.purseBalance=ReadCardData.RemainMoney/100.0;                  //�������
							gUserDB.leftMoney= ReadCardData.RemainMoney;
					//	gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //���ˢ����֤����	
						if(ReadCardData.CardStatus.Bit.BusinessStatus==2) //�ж��Ƿ�Ϊ�쳣��,����1��Ϊ���ǰ���쳣��,������
		        {
			           gUserDB.card_cmd_err=0;	                //�쳣ˢ��
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //���γ�����
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //���γ��ʱ��
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //���γ����	
                 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;              //���佻��ˢ��							
                 					
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==1) //�ж��Ƿ�Ϊ�쳣��,����1��Ϊ���ǰ���쳣�����ҿ�
		        {
			           //gUserDB.card_cmd_err=1;	                //�쳣ˢ��  ���ڳ������ã�����ȡ��ˢ������
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //���γ�����
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //���γ��ʱ��
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //���γ����	
                // gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;                //����ˢ��									
                 gUserDB.user_CMD_timeout=0;
						     gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ
						     gUserDB.user_CMD_return_status=1;     //�ɹ�ˢ��״̬		
                 beep_open();							
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==0) //������
					  {
							gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;  //���ˢ��
							gUserDB.card_cmd_err=0;								
					  }						
							
//						gUserDB.user_CMD_timeout=0;
//						gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ
//						gUserDB.user_CMD_return_status=0;     //�ɹ�ˢ��״̬
					 }
					 else  //ϵͳ�޿�
					 {
						 
//						 if(ReadCardData.CardStatus.Bit.SystemCard==1 && ReadCardData.CardStatus.Bit.SelectCard==0) //ϵͳ�п�������Ϊ��ϵͳ�� 
//						 {
//							 gUserDB.card_cmd_err=0;	
//							 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ
//							 gUserDB.user_CMD_return_status=2;     //ʧ��״̬
//						 }
						 
//						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
//						 {
							 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //����Ѱ��
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //��ʱѰ��
//						 {
//						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ
//							 gUserDB.user_CMD_return_status=2;     //ʧ��״̬
//						 }
					 }
				}					
					break;				
			 case CARD_CMD_S_IDENT_END://���ǰˢ����֤����
			 {
				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //���ӵ�ɹ�
					 {
						 
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ�����
						 gUserDB.user_CMD_return_status=1;     //�óɹ�״̬
						 gUserDB.user_CMD_timeout=0;
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ	
             gUserDB.M_thisChargeDegree=0;                 //���γ�����
             gUserDB.M_thisChargeTime=0;                     //���γ��ʱ��
             gUserDB.M_thisChargeMoney=0;                   //���γ����	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //���γ�����						 
							beep_open();
						 //gUserDB.user_CMD_return_status=0;     //�ɹ�ˢ��״̬
					 }
					 
				   
					else  
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
						 }
						 else    //��ʱѰ��
						 {
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //���ˢ����ɣ�Ѱ����ʱ
							 gUserDB.user_CMD_return_status=2;     //��ʧ��״̬
							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //���佻������	���������20140928
						 }						 
					 }	
				 }					 
				  break;	
				 case CARD_CMD_E_IDENT_END://������ˢ����֤����
				 {
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //�������ӵ�ɹ�
					 {
						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ�����
						 gUserDB.cardReader_indicating= CARD_CMD_S_IDENT_END;
						 gUserDB.user_CMD_ing = USER_CMD_SEARCH_CARD_END;          //��������ɱ�־
						 gUserDB.user_CMD_return_status=1;                      //�óɹ�״̬
						 currChargeLog.trade_flag=2;                            //�ó�罻����ɱ�־		
             //gUserDB.card_cmd_err=1;	                //�쳣ˢ��	
            
            beep_open();						 
					 }
           else if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
					{
							 gUserDB.user_CMD_timeout++;
					}
					else    //��ʱѰ��
					{
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //���ˢ����ɣ�Ѱ����ʱ
							 gUserDB.user_CMD_return_status=2;     //��ʧ��״̬
							 //gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //���佻������	���������20140928
					}							 
				 }					 
				  break;         				 
			}	
}

void discharge_endCard(void)
{
	INT8U cardNo_err=0;
	static INT8U re_cnt=0,firstFlag=0;
	
	switch(gUserDB.cardReader_indicating) //������״̬
			{
				case CARD_CMD_S_SEARCH_END: //������Ѱ�����
					{	
            cardNo_err=0;           //��ʼ��Ϊ������ͬ
//            for(INT8U i=0;i<16;i++)  //���ŶԱȣ��Ƿ�Ϊͬһ����,��ֻ��һ�Ų��Կ�,����ֻ��������
//            {
//							if(currChargeLog.pay_cardID[i] !=ReadCardData.CardNo[i])
//							{
//								cardNo_err=1;      //���Ų���ͬ
//								gUserDB.userCardState=1;
//								gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //���ˢ����֤����	
//								break;
//							}
//            }	
						
//						if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
//							&& ReadCardData.CardStatus.Bit.BusinessStatus==1 && cardNo_err==0) //ϵͳ�п�������Ϊϵͳ��
							if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus==1&&  cardNo_err==0
							&& ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 	) //Ԥ����ɹ�,��ϵͳ�п�������Ϊϵͳ��
					  {						
							 //memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //����							  
						  
						    gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //���ˢ����֤����	
						    gUserDB.user_CMD_timeout=0;
							  gUserDB.card_cmd_err=0;						  				
					 }
					 else  //ϵͳ�޿�
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
							 
							 if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus ==0 &&  cardNo_err==0
							 && ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 ) //Ԥ����ɹ�,��ϵͳ�п�������Ϊϵͳ��
					    {
								  gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;         //�ӵ�
                  // gUserDB.user_indicating=USER_CMD_END_CARD_END;         //ˢ�����
						       gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //��������ɱ�־	
                    firstFlag=0 ;	                  							
							}
							else
							{							
							   gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //����Ѱ��
							}
						 }
						 else    //��ʱѰ��
						 {
							 if(ReadCardData.CardStatus.Bit.BusinessStatus !=1) //�ж��Ƿ�Ϊ�쳣��,����1��Ϊ���ǰ���쳣��
		           {
			           //gUserDB.card_cmd_err=1;			           
		           }							
						   gUserDB.user_indicating=USER_CMD_END_CARD_END;         //������ˢ����ɣ�Ѱ����ʱ
							 gUserDB.user_CMD_return_status=2;     //ʧ��״̬
						 }
					 }
				}					
					break;
				case CARD_CMD_S_IDENT_END://���ǰˢ����֤����
			 {
				 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //����Ѱ��
//				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //���ӵ�ɹ�
//					 {
//						 
//						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ�����
//						 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //����Ѱ��
////						 gUserDB.user_CMD_timeout=0;
////						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //ˢ����ɣ�Ѱ����ʱ						 
////							beep_open();
//						 //gUserDB.user_CMD_return_status=0;     //�ɹ�ˢ��״̬
//					 }					 
//				   
//					else  
//					 {
//						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
//						 {
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //��ʱѰ��
//						 {
//						   //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //���ˢ����ɣ�Ѱ����ʱ
//							 //gUserDB.user_CMD_return_status=2;     //��ʧ��״̬
//							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //���佻������	���������20140928
//						 }						 
//					 }	
				 }					 
				  break;
			 case CARD_CMD_E_IDENT_END://������ˢ����֤����
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //�������ӵ�ɹ�
					 {
						 gUserDB.user_indicating=USER_CMD_END_CARD_END;         //ˢ�����
						 gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //��������ɱ�־
						 gUserDB.user_CMD_return_status=1;                      //�óɹ�״̬
						 currChargeLog.trade_flag=2;                            //�ó�罻����ɱ�־	
             gUserDB.userRunRequestFlag=STOP;          //�û���������						 
             beep_open();						 
					 }				   
					else  //��������3���Ժ���֤ʧ��
					 {						 
						   gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //���ˢ����֤����					 						 
					 }						
				  break;								
			}
}
/*********************************************************************************************************************
��������: 	userIndicating_execute          
��������: 	�û�����ִ�� , gUserDB.user_indicating ���� 
            ���ֽڣ�0 ��ʾ�����·�     1��ʾ��������ִ��  2��ʾ����ִ�н���
            ���ֽڣ���ʾ��������

�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void userIndicating_execute(void)
{	
	
	INT32U Addr=0;
	
	switch(gUserDB.user_indicating)
	{
		case USER_CMD_WAIT:  //����
		{
//			 gUserDB.CCS_chargeRunStatus=SYS_WAIT;       //���״̬--����	   
//        
//			
//		   if(gUserDB.EVD_systemAlarm !=ALARM)
//			 {  
//				 //gUserDB.EVD_systemStatus= SYS_WAIT;                  //������״̬				 
//			 				 
//			 }						
		 }
			break;
		case USER_CMD_SEARCH_CARD_REQUEST:  //Ѱ������
		{
			gUserDB.user_indicating=USER_CMD_SEARCH_CARD_ING;        //����ִ��
		  gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //Ѱ��
		  
		  gUserDB.user_CMD_timeout=0;          //��ʱ����
		  
		 // re_cnt=0;
			
		  break;
		}
		case USER_CMD_SEARCH_CARD_ING:  //����ִ��Ѱ��		
		{	
			charge_searchCard();
		}
     break;		
		case USER_CMD_SEARCH_CARD_END:  //���ǰѰ������
			//gUserDB.user_indicating=0x11;        //����ִ��
		  //gUserDB.cardReader_indicating=0x01;  //�
      gUserDB.user_indicating=USER_CMD_CHARGE_REQUEST;
			    
			break;
		case USER_CMD_CHARGE_REQUEST:                         //�����������
		{			  
		    //re_cnt=0;
		    //osDelay(100); 			
		  // if( firstFlag==0)
		   {
				 //firstFlag=1;			   	
			   gUserDB.user_CMD_timeout=0;          //��ʱ����
		     gUserDB.user_CMD_return_status=0;    //��Ч״̬
//			   gUserDB.CCS_fullFlag=0;              //��δ������־				 
				gUserDB.M_thisChargeDegree=0;                 //���γ�����
             gUserDB.M_thisChargeTime=0;                     //���γ��ʱ��
             gUserDB.M_thisChargeMoney=0;                   //���γ����	
              gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //���γ�����	 				 
				 gUserDB.M_thisChargeDegree=0;                                  //���γ���������
				 gUserDB.M_thisChargeTime=0;                                    //���γ��ʱ������
				 gUserDB.M_thisChargeTime_en=0;                                 //����ʱʹ��
				 gUserDB.M_thisChargeMoney=0;                                   //���γ��������
				 gUserDB.userRunRequestFlag=RUN; 
		   }	
       
			 
       gUserDB.user_indicating=USER_CMD_CHARGE_ING;        //����ִ��
			 gUserDB.user_CMD_ing = USER_CMD_CHARGE_ING;	
       gUserDB.userRunRequestFlag=RUN;          //�û���������	
			   
		} 
			break;
		case USER_CMD_CHARGE_ING:  //���ڳ����....
		{
		}
			break;
		case USER_CMD_END_CARD_REQUEST:                              //������ˢ������
		{
			ReadCardData.ExpenseMoney=gUserDB.M_thisChargeMoney*100;       //�����
			gUserDB.user_indicating=USER_CMD_END_CARD_ING;            //����ִ��
			
			//if((gUserDB.user_CMD_ing & 0x0f) != 0x03)   //�жϽ����ӵ������Ƿ�����ִ�У�����ִ��ʱ�����޶���
			{
		     gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //Ѱ��
				 gUserDB.user_CMD_ing = USER_CMD_END_CARD_ING;
			}			
		 
		  gUserDB.user_CMD_timeout=0;          //��ʱ����
		  gUserDB.user_CMD_return_status=0;    //��Ч״̬
	//	  re_cnt=0;
		  
	//	 gUserDB.CCS_chargeEnable=0;		         //�����澯����ֹ���
		
			break;
		}
		case USER_CMD_END_CARD_ING://������ˢ������ִ��
		{
			  discharge_endCard();
			break;
		} 
	 
	}	
}

 /*********************************************************************************************************************
��������: 	task_canProcess          
��������: 	�������߳�
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void task_powerManage(void *pdata)			//MODBUS����
{    
   static INT8U relay_flag=0xff;
   OUT2_ON ;	
   OSTimeDlyHMSM(0,0,3,0);				   //2S��ʱ  
   //BMS_Power_ctl(ON);                      //BMS��Դ�ϵ�	 
   //fans_ctl(OFF);

  while(1)
  {
//  	 if(gUserDB.insulationAlarm==ALARM  /*|| gUserDB.C_outVolt<200 */ ||  gUserDB.inputOverVoltAlrm==ALARM || gUserDB.inputLowVoltAlrm==ALARM ||
//	    gUserDB.CCS_outOverVoltAlarm==ALARM  || gUserDB.CCS_outOverCurrAlarm==ALARM 
//      || (gUserDB.bms_readyStatus==OK  && gUserDB.cellBatVoltMax < gUserDB.cellOverProtect 
//		     && gUserDB.BatTempMAx > gUserDB.BatTempProtect) || gUserDB.m485comLinkStatus==ALARM		 ) 	   //��Ե���澯,�����ѹ,����Ƿѹ,�����ѹ,���������ʱ,������ֹ���
//	   
//		{				
//			 gUserDB.CCS_chargeEnable=NOK;		     					//��ֹ���
//			 gUserDB.C_CCSFaultCode=1;
//		}
//		else
//		{			
//			 gUserDB.CCS_chargeEnable=OK;		     					//������			
//			 gUserDB.C_CCSFaultCode=2;		
//		}


	  
	 chargeManage();                       //������
	 userIndicating_execute();
	  
	  OSTimeDlyHMSM(0,0,0,100);				//1S����һ��  	   
  }

}


/*********************************************************************************************************************
��������: 	adjCcsOutVoltCurr          
��������: 	���������������
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void adjCcsOutVoltCurr(void)
{

}


/*********************************************************************************************************************
��������: 	BMS_Power_ctl          
��������: 	BMS��Դ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void BMS_Power_ctl(INT8U onoff)
{

//   if(onoff==1)
//   {
//     	OUT1_ON;
//     	OSTimeDlyHMSM(0,0,0,200);
//   }
//   else
//   {
//   		OUT1_OFF;
//     	OSTimeDlyHMSM(0,0,0,200);
//   }

}


/*********************************************************************************************************************
��������: 	setCcsVoltCurr          
��������: 	���ó�����ѹ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void BMS_relay_ctl(INT8U onoff)
{
//  static INT8U flag=0xff;
//
//  if(onoff !=flag)
//  {
//  	 flag =onoff;
//  }
//  else
//  {
//  	return ;
//  }
//
//  if(flag==1)
//  {
//     OUT1_ON;
//     OSTimeDlyHMSM(0,0,0,200);
//     OUT2_ON;
//     OSTimeDlyHMSM(0,0,0,200);
//	 OUT3_ON;
//     OSTimeDlyHMSM(0,0,0,200);
//	 OUT4_ON;
//     OSTimeDlyHMSM(0,0,0,200);	 
//	 OUT5_ON;
//     OSTimeDlyHMSM(0,0,0,200);
////	 OUT6_ON;
////     OSTimeDlyHMSM(0,0,0,200);
////	 OUT7_ON;
////     OSTimeDlyHMSM(0,0,0,200);
//	 OUT8_ON;
//     OSTimeDlyHMSM(0,0,0,200);
//
//  }
//  else
//  {
//     OUT1_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
//     OUT2_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
//	 OUT3_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
//	 OUT4_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
//	 
//	 OUT5_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
////	 OUT6_OFF;
////     OSTimeDlyHMSM(0,0,0,200);
////	 OUT7_OFF;
////     OSTimeDlyHMSM(0,0,0,200);
//	 OUT8_OFF;
//     OSTimeDlyHMSM(0,0,0,200);
//  }


}

/*********************************************************************************************************************
��������: 	fans_ctl          
��������: 	���ó�����ѹ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void fans_ctl(INT8U onoff)
{
//  static INT8U flag=0xff;
//
//  if(onoff !=flag)
//  {
//  	 flag =onoff;
//  }
//  else
//  {
//  	return ;
//  }
//
//  if(flag==1)
//  {
//	OUT4_OFF;
//     OSTimeDlyHMSM(0,0,0,100); 
//   OUT5_ON;
//     OSTimeDlyHMSM(0,0,0,100);  
//	 OUT6_ON;
//     OSTimeDlyHMSM(0,0,0,100);
//	 OUT7_ON;
//     OSTimeDlyHMSM(0,0,0,100);
//	OUT8_ON;
//     OSTimeDlyHMSM(0,0,0,100);
//				   
//  }
//  else
//  { 
//	OUT4_ON;
//     OSTimeDlyHMSM(0,0,0,100); 
//   OUT5_OFF;
//     OSTimeDlyHMSM(0,0,0,100);    
//	 OUT6_OFF;
//     OSTimeDlyHMSM(0,0,0,100);
//	 OUT7_OFF;
//     OSTimeDlyHMSM(0,0,0,100);	
//   OUT8_OFF;
//     OSTimeDlyHMSM(0,0,0,100);		
//  }


}

/*********************************************************************************************************************
��������: 	alarmTest          
��������: 	���ϼ��
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void alarmTest(void)
{
    static INT8U errCnt1=0,errCnt2=0,errCnt3=0,errCnt4=0,errCnt5=0,errCnt6=0,errCnt7=0;
    static INT8U noErrCnt1=0,noErrCnt2=0,noErrCnt3=0,noErrCnt4=0,noErrCnt5=0,noErrCnt6=0,noErrCnt7=0;	

/*		if(	gUserDB.inputLowVoltAlrm==NOALARM)	   											//����Ƿѹ���
		{
		    noErrCnt1=0;
			if( gAcPduInfo.A_volt<302 || gAcPduInfo.B_volt<302 || gAcPduInfo.C_volt<302 )  	
			{
				
		 		errCnt1=(errCnt1++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt1;			
				if( errCnt1>=ERR_MAX_CNT)
				{						
					gUserDB.inputLowVoltAlrm=ALARM;	                                       //����Ƿѹ�澯����
				}			
			}
		}
		else  
		{
		    errCnt1=0;
			if( gAcPduInfo.A_volt>322 && gAcPduInfo.B_volt>322 && gAcPduInfo.C_volt>322 )
			{				
				noErrCnt1=(noErrCnt1++> ERR_MAX_CNT)?(ERR_MAX_CNT):noErrCnt1;
				if( noErrCnt1>=ERR_MAX_CNT)
				{ 						
					gUserDB.inputLowVoltAlrm=NOALARM;	                                  //����Ƿѹ�澯�ָ�
				}
			}
		}

		if(	gUserDB.inputOverVoltAlrm==NOALARM)		                                       //�����ѹ���
		{
		    noErrCnt2=0;
			if( gAcPduInfo.A_volt>440 || gAcPduInfo.B_volt>440 || gAcPduInfo.C_volt>440 )  	
			{
				
		 		errCnt2=(errCnt2++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt2;			
				if( errCnt2>=ERR_MAX_CNT)
				{						
					gUserDB.inputOverVoltAlrm=ALARM;	                                    //�����ѹ�澯����
				}			
			}
		}
		else  
		{
		    errCnt2=0;
			if( gAcPduInfo.A_volt<420 && gAcPduInfo.B_volt<420 && gAcPduInfo.C_volt<420 )
			{				
				noErrCnt2=(noErrCnt2++> ERR_MAX_CNT)?(ERR_MAX_CNT):noErrCnt2;
				if( noErrCnt2>=ERR_MAX_CNT)
				{ 						
					gUserDB.inputOverVoltAlrm=NOALARM;	                                   //�����ѹ�澯�ָ�
				}
			}
		}

  */
		if(	gUserDB.CCS_outOverVoltAlarm==NOALARM)	   											    //�����ѹ���
		{
		    noErrCnt3=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outVolt >gUserDB.CCS_setOutVolt+10 && gUserDB.C_outVolt>200  )  	//��������
			{				
		 		errCnt3=(errCnt3++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt3;			
				if( errCnt3>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverVoltAlarm=ALARM;	                                             //�����ѹ�澯����
				}			
			}
		}
		else  
		{
		    errCnt3=0;
			if( gUserDB.C_outVolt <gUserDB.CCS_setOutVolt-10 )
			{				
				noErrCnt3=(noErrCnt3++> ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):noErrCnt3;
				if( noErrCnt3>=ERR_OUT_VOLTCURR_CNT)
				{ 						
					gUserDB.CCS_outOverVoltAlarm=NOALARM;	                                          //�����ѹ�澯�ָ�
				}
			}
		}
			
		
		if(	gUserDB.CCS_outOverCurrAlarm==NOALARM)	   											    //����������
		{
		    noErrCnt4=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outCurr >gUserDB.CCS_setOutCurr+5  )  	//��������
			{				
		 		errCnt4=(errCnt4++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt4;			
				if( errCnt4>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverCurrAlarm=ALARM;	                                             //��������澯����
				}			
			}
		}
		else  
		{
		    errCnt4=0;
			if( gUserDB.C_outCurr <gUserDB.CCS_setOutCurr-5 )
			{				
				noErrCnt4=(noErrCnt4++> ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):noErrCnt4;
				if( noErrCnt4>=ERR_OUT_VOLTCURR_CNT)
				{ 						
					gUserDB.CCS_outOverCurrAlarm=NOALARM;	                                          //��������澯�ָ�
				}
			}
		} 
		
/*		
		if(	gUserDB.M_carLinkState==NOALARM)	   											    //���ǹ���Ӽ��
		{
		    noErrCnt5=0;
			if( gUserDB.car_link_volt < 2 || gUserDB.car_link_volt > 4.2  )  	 
			{				
		 		errCnt5=(errCnt5++> 3)?(3):errCnt5;			
				if( errCnt5>=3)
				{						
					gUserDB.M_carLinkState=ALARM;	                                             //���ǹ���Ӹ澯����
					if(gUserDB.CCS_chargeRunStatus ==RUN)                                        //ϵͳ�������г��������쳣����Ͽ��Ӵ���
					{   
					       OUT4_ON;
						   gUserDB.car_linkAlarm=ALARM;
					}
				}			
			}
		}
		else  
		{
		    errCnt5=0;
			if( gUserDB.car_link_volt >= 2 && gUserDB.car_link_volt <= 4.2  )
			{				
				noErrCnt5=(noErrCnt5++> 3)?(3):noErrCnt5;
				if( noErrCnt5>=3)
				{ 						
					//gUserDB.M_carLinkState=ALARM	                                             //���ǹ���Ӹ澯�ָ�
					if(gUserDB.car_linkAlarm !=ALARM)                                            //�����쳣�Ͽ��󣬲����Զ����ϼ̵�������������ϵͳ
					   OUT4_OFF;
				}
			}
		}		
		
*/		

	return;
}

/*********************************************************************************************************************
��������: 	pwmSetEnable          
��������: 	
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void ledShow(void)
{
  static INT8U flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0;
  static INT8U runLedFlag=0;

 
  	if(flag1==0)   //ϵͳ����LED1
	{
		flag1=1;
		LED1_OFF;
	}
	else
	{
		flag1=0;
		LED1_ON;
	} 


  if(gUserDB.m485comLinkStatus==ALARM)       //CCU����̬��ͨ��LED
  {
  	LED2_OFF;
  }
  else
  {
  	if(flag2==0)
	{
		flag2=1;
		LED2_OFF;
	}
	else
	{
		flag2=0;
		LED2_ON;
	}
  }
//
//  
//
//  if(gUserDB.ILB_comAlarm==ALARM)       //CCU���Ե����ͨ��״̬LED
//  {
//  	LED4_OFF;
//  }
//  else
//  {
//  	if(flag4==0)
//	{
//		flag4=1;
//		LED4_OFF;
//	}
//	else
//	{
//		flag4=0;
//		LED4_ON;
//	}
//  }
//
//
//  if(gUserDB.DcPduComAlarm==ALARM)       //CCU��ֱ������ͨ��״̬LED
//  {
//  	LED5_OFF;
//  }
//  else
//  {
//  	if(flag5==0)
//	{
//		flag5=1;
//		LED5_OFF;
//	}
//	else
//	{
//		flag5=0;
//		LED5_ON;
//	}
//  }
//
//
//  if(gUserDB.bms_linkStatus==ALARM)       //BMS����״̬LED
//  {
//  	LED8_OFF;
//  }
//  else
//  {
//  	if(flag8==0)
//	{
//		flag8=1;
//		LED8_OFF;
//	}
//	else
//	{
//		flag8=0;
//		LED8_ON;
//	}
//  }
//
//  if(gUserDB.CCS_chargeRunStatus==RUN)
//  {
//  	OUT1_OFF;  //��
//    OUT2_OFF;   //��
//	//OUT3_ON;   //��
//  //  OUT4_ON;  //��
//  }
//  else
//  {
//  	OUT1_ON;
//	  OUT2_OFF;  //��
////	OUT3_ON; //��
////	OUT4_ON; //��
//  }
//	
//	if(gUserDB.C_CCSFaultCode==1)  //ϵͳ����
//	{
//		 OUT3_OFF;  //��
//	}
//	else
//	{
//			OUT3_ON; //��
//	}
	
}

