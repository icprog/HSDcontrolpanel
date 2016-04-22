 /*********************************************************************************************************************
*                                            头文件包含
***********************************************************************************************************************/

#include "ucos_ii.h"
#include "stm32f10x.h"
#include "global.h"
#include "includes.h"
#include "lcd.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define BOOL    INT8U
//#define MID_MODBUS_COM       COM2  

/********************************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************************/
uint16 b0001Value;
static uint8_t rcvBuf[128];
extern CHARGE_LOG_INFO currChargeLog;
////==========================================================================================================
//// 描述:	读取串口数据
//// 参数： uint8_t *buf, uint8_t MaxSize, uint16 timeout
//// 返回： uint8
////===========================================================================================================
uint8 TouchLinkReadFrame(uint8_t *buf, uint8_t MaxSize, uint16 timeout)
{
    uint8_t i, rcvLen = 0, tmpLen = 0;
//    uint32 CurrentTime;
    //gUserDB.startTime = gUserDB.tickMs;
	
    memset(rcvBuf,0,128);
    //do 
    {
			  tmpLen = uart_read(MID_MODBUS_COM , rcvBuf, timeout);
        {			

        } 
//				if(tmpLen<128)
//        memset(rcvBuf,0,128);				
        
    }
		
    return tmpLen;
}
////==========================================================================================================
//// 描述:	CRC校验
//// 参数： uint8_t *addr, uint16_t num
//// 返回： uint16_t
////===========================================================================================================
uint16_t count_TouchCRC(uint8_t *addr, uint16_t num)
{
    uint16_t CRCword = 0xFFFF;
    int i;
    while (num--)
    {
        CRCword ^= *addr++;
        for (i = 0; i < 8; i++)
        {
            if (CRCword & 1)
            {
                CRCword >>= 1;
                CRCword ^= 0xA001;
            }
            else
            {
                CRCword >>= 1;
            }
        }
    }
    return CRCword;
}
uint8 ComposeTouchTelegramData(uint8 cmdType,uint16 startAddr,uint8 *buf,uint8 bufLen,uint8 readLen)
{
    uint8 tmpBuf[128],tmpByte,i=0;
    memset(tmpBuf,0,sizeof(tmpBuf));
    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    switch(cmdType)
    {
        case TOUCH_CMD_WRITE_REGISTER:
        {
            tmpBuf[i++]=TOUCH_CMD_WRITE_REGISTER;
            tmpBuf[i++]=(startAddr&0x00FF);
            memcpy(&tmpBuf[i],buf,bufLen);
            i+=bufLen;
            
            break;
        }
        case TOUCH_CMD_READ_REGISTER:
        {
            tmpBuf[i++]=TOUCH_CMD_READ_REGISTER;
            tmpBuf[i++]=(startAddr&0x00FF);
            tmpBuf[i++]=readLen;
            break;
        }
        case TOUCH_CMD_WRITE_VARIABLE:
        {
            tmpBuf[i++]=TOUCH_CMD_WRITE_VARIABLE;
            tmpBuf[i++]=(startAddr&0x00FF);
            tmpBuf[i++]=((startAddr&0xFF00)>>8);
            memcpy(&tmpBuf[i],buf,bufLen);
            i+=bufLen; 
            break;
        }
        case TOUCH_CMD_READ_VARIABLE:
        {
            tmpBuf[i++]=TOUCH_CMD_WRITE_VARIABLE;
            tmpBuf[i++]=(startAddr&0x00FF);
            tmpBuf[i++]=((startAddr&0xFF00)>>8);
            tmpBuf[i++]=readLen;
            break;
        }
        default:
        {
            break;
        }
    }
    tmpBuf[2]=i-3;
    memcpy(buf,&tmpBuf,i);
    return i;	
}
void DealReadVariableVale(uint16 addr, uint8 *buf)
{
    uint16 btnValue;
    switch(addr)
    {
        case BUTTON_ADDR_GO_RUSH_CARD:
        {
            btnValue=(buf[7]<<8)+buf[8];
            b0001Value=btnValue;
            break;
        }
        default:
        {
            break;
        }
    }
}
uint16 AnalysisCurrentPageIdFrame(uint8 cmdType,uint8 *buf,uint8 rcvLen)
{
    BOOL rtnBool=FALSE,i=0;
    uint8 dataLen=0;
    uint32 tmpValue;
    uint16 readAddr,currentId;
    
    if(buf[0]!=0XA5)
    {
        return 0XFFFF;
    }
    if(buf[1]!=0X5A)
    {
        return 0XFFFF;
    }
    if(cmdType!=buf[3])
    {
        return 0XFFFF; 
    }
    if(buf[4]==0x03)
    {
       currentId=(buf[6]<<8)+buf[7];
	   gUserDB.m485CommAlarmCnt=0;
		gUserDB.m485comLinkStatus=NOALARM;
       return currentId;

    }
    return 0XFFFF;
}
uint16 AnalysisLCDButtonFrame(uint16 Addr,uint8*buf,uint8 dataLen)
{
    uint32 tmpValue;
    uint16 readAddr,btnValue;
    if(dataLen!=9)
    {
        return 0XFFFF;
    }
    if(buf[0]!=0XA5)
    {
        return 0XFFFF;
    }
    if(buf[1]!=0X5A)
    {
        return 0XFFFF;
    }
    if(TOUCH_CMD_READ_VARIABLE!=buf[3])
    {
        return 0XFFFF; 
    }		
    readAddr=(buf[4]<<8)+buf[5];
    if(readAddr!=Addr)
    {
        return 0XFFFF;
    }

    btnValue=(buf[7]<<8)+buf[8];
    return btnValue;
}

