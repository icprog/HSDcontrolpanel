/*********************************************************************************************************
*                                            Í·ÎÄ¼þ°üº¬
*********************************************************************************************************/
#include "Global.h"
#include "../mod/moduleCom/moduleCom.h"

/*********************************************************************************************************
*                                            ºê¶¨Òå
*********************************************************************************************************/
#define  MINITUE_3_TICK                 120

/*********************************************************************************************************
*                                            º¯Êý»òÆäËüµ÷ÓÃÉùÃ÷
*********************************************************************************************************/	


/*********************************************************************************************************
*                                            È«¾Ö±äÁ¿¶¨Òå
*********************************************************************************************************/
extern USER_DB gUserDB;
void alarmTest( void );
void pwmVoltSetEnable(void);
void pwmCurrSetEnable(void);
void ledShow(void);
void chargeManage(void);

INT16U gSetVoltage=0;										//ÉèÖÃµçÑ¹Öµ
INT16U gSetCurrent=0;										//ÉèÖÃµçÁ÷Öµ
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
º¯ÊýÃû³Æ: 	bmsDataInfoDeal          
¹¦ÄÜÃèÊö: 	BMSÐÅÏ¢´¦Àí
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void bmsDataInfoDeal(void)
{
	

	

}

/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	setCcsVoltCurr          
¹¦ÄÜÃèÊö: 	ÉèÖÃ³äµç»úµçÑ¹µçÁ÷
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void setCcsVoltCurr(void)
{
    
}

/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	moduleCurrReset          
¹¦ÄÜÃèÊö: 	Ä£¿éµçÁ÷ÖØÐÂ·ÖÅäÓëÆô¶¯Ä£Ê½²ßÂÔ
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void moduleVoltCurrSet(FP32 setOutCurr)
{
   
}

/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	chargeFullStrege          
¹¦ÄÜÃèÊö: 	³äÂú×ã²ßÂÔ
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void chargeFullStrege(void)
{
//	gUserDB.CCS_fullFlag=1;                           //³äÂú±êÖ¾
	gUserDB.manualRunCtlFlag=STOP;	                  //³äÂúÊ±£¬ÇåÊÖ¶¯Æô¶¯±êÖ¾
//	gUserDB.MCT_switch =STOP;		                      //³äÂúÊ±£¬Çå×Ô¶¯Æô¶¯±êÖ¾
//	gUserDB.BMS_setStartFlag =1;	        //ÖÃÆô¶¯BMSÁ¬½Ó±êÖ¾	
}

