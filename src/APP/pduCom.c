// pduCom.cpp : 实现文件
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "pduCom.h"
#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<stdarg.h>

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define RXBUF_MAX		300


/********************************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************************/
uint8	rx2_Buf[RXBUF_MAX];
uint16  pdu_rxdata_len=0;
uint8	*pRx2_Buf=rx2_Buf;
uint8	rx2_tBuf[RXBUF_MAX];

#define PDU_UART		UART_3 
#define PDU_RX_BUF		rx2_Buf 
#define PDU_TX_BUF		rx2_Buf 

PDU_INFO pdu_info;

uint8 pduAdr[6]={0x00,0x00,0x00,0x00,0x00,0x00};	//PDU地址初始化

/*******************************************************************************************
函数名称: CpduComInit
描    述: PDU参数初始化
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void CpduComInit(void)
{
	pdu_info.A_curr=0; //数据初始化
	pdu_info.B_curr=0;
	pdu_info.C_curr=0;
	pdu_info.A_volt=0;
	pdu_info.B_volt=0;
	pdu_info.C_volt=0;
	pdu_info.power_total=0;
	memcpy(pdu_info.addr,pduAdr,6);
}

/*******************************************************************************************
函数名称: pdu_Write
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_Write(uint8 * sbuf, uint16 * plen , uint8  cmd)
{
	uint8 ret=0,i=0;
	PDU_FRAME  pdu_frame;

	memset((uint8 *)&pdu_frame,sizeof(PDU_FRAME),0);

	pdu_frame.stx1=PDU_STX;
	pdu_frame.stx2=PDU_STX;
	pdu_frame.etx=PDU_ETX;
	memcpy(pdu_frame.padr,pdu_info.addr,6);
	

	switch(cmd)
	{
	case POWER_TOTAL_CMD:	//读电能总电量
		pdu_frame.ctl=M_R_DATA_CODE;						//控制码
		pdu_frame.len=0x04;									//数据长度
		pdu_frame.pdata[0]=(uint8)(POSITIVE_POWER_TOTAL_F>>0);
		pdu_frame.pdata[1]=(uint8)(POSITIVE_POWER_TOTAL_F>>8);
		pdu_frame.pdata[2]=(uint8)(POSITIVE_POWER_TOTAL_F>>16);
		pdu_frame.pdata[3]=(uint8)(POSITIVE_POWER_TOTAL_F>>24);
		
		break;
	case A_VOLT_CMD:	//读A相电压
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(A_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(A_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(A_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(A_VOLT_F>>24);
		break;
	case B_VOLT_CMD:	 //读B相电压
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(B_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(B_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(B_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(B_VOLT_F>>24);
		break;
	case C_VOLT_CMD:	//读C相电压
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(C_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(C_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(C_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(C_VOLT_F>>24);
		break;
	case A_CURR_CMD:	//读A相电流
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(A_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(A_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(A_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(A_CURR_F>>24);
		break;
	case B_CURR_CMD:	//读B相电流
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(B_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(B_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(B_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(B_CURR_F>>24);
		break;
	case C_CURR_CMD:	//读C相电流
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(C_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(C_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(C_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(C_CURR_F>>24);
		break;
	case R_ADDR_CMD:	//读电表地址
		memset(pdu_frame.padr,0xaa,6);	//读电表地址为0XAAAAAAAAAAAA
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=0X01;  //34 37 33 37
		pdu_frame.pdata[1]=0x04;
		pdu_frame.pdata[2]=0x00;
		pdu_frame.pdata[3]=0x04;
		break;
	default:
		break;
	}

	pdu_makePackage(sbuf,plen,&pdu_frame); //命令组包
	
//	/***************1997规约*****************/
//	sbuf[0]=0xfe;
//	sbuf[1]=0xfe;
//	sbuf[2]=0xfe;	
//	sbuf[3]=0x68;
//	
//	sbuf[4]=pdu_info.addr[0];      //电表地址低位
//	sbuf[5]=pdu_info.addr[1];
//	sbuf[6]=pdu_info.addr[2];
//	sbuf[7]=pdu_info.addr[3];
//	sbuf[8]=pdu_info.addr[4];
//	sbuf[9]=pdu_info.addr[5];     //电表地址高位
//	
//	sbuf[10]=0x68;
//	sbuf[11]=0x01;
//	sbuf[12]=0x02;
//	sbuf[13]=0x43;
//	sbuf[14]=0xc3;
//	sbuf[15]=0xd9;
//	sbuf[16]=0x16;
//	
//	*plen =17;
//	/***************1997规约*****************/

	return ret;
}

