/*********************************************************************************************************
*                                            头文件包含
*********************************************************************************************************/
#include "moduleCom.h"

/*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************/
#define  COM_BUF_RN   2048   //接收数据的长度
#define  COM_BUF_SN   1024   //发送数据的长度
#define  HANDLE_LEN   6     //（同步头+数据包长度+设备类型 ）字节数
#define  MODULE_REG_MAX 15


#define MODULE_UART     UART_3
//#define MODULE_TX_BUF   (rx3_Buf)
//#define MODULE_RX_BUF   (rx3_Buf)

/*********************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************/	

INT16U getCRC16(INT8U *crcdata, INT8U datalen);
uint8 dataExchange(uint8 * pdat, uint8 len);
uint8  module_releasePackage(uint8* rbuf, uint16 len,uint8 cmd);

/*********************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************/
uint8 RevComBuff[COM_BUF_RN],SendComBuff[COM_BUF_SN];  //注意数据类型
_MODULE_INFO gModuleInfo[30];
uint16 moduleReg[MODULE_REG_MAX];   //15个充电模块寄存器

uint8 ModlueRevComBuff[COM_BUF_RN],ModlueSendComBuff[COM_BUF_SN];  //注意数据类型

uint8 tBuf[1024]={0};
uint8 ABuf[10]={0};
uint8 change_to_acsii(uint8 data);
uint8 change_to_byte(uint8 *data);
void moduleComManage(void);