/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	chargeManage          
¹¦ÄÜÃèÊö: 	³äµç¹ÜÀíº¯Êý
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void chargeManage(void)
{	
	static INT16U 	delayCnt=0 ;
	
	
	  gUserDB.C_outVolt=pdu_info.A_volt;
	  gUserDB.C_outCurr=pdu_info.A_curr;
	  gUserDB.M_meterDegree=pdu_info.power_total;
	
	if(gUserDB.CCS_runCtlFlag==RUN && gUserDB.chargeFull ==0)	                           //Æô¶¯
	  {
			
			if(gUserDB.CCS_chargeRunStatus !=RUN)
		  {
		     gUserDB.charge_temeEn=1;
		     gUserDB.CCS_chargeRunStatus =RUN;                  //ÔËÐÐ×´Ì¬ 
             OUT1_ON ;	
             OUT2_ON ;	
		   }

		   if(  gUserDB.M_thisChargeTime  <10)
		  {
		     gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //±¾´Î³äµçÊ±¼ä
		     gUserDB.M_thisChargeDegree=0;                 //±¾´Î³äµçµçÁ¿
             
             gUserDB.M_thisChargeMoney=0;                   //±¾´Î³äµç½ð¶î	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //±¾´Î³äµçµçÁ¿
		   }
		   else
		   {
		    gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //±¾´Î³äµçÊ±¼ä
            gUserDB.M_thisChargeDegree=(gUserDB.M_meterDegree-gUserDB.M_startDegree);                 //±¾´Î³äµçµçÁ¿                                                           
            gUserDB.M_thisChargeMoney=(gUserDB.M_meterDegree-gUserDB.M_startDegree)*1;                //±¾´Î³äµç½ð¶î
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
	  else                                                     //Í£»ú×´Ì¬
	  {
			gUserDB.charge_temeEn=0;
			 gUserDB.CCS_chargeRunStatus =STOP;                  //ÔËÐÐ×´Ì¬ 
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
º¯ÊýÃû³Æ: 	alrm_Log_Test          
¹¦ÄÜÃèÊö: 	¸æ¾¯¼ÇÂ¼¼ì²â
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:     ÎÞ                 
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
º¯ÊýÃû³Æ: 	alrm_Log_Test          
¹¦ÄÜÃèÊö: 	¸æ¾¯¼ÇÂ¼¼ì²â
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:     ÎÞ                 
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
º¯ÊýÃû³Æ: 	userIndicating_execute          
¹¦ÄÜÃèÊö: 	ÓÃ»§ÃüÁîÖ´ÐÐ , gUserDB.user_indicating ÃüÁî 
            ¸ß×Ö½Ú£º0 ±íÊ¾ÃüÁîÏÂ·¢     1±íÊ¾ÃüÁîÕýÔÚÖ´ÐÐ  2±íÊ¾ÃüÁîÖ´ÐÐ½áÊø
            µÍ×Ö½Ú£º±íÊ¾¾ßÌåÃüÁî

Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void charge_searchCard(void)
{
	
			  switch(gUserDB.cardReader_indicating) //¶Á¿¨Æ÷×´Ì¬
			  {
				case CARD_CMD_S_SEARCH_END: //Ñ°¿¨½áÊø
					{		
            gUserDB.userCardState=0; 
						
						if(ReadCardData.CardStatus.Bit.AdvanceHandle==0 
							&& ReadCardData.CardStatus.Bit.SystemCard==0 
						&& ReadCardData.CardStatus.Bit.SelectCard==0
						&& ReadCardData.CardStatus.Bit.ResetUserCard==0
						&& ReadCardData.CardStatus.Bit.ResetPSAM==0) //Ô¤´¦Àí³É¹¦,ÇÒÏµÍ³ÓÐ¿¨£¬²¢ÇÒÎªÏµÍ³¿¨ 
					  {
						
							memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //¿¨ºÅ							  
							currChargeLog.purseBalance=ReadCardData.RemainMoney/100.0;                  //¿¨ÄÚÓà¶î
							gUserDB.leftMoney= ReadCardData.RemainMoney;
					//	gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //³äµçË¢¿¨ÈÏÖ¤ÇëÇó	
						if(ReadCardData.CardStatus.Bit.BusinessStatus==2) //ÅÐ¶ÏÊÇ·ñÎªÒì³£¿¨,´óÓÚ1ÔòÎª³äµçÇ°µÄÒì³£¿¨,»ÒËø¿¨
		        {
			           gUserDB.card_cmd_err=0;	                //Òì³£Ë¢¿¨
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //±¾´Î³äµçµçÁ¿
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //±¾´Î³äµçÊ±¼ä
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //±¾´Î³äµç½ð¶î	
                 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;              //²¹³ä½»Ò×Ë¢¿¨							
                 					
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==1) //ÅÐ¶ÏÊÇ·ñÎªÒì³£¿¨,´óÓÚ1ÔòÎª³äµçÇ°µÄÒì³£¿¨£¬»Ò¿¨
		        {
			           //gUserDB.card_cmd_err=1;	                //Òì³£Ë¢¿¨  ÒòÔÚ³§¼Ò×ÔÓÃ£¬ËùÒÔÈ¡ÏûË¢¿¨ÏÞÖÆ
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //±¾´Î³äµçµçÁ¿
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //±¾´Î³äµçÊ±¼ä
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //±¾´Î³äµç½ð¶î	
                // gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;                //½áÊøË¢¿¨									
                 gUserDB.user_CMD_timeout=0;
						     gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
						     gUserDB.user_CMD_return_status=1;     //³É¹¦Ë¢¿¨×´Ì¬		
                 beep_open();							
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==0) //Õý³£¿¨
					  {
							gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;  //³äµçË¢¿¨
							gUserDB.card_cmd_err=0;								
					  }						
							
//						gUserDB.user_CMD_timeout=0;
//						gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
//						gUserDB.user_CMD_return_status=0;     //³É¹¦Ë¢¿¨×´Ì¬
					 }
					 else  //ÏµÍ³ÎÞ¿¨
					 {
						 
//						 if(ReadCardData.CardStatus.Bit.SystemCard==1 && ReadCardData.CardStatus.Bit.SelectCard==0) //ÏµÍ³ÓÐ¿¨£¬²¢ÇÒÎª·ÇÏµÍ³¿¨ 
//						 {
//							 gUserDB.card_cmd_err=0;	
//							 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
//							 gUserDB.user_CMD_return_status=2;     //Ê§°Ü×´Ì¬
//						 }
						 
//						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
//						 {
							 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //¼ÌÐøÑ°¿¨
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //³¬Ê±Ñ°¿¨
//						 {
//						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
//							 gUserDB.user_CMD_return_status=2;     //Ê§°Ü×´Ì¬
//						 }
					 }
				}					
					break;				
			 case CARD_CMD_S_IDENT_END://³äµçÇ°Ë¢¿¨ÈÏÖ¤½áÊø
			 {
				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //¿¨¼Óµç³É¹¦
					 {
						 
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É
						 gUserDB.user_CMD_return_status=1;     //ÖÃ³É¹¦×´Ì¬
						 gUserDB.user_CMD_timeout=0;
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±	
             gUserDB.M_thisChargeDegree=0;                 //±¾´Î³äµçµçÁ¿
             gUserDB.M_thisChargeTime=0;                     //±¾´Î³äµçÊ±¼ä
             gUserDB.M_thisChargeMoney=0;                   //±¾´Î³äµç½ð¶î	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //±¾´Î³äµçµçÁ¿						 
							beep_open();
						 //gUserDB.user_CMD_return_status=0;     //³É¹¦Ë¢¿¨×´Ì¬
					 }
					 
				   
					else  
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
						 }
						 else    //³¬Ê±Ñ°¿¨
						 {
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //³äµçË¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
							 gUserDB.user_CMD_return_status=2;     //ÖÃÊ§°Ü×´Ì¬
							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //²¹³ä½»Ò×ÇëÇó	²âÊÔÌí¼ÓÓÃ20140928
						 }						 
					 }	
				 }					 
				  break;	
				 case CARD_CMD_E_IDENT_END://³äµç½áÊøË¢¿¨ÈÏÖ¤½áÊø
				 {
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //¿¨½áÊø¼Óµç³É¹¦
					 {
						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É
						 gUserDB.cardReader_indicating= CARD_CMD_S_IDENT_END;
						 gUserDB.user_CMD_ing = USER_CMD_SEARCH_CARD_END;          //ÖÃÃüÁîÍê³É±êÖ¾
						 gUserDB.user_CMD_return_status=1;                      //ÖÃ³É¹¦×´Ì¬
						 currChargeLog.trade_flag=2;                            //ÖÃ³äµç½»Ò×Íê³É±êÖ¾		
             //gUserDB.card_cmd_err=1;	                //Òì³£Ë¢¿¨	
            
            beep_open();						 
					 }
           else if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
					{
							 gUserDB.user_CMD_timeout++;
					}
					else    //³¬Ê±Ñ°¿¨
					{
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //³äµçË¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
							 gUserDB.user_CMD_return_status=2;     //ÖÃÊ§°Ü×´Ì¬
							 //gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //²¹³ä½»Ò×ÇëÇó	²âÊÔÌí¼ÓÓÃ20140928
					}							 
				 }					 
				  break;         				 
			}	
}

void discharge_endCard(void)
{
	INT8U cardNo_err=0;
	static INT8U re_cnt=0,firstFlag=0;
	
	switch(gUserDB.cardReader_indicating) //¶Á¿¨Æ÷×´Ì¬
			{
				case CARD_CMD_S_SEARCH_END: //³äµç½áÊøÑ°¿¨Íê³É
					{	
            cardNo_err=0;           //³õÊ¼»¯Îª¿¨ºÅÏàÍ¬
//            for(INT8U i=0;i<16;i++)  //¿¨ºÅ¶Ô±È£¬ÊÇ·ñÎªÍ¬Ò»¿¨ºÅ,ÒòÖ»ÓÐÒ»ÕÅ²âÊÔ¿¨,ËùÒÔÖ»ÄÜÏÈÆÁ±Î
//            {
//							if(currChargeLog.pay_cardID[i] !=ReadCardData.CardNo[i])
//							{
//								cardNo_err=1;      //¿¨ºÅ²»ÏàÍ¬
//								gUserDB.userCardState=1;
//								gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //³äµçË¢¿¨ÈÏÖ¤ÇëÇó	
//								break;
//							}
//            }	
						
//						if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
//							&& ReadCardData.CardStatus.Bit.BusinessStatus==1 && cardNo_err==0) //ÏµÍ³ÓÐ¿¨£¬²¢ÇÒÎªÏµÍ³¿¨
							if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus==1&&  cardNo_err==0
							&& ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 	) //Ô¤´¦Àí³É¹¦,ÇÒÏµÍ³ÓÐ¿¨£¬²¢ÇÒÎªÏµÍ³¿¨
					  {						
							 //memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //¿¨ºÅ							  
						  
						    gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //³äµçË¢¿¨ÈÏÖ¤ÇëÇó	
						    gUserDB.user_CMD_timeout=0;
							  gUserDB.card_cmd_err=0;						  				
					 }
					 else  //ÏµÍ³ÎÞ¿¨
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
							 
							 if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus ==0 &&  cardNo_err==0
							 && ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 ) //Ô¤´¦Àí³É¹¦,ÇÒÏµÍ³ÓÐ¿¨£¬²¢ÇÒÎªÏµÍ³¿¨
					    {
								  gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;         //¼Óµç
                  // gUserDB.user_indicating=USER_CMD_END_CARD_END;         //Ë¢¿¨Íê³É
						       gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //ÖÃÃüÁîÍê³É±êÖ¾	
                    firstFlag=0 ;	                  							
							}
							else
							{							
							   gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //¼ÌÐøÑ°¿¨
							}
						 }
						 else    //³¬Ê±Ñ°¿¨
						 {
							 if(ReadCardData.CardStatus.Bit.BusinessStatus !=1) //ÅÐ¶ÏÊÇ·ñÎªÒì³£¿¨,´óÓÚ1ÔòÎª³äµçÇ°µÄÒì³£¿¨
		           {
			           //gUserDB.card_cmd_err=1;			           
		           }							
						   gUserDB.user_indicating=USER_CMD_END_CARD_END;         //³äµç½áÊøË¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
							 gUserDB.user_CMD_return_status=2;     //Ê§°Ü×´Ì¬
						 }
					 }
				}					
					break;
				case CARD_CMD_S_IDENT_END://³äµçÇ°Ë¢¿¨ÈÏÖ¤½áÊø
			 {
				 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //¼ÌÐøÑ°¿¨
//				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //¿¨¼Óµç³É¹¦
//					 {
//						 
//						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É
//						 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //¼ÌÐøÑ°¿¨
////						 gUserDB.user_CMD_timeout=0;
////						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //Ë¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±						 
////							beep_open();
//						 //gUserDB.user_CMD_return_status=0;     //³É¹¦Ë¢¿¨×´Ì¬
//					 }					 
//				   
//					else  
//					 {
//						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
//						 {
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //³¬Ê±Ñ°¿¨
//						 {
//						   //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //³äµçË¢¿¨Íê³É£¬Ñ°¿¨³¬Ê±
//							 //gUserDB.user_CMD_return_status=2;     //ÖÃÊ§°Ü×´Ì¬
//							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //²¹³ä½»Ò×ÇëÇó	²âÊÔÌí¼ÓÓÃ20140928
//						 }						 
//					 }	
				 }					 
				  break;
			 case CARD_CMD_E_IDENT_END://³äµç½áÊøË¢¿¨ÈÏÖ¤½áÊø
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //¿¨½áÊø¼Óµç³É¹¦
					 {
						 gUserDB.user_indicating=USER_CMD_END_CARD_END;         //Ë¢¿¨Íê³É
						 gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //ÖÃÃüÁîÍê³É±êÖ¾
						 gUserDB.user_CMD_return_status=1;                      //ÖÃ³É¹¦×´Ì¬
						 currChargeLog.trade_flag=2;                            //ÖÃ³äµç½»Ò×Íê³É±êÖ¾	
             gUserDB.userRunRequestFlag=STOP;          //ÓÃ»§¿ØÖÆÆô¶¯						 
             beep_open();						 
					 }				   
					else  //ÊäÈëÃÜÂë3´ÎÒÔºó£¬ÈÏÖ¤Ê§°Ü
					 {						 
						   gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //³äµçË¢¿¨ÈÏÖ¤ÇëÇó					 						 
					 }						
				  break;								
			}
}
/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	userIndicating_execute          
¹¦ÄÜÃèÊö: 	ÓÃ»§ÃüÁîÖ´ÐÐ , gUserDB.user_indicating ÃüÁî 
            ¸ß×Ö½Ú£º0 ±íÊ¾ÃüÁîÏÂ·¢     1±íÊ¾ÃüÁîÕýÔÚÖ´ÐÐ  2±íÊ¾ÃüÁîÖ´ÐÐ½áÊø
            µÍ×Ö½Ú£º±íÊ¾¾ßÌåÃüÁî

Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void userIndicating_execute(void)
{	
	
	INT32U Addr=0;
	
	switch(gUserDB.user_indicating)
	{
		case USER_CMD_WAIT:  //´ý»ú
		{
//			 gUserDB.CCS_chargeRunStatus=SYS_WAIT;       //³äµç×´Ì¬--´ý»ú	   
//        
//			
//		   if(gUserDB.EVD_systemAlarm !=ALARM)
//			 {  
//				 //gUserDB.EVD_systemStatus= SYS_WAIT;                  //³äµç½áÊø×´Ì¬				 
//			 				 
//			 }						
		 }
			break;
		case USER_CMD_SEARCH_CARD_REQUEST:  //Ñ°¿¨ÇëÇó
		{
			gUserDB.user_indicating=USER_CMD_SEARCH_CARD_ING;        //ÕýÔÚÖ´ÐÐ
		  gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //Ñ°¿¨
		  
		  gUserDB.user_CMD_timeout=0;          //³¬Ê±ÇåÁã
		  
		 // re_cnt=0;
			
		  break;
		}
		case USER_CMD_SEARCH_CARD_ING:  //ÕýÔÚÖ´ÐÐÑ°¿¨		
		{	
			charge_searchCard();
		}
     break;		
		case USER_CMD_SEARCH_CARD_END:  //³äµçÇ°Ñ°¿¨½áÊø
			//gUserDB.user_indicating=0x11;        //ÕýÔÚÖ´ÐÐ
		  //gUserDB.cardReader_indicating=0x01;  //Ñ
      gUserDB.user_indicating=USER_CMD_CHARGE_REQUEST;
			    
			break;
		case USER_CMD_CHARGE_REQUEST:                         //Æô¶¯³äµçÇëÇó
		{			  
		    //re_cnt=0;
		    //osDelay(100); 			
		  // if( firstFlag==0)
		   {
				 //firstFlag=1;			   	
			   gUserDB.user_CMD_timeout=0;          //³¬Ê±ÇåÁã
		     gUserDB.user_CMD_return_status=0;    //ÎÞÐ§×´Ì¬
//			   gUserDB.CCS_fullFlag=0;              //ÖÃÎ´³äÂú±êÖ¾				 
				gUserDB.M_thisChargeDegree=0;                 //±¾´Î³äµçµçÁ¿
             gUserDB.M_thisChargeTime=0;                     //±¾´Î³äµçÊ±¼ä
             gUserDB.M_thisChargeMoney=0;                   //±¾´Î³äµç½ð¶î	
              gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //±¾´Î³äµçµçÁ¿	 				 
				 gUserDB.M_thisChargeDegree=0;                                  //±¾´Î³äµçµçÁ¿ÇåÁã
				 gUserDB.M_thisChargeTime=0;                                    //±¾´Î³äµçÊ±¼äÇåÁã
				 gUserDB.M_thisChargeTime_en=0;                                 //³äµç¼ÆÊ±Ê¹ÄÜ
				 gUserDB.M_thisChargeMoney=0;                                   //±¾´Î³äµç½ð¶îÇåÁã
				 gUserDB.userRunRequestFlag=RUN; 
		   }	
       
			 
       gUserDB.user_indicating=USER_CMD_CHARGE_ING;        //ÕýÔÚÖ´ÐÐ
			 gUserDB.user_CMD_ing = USER_CMD_CHARGE_ING;	
       gUserDB.userRunRequestFlag=RUN;          //ÓÃ»§¿ØÖÆÆô¶¯	
			   
		} 
			break;
		case USER_CMD_CHARGE_ING:  //ÕýÔÚ³äµçÖÐ....
		{
		}
			break;
		case USER_CMD_END_CARD_REQUEST:                              //³äµç½áÊøË¢¿¨ÇëÇó
		{
			ReadCardData.ExpenseMoney=gUserDB.M_thisChargeMoney*100;       //³äµç½ð¶î
			gUserDB.user_indicating=USER_CMD_END_CARD_ING;            //ÕýÔÚÖ´ÐÐ
			
			//if((gUserDB.user_CMD_ing & 0x0f) != 0x03)   //ÅÐ¶Ï½áÊø¼ÓµçÃüÁîÊÇ·ñÕýÔÚÖ´ÐÐ£¬ÕýÔÚÖ´ÐÐÊ±£¬ÔòÎÞ¶¯×÷
			{
		     gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //Ñ°¿¨
				 gUserDB.user_CMD_ing = USER_CMD_END_CARD_ING;
			}			
		 
		  gUserDB.user_CMD_timeout=0;          //³¬Ê±ÇåÁã
		  gUserDB.user_CMD_return_status=0;    //ÎÞÐ§×´Ì¬
	//	  re_cnt=0;
		  
	//	 gUserDB.CCS_chargeEnable=0;		         //³äµç»ú¸æ¾¯£¬½ûÖ¹³äµç
		
			break;
		}
		case USER_CMD_END_CARD_ING://³äµç½áÊøË¢¿¨ÕýÔÚÖ´ÐÐ
		{
			  discharge_endCard();
			break;
		} 
	 
	}	
}

 /*********************************************************************************************************************
º¯ÊýÃû³Æ: 	task_canProcess          
¹¦ÄÜÃèÊö: 	³äµç¹ÜÀíÏß³Ì
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void task_powerManage(void *pdata)			//MODBUSÈÎÎñ
{    
   static INT8U relay_flag=0xff;
   OUT2_ON ;	
   OSTimeDlyHMSM(0,0,3,0);				   //2SÑÓÊ±  
   //BMS_Power_ctl(ON);                      //BMSµçÔ´ÉÏµç	 
   //fans_ctl(OFF);

  while(1)
  {
//  	 if(gUserDB.insulationAlarm==ALARM  /*|| gUserDB.C_outVolt<200 */ ||  gUserDB.inputOverVoltAlrm==ALARM || gUserDB.inputLowVoltAlrm==ALARM ||
//	    gUserDB.CCS_outOverVoltAlarm==ALARM  || gUserDB.CCS_outOverCurrAlarm==ALARM 
//      || (gUserDB.bms_readyStatus==OK  && gUserDB.cellBatVoltMax < gUserDB.cellOverProtect 
//		     && gUserDB.BatTempMAx > gUserDB.BatTempProtect) || gUserDB.m485comLinkStatus==ALARM		 ) 	   //¾øÔµ¼ì²â¸æ¾¯,ÊäÈë¹ýÑ¹,ÊäÈëÇ·Ñ¹,Êä³ö¹ýÑ¹,Êä³ö¹ýÁ÷µÈÊ±,³äµç»ú½ûÖ¹³äµç
//	   
//		{				
//			 gUserDB.CCS_chargeEnable=NOK;		     					//½ûÖ¹³äµç
//			 gUserDB.C_CCSFaultCode=1;
//		}
//		else
//		{			
//			 gUserDB.CCS_chargeEnable=OK;		     					//ÔÊÐí³äµç			
//			 gUserDB.C_CCSFaultCode=2;		
//		}


	  
	 chargeManage();                       //³äµç¹ÜÀí
	 userIndicating_execute();
	  
	  OSTimeDlyHMSM(0,0,0,100);				//1SÔËÐÐÒ»´Î  	   
  }

}