/*******************************************************************************************
函数名称: makePackage
描    述: 充电机数据组包
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_makePackage(uint8 * sbuf, uint16 *len , PDU_FRAME * pFrame)
{
	uint8 ret=1;
	uint8 t=0;
	uint8 *pbuf=&sbuf[0];

	t =0;
	memset(pbuf,0xfe,4);								//传输前加四个0XFE传导字
	t +=4;
	pbuf += 4;	
	*pbuf =PDU_STX;										//起始位
	t++;
	pbuf++;
	memcpy(pbuf,pFrame->padr,6);					//地址
	t +=6;
	pbuf +=6;
	*pbuf =PDU_STX;									//起始位
	t++;
	pbuf++;
	*pbuf =pFrame->ctl;								//控制码
	t++;
	pbuf++;
	*pbuf =pFrame->len;								//数据长度
	t++;
	pbuf++;
	if(pFrame->len >0)
	{
		memcpy(pbuf,pFrame->pdata,pFrame->len);		//拷贝数据域
		add33H(pbuf,pFrame->len);					//数据域加0X33
		t +=pFrame->len;
		pbuf += pFrame->len;
	}

	pFrame->cs=get_dataCheck(&sbuf[4],t-4);				   //计算校验码
	//pbuf += t;
	*pbuf ++=pFrame->cs;									//校验码
	t++;
	*pbuf ++=PDU_ETX;									//结束位
	t++;

	*len=t;

	return ret;
}

/*******************************************************************************************
函数名称: get_dataCheck
描    述: 获取数据校验码
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 get_dataCheck(uint8 * pbuf, uint8 len)
{
	uint8 ret=0;
	uint8 i,tmp;

	for( i=0;i<len;i++)
	{
		ret +=*pbuf;
		tmp=*pbuf;
		pbuf++;
	}
	return ret;
}

/*******************************************************************************************
函数名称: add33H
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 add33H(uint8* pbuf, uint8 datalen)
{
	uint8 *sbuf=&pbuf[0];
	uint8 i=0;

	for( i=0;i<datalen;i++)
	{
		*sbuf =*sbuf + 0x33;
		sbuf++;
	}
	return 1;
}

/*******************************************************************************************
函数名称: dec33H
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 dec33H(uint8* pbuf, uint8 datalen)
{
	uint8 i=0;

	for( i=0;i<datalen;i++)
	{
		*pbuf =*pbuf - 0x33;
		pbuf++;
	}

	return 1;
}
/*******************************************************************************************
函数名称: releasePackage
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_releasePackage(uint8* rbuf, uint16 len, PDU_FRAME * pFrame)
{
	uint8 ret=1;
	uint8 *pbuf=rbuf,i;

	for(i=0;i<len;i++)
	{
	  if(*pbuf ==0X68 && pbuf[1]==0X91)
	  {
	    break;
	  }
	  else
	  {
	    pbuf++;
	  }
	}

//	if(*pbuf++ !=0Xfe)
//	  return 0;

//	if(*pbuf++ !=0Xfe)
//	  return 0;

//	if(*pbuf++ !=0Xfe)
//	  return 0;	

//	if(*pbuf++ !=0Xfe)
//	  return 0;	


//	if(*pbuf++ !=PDU_STX || ( rbuf[4]!=0X68 && rbuf[5]!=0X91) )						//起始位
//	{	
//	   return 0;
//	}

	if(*pbuf !=0X68 && pbuf[1]!=0X91)
	  {
	     return 0;
	  }

	
//	for(i=0;i<6;i++)							//地址检查 注：科陆电表返回地址只有5个字节，与标准的6个字节地址
//	{
////		if(rbuf[8]!=S_R_ADR_CODE)             //如果是读通信地址命令，则不判断地址
////		if(*pbuf != pFrame->padr[i])
////			return 0;
//		 pbuf++;
//	}	
//
//	if(*pbuf++ !=PDU_STX)						//起始位
//		return 0;
	*pbuf++;
	pFrame->ctl=*pbuf++;						//控制码
	pFrame->len=*pbuf++;						//数据长度

	if (pFrame->len >20)
		  return 0;
	for(i=0;i<pFrame->len;i++)					//数据拷贝
	{
		pFrame->pdata[i] = *pbuf++;
		pFrame->pdata[i] -=0x33;
	}

	pFrame->cs  =*pbuf++;	
//	if(pFrame->cs != get_dataCheck(&rbuf[4],len-2))	//数据校验
//		return 0;

	pFrame->etx =*pbuf++;
	if(pFrame->etx != PDU_ETX)					//结束字节
		return 0;

	return ret;
}
/*******************************************************************************************
函数名称: pduRead
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_Read(uint8* rbuf, uint16 len, uint8* cmd)
{
	uint8 ret=1;
	PDU_FRAME  pdu_frame={0};

	memcpy(pdu_frame.padr,pdu_info.addr,6);					//地址

	ret=pdu_releasePackage(rbuf,len,&pdu_frame);			//数据解包
	if(ret==0)
		return 0;

	ret=pdu_readCallback(pdu_frame,cmd);					//数据接收回调

	return ret;
}
/*******************************************************************************************
函数名称: pdu_readCallback
描    述: 接收数据回调
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_readCallback(PDU_FRAME pdu_frame,uint8 *cmd)
{
	uint8 ret=1;
	
	ret=pdu_data_separate(pdu_frame.len,pdu_frame.pdata,cmd);

	switch(pdu_frame.ctl)	//控制码
	{
	case S_R_DATA_CODE:		//从机读数据帧控制码
		ret=pdu_data_separate(pdu_frame.len,pdu_frame.pdata,cmd);

		break;
	case S_RC_DATA_CODE:	//从机读有后续数据帧控制码
		break;
	case S_ERR_DATA_CODE:	//从机异常应答控制码
		break;
	case S_R_ADR_CODE:		//从机读通信地址控制码
		if(pdu_frame.len==6)
		{
			memcpy(pdu_info.addr,pdu_frame.pdata,6);
			//memcpy(pdu_info.addr,pdu_frame.pdata,6);
			*cmd=R_ADDR_CMD;
			ret=1;
		}
		else
		{
			ret=0;
		}
		break;
	default:
		ret=0;
		break;			
	}

	return ret;
}
/*******************************************************************************************
函数名称: pdu_data_separate
描    述: 数据分离
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_data_separate(uint8 datalen , uint8 * pdata,uint8 * cmd)
{
	uint8 ret=1;
	uint32 tmp,tmp1,tmp2,tmp3,tmp4;
	fp32   fp=0;
	uint8 flag=0;	//正负数标志

	memcpy(&tmp,pdata,4);
	pdata +=4;

	switch(tmp) //数据标识
	{
	case POSITIVE_POWER_TOTAL_F://4个字节

		tmp1=(pdata[3]>>4 )*10+ (pdata[3]&0x0f);
		tmp2=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
		tmp3=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp4=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*1000000+tmp2*10000+tmp3*100+tmp4;
		fp=(fp32)tmp/(fp32)100.0;

		pdu_info.power_total=fp;	

		*cmd=POWER_TOTAL_CMD;
		break;
	case A_VOLT_F://电压2个字节 根据DLT645协议中的电压表示格式用2字节，XXX.X，BCD码
		tmp1=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp2=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);
		tmp=tmp1*100+tmp2;
		fp=(fp32)tmp/(fp32)10.0;
      
		pdu_info.A_volt=fp;
		
		*cmd=A_VOLT_CMD;
		break;
	case B_VOLT_F:
		tmp1=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp2=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);
		tmp=tmp1*100+tmp2;
		fp=(fp32)tmp/(fp32)10.0;
      
		pdu_info.B_volt=fp;

		*cmd=B_VOLT_CMD;
		break;
	case C_VOLT_F:
		tmp1=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp2=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);
		tmp=tmp1*100+tmp2;
		fp=(fp32)tmp/(fp32)10.0;
      
		pdu_info.C_volt=fp;
		*cmd=C_VOLT_CMD;
		break;
	case A_CURR_F://电流3个字节  根据DLT645协议中的电流表示格式用3字节，XXX.XXX，其中最高位表示符号位，BCD码
		if((pdata[2] & 0x80 )==0) //最高位等于0为正，等于1为负
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//负数
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//是负数时?则转换成负数
			fp=-fp;

		pdu_info.A_curr=fp;
		
		*cmd=A_CURR_CMD;
		break;
	case B_CURR_F:

		if((pdata[2] & 0x80 )==0) //最高位等于0为正，等于1为负
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//负数
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//是负数时?则转换成负数
			fp=-fp;

		pdu_info.B_curr=fp;
		*cmd=B_CURR_CMD;
		break;
	case C_CURR_F:
		if((pdata[2] & 0x80 )==0) //最高位等于0为正，等于1为负
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//负数
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//是负数时?则转换成负数
			fp =-fp;

		pdu_info.C_curr=fp;

		*cmd=C_CURR_CMD;
		break;	
  case ADDR_F:
		memcpy(pdu_info.addr,&pdata[0],6);

		break;
	default:
		ret=0;
		break;
	}

	return ret;
}
/*******************************************************************************************
函数名称: pdu_Manage
描    述: PDU电表管理，也即读取电压、电流、功率等
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 pdu_Manage(uint8 CMD)
{
	uint8 ret=0;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;			//获取父窗口句柄	
	uint16 len=0;
	uint8 cmd=0;

	switch(CMD)
	{
	case PDU_READ_A_VOLT:	
		cmd=A_VOLT_CMD;
		break;
	case PDU_READ_B_VOLT:	
		cmd=B_VOLT_CMD;
		break;
	case PDU_READ_C_VOLT:	
		cmd=C_VOLT_CMD;
		break;
	case PDU_READ_A_CURR:
		cmd=A_CURR_CMD;
		break;
	case PDU_READ_B_CURR:
		cmd=B_CURR_CMD;
		break;
	case PDU_READ_C_CURR:
		cmd=C_CURR_CMD;
		break;
	case PDU_READ_POWER:
		cmd=POWER_TOTAL_CMD;
		break;
	case PDU_READ_ADDR:
		cmd=R_ADDR_CMD;
		break;	
	default:
		return 0;
	}
	 pdu_rxdata_len=0;
	 pdu_Write(rx2_Buf, &len , cmd);				//命令组包
	 ret=uart_write(PDU_UART,PDU_RX_BUF,len);		//串口发送命令
	 if(ret==0)
		   return 0;

	 // Sleep(100);
	  pdu_rxdata_len=uart_read(PDU_UART,PDU_RX_BUF,400);		//串口接收命令
	  if(pdu_rxdata_len==0)
		  return 0;
 
	   ret=pdu_Read(rx2_Buf, pdu_rxdata_len , &cmd);	    	//命令解包
	  if(ret==0)
	  	{ 
		
		  return 0;
		  }

	return 1;
}

