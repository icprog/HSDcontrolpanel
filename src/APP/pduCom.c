// pduCom.cpp : ʵ���ļ�
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "pduCom.h"
#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<stdarg.h>

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define RXBUF_MAX		300


/********************************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************************/
uint8	rx2_Buf[RXBUF_MAX];
uint16  pdu_rxdata_len=0;
uint8	*pRx2_Buf=rx2_Buf;
uint8	rx2_tBuf[RXBUF_MAX];

#define PDU_UART		UART_3 
#define PDU_RX_BUF		rx2_Buf 
#define PDU_TX_BUF		rx2_Buf 

PDU_INFO pdu_info;

uint8 pduAdr[6]={0x00,0x00,0x00,0x00,0x00,0x00};	//PDU��ַ��ʼ��

/*******************************************************************************************
��������: CpduComInit
��    ��: PDU������ʼ��
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void CpduComInit(void)
{
	pdu_info.A_curr=0; //���ݳ�ʼ��
	pdu_info.B_curr=0;
	pdu_info.C_curr=0;
	pdu_info.A_volt=0;
	pdu_info.B_volt=0;
	pdu_info.C_volt=0;
	pdu_info.power_total=0;
	memcpy(pdu_info.addr,pduAdr,6);
}

/*******************************************************************************************
��������: pdu_Write
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
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
	case POWER_TOTAL_CMD:	//�������ܵ���
		pdu_frame.ctl=M_R_DATA_CODE;						//������
		pdu_frame.len=0x04;									//���ݳ���
		pdu_frame.pdata[0]=(uint8)(POSITIVE_POWER_TOTAL_F>>0);
		pdu_frame.pdata[1]=(uint8)(POSITIVE_POWER_TOTAL_F>>8);
		pdu_frame.pdata[2]=(uint8)(POSITIVE_POWER_TOTAL_F>>16);
		pdu_frame.pdata[3]=(uint8)(POSITIVE_POWER_TOTAL_F>>24);
		
		break;
	case A_VOLT_CMD:	//��A���ѹ
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(A_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(A_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(A_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(A_VOLT_F>>24);
		break;
	case B_VOLT_CMD:	 //��B���ѹ
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(B_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(B_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(B_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(B_VOLT_F>>24);
		break;
	case C_VOLT_CMD:	//��C���ѹ
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(C_VOLT_F>>0);
		pdu_frame.pdata[1]=(uint8)(C_VOLT_F>>8);
		pdu_frame.pdata[2]=(uint8)(C_VOLT_F>>16);
		pdu_frame.pdata[3]=(uint8)(C_VOLT_F>>24);
		break;
	case A_CURR_CMD:	//��A�����
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(A_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(A_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(A_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(A_CURR_F>>24);
		break;
	case B_CURR_CMD:	//��B�����
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(B_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(B_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(B_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(B_CURR_F>>24);
		break;
	case C_CURR_CMD:	//��C�����
		pdu_frame.ctl=M_R_DATA_CODE;
		pdu_frame.len=0x04;
		pdu_frame.pdata[0]=(uint8)(C_CURR_F>>0);
		pdu_frame.pdata[1]=(uint8)(C_CURR_F>>8);
		pdu_frame.pdata[2]=(uint8)(C_CURR_F>>16);
		pdu_frame.pdata[3]=(uint8)(C_CURR_F>>24);
		break;
	case R_ADDR_CMD:	//������ַ
		memset(pdu_frame.padr,0xaa,6);	//������ַΪ0XAAAAAAAAAAAA
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

	pdu_makePackage(sbuf,plen,&pdu_frame); //�������
	
//	/***************1997��Լ*****************/
//	sbuf[0]=0xfe;
//	sbuf[1]=0xfe;
//	sbuf[2]=0xfe;	
//	sbuf[3]=0x68;
//	
//	sbuf[4]=pdu_info.addr[0];      //����ַ��λ
//	sbuf[5]=pdu_info.addr[1];
//	sbuf[6]=pdu_info.addr[2];
//	sbuf[7]=pdu_info.addr[3];
//	sbuf[8]=pdu_info.addr[4];
//	sbuf[9]=pdu_info.addr[5];     //����ַ��λ
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
//	/***************1997��Լ*****************/

	return ret;
}