char timeflag=0;
uint16 ReadCurrentPageId(void)
{
		
    uint16 pageId;
    uint16 readAddr;
    uint8 tmpBuf[16],i=0,sendLen,rcvLen;
    memset(tmpBuf,0,sizeof(tmpBuf));
	    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    tmpBuf[i++]=TOUCH_CMD_READ_REGISTER;
    tmpBuf[i++]=0X03;
    tmpBuf[i++]=0X02;
    tmpBuf[2]=i-3;
    sendLen=i;
		//flag=1;
    if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE)   
    {
        memset(tmpBuf,0,sizeof(tmpBuf));
        rcvLen=TouchLinkReadFrame(tmpBuf, 128, 100);
        if(rcvLen>0)
        {
		   
           pageId=AnalysisCurrentPageIdFrame(TOUCH_CMD_READ_REGISTER,rcvBuf,rcvLen);
           return pageId;
        }
    }
    return 0XFFFF;
    
}
uint16 ReadLCDButton(uint16 readAddr)
{   
    uint8 tmpBuf[16],tmpByte,rcvLen,i=0,sendLen;
    uint16 btnV;
    memset(tmpBuf,0,sizeof(tmpBuf));
    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    tmpBuf[i++]=TOUCH_CMD_READ_VARIABLE;
    tmpBuf[i++]=((readAddr&0xFF00)>>8);
    tmpBuf[i++]=(readAddr&0x00FF);    
    tmpBuf[i++]=1;

    tmpBuf[2]=i-3;
    sendLen=i;

    if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
    {
        memset(tmpBuf,0,sizeof(tmpBuf));
        rcvLen=TouchLinkReadFrame(tmpBuf, 128, 100);
        if(rcvLen>0)
        {
           btnV=AnalysisLCDButtonFrame(readAddr,rcvBuf,rcvLen); 
           return btnV;
        }
				i=0;
    }
    return 0xFFFF;    
}
BOOL SetPageId(uint16 setId)
{
	//setId=0x0001;//for test
    uint8 buf[16],sendLen,rcvLen;
    uint8 tmpBuf[128],i=0;
    uint16 pageId;

    buf[0]=((setId&0XFF00)>>8);
    buf[1]=(setId&0X00FF);
        
    memset(tmpBuf,0,sizeof(tmpBuf));
    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    tmpBuf[i++]=TOUCH_CMD_WRITE_REGISTER;
    tmpBuf[i++]=0X03;
    tmpBuf[i++]=((setId&0XFF00)>>8);
    tmpBuf[i++]=(setId&0X00FF);
    tmpBuf[2]=i-3;
    sendLen=i;
    if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
    {
        if(ReadCurrentPageId()==setId)
        {
            return TRUE;
        }
    }
    return FALSE;
    
}




