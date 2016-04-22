
/*********************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************/
#include "ILBCom.h"

/*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************/
#define  ILBCOM_BUF_RN   			200   //�������ݵĳ���
#define  ILBCOM_BUF_SN   			200   //�������ݵĳ���	
#define  ILBMODULE_REG_MAX 			5


#define ILBPDU_UART          UART_5
#define ILB_DEV_ADR          1


/*********************************************************************************************************
*                                            ������������������
*********************************************************************************************************/	
uint8 ILB_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_6_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
uint8 ILB_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_2_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
extern INT16U getCRC16(INT8U *crcdata, INT8U datalen);
//uint8 dataExchange(uint8 * pdat, uint8 len);

/*********************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************/
uint8 ILBRevComBuff[ILBCOM_BUF_RN],ILBSendComBuff[ILBCOM_BUF_SN];  //ע����������
//_MODULE_INFO gModuleInfo[MODULE_SUM];
uint16 ILBModuleReg[ILBMODULE_REG_MAX];   //15�����ģ��Ĵ���
//ILB_PDU_INFO gILBPduInfo;


/*******************************************************************************************  
��������: Modbus_Function_3_main  
��    ��:��Ĵ����� �����˳������ 
�������:   SlaveAddr �ӻ���ַ    regAdr �Ĵ�����ַ    regBuf ���ջ���  Reg_sum �Ĵ�������
�������:   
��    ��: 1 �ɹ�  0ʧ��
********************************************************************************************/ 
uint8 ILB_Modbus_Function_3_main(uint8 SlaveAddr, uint16 regAdr,uint16 * regBuf,  uint16 Reg_sum)
{    
    uint16 crcresult=0; 
    uint8  Send_counter;
	uint8  dataLen,ret=0;
	uint8  *Rev_buff=ILBRevComBuff;  
	uint8  *Send_buff=ILBSendComBuff;


	memset(ILBRevComBuff,0,ILBCOM_BUF_RN);
	memset(ILBSendComBuff,0,ILBCOM_BUF_SN);   


	Send_buff[0] = SlaveAddr;			//�ӻ���ַ
	Send_buff[1] =3;			        //������
    Send_buff[2] = regAdr>>8;			//�Ĵ����׵�ַ
	Send_buff[3] = (uint8)regAdr;
	Send_buff[4] = Reg_sum>>8;			//���Ĵ�������
	Send_buff[5] =(uint8)Reg_sum;
       
    Send_counter = 6;								 //�����豸��ַ�������룬������3���ֽ� 
    crcresult = getCRC16(Send_buff,Send_counter);    			//���CRCУ����
    Send_buff[Send_counter] = crcresult & 0xff;	     			//�ȵ�λ
    Send_buff[Send_counter+1] = (crcresult >> 8) & 0xff;  		//���λ
  //     UartSendStr(Rev_buff,8);  //�����յ���ǰ8���ֽ�����ԭ�ⲻ���ķ��� 

	ret=uart_write(ILBPDU_UART,Send_buff,8);

	ret=ILB_Modbus_Function_3_main_rev(Rev_buff,SlaveAddr,&dataLen,regBuf);
	if(ret==0)
		return 0;
	return 1;
}