/*******************************************************************************************
��������: makePackage
��    ��: �����������
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 pdu_makePackage(uint8 * sbuf, uint16 *len , PDU_FRAME * pFrame)
{
	uint8 ret=1;
	uint8 t=0;
	uint8 *pbuf=&sbuf[0];

	t =0;
	memset(pbuf,0xfe,4);								//����ǰ���ĸ�0XFE������
	t +=4;
	pbuf += 4;	
	*pbuf =PDU_STX;										//��ʼλ
	t++;
	pbuf++;
	memcpy(pbuf,pFrame->padr,6);					//��ַ
	t +=6;
	pbuf +=6;
	*pbuf =PDU_STX;									//��ʼλ
	t++;
	pbuf++;
	*pbuf =pFrame->ctl;								//������
	t++;
	pbuf++;
	*pbuf =pFrame->len;								//���ݳ���
	t++;
	pbuf++;
	if(pFrame->len >0)
	{
		memcpy(pbuf,pFrame->pdata,pFrame->len);		//����������
		add33H(pbuf,pFrame->len);					//�������0X33
		t +=pFrame->len;
		pbuf += pFrame->len;
	}

	pFrame->cs=get_dataCheck(&sbuf[4],t-4);				   //����У����
	//pbuf += t;
	*pbuf ++=pFrame->cs;									//У����
	t++;
	*pbuf ++=PDU_ETX;									//����λ
	t++;

	*len=t;

	return ret;
}

/*******************************************************************************************
��������: get_dataCheck
��    ��: ��ȡ����У����
�������: ��
�������: ��
��    ��: ��
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
��������: add33H
��    ��: 
�������: ��
�������: ��
��    ��: ��
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
��������: dec33H
��    ��: 
�������: ��
�������: ��
��    ��: ��
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
��������: releasePackage
��    ��: 
�������: ��
�������: ��
��    ��: ��
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


//	if(*pbuf++ !=PDU_STX || ( rbuf[4]!=0X68 && rbuf[5]!=0X91) )						//��ʼλ
//	{	
//	   return 0;
//	}

	if(*pbuf !=0X68 && pbuf[1]!=0X91)
	  {
	     return 0;
	  }

	
//	for(i=0;i<6;i++)							//��ַ��� ע����½����ص�ַֻ��5���ֽڣ����׼��6���ֽڵ�ַ
//	{
////		if(rbuf[8]!=S_R_ADR_CODE)             //����Ƕ�ͨ�ŵ�ַ������жϵ�ַ
////		if(*pbuf != pFrame->padr[i])
////			return 0;
//		 pbuf++;
//	}	
//
//	if(*pbuf++ !=PDU_STX)						//��ʼλ
//		return 0;
	*pbuf++;
	pFrame->ctl=*pbuf++;						//������
	pFrame->len=*pbuf++;						//���ݳ���

	if (pFrame->len >20)
		  return 0;
	for(i=0;i<pFrame->len;i++)					//���ݿ���
	{
		pFrame->pdata[i] = *pbuf++;
		pFrame->pdata[i] -=0x33;
	}

	pFrame->cs  =*pbuf++;	
//	if(pFrame->cs != get_dataCheck(&rbuf[4],len-2))	//����У��
//		return 0;

	pFrame->etx =*pbuf++;
	if(pFrame->etx != PDU_ETX)					//�����ֽ�
		return 0;

	return ret;
}
/*******************************************************************************************
��������: pduRead
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 pdu_Read(uint8* rbuf, uint16 len, uint8* cmd)
{
	uint8 ret=1;
	PDU_FRAME  pdu_frame={0};

	memcpy(pdu_frame.padr,pdu_info.addr,6);					//��ַ

	ret=pdu_releasePackage(rbuf,len,&pdu_frame);			//���ݽ��
	if(ret==0)
		return 0;

	ret=pdu_readCallback(pdu_frame,cmd);					//���ݽ��ջص�

	return ret;
}
/*******************************************************************************************
��������: pdu_readCallback
��    ��: �������ݻص�
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 pdu_readCallback(PDU_FRAME pdu_frame,uint8 *cmd)
{
	uint8 ret=1;
	
	ret=pdu_data_separate(pdu_frame.len,pdu_frame.pdata,cmd);

	switch(pdu_frame.ctl)	//������
	{
	case S_R_DATA_CODE:		//�ӻ�������֡������
		ret=pdu_data_separate(pdu_frame.len,pdu_frame.pdata,cmd);

		break;
	case S_RC_DATA_CODE:	//�ӻ����к�������֡������
		break;
	case S_ERR_DATA_CODE:	//�ӻ��쳣Ӧ�������
		break;
	case S_R_ADR_CODE:		//�ӻ���ͨ�ŵ�ַ������
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
��������: pdu_data_separate
��    ��: ���ݷ���
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 pdu_data_separate(uint8 datalen , uint8 * pdata,uint8 * cmd)
{
	uint8 ret=1;
	uint32 tmp,tmp1,tmp2,tmp3,tmp4;
	fp32   fp=0;
	uint8 flag=0;	//��������־

	memcpy(&tmp,pdata,4);
	pdata +=4;

	switch(tmp) //���ݱ�ʶ
	{
	case POSITIVE_POWER_TOTAL_F://4���ֽ�

		tmp1=(pdata[3]>>4 )*10+ (pdata[3]&0x0f);
		tmp2=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
		tmp3=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp4=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*1000000+tmp2*10000+tmp3*100+tmp4;
		fp=(fp32)tmp/(fp32)100.0;

		pdu_info.power_total=fp;	

		*cmd=POWER_TOTAL_CMD;
		break;
	case A_VOLT_F://��ѹ2���ֽ� ����DLT645Э���еĵ�ѹ��ʾ��ʽ��2�ֽڣ�XXX.X��BCD��
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
	case A_CURR_F://����3���ֽ�  ����DLT645Э���еĵ�����ʾ��ʽ��3�ֽڣ�XXX.XXX���������λ��ʾ����λ��BCD��
		if((pdata[2] & 0x80 )==0) //���λ����0Ϊ��������1Ϊ��
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//����
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//�Ǹ���ʱ?��ת���ɸ���
			fp=-fp;

		pdu_info.A_curr=fp;
		
		*cmd=A_CURR_CMD;
		break;
	case B_CURR_F:

		if((pdata[2] & 0x80 )==0) //���λ����0Ϊ��������1Ϊ��
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//����
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//�Ǹ���ʱ?��ת���ɸ���
			fp=-fp;

		pdu_info.B_curr=fp;
		*cmd=B_CURR_CMD;
		break;
	case C_CURR_F:
		if((pdata[2] & 0x80 )==0) //���λ����0Ϊ��������1Ϊ��
		{
			tmp1=(pdata[2]>>4 )*10+ (pdata[2]&0x0f);
			flag=0;
		}
		else					//����
		{
			tmp1=((pdata[2]>>4) &0x07)*10+ (pdata[2]&0x0f);
			flag=1;
		}

		tmp2=(pdata[1]>>4 )*10+ (pdata[1]&0x0f);
		tmp3=(pdata[0]>>4 )*10+ (pdata[0]&0x0f);

		tmp=tmp1*10000+tmp2*100+tmp3;
		fp=(fp32)tmp/(fp32)1000.0;

		if(flag==1)//�Ǹ���ʱ?��ת���ɸ���
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
��������: pdu_Manage
��    ��: PDU������Ҳ����ȡ��ѹ�����������ʵ�
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8 pdu_Manage(uint8 CMD)
{
	uint8 ret=0;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;			//��ȡ�����ھ��	
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
	 pdu_Write(rx2_Buf, &len , cmd);				//�������
	 ret=uart_write(PDU_UART,PDU_RX_BUF,len);		//���ڷ�������
	 if(ret==0)
		   return 0;

	 // Sleep(100);
	  pdu_rxdata_len=uart_read(PDU_UART,PDU_RX_BUF,400);		//���ڽ�������
	  if(pdu_rxdata_len==0)
		  return 0;
 
	   ret=pdu_Read(rx2_Buf, pdu_rxdata_len , &cmd);	    	//������
	  if(ret==0)
	  	{ 
		
		  return 0;
		  }

	return 1;
}