/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	adjCcsOutVoltCurr          
¹¦ÄÜÃèÊö: 	µ÷Õû³äµç»úÊä³öµçÁ÷
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void adjCcsOutVoltCurr(void)
{

}


/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	BMS_Power_ctl          
¹¦ÄÜÃèÊö: 	BMSµçÔ´¿ØÖÆ
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
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
º¯ÊýÃû³Æ: 	setCcsVoltCurr          
¹¦ÄÜÃèÊö: 	ÉèÖÃ³äµç»úµçÑ¹µçÁ÷
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
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
º¯ÊýÃû³Æ: 	fans_ctl          
¹¦ÄÜÃèÊö: 	ÉèÖÃ³äµç»úµçÑ¹µçÁ÷
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
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
º¯ÊýÃû³Æ: 	alarmTest          
¹¦ÄÜÃèÊö: 	¹ÊÕÏ¼ì²â
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void alarmTest(void)
{
    static INT8U errCnt1=0,errCnt2=0,errCnt3=0,errCnt4=0,errCnt5=0,errCnt6=0,errCnt7=0;
    static INT8U noErrCnt1=0,noErrCnt2=0,noErrCnt3=0,noErrCnt4=0,noErrCnt5=0,noErrCnt6=0,noErrCnt7=0;	

/*		if(	gUserDB.inputLowVoltAlrm==NOALARM)	   											//ÊäÈëÇ·Ñ¹¼ì²â
		{
		    noErrCnt1=0;
			if( gAcPduInfo.A_volt<302 || gAcPduInfo.B_volt<302 || gAcPduInfo.C_volt<302 )  	
			{
				
		 		errCnt1=(errCnt1++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt1;			
				if( errCnt1>=ERR_MAX_CNT)
				{						
					gUserDB.inputLowVoltAlrm=ALARM;	                                       //ÊäÈëÇ·Ñ¹¸æ¾¯²úÉú
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
					gUserDB.inputLowVoltAlrm=NOALARM;	                                  //ÊäÈëÇ·Ñ¹¸æ¾¯»Ö¸´
				}
			}
		}

		if(	gUserDB.inputOverVoltAlrm==NOALARM)		                                       //ÊäÈë¹ýÑ¹¼ì²â
		{
		    noErrCnt2=0;
			if( gAcPduInfo.A_volt>440 || gAcPduInfo.B_volt>440 || gAcPduInfo.C_volt>440 )  	
			{
				
		 		errCnt2=(errCnt2++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt2;			
				if( errCnt2>=ERR_MAX_CNT)
				{						
					gUserDB.inputOverVoltAlrm=ALARM;	                                    //ÊäÈë¹ýÑ¹¸æ¾¯²úÉú
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
					gUserDB.inputOverVoltAlrm=NOALARM;	                                   //ÊäÈë¹ýÑ¹¸æ¾¯»Ö¸´
				}
			}
		}

  */
		if(	gUserDB.CCS_outOverVoltAlarm==NOALARM)	   											    //Êä³ö¹ýÑ¹¼ì²â
		{
		    noErrCnt3=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outVolt >gUserDB.CCS_setOutVolt+10 && gUserDB.C_outVolt>200  )  	//³äµç¹ý³ÌÖÐ
			{				
		 		errCnt3=(errCnt3++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt3;			
				if( errCnt3>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverVoltAlarm=ALARM;	                                             //Êä³ö¹ýÑ¹¸æ¾¯²úÉú
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
					gUserDB.CCS_outOverVoltAlarm=NOALARM;	                                          //Êä³ö¹ýÑ¹¸æ¾¯»Ö¸´
				}
			}
		}
			
		
		if(	gUserDB.CCS_outOverCurrAlarm==NOALARM)	   											    //Êä³ö¹ýÁ÷¼ì²â
		{
		    noErrCnt4=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outCurr >gUserDB.CCS_setOutCurr+5  )  	//³äµç¹ý³ÌÖÐ
			{				
		 		errCnt4=(errCnt4++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt4;			
				if( errCnt4>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverCurrAlarm=ALARM;	                                             //Êä³ö¹ýÁ÷¸æ¾¯²úÉú
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
					gUserDB.CCS_outOverCurrAlarm=NOALARM;	                                          //Êä³ö¹ýÁ÷¸æ¾¯»Ö¸´
				}
			}
		} 
		
/*		
		if(	gUserDB.M_carLinkState==NOALARM)	   											    //³äµçÇ¹Á¬½Ó¼ì²â
		{
		    noErrCnt5=0;
			if( gUserDB.car_link_volt < 2 || gUserDB.car_link_volt > 4.2  )  	 
			{				
		 		errCnt5=(errCnt5++> 3)?(3):errCnt5;			
				if( errCnt5>=3)
				{						
					gUserDB.M_carLinkState=ALARM;	                                             //³äµçÇ¹Á¬½Ó¸æ¾¯²úÉú
					if(gUserDB.CCS_chargeRunStatus ==RUN)                                        //ÏµÍ³³äµç¹ý³ÌÖÐ³µÁ¾Á¬½ÓÒì³££¬Ôò¶Ï¿ª½Ó´¥Æ÷
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
					//gUserDB.M_carLinkState=ALARM	                                             //³äµçÇ¹Á¬½Ó¸æ¾¯»Ö¸´
					if(gUserDB.car_linkAlarm !=ALARM)                                            //³µÁ¾Òì³£¶Ï¿ªºó£¬²»ÔÙ×Ô¶¯ÎüºÏ¼ÌµçÆ÷£¬³ý·ÇÖØÆðÏµÍ³
					   OUT4_OFF;
				}
			}
		}		
		
*/		

	return;
}