/*******************************************************************************************  
��������: ILB_Modbus_Function_3_main_rev  
��    ��:�Ĵ����� �����˳�����ճ��� 
�������:            
�������:   
��    ��: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff)
{    
    uint16 crcresult=0;     
	uint16 i, revLen=0;
	INT8U * pbuf =(INT8U*)Reg_buff;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;  //��ȡ�����ھ��	

	revLen=uart_read(ILBPDU_UART,Rev_buff,200);
	//uartRead(port,Rev_buff,*revLen);
	if(revLen<5)
		return 0;  	
	
	if(Rev_buff[0] != SlavAdr) //�豸��ַ
		return 0;
	if(Rev_buff[1] != 3) //������
		return 0;
	crcresult = getCRC16(ILBRevComBuff,(uint8)revLen); 
    if(crcresult != 0) //CRCУ�鲻��ȷ
		return 0;

    *dataLen=Rev_buff[2];
	for( i=0;i<*dataLen;i++) //���ݽ���
	{
		*pbuf++ =Rev_buff[3+i];
	}

   return 1;
}


/*******************************************************************************************  
��������: Modbus_Function_16_main  
��    ��:��Ĵ���д �����˳������ 
�������:            
�������:   
��    ��: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_16_main(uint8 SlaveAddr, uint16 regAdr,uint16 * regBuf,  uint16 Reg_sum)
{	uint16 crcresult=0; 
    uint8  Send_counter;
	uint8  dataLen,ret=0;
	uint8  *Rev_buff=ILBRevComBuff;  
	uint8  *Send_buff=ILBSendComBuff;

	memset(ILBRevComBuff,0,ILBCOM_BUF_RN);
	memset(ILBSendComBuff,0,ILBCOM_BUF_SN);
   
	Send_buff[0] = SlaveAddr;		//�ӻ���ַ
	Send_buff[1] = 16;				//������
    Send_buff[2] = regAdr>>8;		//�Ĵ����׵�ַ
	Send_buff[3] = (uint8)regAdr;
	Send_buff[4] = Reg_sum>>8;		//���Ĵ�������
	Send_buff[5] = (uint8)Reg_sum;
	Send_buff[6] = Reg_sum*2;       //�����ֽ�
	memcpy(&Send_buff[7],(uint8 *)regBuf,Reg_sum*2);  //����
       
    Send_counter = 7+ Reg_sum*2;								 //�����豸��ַ�������룬������3���ֽ� 
    crcresult = getCRC16(Send_buff,Send_counter);    //���CRCУ����
    Send_buff[Send_counter] = crcresult & 0xff;	     //�ȵ�λ
    Send_buff[Send_counter+1] = (crcresult >> 8) & 0xff;  //���λ	  

	ret=uart_write(ILBPDU_UART,Send_buff,Send_counter+2);

	ret=ILB_Modbus_Function_16_main_rev(Rev_buff,SlaveAddr,&dataLen,regBuf);
	if(ret==0)
		return 0;

	return 1;
}
/*******************************************************************************************  
��������: Modbus_Function_16_main_rev  
��    ��:��Ĵ���д �����˳�����ճ��� 
�������:            
�������:   
��    ��: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff)
{    
    uint16 crcresult=0;     
	uint16  revLen=0;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;  //��ȡ�����ھ��

	//uartRead(port,Rev_buff,*revLen);
	revLen=uart_read(ILBPDU_UART,Rev_buff,200);
	
	if(revLen<8)
		return 0;
	
	if(Rev_buff[0] != SlavAdr) //�豸��ַ
		return 0;
	if(Rev_buff[1] != 16) //������
		return 0;
	crcresult = getCRC16(ILBRevComBuff,(uint8)revLen); 
    if(crcresult != 0) //CRCУ�鲻��ȷ
		return 0;
    
   return 1;
}




/*******************************************************************************************
��������: readILBDat
��    ��: ��ȡ�����������
�������: pdat ���ݻ��� len ���ݳ���
�������: ��
��    ��: ��
********************************************************************************************/
uint8 readILBDat(INT16U startAdr ,INT8U num)
{
	INT8U ret=0,i;
	static INT8U comAlmCnt=0;


	ret=ILB_Modbus_Function_3_main(ILB_DEV_ADR, startAdr,&ILBModuleReg[0], num);//���Ĵ���
	if(ret==0 )      //�����򷵻�
	{
		comAlmCnt ++;
		if(comAlmCnt>=5)
		{
			//gModuleInfo[i].comAlarmCnt=5;
			gUserDB.ILB_comAlarm=ALARM;
		}
		return 0;
	}
	else             //��ȷ���������
	{		
		
		dataExchange( (INT8U*)&ILBModuleReg[1],2);	           //�ѽ��յ��������Ƚ��иߵ��ֽڽ���	
		gUserDB.activeILB_value = ILBModuleReg[1];
		dataExchange( (INT8U*)&ILBModuleReg[2],2);	           //�ѽ��յ��������Ƚ��иߵ��ֽڽ���	
		gUserDB.inactiveILB_value = ILBModuleReg[2];
		dataExchange( (INT8U*)&ILBModuleReg[0],2);	           //�ѽ��յ��������Ƚ��иߵ��ֽڽ���		
		gUserDB.insulationAlarm = ILBModuleReg[0] & 0x01;		
		
		gUserDB.ILB_comAlarm = NOALARM;
		comAlmCnt=0;
	}

   return ret;
}  

/*******************************************************************************************
��������: readILBDat
��    ��: ��ȡģ���������������Ϣ
�������: pdat ���ݻ��� len ���ݳ���
�������: ��
��    ��: ��
********************************************************************************************/
void task_ILBCom(void )
{
	INT8U ret=0;  	
	
//	while(1)
//	{
		ret=readILBDat(0X00,5);        //��ȡ��Ե����Ĵ���
		OSTimeDlyHMSM(0,0,0,200);  
	  			
//	}   
}