/*******************************************************************************************
函数名称: dataExchange
描    述: 数据高低位交换
输入参数: pdat 数据缓冲 len 数据长度
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 dataExchange(uint8 * pdat, uint8 len)
{
	uint8 i;
	uint8 tmp;

	for(i=0;i<len/2;i++)
	{	
		tmp=pdat[i];
		pdat[i]=pdat[len-1-i];
		pdat[len-1-i]=tmp;
	}

	return 1;
} 


/*******************************************************************************************
函数名称: makeChecksum
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint16  makeChecksum(uint8* data ,uint16 len)
{
   uint8 *p=data; 
   uint16 tmp=0,i=0;  
	 
	  for(i=0;i<len;i++)
	  {
	 	tmp +=*data++;
	 }
//	 tmp=tmp mod 65536;	 
	 tmp=~tmp;
	 tmp=tmp+1;

   return tmp;
} 

/*******************************************************************************************
函数名称: change_to_acsii
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 change_to_acsii(uint8 data)
{
    uint8 tt;

   if(data<0x0a)
	{
		tt= data +'0';
	}
	else
	{
		tt= data +0x37;
	}

	return tt;

}

/*******************************************************************************************
函数名称: change_to_acsii
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 change_to_HEX(uint8 data)
{
    uint8 tt;

   if(data<'A')
	{
		tt= data -'0';
	}
	else
	{
		tt= data -0x37;
	}

	return tt;

}  /*******************************************************************************************
函数名称: change_to_acsii
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 change_to_byte(uint8 *data)
{
    uint8 t1,t2;

   if(data[0]<'A')
	{
		t1= data[0] -'0';
	}
	else
	{
		t1= data[0] -0x37;
	}

	if(data[1]<'A')
	{
		t2= data[1] -'0';
	}									
	else
	{
		t2= data[1] -0x37;
	}

	return (t1<<4)+t2;

} 

/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  hex_to_acsii(uint8 *sbuf,uint8* data ,uint8 len)
{
   uint8 *p=sbuf,i=0,tmp; 

      
	 
	  for(i=0;i<len;i++)
	  {
	 	tmp=(*data>>4);
		*p=change_to_acsii(tmp);
	 	p++;

	 	tmp=(*data&0x0f);
		*p=change_to_acsii(tmp);
		p++;
		data++;
	 }	 
} 

/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 make_2_to_7_Package_hex(uint8 *sbuf,uint8 adr,uint8 CID2,uint8 dataLen, uint8 *data)
{
  uint8 i,*p=sbuf;
  uint16 infoLen,tmp=0,lenchk;
  uint8 tmpbuf[5]={0};	

  //*p++=0x7e;
  *p++=0x20;
  *p++=adr;
  *p++=0x41;
  *p++=CID2;

  tmp=dataLen*2;
//  hex_to_acsii(&tmpbuf[1], (uint8 *)tmp,2);	
  
  tmpbuf[0]=tmp>>8 &0x0f;	       //长度的HEX高低位交换
  tmpbuf[1]=tmp>>4 &0x0f;
  tmpbuf[2]=tmp &0x0f;
   
   memcpy(ABuf,tmpbuf,4);
  lenchk=makeChecksum(ABuf,3);
  *p++= (lenchk<<4)+tmpbuf[0];		            //长度的校验码
  *p++= (tmpbuf[1]<<4)+tmpbuf[2];				//长度的HEX	 
  

  if(dataLen>0)
  for(i=0;i<dataLen;i++)
  {
	*p++= data[i];
  }  

   return  (6+ dataLen);
}


/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 make_modluePackage_CMD(uint8 * sbuf, uint8 adr,uint8 CID2,uint8 dataLen, uint8 *data)
{   
   uint8 *p=sbuf,len;
  // uint8 tmpbuf[100]={0};
   uint16 chk;

   *sbuf++=0x7E;
     
   len=make_2_to_7_Package_hex(tBuf,adr,CID2,dataLen,data);	   //组包协议2~7字段的HEX
   hex_to_acsii(sbuf,tBuf,len);   			               //协议2~7字段的acsii
   sbuf=sbuf+2*len;

   chk=	makeChecksum(&p[1],2*len);
   *sbuf++=change_to_acsii(chk>>12 &0x0f);			                           //协议checksum字段的acsii
   *sbuf++=change_to_acsii(chk>>8 &0x0f);
   *sbuf++=change_to_acsii(chk>>4 &0x0f);
   *sbuf++=	change_to_acsii(chk&0x0f);
   *sbuf++=	0x0D ; 

 //  change_to_HEX(sbuf);
   
	
  return  (2*len+6);	                                       //返回总命令长度
}
 
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 get_module_analog(uint8 adr)
{
     uint8 tmpbuf[4]={0};
	 uint8 ret,CID2=0X41;
	 uint16 len=100;

    len=make_modluePackage_CMD(ModlueSendComBuff,adr,GET_ANALOG_CMD,0,tmpbuf);
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	
	len=uart_read(MODULE_UART,ModlueRevComBuff,1000);
	if(len>8)
	{
	module_releasePackage(ModlueRevComBuff,len,GET_ANALOG_CMD);
	}

}
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 get_status_CMD(uint8 adr)
{
     uint8 tmpbuf[4]={0};
	 uint16 len,ret;

    len=make_modluePackage_CMD(ModlueSendComBuff,adr,GET_STATUS_CMD,0,tmpbuf);
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	
	len=uart_read(MODULE_UART,ModlueRevComBuff,1000);
	module_releasePackage(ModlueRevComBuff,len,GET_STATUS_CMD);
}
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 get_alarm_CMD(uint8 adr)
{
     uint8 tmpbuf[4]={0};
	  uint16 len,ret;

    len=make_modluePackage_CMD(ModlueSendComBuff,adr,GET_ALARM_CMD,0,tmpbuf); 
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	
	len=uart_read(MODULE_UART,ModlueRevComBuff,1000);
	module_releasePackage(ModlueRevComBuff,len,GET_ALARM_CMD);
}
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 ctl_CMD(uint8 devAdr,uint8 moduleAdr,uint8 onoff,uint8 ac_flag)	//开关模块
{
     //uint8 databuf[5]={0};
	 uint8 len,ret=0;

	 if(onoff==1)
	 {
      if(ac_flag==AC_MODULE)		            //交流
	    SendComBuff[0]=0xE5;
	  else
	   SendComBuff[0]=0x20;

	 }
	 else
	 {
      if(ac_flag==AC_MODULE)		            //交流
	    SendComBuff[0]=0xE6;
      else
		SendComBuff[0]=0x2F;

	 } 
	 
	 SendComBuff[1]=moduleAdr;  

    len=make_modluePackage_CMD(ModlueSendComBuff,devAdr,CTL_CMD,2,SendComBuff);
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	
	len=uart_read(MODULE_UART,ModlueRevComBuff,200);
	ret=module_releasePackage(ModlueRevComBuff,len,CTL_CMD);
	if(ret!=0)
	{
	  return 0;
	}

return 1;		   
}
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 set_curr_CMD(uint8 devAdr,uint8 moduleAdr,fp32 curr)  //设置模块电压电流
{
    // uint8 databuf[10]={0};
	 uint8 len,ret;
	 uint32 tmp;

	 curr = curr /10*100;
	 memcpy((uint8 *)&tmp,(uint8*)&curr,4);

	 SendComBuff[0]=0XE0;
	 SendComBuff[1]=moduleAdr;
	 SendComBuff[5]=(uint32)tmp>>24 &0xff;
	 SendComBuff[4]=(uint32)tmp>>16 &0xff;
	 SendComBuff[3]=(uint32)tmp>>8 &0xff;
	 SendComBuff[2]=(uint32)tmp &0xff;

    len=make_modluePackage_CMD(ModlueSendComBuff,devAdr,REMOTE_CMD,6,SendComBuff);
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	
	len=uart_read(MODULE_UART,ModlueRevComBuff,200);
	module_releasePackage(ModlueRevComBuff,len,REMOTE_CMD);
}
/*******************************************************************************************
函数名称: getModlueAnalogData_send_CMD
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 set_volt_CMD(uint8 devAdr,uint8 moduleAdr,fp32 volt)  //设置模块电压电流
{
     
	 uint8 len,ret,CID2=0XE1;
	 uint32 tmp;
//	 uint8 Dbuf[10]={0};

     memcpy((uint8 *)&tmp,(uint8*)&volt,4);

	 SendComBuff[0]=0xE1;
	 SendComBuff[1]=moduleAdr;
	 SendComBuff[5]=(uint32)tmp>>24 &0xff;
	 SendComBuff[4]=(uint32)tmp>>16 &0xff;
	 SendComBuff[3]=(uint32)tmp>>8 &0xff;
	 SendComBuff[2]=(uint32)tmp &0xff;

    len=make_modluePackage_CMD(ModlueSendComBuff,devAdr,REMOTE_CMD,6,SendComBuff);
	ret=uart_write(MODULE_UART,ModlueSendComBuff,len);
	len=uart_read(MODULE_UART,ModlueRevComBuff,200);
	module_releasePackage(ModlueRevComBuff,len,REMOTE_CMD);
}

/*******************************************************************************************
函数名称: module_releasePackage
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8  module_releasePackage(uint8* pbuf, uint16 len,uint8 cmd)
{
   uint8  ret,tmp,k=0;
   uint8  CID1,CID2;
   uint16 chk,i,j;
   uint32 length,cheksum2,dat32;
   uint8* rbuf;	

   
  // uint8  tmpbuf[100]={0};
   FP32   fp=652.048,fp2;

   for(j=0;j<5;j++)
   {
     if (*pbuf!=0x7E)
      { 
	    pbuf++;	
		len--;		
	  }
	  else
	    break;
	}
  rbuf=pbuf;

  if(rbuf[0] !=0x7E || rbuf[1] !='2' || rbuf[2] !='0' || rbuf[len-1] !=0x0D   )		//SOI ,EOI,VER是否正确
  {
     return 0;
  }

  if( rbuf[3] !='0' || rbuf[4] !=gUserDB.dev_adr+0x30   )    //地址是否正确
  {
     return 0;
  }
   tmp=change_to_HEX(rbuf[5]);
   tmp=change_to_HEX(rbuf[6]);
   CID1= (change_to_HEX(rbuf[5])<<4)+ (change_to_HEX(rbuf[6]));	
   CID2= (change_to_HEX(rbuf[7])<<4)+ (change_to_HEX(rbuf[8]));
   if(CID1 !=0x41 || CID2 !=0x00  )				 //CID1，CID2是否正确
     return 0;

   chk=(change_to_HEX(rbuf[9]));
   length= (change_to_HEX(rbuf[10])<<8)+(change_to_HEX(rbuf[11])<<4)+(change_to_HEX(rbuf[12]));	     //长度校验码是否正确
   ABuf[0]=length&0xf;
   ABuf[1]=(length>>4)&0xf;
   ABuf[2]=(length>>8)&0xf;
   tmp=	makeChecksum(ABuf,4)&0x0f;   
   if(chk != tmp)
   {
      return 0;
   }  

   cheksum2 = (change_to_HEX(rbuf[len-5])<<12)+(change_to_HEX(rbuf[len-4])<<8)+(change_to_HEX(rbuf[len-3])<<4)+(change_to_HEX(rbuf[len-2])); 	   //校验码是否正确
   if( cheksum2 != makeChecksum((uint8*)&rbuf[1],len-6))	  
    return 0;  
  

  for(i=0;i<length/2;i++)	      //转换数据内容
   {
     tBuf[i]= change_to_byte((INT8U*)&rbuf[13+i*2]);
   }


//  if( gUserDB.dev_adr==1)
//  {
//	      k=0;
//   }
//   else
//	{
//	  	k=13;
//	}

  switch(cmd)               //数据信息解释
  {
   case GET_ANALOG_CMD:			 //模拟量

	  gUserDB.moduleNornalSum= tBuf[5];   //模块数量
	  //dat32=((tBuf[4]<<24)+(tBuf[3]<<16)+(tBuf[2]<<8)+tBuf[1]);	 
	  //memcpy((INT8U*)&gModuleInfo[0].outVolt,(INT8U*)&dat32,4);	  	 
	  
	  for(i=0;i<gUserDB.moduleNornalSum;i++)
	  {
      	dat32=((tBuf[9+33*i]<<24)+(tBuf[8+33*i]<<16)+(tBuf[7+33*i]<<8)+tBuf[6+33*i]);	    //模块电流 
	  	memcpy((INT8U*)&gModuleInfo[k+i].outCurr,(INT8U*)&dat32,4);	 
								  
	  	dat32=((tBuf[14]<<24)+(tBuf[13+33*i]<<16)+(tBuf[12+33*i]<<8)+tBuf[11+33*i]);	 //模块温度
	  	memcpy((INT8U*)&gModuleInfo[k+i].moduleTemp,(INT8U*)&dat32,4);	 
							
	  	dat32=((tBuf[18+33*i]<<24)+(tBuf[17+33*i]<<16)+(tBuf[16+33*i]<<8)+tBuf[15+33*i]);	 //模块限流点 
	  	memcpy((INT8U*)&gModuleInfo[k+i].currPiont,(INT8U*)&dat32,4);	
	  
	  	dat32=((tBuf[22+33*i]<<24)+(tBuf[21+33*i]<<16)+(tBuf[20+33*i]<<8)+tBuf[19+33*i]);	 //模块输出电压
	  	memcpy((INT8U*)&gModuleInfo[k+i].outVolt,(INT8U*)&dat32,4);	 
	  } 
	  
	  //gModuleInfo[0].outCurr=(tmpbuf[0]<<24)+(tmpbuf[1]<<16)+(tmpbuf[2]<<8)+tmpbuf[3];
	 // gModuleInfo[0].outCurr=(tmpbuf[0]<<24)+(tmpbuf[1]<<16)+(tmpbuf[2]<<8)+tmpbuf[3];
      break;
   case GET_STATUS_CMD:		 //启动状态
      gUserDB.moduleNornalSum= tBuf[1];   //模块数量
	  for(i=0;i<gUserDB.moduleNornalSum;i++)
	  {
	       if(tBuf[2+9*i]==1)
              gModuleInfo[k+i].runStatus=0;
		   else
		      gModuleInfo[k+i].runStatus=1;
	  }
	  //gModuleInfo[0].moduleTemp=(tmpbuf[6]<<24)+(tmpbuf[7]<<16)+(tmpbuf[8]<<8)+tmpbuf[9];
	  
      break;
   case GET_ALARM_CMD:
      gUserDB.moduleNornalSum= tBuf[1];  //模块数量
	  for(i=0;i<gUserDB.moduleNornalSum;i++)
	  {
      	gModuleInfo[k+i].moduleFail=tBuf[2+9*i];
		if(gModuleInfo[k+i].moduleFail==0xe2)
		{
			gModuleInfo[k+i].comAlarm=ALARM;
		}
		else
		{
			gModuleInfo[k+i].comAlarm=NOALARM;
		}
	  }
					  
	 // tmpbuf[7];    //用户自定义
	  //gModuleInfo[0].moduleFail=tmpbuf[2];
	  //gModuleInfo[0].moduleFail=tmpbuf[2];
	  //gModuleInfo[0].moduleTemp=(tmpbuf[6]<<24)+(tmpbuf[7]<<16)+(tmpbuf[8]<<8)+tmpbuf[9];
      break;
   case CTL_CMD:
      break;
   case REMOTE_CMD:
      break;
  }

 return 1;
}

/*********************************************************************************************************************
函数名称: 	moduleCurrReset          
功能描述: 	模块电流重新分配与启动模式策略
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void moduleStartTatol_strategy(FP32 setOutCurr)
{

    INT8U k=0,i,n;
	FP32 tmp1;	

	n=gUserDB.CCS_setOutCurr/gUserDB.moduleCurrMax;
	tmp1=gUserDB.CCS_setOutCurr/gUserDB.moduleCurrMax;
	if(tmp1>n)
	 	gUserDB.startModuleSum= n+1;
	else
		gUserDB.startModuleSum= n; 				

}
 /*********************************************************************************************************************
函数名称: 	moduleComManage          
功能描述: 	模块通信管理
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void moduleComManage(void)
{
	INT8U i,k;
	static INT8U loopCnt=0, runFlg=0xff;
	static fp32  lastSetVolt,lastSetCurr,tmpFp;
	static INT8U lastRunFlag=0xff;	
	fp32  uSetCurr,uSetVolt;

 
			   
	get_module_analog(gUserDB.dev_adr); 
	OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 

	get_status_CMD(gUserDB.dev_adr);
	OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 

	get_alarm_CMD(gUserDB.dev_adr);
	OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 

	
	moduleStartTatol_strategy(gUserDB.CCS_setOutCurr);	   //模块启动数量策略
	if(gUserDB.startModuleSum>0)                           //启动模块数大于0时
    { 
		tmpFp = gUserDB.CCS_setOutCurr /gUserDB.startModuleSum;		//平均分配每个模块电流
		 if( tmpFp> gUserDB.moduleCurrMax)
		 {
		 	tmpFp= gUserDB.moduleCurrMax;
		 }
		 uSetCurr=tmpFp;							   //得出每个模块电流 
	}
	else
		uSetCurr=0;

//	if(gUserDB.CCS_setOutVolt != lastSetVolt && gUserDB.moduleNornalSum>0)	//设置模块电压
    {      
		 tmpFp = gUserDB.CCS_setOutVolt;
		 if(tmpFp>gUserDB.CCS_outVoltMax)
		 {
		    
			tmpFp=gUserDB.CCS_outVoltMax;
		 }
		 uSetVolt=tmpFp;						 //得出每个模块电压	 

	}

	//if(lastRunFlag !=gUserDB.CCS_runCtlFlag && gUserDB.moduleNornalSum>0)	 //启动模块
    {        
         for(i=0;i<MODULE_SUM;i++ )
		 {	
		    if(i<gUserDB.startModuleSum)                              //模块启动控制
		    	gModuleInfo[i ].onoffCtl=gUserDB.CCS_runCtlFlag;
			else
				gModuleInfo[i ].onoffCtl=STOP;

			gModuleInfo[i ].setVolt=uSetVolt;		                 //设置模块电压
			gModuleInfo[i ].setCurr=uSetCurr;						 //设置模块电流
			k=i+1;	

			set_curr_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].setCurr);	  //下发模块电流命令
			OSTimeDlyHMSM(0,0,0,100);				 

			set_volt_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].setVolt);	  //下发模块电压命令
		 	OSTimeDlyHMSM(0,0,0,100);	

			if(gModuleInfo[i ].onoffCtl==RUN)	  //启动时，直流交流都开
			{
				ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,AC_MODULE);
				OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 
			
					    
		 		ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,DC_MODULE);			   
		 		OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 
			}
			else  //关闭时只关直流
			{
				ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,AC_MODULE);			   
		 		OSTimeDlyHMSM(0,0,0,100);				//1S运行一次
			}

			get_module_analog(gUserDB.dev_adr); 
	        OSTimeDlyHMSM(0,0,0,100);				//1S运行一次 
		 }
	 }
	
}

 /*********************************************************************************************************************
函数名称: 	moduleComManage          
功能描述: 	模块通信管理
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void task_moduleComManage(void *pdata)			//MODBUS任务
{
  OSTimeDlyHMSM(0,0,3,0);				   //2S延时
  while(1)
  {
   	moduleComManage();
	OSTimeDlyHMSM(0,0,0,200);				//1S运行一次
   }
}

