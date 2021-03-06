/*********************************************************************************************************
*                                            头文件包含
*********************************************************************************************************/
#include "Global.h"
#include "../mod/moduleCom/moduleCom.h"

/*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************/
#define  MINITUE_3_TICK                 120

/*********************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************/	


/*********************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************/
extern USER_DB gUserDB;
void alarmTest( void );
void pwmVoltSetEnable(void);
void pwmCurrSetEnable(void);
void ledShow(void);
void chargeManage(void);

INT16U gSetVoltage=0;										//设置电压值
INT16U gSetCurrent=0;										//设置电流值
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
函数名称: 	bmsDataInfoDeal          
功能描述: 	BMS信息处理
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void bmsDataInfoDeal(void)
{
	

	

}

/*********************************************************************************************************************
函数名称: 	setCcsVoltCurr          
功能描述: 	设置充电机电压电流
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void setCcsVoltCurr(void)
{
    
}

/*********************************************************************************************************************
函数名称: 	moduleCurrReset          
功能描述: 	模块电流重新分配与启动模式策略
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void moduleVoltCurrSet(FP32 setOutCurr)
{
   
}

/*********************************************************************************************************************
函数名称: 	chargeFullStrege          
功能描述: 	充满足策略
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void chargeFullStrege(void)
{
//	gUserDB.CCS_fullFlag=1;                           //充满标志
	gUserDB.manualRunCtlFlag=STOP;	                  //充满时，清手动启动标志
//	gUserDB.MCT_switch =STOP;		                      //充满时，清自动启动标志
//	gUserDB.BMS_setStartFlag =1;	        //置启动BMS连接标志	
}

/*********************************************************************************************************************
函数名称: 	chargeManage          
功能描述: 	充电管理函数
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void chargeManage(void)
{	
	static INT16U 	delayCnt=0 ;
	
	
	  gUserDB.C_outVolt=pdu_info.A_volt;
	  gUserDB.C_outCurr=pdu_info.A_curr;
	  gUserDB.M_meterDegree=pdu_info.power_total;
	
	if(gUserDB.CCS_runCtlFlag==RUN && gUserDB.chargeFull ==0)	                           //启动
	  {
			
			if(gUserDB.CCS_chargeRunStatus !=RUN)
		  {
		     gUserDB.charge_temeEn=1;
		     gUserDB.CCS_chargeRunStatus =RUN;                  //运行状态 
             OUT1_ON ;	
             OUT2_ON ;	
		   }

		   if(  gUserDB.M_thisChargeTime  <10)
		  {
		     gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //本次充电时间
		     gUserDB.M_thisChargeDegree=0;                 //本次充电电量
             
             gUserDB.M_thisChargeMoney=0;                   //本次充电金额	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //本次充电电量
		   }
		   else
		   {
		    gUserDB.M_thisChargeTime=gUserDB.charge_timeCnt;   //本次充电时间
            gUserDB.M_thisChargeDegree=(gUserDB.M_meterDegree-gUserDB.M_startDegree);                 //本次充电电量                                                           
            gUserDB.M_thisChargeMoney=(gUserDB.M_meterDegree-gUserDB.M_startDegree)*1;                //本次充电金额
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
	  else                                                     //停机状态
	  {
			gUserDB.charge_temeEn=0;
			 gUserDB.CCS_chargeRunStatus =STOP;                  //运行状态 
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
函数名称: 	alrm_Log_Test          
功能描述: 	告警记录检测
输　入:   	无

输　出:     无                 
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
函数名称: 	alrm_Log_Test          
功能描述: 	告警记录检测
输　入:   	无

输　出:     无                 
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
函数名称: 	userIndicating_execute          
功能描述: 	用户命令执行 , gUserDB.user_indicating 命令 
            高字节：0 表示命令下发     1表示命令正在执行  2表示命令执行结束
            低字节：表示具体命令

输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void charge_searchCard(void)
{
	
			  switch(gUserDB.cardReader_indicating) //读卡器状态
			  {
				case CARD_CMD_S_SEARCH_END: //寻卡结束
					{		
            gUserDB.userCardState=0; 
						
						if(ReadCardData.CardStatus.Bit.AdvanceHandle==0 
							&& ReadCardData.CardStatus.Bit.SystemCard==0 
						&& ReadCardData.CardStatus.Bit.SelectCard==0
						&& ReadCardData.CardStatus.Bit.ResetUserCard==0
						&& ReadCardData.CardStatus.Bit.ResetPSAM==0) //预处理成功,且系统有卡，并且为系统卡 
					  {
						
							memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //卡号							  
							currChargeLog.purseBalance=ReadCardData.RemainMoney/100.0;                  //卡内余额
							gUserDB.leftMoney= ReadCardData.RemainMoney;
					//	gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //充电刷卡认证请求	
						if(ReadCardData.CardStatus.Bit.BusinessStatus==2) //判断是否为异常卡,大于1则为充电前的异常卡,灰锁卡
		        {
			           gUserDB.card_cmd_err=0;	                //异常刷卡
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //本次充电电量
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //本次充电时间
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //本次充电金额	
                 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;              //补充交易刷卡							
                 					
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==1) //判断是否为异常卡,大于1则为充电前的异常卡，灰卡
		        {
			           //gUserDB.card_cmd_err=1;	                //异常刷卡  因在厂家自用，所以取消刷卡限制
//                 gUserDB.M_thisChargeDegree=currChargeLog.charge_degree;                 //本次充电电量
//				         gUserDB.M_thisChargeTime=currChargeLog.charge_time;                     //本次充电时间
//				         gUserDB.M_thisChargeMoney=currChargeLog.consum_money;                   //本次充电金额	
                // gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;                //结束刷卡									
                 gUserDB.user_CMD_timeout=0;
						     gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时
						     gUserDB.user_CMD_return_status=1;     //成功刷卡状态		
                 beep_open();							
		        }
						else if(ReadCardData.CardStatus.Bit.BusinessStatus==0) //正常卡
					  {
							gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;  //充电刷卡
							gUserDB.card_cmd_err=0;								
					  }						
							
//						gUserDB.user_CMD_timeout=0;
//						gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时
//						gUserDB.user_CMD_return_status=0;     //成功刷卡状态
					 }
					 else  //系统无卡
					 {
						 
//						 if(ReadCardData.CardStatus.Bit.SystemCard==1 && ReadCardData.CardStatus.Bit.SelectCard==0) //系统有卡，并且为非系统卡 
//						 {
//							 gUserDB.card_cmd_err=0;	
//							 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时
//							 gUserDB.user_CMD_return_status=2;     //失败状态
//						 }
						 
//						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
//						 {
							 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //继续寻卡
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //超时寻卡
//						 {
//						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时
//							 gUserDB.user_CMD_return_status=2;     //失败状态
//						 }
					 }
				}					
					break;				
			 case CARD_CMD_S_IDENT_END://充电前刷卡认证结束
			 {
				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //卡加电成功
					 {
						 
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成
						 gUserDB.user_CMD_return_status=1;     //置成功状态
						 gUserDB.user_CMD_timeout=0;
						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时	
             gUserDB.M_thisChargeDegree=0;                 //本次充电电量
             gUserDB.M_thisChargeTime=0;                     //本次充电时间
             gUserDB.M_thisChargeMoney=0;                   //本次充电金额	
             gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //本次充电电量						 
							beep_open();
						 //gUserDB.user_CMD_return_status=0;     //成功刷卡状态
					 }
					 
				   
					else  
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
						 }
						 else    //超时寻卡
						 {
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //充电刷卡完成，寻卡超时
							 gUserDB.user_CMD_return_status=2;     //置失败状态
							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //补充交易请求	测试添加用20140928
						 }						 
					 }	
				 }					 
				  break;	
				 case CARD_CMD_E_IDENT_END://充电结束刷卡认证结束
				 {
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //卡结束加电成功
					 {
						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成
						 gUserDB.cardReader_indicating= CARD_CMD_S_IDENT_END;
						 gUserDB.user_CMD_ing = USER_CMD_SEARCH_CARD_END;          //置命令完成标志
						 gUserDB.user_CMD_return_status=1;                      //置成功状态
						 currChargeLog.trade_flag=2;                            //置充电交易完成标志		
             //gUserDB.card_cmd_err=1;	                //异常刷卡	
            
            beep_open();						 
					 }
           else if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
					{
							 gUserDB.user_CMD_timeout++;
					}
					else    //超时寻卡
					{
						   gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //充电刷卡完成，寻卡超时
							 gUserDB.user_CMD_return_status=2;     //置失败状态
							 //gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //补充交易请求	测试添加用20140928
					}							 
				 }					 
				  break;         				 
			}	
}

void discharge_endCard(void)
{
	INT8U cardNo_err=0;
	static INT8U re_cnt=0,firstFlag=0;
	
	switch(gUserDB.cardReader_indicating) //读卡器状态
			{
				case CARD_CMD_S_SEARCH_END: //充电结束寻卡完成
					{	
            cardNo_err=0;           //初始化为卡号相同
//            for(INT8U i=0;i<16;i++)  //卡号对比，是否为同一卡号,因只有一张测试卡,所以只能先屏蔽
//            {
//							if(currChargeLog.pay_cardID[i] !=ReadCardData.CardNo[i])
//							{
//								cardNo_err=1;      //卡号不相同
//								gUserDB.userCardState=1;
//								gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //充电刷卡认证请求	
//								break;
//							}
//            }	
						
//						if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
//							&& ReadCardData.CardStatus.Bit.BusinessStatus==1 && cardNo_err==0) //系统有卡，并且为系统卡
							if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus==1&&  cardNo_err==0
							&& ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 	) //预处理成功,且系统有卡，并且为系统卡
					  {						
							 //memcpy(currChargeLog.pay_cardID,ReadCardData.CardNo,16);                    //卡号							  
						  
						    gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //充电刷卡认证请求	
						    gUserDB.user_CMD_timeout=0;
							  gUserDB.card_cmd_err=0;						  				
					 }
					 else  //系统无卡
					 {
						 if(gUserDB.user_CMD_timeout++ <1200)      //1000*60*1/50=1,200 ms
						 {
							 gUserDB.user_CMD_timeout++;
							 
							 if(ReadCardData.CardStatus.Bit.SystemCard==0 && ReadCardData.CardStatus.Bit.SelectCard==0
							&& ReadCardData.CardStatus.Bit.BusinessStatus ==0 &&  cardNo_err==0
							 && ReadCardData.CardStatus.Bit.AdvanceHandle==0 && ReadCardData.CardStatus.Bit.ResetPSAM==0
              && ReadCardData.CardStatus.Bit.ResetUserCard==0 ) //预处理成功,且系统有卡，并且为系统卡
					    {
								  gUserDB.cardReader_indicating=CARD_CMD_S_IDENT_REQUEST;         //加电
                  // gUserDB.user_indicating=USER_CMD_END_CARD_END;         //刷卡完成
						       gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //置命令完成标志	
                    firstFlag=0 ;	                  							
							}
							else
							{							
							   gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //继续寻卡
							}
						 }
						 else    //超时寻卡
						 {
							 if(ReadCardData.CardStatus.Bit.BusinessStatus !=1) //判断是否为异常卡,大于1则为充电前的异常卡
		           {
			           //gUserDB.card_cmd_err=1;			           
		           }							
						   gUserDB.user_indicating=USER_CMD_END_CARD_END;         //充电结束刷卡完成，寻卡超时
							 gUserDB.user_CMD_return_status=2;     //失败状态
						 }
					 }
				}					
					break;
				case CARD_CMD_S_IDENT_END://充电前刷卡认证结束
			 {
				 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //继续寻卡
//				 if(ReadCardData.CardStatus.Bit.ChargeStatus==0)  //卡加电成功
//					 {
//						 
//						 //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成
//						 gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //继续寻卡
////						 gUserDB.user_CMD_timeout=0;
////						 gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //刷卡完成，寻卡超时						 
////							beep_open();
//						 //gUserDB.user_CMD_return_status=0;     //成功刷卡状态
//					 }					 
//				   
//					else  
//					 {
//						 if(gUserDB.user_CMD_timeout++ <1200)   // 1000*60*1/50=1,200 ms
//						 {
//							 gUserDB.user_CMD_timeout++;
//						 }
//						 else    //超时寻卡
//						 {
//						   //gUserDB.user_indicating=USER_CMD_SEARCH_CARD_END;         //充电刷卡完成，寻卡超时
//							 //gUserDB.user_CMD_return_status=2;     //置失败状态
//							 gUserDB.cardReader_indicating=CARD_CMD_ADDCHARGE_REQUEST;         //补充交易请求	测试添加用20140928
//						 }						 
//					 }	
				 }					 
				  break;
			 case CARD_CMD_E_IDENT_END://充电结束刷卡认证结束
				   if(ReadCardData.CardStatus.Bit.EndChargeStatus==0)  //卡结束加电成功
					 {
						 gUserDB.user_indicating=USER_CMD_END_CARD_END;         //刷卡完成
						 gUserDB.user_CMD_ing = USER_CMD_END_CARD_END;          //置命令完成标志
						 gUserDB.user_CMD_return_status=1;                      //置成功状态
						 currChargeLog.trade_flag=2;                            //置充电交易完成标志	
             gUserDB.userRunRequestFlag=STOP;          //用户控制启动						 
             beep_open();						 
					 }				   
					else  //输入密码3次以后，认证失败
					 {						 
						   gUserDB.cardReader_indicating=CARD_CMD_E_IDENT_REQUEST;         //充电刷卡认证请求					 						 
					 }						
				  break;								
			}
}
/*********************************************************************************************************************
函数名称: 	userIndicating_execute          
功能描述: 	用户命令执行 , gUserDB.user_indicating 命令 
            高字节：0 表示命令下发     1表示命令正在执行  2表示命令执行结束
            低字节：表示具体命令

输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void userIndicating_execute(void)
{	
	
	INT32U Addr=0;
	
	switch(gUserDB.user_indicating)
	{
		case USER_CMD_WAIT:  //待机
		{
//			 gUserDB.CCS_chargeRunStatus=SYS_WAIT;       //充电状态--待机	   
//        
//			
//		   if(gUserDB.EVD_systemAlarm !=ALARM)
//			 {  
//				 //gUserDB.EVD_systemStatus= SYS_WAIT;                  //充电结束状态				 
//			 				 
//			 }						
		 }
			break;
		case USER_CMD_SEARCH_CARD_REQUEST:  //寻卡请求
		{
			gUserDB.user_indicating=USER_CMD_SEARCH_CARD_ING;        //正在执行
		  gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //寻卡
		  
		  gUserDB.user_CMD_timeout=0;          //超时清零
		  
		 // re_cnt=0;
			
		  break;
		}
		case USER_CMD_SEARCH_CARD_ING:  //正在执行寻卡		
		{	
			charge_searchCard();
		}
     break;		
		case USER_CMD_SEARCH_CARD_END:  //充电前寻卡结束
			//gUserDB.user_indicating=0x11;        //正在执行
		  //gUserDB.cardReader_indicating=0x01;  //�
      gUserDB.user_indicating=USER_CMD_CHARGE_REQUEST;
			    
			break;
		case USER_CMD_CHARGE_REQUEST:                         //启动充电请求
		{			  
		    //re_cnt=0;
		    //osDelay(100); 			
		  // if( firstFlag==0)
		   {
				 //firstFlag=1;			   	
			   gUserDB.user_CMD_timeout=0;          //超时清零
		     gUserDB.user_CMD_return_status=0;    //无效状态
//			   gUserDB.CCS_fullFlag=0;              //置未充满标志				 
				gUserDB.M_thisChargeDegree=0;                 //本次充电电量
             gUserDB.M_thisChargeTime=0;                     //本次充电时间
             gUserDB.M_thisChargeMoney=0;                   //本次充电金额	
              gUserDB.M_startDegree=gUserDB.M_meterDegree;                 //本次充电电量	 				 
				 gUserDB.M_thisChargeDegree=0;                                  //本次充电电量清零
				 gUserDB.M_thisChargeTime=0;                                    //本次充电时间清零
				 gUserDB.M_thisChargeTime_en=0;                                 //充电计时使能
				 gUserDB.M_thisChargeMoney=0;                                   //本次充电金额清零
				 gUserDB.userRunRequestFlag=RUN; 
		   }	
       
			 
       gUserDB.user_indicating=USER_CMD_CHARGE_ING;        //正在执行
			 gUserDB.user_CMD_ing = USER_CMD_CHARGE_ING;	
       gUserDB.userRunRequestFlag=RUN;          //用户控制启动	
			   
		} 
			break;
		case USER_CMD_CHARGE_ING:  //正在充电中....
		{
		}
			break;
		case USER_CMD_END_CARD_REQUEST:                              //充电结束刷卡请求
		{
			ReadCardData.ExpenseMoney=gUserDB.M_thisChargeMoney*100;       //充电金额
			gUserDB.user_indicating=USER_CMD_END_CARD_ING;            //正在执行
			
			//if((gUserDB.user_CMD_ing & 0x0f) != 0x03)   //判断结束加电命令是否正在执行，正在执行时，则无动作
			{
		     gUserDB.cardReader_indicating=CARD_CMD_S_SEARCH_REQUEST;  //寻卡
				 gUserDB.user_CMD_ing = USER_CMD_END_CARD_ING;
			}			
		 
		  gUserDB.user_CMD_timeout=0;          //超时清零
		  gUserDB.user_CMD_return_status=0;    //无效状态
	//	  re_cnt=0;
		  
	//	 gUserDB.CCS_chargeEnable=0;		         //充电机告警，禁止充电
		
			break;
		}
		case USER_CMD_END_CARD_ING://充电结束刷卡正在执行
		{
			  discharge_endCard();
			break;
		} 
	 
	}	
}

 /*********************************************************************************************************************
函数名称: 	task_canProcess          
功能描述: 	充电管理线程
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void task_powerManage(void *pdata)			//MODBUS任务
{    
   static INT8U relay_flag=0xff;
   OUT2_ON ;	
   OSTimeDlyHMSM(0,0,3,0);				   //2S延时  
   //BMS_Power_ctl(ON);                      //BMS电源上电	 
   //fans_ctl(OFF);

  while(1)
  {
//  	 if(gUserDB.insulationAlarm==ALARM  /*|| gUserDB.C_outVolt<200 */ ||  gUserDB.inputOverVoltAlrm==ALARM || gUserDB.inputLowVoltAlrm==ALARM ||
//	    gUserDB.CCS_outOverVoltAlarm==ALARM  || gUserDB.CCS_outOverCurrAlarm==ALARM 
//      || (gUserDB.bms_readyStatus==OK  && gUserDB.cellBatVoltMax < gUserDB.cellOverProtect 
//		     && gUserDB.BatTempMAx > gUserDB.BatTempProtect) || gUserDB.m485comLinkStatus==ALARM		 ) 	   //绝缘检测告警,输入过压,输入欠压,输出过压,输出过流等时,充电机禁止充电
//	   
//		{				
//			 gUserDB.CCS_chargeEnable=NOK;		     					//禁止充电
//			 gUserDB.C_CCSFaultCode=1;
//		}
//		else
//		{			
//			 gUserDB.CCS_chargeEnable=OK;		     					//允许充电			
//			 gUserDB.C_CCSFaultCode=2;		
//		}


	  
	 chargeManage();                       //充电管理
	 userIndicating_execute();
	  
	  OSTimeDlyHMSM(0,0,0,100);				//1S运行一次  	   
  }

}