BOOL ReadVariableValue(uint16 getAddr,uint8* buf,uint8 dataLen)
{
    uint16 sendLen;
    uint8 tmpBuf[256],i=0,rcvLen;
    memset(tmpBuf,0,sizeof(tmpBuf));
    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    tmpBuf[i++]=TOUCH_CMD_READ_VARIABLE;
    tmpBuf[i++]=((getAddr&0xFF00)>>8);
    tmpBuf[i++]=(getAddr&0x00FF);
		tmpBuf[i]=dataLen;
 //   memcpy(&tmpBuf[i],buf,dataLen);
    i++;
    tmpBuf[2]=i-3;
    sendLen=i;

	if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
	{
		memset(tmpBuf,0,sizeof(tmpBuf));
		rcvLen=TouchLinkReadFrame(tmpBuf, 128, 100);
		if(rcvLen>0)
		{
			memcpy(buf,tmpBuf,rcvLen+3);
		    return TRUE;
		}
	}

	return FALSE;
}
char Receivetimeflag=0;


BOOL ResetButtonValue(uint16 setAddr,uint16 setValue)
{
    uint16 sendLen,readValue;
    uint8 tmpBuf[16],i=0;
    memset(tmpBuf,0,sizeof(tmpBuf));
    
    tmpBuf[i++]=0XA5;
    tmpBuf[i++]=0X5A;
    i++;                   //数据长度字节
    tmpBuf[i++]=TOUCH_CMD_WRITE_VARIABLE;
    tmpBuf[i++]=((setAddr&0xFF00)>>8);
    tmpBuf[i++]=(setAddr&0x00FF);
    
    tmpBuf[i++]=((setValue&0xFF00)>>8);
    tmpBuf[i++]=(setValue&0x00FF);
    tmpBuf[2]=i-3;
    sendLen= i;
    if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
    {
        readValue=ReadLCDButton(setAddr); 
        if(readValue==setValue)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WriteCurveValue(uint8 CH_Mode,uint8* buf,uint8 dataLen)
{
	uint16 sendLen,i=0;
	uint8 tmpBuf[256];
	memset(tmpBuf,0,sizeof(tmpBuf));

	tmpBuf[i++]=0XA5;
	tmpBuf[i++]=0X5A;
	i++;                   //数据长度字节
	tmpBuf[i++]=TOUCH_CMD_DRAW_CURVE;
	tmpBuf[i++]=CH_Mode;

	memcpy(&tmpBuf[i],buf,dataLen);
	i+=dataLen;
	tmpBuf[2]=i-3;
	sendLen=i;
	
	if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
	{
			return TRUE;
	}
	return FALSE;
}

BOOL WriteVariableValue(uint16 setAddr,uint8* buf,uint8 dataLen)
{
	uint16 sendLen;
	uint8 tmpBuf[256],i=0;
	memset(tmpBuf,0,sizeof(tmpBuf));

	tmpBuf[i++]=0XA5;
	tmpBuf[i++]=0X5A;
	i++;                   //数据长度字节
	tmpBuf[i++]=TOUCH_CMD_WRITE_VARIABLE;
	tmpBuf[i++]=((setAddr&0xFF00)>>8);
	tmpBuf[i++]=(setAddr&0x00FF);

	memcpy(&tmpBuf[i],buf,dataLen);
	i+=dataLen;
	tmpBuf[2]=i-3;
	sendLen=i;
	if(uart_write(MID_MODBUS_COM,tmpBuf,sendLen) == TRUE) 
	{

			return TRUE;

	}
	return FALSE;
}

void ShowGrdMsgDeal()
{
		uint8 dataBuf[256],len=0;
    uint16 writeAddr;
//	  gUserDB.C_outVolt=223;
	  //gUserDB.C_outCurr=15;
	  //gUserDB.M_thisChargeDegree=50;
	  //gUserDB.M_thisChargeMoney=100; 
	  //gUserDB.M_thisChargeTime=10000;  
	  gUserDB.nowPrice=3;
	
	
		writeAddr=0X0010; 
		//gUserDB.C_outVolt+=10;//for test
		dataBuf[0]=(INT16U)(gUserDB.C_outVolt*10)>>8;
		len++;
		dataBuf[1]=(INT16U)(gUserDB.C_outVolt*10)>>0;
		len++; 
		//g_emeterData.Volt=2200;//for test
		dataBuf[2]=(INT16U)(gUserDB.C_outCurr*10)>>8;
		len++;
		dataBuf[3]=(INT16U)(gUserDB.C_outCurr*10)>>0;
		len++;
		dataBuf[4]=(INT16U)(gUserDB.M_thisChargeDegree*10)>>8;
		len++;
		dataBuf[5]=(INT16U)(gUserDB.M_thisChargeDegree*10)>>0;
		len++;
	  dataBuf[6]=(INT16U)(gUserDB.M_thisChargeMoney*10)>>8;
		len++;
		dataBuf[7]=(INT16U)(gUserDB.M_thisChargeMoney*10)>>0;
		len++;		
		dataBuf[8]=(INT16U)(gUserDB.M_thisChargeTime/3600)>>8;
		len++;
		dataBuf[9]=(INT16U)(gUserDB.M_thisChargeTime/3600)>>0;
		len++;
		dataBuf[10]=(INT16U)(gUserDB.M_thisChargeTime%3600/60)>>8;
		len++;
		dataBuf[11]=(INT16U)(gUserDB.M_thisChargeTime%3600/60)>>0;
		len++;
		dataBuf[12]=(INT16U)(gUserDB.M_thisChargeTime%60)>>8;
		len++;
		dataBuf[13]=(INT16U)(gUserDB.M_thisChargeTime%60)>>0;
		len++;
		dataBuf[14]=(INT16U)(gUserDB.M_meterDegree*10)>>8;
		len++;
		dataBuf[15]=(INT16U)(gUserDB.M_meterDegree*10)>>0;
		len++;
		//gUserDB.nowPrice=80;//for test
		
								
		WriteVariableValue(writeAddr,dataBuf,len);
}

void ShowConsumInfo(void)
{
		uint8 dataBuf[256],len=0;
    uint16 writeAddr;
//	  gUserDB.C_outVolt=223;
//	  gUserDB.C_outCurr=15;
//	  gUserDB.M_thisChargeDegree=50;
//	  gUserDB.M_thisChargeMoney=100; 
//	  gUserDB.M_thisChargeTime=10000;  
//	  gUserDB.nowPrice=3;
	
	
		writeAddr=0X0010; 
		//gUserDB.C_outVolt+=10;//for test
		dataBuf[0]=(INT16U)(gUserDB.C_outVolt*10)>>8;
		len++;
		dataBuf[1]=(INT16U)(gUserDB.C_outVolt*10)>>0;
		len++; 
		//g_emeterData.Volt=2200;//for test
		dataBuf[2]=(INT16U)(gUserDB.C_outCurr*10)>>8;
		len++;
		dataBuf[3]=(INT16U)(gUserDB.C_outCurr*10)>>0;
		len++;
		dataBuf[4]=(INT16U)(gUserDB.M_thisChargeDegree*10)>>8;
		len++;
		dataBuf[5]=(INT16U)(gUserDB.M_thisChargeDegree*10)>>0;
		len++;
	  dataBuf[6]=(INT16U)(gUserDB.M_thisChargeMoney*10)>>8;
		len++;
		dataBuf[7]=(INT16U)(gUserDB.M_thisChargeMoney*10)>>0;
		len++;
		dataBuf[8]=(INT16U)(gUserDB.M_thisChargeTime/3600)>>8;
		len++;
		dataBuf[9]=(INT16U)(gUserDB.M_thisChargeTime/3600)>>0;
		len++;
		dataBuf[10]=(INT16U)(gUserDB.M_thisChargeTime%3600/60)>>8;
		len++;
		dataBuf[11]=(INT16U)(gUserDB.M_thisChargeTime%3600/60)>>0;
		len++;
		dataBuf[12]=(INT16U)(gUserDB.M_thisChargeTime%60)>>8;
		len++;
		dataBuf[13]=(INT16U)(gUserDB.M_thisChargeTime%60)>>0;
		len++;
		//gUserDB.nowPrice=80;//for test
//		dataBuf[14]=gUserDB.nowPrice*10%0x10000/0x100;
//		len++;
//		dataBuf[15]=gUserDB.nowPrice*10%0x100;
//		len++;
//		
//		dataBuf[16]=gUserDB.nowPrice*10%0x10000/0x100;
//		len++;
//		dataBuf[17]=gUserDB.nowPrice*10%0x100;
//		len++;
//		
//		dataBuf[18]=gUserDB.nowPrice*10%0x10000/0x100;
//		len++;
//		dataBuf[19]=gUserDB.nowPrice*10%0x100;
//		len++;
		
		dataBuf[14]=(INT32U)((gUserDB.leftMoney-gUserDB.M_thisChargeMoney)/10)>>24;
		len++;
		dataBuf[15]=(INT32U)((gUserDB.leftMoney-gUserDB.M_thisChargeMoney)/10)>>16;
		len++;
		dataBuf[16]=(INT32U)((gUserDB.leftMoney-gUserDB.M_thisChargeMoney)/10)>>8;
		len++;
		dataBuf[17]=(INT32U)((gUserDB.leftMoney-gUserDB.M_thisChargeMoney)/10)>>0;
		len++;
								
		WriteVariableValue(writeAddr,dataBuf,len);
}

void OnClickNextPage(uint16 ClkID,uint16 PgID)
{
	uint16 readAddr,btnValue;

	readAddr=ClkID;
	btnValue=ReadLCDButton(readAddr);
	if(btnValue==1)
	{				 
		 if(ResetButtonValue(readAddr,0)==TRUE)
		 {
				gUserDB.setPageId=PgID;
		 }
	}
}

void ShowCardLeftMoney()
{
	uint8 dataBuf[20],len=0;
	uint16 writeAddr;
	
	writeAddr=0X0017;
	dataBuf[0]=gUserDB.leftMoney/0x1000000;
	len++;
	dataBuf[1]=gUserDB.leftMoney%0x1000000/0x10000;
	len++;
	dataBuf[2]=gUserDB.leftMoney%0x10000/0x100;
	len++;
	dataBuf[3]=gUserDB.leftMoney%0x100;
	len++;
	dataBuf[4]=currChargeLog.pay_cardID[0];
	len++;
	dataBuf[5]=currChargeLog.pay_cardID[1];
	len++;
	dataBuf[6]=currChargeLog.pay_cardID[2];
	len++;
	dataBuf[7]=currChargeLog.pay_cardID[3];
	len++;
	dataBuf[8]=currChargeLog.pay_cardID[4];
	len++;
	dataBuf[9]=currChargeLog.pay_cardID[5];
	len++;
	dataBuf[10]=currChargeLog.pay_cardID[6];
	len++;
	dataBuf[11]=currChargeLog.pay_cardID[7];
	len++;
	dataBuf[12]=currChargeLog.pay_cardID[8];
	len++;
	dataBuf[13]=currChargeLog.pay_cardID[9];
	len++;
	dataBuf[14]=currChargeLog.pay_cardID[10];
	len++;
	dataBuf[15]=currChargeLog.pay_cardID[11];
	len++;
	dataBuf[16]=currChargeLog.pay_cardID[12];
	len++;
	dataBuf[17]=currChargeLog.pay_cardID[13];
	len++;
	dataBuf[18]=currChargeLog.pay_cardID[14];
	len++;
	dataBuf[19]=currChargeLog.pay_cardID[15];
	len++;

	WriteVariableValue(writeAddr,dataBuf,len);
               
}

/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void StatusLinkPageId(void)
{
    uint8 len=0;
    uint16 readAddr,btnValue,setValue,writeAddr;
    uint16 currentPageId;
		char ChgOrIdleCurve=0;
//		static char StCfgSetFlg1=1,StCfgSetFlg2=1,StCfgSetFlg3=1;	
//		static char FreshTimeFlag=1;
//	  uint16 DevicePage=0;
//    _TIME nowTime;
		static uint8 flag=0;
	  static INT16U timeOutCnt=0;
 
	
		currentPageId=ReadCurrentPageId();//经过处理，页面有变化，立即响应切换显示
//    if(currentPageId!=gUserDB.setPageId)
//    {
//       SetPageId(gUserDB.setPageId); 
//    }	

    switch(currentPageId)
    {
        case PAGE_ID_MAIN: //首页
        {     
		 flag =0;
		 gUserDB.chargeFull =0	;
         gUserDB.userRunRequestFlag=STOP; 	
				 gUserDB.CCS_runCtlFlag=STOP;	
         timeOutCnt=0;	
         gUserDB.user_indicating=USER_CMD_WAIT;					
//                    memcpy(QR_ASCII,"www.kehua.co",12);
//                    Show_QR();
//					if(gUserDB.deviceStatus!=0)
//					{
//						gUserDB.setPageId=PAGE_ID_SYSTEM_FAULT;//有告警，显示告警界面，不允许充电操作。
//						
//						break;
//					}
//						readAddr=0X0001;
//						btnValue=ReadLCDButton(readAddr);
//						if(btnValue==0x01)
//						{                
//							 if(ResetButtonValue(readAddr,0)==TRUE)
//							 {
//									 SendMsgToTask(MSG_ITEM_START_CHECK_CARD,TASK_INDEX_MAIN_CONTROL);
//										 								 
//							 }
//						}
//				
						readAddr=0X0041;
					//	osDelay(100);
						btnValue=ReadLCDButton(readAddr);
						if(btnValue==8008)
						{                
							 if(ResetButtonValue(readAddr,0)==TRUE)
							 {
									gUserDB.setPageId=PAGE_ID_CRGING_MSG;  //密码正确,跳转到充电信息界面
                  SetPageId(gUserDB.setPageId);                                 
							 }
						}
//                        else if (btnValue == 9999)
//                        {
//                            if(ResetButtonValue(readAddr,0)==TRUE)
//							 {
//									gUserDB.crgStatus=AC_STATUS_SYSTEM_MANAGE;
//									gUserDB.setPageId=PAGE_ID_SYSTEM_MANAGE;
//                                    VipUser=1;
//							 }
//                        }
						else if(btnValue!=0&&btnValue!=0xFFFF)
						{
							 if(ResetButtonValue(readAddr,0)==TRUE)
							 {
									gUserDB.setPageId=PAGE_ID_PWD_ERR;  //密码错误界面
                  SetPageId(gUserDB.setPageId);   
							 }							
						}
            break;      		
        }
        case PAGE_ID_RUSH_CARD:  //刷卡
        {
					////for test/////
//					osDelay(2000); 
//          gUserDB.setPageId=PAGE_ID_SELECT_FUN;
					if((gUserDB.user_indicating &0x0f) !=USER_CMD_SEARCH_CARD_REQUEST && 
						(gUserDB.user_indicating&0x0f)  !=USER_CMD_CHARGE_REQUEST)
					{
						gUserDB.user_indicating =USER_CMD_SEARCH_CARD_REQUEST;
					}
					if(gUserDB.userRunRequestFlag==RUN)
					{
					    gUserDB.setPageId=PAGE_ID_INFO_CARD;  //刷卡卡成功,跳转到充电信息界面
              SetPageId(gUserDB.setPageId); 
					}
					
					timeOutCnt++;
					if(timeOutCnt>400)  //超时
					{
						gUserDB.setPageId=PAGE_ID_MAIN;  //刷卡卡成功,跳转到充电信息界面
              SetPageId(gUserDB.setPageId);
            timeOutCnt=0;						
					}
					/////////////////
				//	OnClickHomePage();//主页键
          break;
        }
        case PAGE_ID_INFO_CARD:  //卡号信息
        {
				   ShowCardLeftMoney();//显示卡内余额
					 timeOutCnt++;
					 flag =0;
					 if(timeOutCnt>400)  //超时
					 {
						gUserDB.setPageId=PAGE_ID_MAIN;  //刷卡卡成功,跳转到充电信息界面
                         SetPageId(gUserDB.setPageId); 
						 timeOutCnt=0;
						 
					 }
//					OnClickHomePage();//主页键
					
//					{
//						readAddr=0X0009;
//						btnValue=ReadLCDButton(readAddr);
//						if(btnValue==0x01)
//						{               
//							 if(ResetButtonValue(readAddr,0)==TRUE)
//							 {
//									 gUserDB.crgStyle = CRG_STYLE_IMMEDIATELY;
//									 SendMsgToTask(MSG_ITEM_CRG_STYLE_SUCCESS,TASK_INDEX_MAIN_CONTROL); 
//							 } 
//						}
//						readAddr=0X000A;
//						btnValue=ReadLCDButton(readAddr);
//						if(btnValue!=0x00&&btnValue!=0xFFFF)
//						{ 
//							 gUserDB.delayTime=btnValue;
//							 if(ResetButtonValue(readAddr,0)==TRUE)
//							 {
//									 gUserDB.crgStyle=CRG_STYLE_DELAY;
//									 SendMsgToTask(MSG_ITEM_CRG_STYLE_SUCCESS,TASK_INDEX_MAIN_CONTROL); 
//							 } 
//						}               
//					}
           break;
        }
        
        case PAGE_ID_CRGING_MSG: //充电状态信息
        {   
					//////////显示充电信息/////		
  					ShowGrdMsgDeal();
					if (flag==0  ) //在此只运行一次
					{
					  gUserDB.CCS_runCtlFlag=RUN;
					  flag =1;	 //
					}
					  //gUserDB.userRunRequestFlag=RUN; 
//					//////////////////////////
//					OnClickButton(0X002C,PAGE_ID_CURVE_MSG);//查看曲线

//					ChgOrIdleCurve=1;
//					
           readAddr=0X0041;
					//	osDelay(100);
						btnValue=ReadLCDButton(readAddr);
						if(btnValue==8008)
						{                
							 if(ResetButtonValue(readAddr,0)==TRUE)
							 {
									gUserDB.setPageId=PAGE_ID_CHARGE_INFO;  //密码正确,跳转到充电信息界面
                  SetPageId(gUserDB.setPageId);                                 
							 }
						}
//                        else if (btnValue == 9999)
//                        {
//                            if(ResetButtonValue(readAddr,0)==TRUE)
//							 {
//									gUserDB.crgStatus=AC_STATUS_SYSTEM_MANAGE;
//									gUserDB.setPageId=PAGE_ID_SYSTEM_MANAGE;
//                                    VipUser=1;
//							 }
//                        }
						else if(btnValue!=0&&btnValue!=0xFFFF)
						{
							 if(ResetButtonValue(readAddr,0)==TRUE)
							 {
									gUserDB.setPageId=PAGE_ID_PWD2_ERR;  //密码错误界面
                  SetPageId(gUserDB.setPageId);   
							 }							
						}
					timeOutCnt=0;
					
					break;
        }	
				case PAGE_ID_OVER_RUSH_CARD:  //刷卡
        {
					if((gUserDB.user_indicating &0x0f) !=USER_CMD_END_CARD_REQUEST)
					{
						gUserDB.user_indicating =USER_CMD_END_CARD_REQUEST;
					}
					
					//if(gUserDB.user_indicating==USER_CMD_END_CARD_END)
					if(gUserDB.userRunRequestFlag==STOP)
					{
					    gUserDB.setPageId=PAGE_ID_CHARGE_INFO;
              SetPageId(gUserDB.setPageId); 
					}
          timeOutCnt++;
					if(timeOutCnt>400)  //超时
					{
						gUserDB.setPageId=PAGE_ID_CRGING_MSG;  //刷卡卡成功,跳转到充电信息界面
              SetPageId(gUserDB.setPageId); 
						 timeOutCnt=0;
					}					
					 
					/////////////////
				//	OnClickHomePage();//主页键
          break;
        }
        case PAGE_ID_CHARGE_INFO: //本次消费信息
        {   
					
          gUserDB.userRunRequestFlag=STOP;
          gUserDB.CCS_runCtlFlag=STOP;					
					ShowConsumInfo();//显示消费信息
					timeOutCnt++;
					if(timeOutCnt>400)  //超时
					{
						gUserDB.setPageId=PAGE_ID_MAIN;  //刷卡卡成功,跳转到充电信息界面
              SetPageId(gUserDB.setPageId); 
						 timeOutCnt=0;
					}	
//					//////////////////////////
//					OnClickButton(0X002C,PAGE_ID_CURVE_MSG);//查看曲线

//					ChgOrIdleCurve=1;
//					
//					readAddr=0X002B;
//					btnValue=ReadLCDButton(readAddr);
//					if(btnValue==0x01)
//					{ 
//						 if(ResetButtonValue(readAddr,0)==TRUE)
//						 {
//								 SendMsgToTask(MSG_ITEM_MANUAL_STOP,TASK_INDEX_MAIN_CONTROL); 
//						 } 
//					} 
					
					break;
        }						
				default:
        {
            break;
        }
    }   
//		if(FreshTimeFlag==1) 
//		{
//			FreshTimeFlag=0;
////			OnClickReset();
////			GetTimeSet();
//		}
		
		
}