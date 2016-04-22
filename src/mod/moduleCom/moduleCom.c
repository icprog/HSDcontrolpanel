/*********************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************/
#include "moduleCom.h"

/*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************/
#define  COM_BUF_RN   2048   //�������ݵĳ���
#define  COM_BUF_SN   1024   //�������ݵĳ���
#define  HANDLE_LEN   6     //��ͬ��ͷ+���ݰ�����+�豸���� ���ֽ���
#define  MODULE_REG_MAX 15


#define MODULE_UART     UART_3
//#define MODULE_TX_BUF   (rx3_Buf)
//#define MODULE_RX_BUF   (rx3_Buf)

/*********************************************************************************************************
*                                            ������������������
*********************************************************************************************************/	

INT16U getCRC16(INT8U *crcdata, INT8U datalen);
uint8 dataExchange(uint8 * pdat, uint8 len);
uint8  module_releasePackage(uint8* rbuf, uint16 len,uint8 cmd);

/*********************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************/
uint8 RevComBuff[COM_BUF_RN],SendComBuff[COM_BUF_SN];  //ע����������
_MODULE_INFO gModuleInfo[30];
uint16 moduleReg[MODULE_REG_MAX];   //15�����ģ��Ĵ���

uint8 ModlueRevComBuff[COM_BUF_RN],ModlueSendComBuff[COM_BUF_SN];  //ע����������

uint8 tBuf[1024]={0};
uint8 ABuf[10]={0};
uint8 change_to_acsii(uint8 data);
uint8 change_to_byte(uint8 *data);
void moduleComManage(void);