/*********************************************************************************************************************
函数名称: 	adjCcsOutVoltCurr          
功能描述: 	调整充电机输出电流
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void adjCcsOutVoltCurr(void)
{

}


/*********************************************************************************************************************
函数名称: 	BMS_Power_ctl          
功能描述: 	BMS电源控制
输　入:   	无

输　出:   	无                 
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
函数名称: 	setCcsVoltCurr          
功能描述: 	设置充电机电压电流
输　入:   	无

输　出:   	无                 
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
函数名称: 	fans_ctl          
功能描述: 	设置充电机电压电流
输　入:   	无

输　出:   	无                 
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
函数名称: 	alarmTest          
功能描述: 	故障检测
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void alarmTest(void)
{
    static INT8U errCnt1=0,errCnt2=0,errCnt3=0,errCnt4=0,errCnt5=0,errCnt6=0,errCnt7=0;
    static INT8U noErrCnt1=0,noErrCnt2=0,noErrCnt3=0,noErrCnt4=0,noErrCnt5=0,noErrCnt6=0,noErrCnt7=0;	

/*		if(	gUserDB.inputLowVoltAlrm==NOALARM)	   											//输入欠压检测
		{
		    noErrCnt1=0;
			if( gAcPduInfo.A_volt<302 || gAcPduInfo.B_volt<302 || gAcPduInfo.C_volt<302 )  	
			{
				
		 		errCnt1=(errCnt1++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt1;			
				if( errCnt1>=ERR_MAX_CNT)
				{						
					gUserDB.inputLowVoltAlrm=ALARM;	                                       //输入欠压告警产生
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
					gUserDB.inputLowVoltAlrm=NOALARM;	                                  //输入欠压告警恢复
				}
			}
		}

		if(	gUserDB.inputOverVoltAlrm==NOALARM)		                                       //输入过压检测
		{
		    noErrCnt2=0;
			if( gAcPduInfo.A_volt>440 || gAcPduInfo.B_volt>440 || gAcPduInfo.C_volt>440 )  	
			{
				
		 		errCnt2=(errCnt2++>ERR_MAX_CNT)?(ERR_MAX_CNT):errCnt2;			
				if( errCnt2>=ERR_MAX_CNT)
				{						
					gUserDB.inputOverVoltAlrm=ALARM;	                                    //输入过压告警产生
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
					gUserDB.inputOverVoltAlrm=NOALARM;	                                   //输入过压告警恢复
				}
			}
		}

  */
		if(	gUserDB.CCS_outOverVoltAlarm==NOALARM)	   											    //输出过压检测
		{
		    noErrCnt3=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outVolt >gUserDB.CCS_setOutVolt+10 && gUserDB.C_outVolt>200  )  	//充电过程中
			{				
		 		errCnt3=(errCnt3++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt3;			
				if( errCnt3>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverVoltAlarm=ALARM;	                                             //输出过压告警产生
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
					gUserDB.CCS_outOverVoltAlarm=NOALARM;	                                          //输出过压告警恢复
				}
			}
		}
			
		
		if(	gUserDB.CCS_outOverCurrAlarm==NOALARM)	   											    //输出过流检测
		{
		    noErrCnt4=0;
			if( gUserDB.CCS_chargeRunStatus == RUN && gUserDB.C_outCurr >gUserDB.CCS_setOutCurr+5  )  	//充电过程中
			{				
		 		errCnt4=(errCnt4++>ERR_OUT_VOLTCURR_CNT)?(ERR_OUT_VOLTCURR_CNT):errCnt4;			
				if( errCnt4>=ERR_OUT_VOLTCURR_CNT)
				{						
					gUserDB.CCS_outOverCurrAlarm=ALARM;	                                             //输出过流告警产生
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
					gUserDB.CCS_outOverCurrAlarm=NOALARM;	                                          //输出过流告警恢复
				}
			}
		} 
		