/*********************************************************************************************************************
º¯ÊýÃû³Æ: 	pwmSetEnable          
¹¦ÄÜÃèÊö: 	
Êä¡¡Èë:   	ÎÞ

Êä¡¡³ö:   	ÎÞ                 
*********************************************************************************************************************/
void ledShow(void)
{
  static INT8U flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0;
  static INT8U runLedFlag=0;

 
  	if(flag1==0)   //ÏµÍ³ÔËÐÐLED1
	{
		flag1=1;
		LED1_OFF;
	}
	else
	{
		flag1=0;
		LED1_ON;
	} 


  if(gUserDB.m485comLinkStatus==ALARM)       //CCUÓë×éÌ¬ÆÁÍ¨ÐÅLED
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
//  if(gUserDB.ILB_comAlarm==ALARM)       //CCUÓë¾øÔµ¼ì²â°åÍ¨ÐÅ×´Ì¬LED
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
//  if(gUserDB.DcPduComAlarm==ALARM)       //CCUÓëÖ±Á÷Á÷±íÍ¨ÐÅ×´Ì¬LED
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
//  if(gUserDB.bms_linkStatus==ALARM)       //BMSÁ¬½Ó×´Ì¬LED
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
//  	OUT1_OFF;  //ÁÁ
//    OUT2_OFF;   //Ãð
//	//OUT3_ON;   //Ãð
//  //  OUT4_ON;  //Ãð
//  }
//  else
//  {
//  	OUT1_ON;
//	  OUT2_OFF;  //ÁÁ
////	OUT3_ON; //Ãð
////	OUT4_ON; //Ãð
//  }
//	
//	if(gUserDB.C_CCSFaultCode==1)  //ÏµÍ³¹ÊÕÏ
//	{
//		 OUT3_OFF;  //ÁÁ
//	}
//	else
//	{
//			OUT3_ON; //Ãð
//	}
	
}