/*******************************************************************************************
��������: dataExchange
��    ��: ���ݸߵ�λ����
�������: pdat ���ݻ��� len ���ݳ���
�������: ��
��    ��: ��
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
��������: makeChecksum
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: change_to_acsii
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: change_to_acsii
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: change_to_acsii
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
  
  tmpbuf[0]=tmp>>8 &0x0f;	       //���ȵ�HEX�ߵ�λ����
  tmpbuf[1]=tmp>>4 &0x0f;
  tmpbuf[2]=tmp &0x0f;
   
   memcpy(ABuf,tmpbuf,4);
  lenchk=makeChecksum(ABuf,3);
  *p++= (lenchk<<4)+tmpbuf[0];		            //���ȵ�У����
  *p++= (tmpbuf[1]<<4)+tmpbuf[2];				//���ȵ�HEX	 
  

  if(dataLen>0)
  for(i=0;i<dataLen;i++)
  {
	*p++= data[i];
  }  

   return  (6+ dataLen);
}


/*******************************************************************************************
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 make_modluePackage_CMD(uint8 * sbuf, uint8 adr,uint8 CID2,uint8 dataLen, uint8 *data)
{   
   uint8 *p=sbuf,len;
  // uint8 tmpbuf[100]={0};
   uint16 chk;

   *sbuf++=0x7E;
     
   len=make_2_to_7_Package_hex(tBuf,adr,CID2,dataLen,data);	   //���Э��2~7�ֶε�HEX
   hex_to_acsii(sbuf,tBuf,len);   			               //Э��2~7�ֶε�acsii
   sbuf=sbuf+2*len;

   chk=	makeChecksum(&p[1],2*len);
   *sbuf++=change_to_acsii(chk>>12 &0x0f);			                           //Э��checksum�ֶε�acsii
   *sbuf++=change_to_acsii(chk>>8 &0x0f);
   *sbuf++=change_to_acsii(chk>>4 &0x0f);
   *sbuf++=	change_to_acsii(chk&0x0f);
   *sbuf++=	0x0D ; 

 //  change_to_HEX(sbuf);
   
	
  return  (2*len+6);	                                       //�����������
}
 
/*******************************************************************************************
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 ctl_CMD(uint8 devAdr,uint8 moduleAdr,uint8 onoff,uint8 ac_flag)	//����ģ��
{
     //uint8 databuf[5]={0};
	 uint8 len,ret=0;

	 if(onoff==1)
	 {
      if(ac_flag==AC_MODULE)		            //����
	    SendComBuff[0]=0xE5;
	  else
	   SendComBuff[0]=0x20;

	 }
	 else
	 {
      if(ac_flag==AC_MODULE)		            //����
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 set_curr_CMD(uint8 devAdr,uint8 moduleAdr,fp32 curr)  //����ģ���ѹ����
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
��������: getModlueAnalogData_send_CMD
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 set_volt_CMD(uint8 devAdr,uint8 moduleAdr,fp32 volt)  //����ģ���ѹ����
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
��������: module_releasePackage
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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

  if(rbuf[0] !=0x7E || rbuf[1] !='2' || rbuf[2] !='0' || rbuf[len-1] !=0x0D   )		//SOI ,EOI,VER�Ƿ���ȷ
  {
     return 0;
  }

  if( rbuf[3] !='0' || rbuf[4] !=gUserDB.dev_adr+0x30   )    //��ַ�Ƿ���ȷ
  {
     return 0;
  }
   tmp=change_to_HEX(rbuf[5]);
   tmp=change_to_HEX(rbuf[6]);
   CID1= (change_to_HEX(rbuf[5])<<4)+ (change_to_HEX(rbuf[6]));	
   CID2= (change_to_HEX(rbuf[7])<<4)+ (change_to_HEX(rbuf[8]));
   if(CID1 !=0x41 || CID2 !=0x00  )				 //CID1��CID2�Ƿ���ȷ
     return 0;

   chk=(change_to_HEX(rbuf[9]));
   length= (change_to_HEX(rbuf[10])<<8)+(change_to_HEX(rbuf[11])<<4)+(change_to_HEX(rbuf[12]));	     //����У�����Ƿ���ȷ
   ABuf[0]=length&0xf;
   ABuf[1]=(length>>4)&0xf;
   ABuf[2]=(length>>8)&0xf;
   tmp=	makeChecksum(ABuf,4)&0x0f;   
   if(chk != tmp)
   {
      return 0;
   }  

   cheksum2 = (change_to_HEX(rbuf[len-5])<<12)+(change_to_HEX(rbuf[len-4])<<8)+(change_to_HEX(rbuf[len-3])<<4)+(change_to_HEX(rbuf[len-2])); 	   //У�����Ƿ���ȷ
   if( cheksum2 != makeChecksum((uint8*)&rbuf[1],len-6))	  
    return 0;  
  

  for(i=0;i<length/2;i++)	      //ת����������
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

  switch(cmd)               //������Ϣ����
  {
   case GET_ANALOG_CMD:			 //ģ����

	  gUserDB.moduleNornalSum= tBuf[5];   //ģ������
	  //dat32=((tBuf[4]<<24)+(tBuf[3]<<16)+(tBuf[2]<<8)+tBuf[1]);	 
	  //memcpy((INT8U*)&gModuleInfo[0].outVolt,(INT8U*)&dat32,4);	  	 
	  
	  for(i=0;i<gUserDB.moduleNornalSum;i++)
	  {
      	dat32=((tBuf[9+33*i]<<24)+(tBuf[8+33*i]<<16)+(tBuf[7+33*i]<<8)+tBuf[6+33*i]);	    //ģ����� 
	  	memcpy((INT8U*)&gModuleInfo[k+i].outCurr,(INT8U*)&dat32,4);	 
								  
	  	dat32=((tBuf[14]<<24)+(tBuf[13+33*i]<<16)+(tBuf[12+33*i]<<8)+tBuf[11+33*i]);	 //ģ���¶�
	  	memcpy((INT8U*)&gModuleInfo[k+i].moduleTemp,(INT8U*)&dat32,4);	 
							
	  	dat32=((tBuf[18+33*i]<<24)+(tBuf[17+33*i]<<16)+(tBuf[16+33*i]<<8)+tBuf[15+33*i]);	 //ģ�������� 
	  	memcpy((INT8U*)&gModuleInfo[k+i].currPiont,(INT8U*)&dat32,4);	
	  
	  	dat32=((tBuf[22+33*i]<<24)+(tBuf[21+33*i]<<16)+(tBuf[20+33*i]<<8)+tBuf[19+33*i]);	 //ģ�������ѹ
	  	memcpy((INT8U*)&gModuleInfo[k+i].outVolt,(INT8U*)&dat32,4);	 
	  } 
	  
	  //gModuleInfo[0].outCurr=(tmpbuf[0]<<24)+(tmpbuf[1]<<16)+(tmpbuf[2]<<8)+tmpbuf[3];
	 // gModuleInfo[0].outCurr=(tmpbuf[0]<<24)+(tmpbuf[1]<<16)+(tmpbuf[2]<<8)+tmpbuf[3];
      break;
   case GET_STATUS_CMD:		 //����״̬
      gUserDB.moduleNornalSum= tBuf[1];   //ģ������
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
      gUserDB.moduleNornalSum= tBuf[1];  //ģ������
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
					  
	 // tmpbuf[7];    //�û��Զ���
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
��������: 	moduleCurrReset          
��������: 	ģ��������·���������ģʽ����
�䡡��:   	��

�䡡��:   	��                 
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
��������: 	moduleComManage          
��������: 	ģ��ͨ�Ź���
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void moduleComManage(void)
{
	INT8U i,k;
	static INT8U loopCnt=0, runFlg=0xff;
	static fp32  lastSetVolt,lastSetCurr,tmpFp;
	static INT8U lastRunFlag=0xff;	
	fp32  uSetCurr,uSetVolt;

 
			   
	get_module_analog(gUserDB.dev_adr); 
	OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 

	get_status_CMD(gUserDB.dev_adr);
	OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 

	get_alarm_CMD(gUserDB.dev_adr);
	OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 

	
	moduleStartTatol_strategy(gUserDB.CCS_setOutCurr);	   //ģ��������������
	if(gUserDB.startModuleSum>0)                           //����ģ��������0ʱ
    { 
		tmpFp = gUserDB.CCS_setOutCurr /gUserDB.startModuleSum;		//ƽ������ÿ��ģ�����
		 if( tmpFp> gUserDB.moduleCurrMax)
		 {
		 	tmpFp= gUserDB.moduleCurrMax;
		 }
		 uSetCurr=tmpFp;							   //�ó�ÿ��ģ����� 
	}
	else
		uSetCurr=0;

//	if(gUserDB.CCS_setOutVolt != lastSetVolt && gUserDB.moduleNornalSum>0)	//����ģ���ѹ
    {      
		 tmpFp = gUserDB.CCS_setOutVolt;
		 if(tmpFp>gUserDB.CCS_outVoltMax)
		 {
		    
			tmpFp=gUserDB.CCS_outVoltMax;
		 }
		 uSetVolt=tmpFp;						 //�ó�ÿ��ģ���ѹ	 

	}

	//if(lastRunFlag !=gUserDB.CCS_runCtlFlag && gUserDB.moduleNornalSum>0)	 //����ģ��
    {        
         for(i=0;i<MODULE_SUM;i++ )
		 {	
		    if(i<gUserDB.startModuleSum)                              //ģ����������
		    	gModuleInfo[i ].onoffCtl=gUserDB.CCS_runCtlFlag;
			else
				gModuleInfo[i ].onoffCtl=STOP;

			gModuleInfo[i ].setVolt=uSetVolt;		                 //����ģ���ѹ
			gModuleInfo[i ].setCurr=uSetCurr;						 //����ģ�����
			k=i+1;	

			set_curr_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].setCurr);	  //�·�ģ���������
			OSTimeDlyHMSM(0,0,0,100);				 

			set_volt_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].setVolt);	  //�·�ģ���ѹ����
		 	OSTimeDlyHMSM(0,0,0,100);	

			if(gModuleInfo[i ].onoffCtl==RUN)	  //����ʱ��ֱ����������
			{
				ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,AC_MODULE);
				OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 
			
					    
		 		ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,DC_MODULE);			   
		 		OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 
			}
			else  //�ر�ʱֻ��ֱ��
			{
				ctl_CMD(gUserDB.dev_adr,k,gModuleInfo[i ].onoffCtl,AC_MODULE);			   
		 		OSTimeDlyHMSM(0,0,0,100);				//1S����һ��
			}

			get_module_analog(gUserDB.dev_adr); 
	        OSTimeDlyHMSM(0,0,0,100);				//1S����һ�� 
		 }
	 }
	
}

 /*********************************************************************************************************************
��������: 	moduleComManage          
��������: 	ģ��ͨ�Ź���
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void task_moduleComManage(void *pdata)			//MODBUS����
{
  OSTimeDlyHMSM(0,0,3,0);				   //2S��ʱ
  while(1)
  {
   	moduleComManage();
	OSTimeDlyHMSM(0,0,0,200);				//1S����һ��
   }
}