/*		
		if(	gUserDB.M_carLinkState==NOALARM)	   											    //充电枪连接检测
		{
		    noErrCnt5=0;
			if( gUserDB.car_link_volt < 2 || gUserDB.car_link_volt > 4.2  )  	 
			{				
		 		errCnt5=(errCnt5++> 3)?(3):errCnt5;			
				if( errCnt5>=3)
				{						
					gUserDB.M_carLinkState=ALARM;	                                             //充电枪连接告警产生
					if(gUserDB.CCS_chargeRunStatus ==RUN)                                        //系统充电过程中车辆连接异常，则断开接触器
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
					//gUserDB.M_carLinkState=ALARM	                                             //充电枪连接告警恢复
					if(gUserDB.car_linkAlarm !=ALARM)                                            //车辆异常断开后，不再自动吸合继电器，除非重起系统
					   OUT4_OFF;
				}
			}
		}		
		
*/		

	return;
}

/*********************************************************************************************************************
函数名称: 	pwmSetEnable          
功能描述: 	
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void ledShow(void)
{
  static INT8U flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0;
  static INT8U runLedFlag=0;

 
  	if(flag1==0)   //系统运行LED1
	{
		flag1=1;
		LED1_OFF;
	}
	else
	{
		flag1=0;
		LED1_ON;
	} 


  if(gUserDB.m485comLinkStatus==ALARM)       //CCU与组态屏通信LED
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
//  if(gUserDB.ILB_comAlarm==ALARM)       //CCU与绝缘检测板通信状态LED
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
//  if(gUserDB.DcPduComAlarm==ALARM)       //CCU与直流流表通信状态LED
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
//  if(gUserDB.bms_linkStatus==ALARM)       //BMS连接状态LED
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
//  	OUT1_OFF;  //亮
//    OUT2_OFF;   //灭
//	//OUT3_ON;   //灭
//  //  OUT4_ON;  //灭
//  }
//  else
//  {
//  	OUT1_ON;
//	  OUT2_OFF;  //亮
////	OUT3_ON; //灭
////	OUT4_ON; //灭
//  }
//	
//	if(gUserDB.C_CCSFaultCode==1)  //系统故障
//	{
//		 OUT3_OFF;  //亮
//	}
//	else
//	{
//			OUT3_ON; //灭
//	}
	
}

