/************************************************************


*************************************************************/
#include "Global.h"
#include "includes.h"

INT8U  gModbusRecArr[100];				//���ڽ������ݵ���ʱ�Ĵ���
INT8U  gModbusRecNum;					//���յ������ݸ��������50


UART_INFO gUartInfo[COM_PORT_NUM];                        //���ڽ��ջ�����Ϣ
//INT16U ModbusREGDat[MBREGNUM]={0};		//����Modbus �Ĵ�����Ҳ��ȫ�ֱ���������						
										
INT16U lSetVoltageBak=0;				//���õ�ѹ����
INT16U lSetCurrentBak=0;				//���õ�������
INT16U lSaveData=0;						//�������ݱ���
INT16U gSetVoltageDA=0;					//���õ�ѹ��Ӧ��DAֵ
INT16U gSetCurrentDA=0;					//���õ�����Ӧ��DAֵ
INT16U gOutVoltageAD=0;					//�����ѹAD����ֵ
INT16U gOutCurrentAD=0;					//�������AD����ֵ

//INT16U gSystmERR=0;					//ģ��Ĺ���״̬
INT16U gModelRunFlag=0;					//ģ�鹤�����ı�־
										
INT16U gAdjOutCur[6]={0};				//��ʾ����У������
INT16U gAdjSetCur[6]={0};				//���õ���У������
INT16U gAdjOutVol[6]={0};				//��ʾ��ѹУ������
INT16U gAdjSetVol[16]={0};				//���õ�ѹУ������
INT16U gAdjSetVol_F[8]={0};				//���õ�ѹУ������ 6-45V��5������
//INT8U  gSystemCommFlag=0;				//if lFlag=1 is commcation OK


void UartSendByte(INT8U lSend);			//�������͵����ֽں���
void ModbusCommunication(void);			//����MODBUSͨѶ������
void StatusLinkPageId(void);
/*********************************************************************************************************************
��������: 	task_modbus          
��������: 	task_modbus����,��Ҫ����PCU�봥����ͨ��
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/

void task_modbus(void *pdata)			//MODBUS����
{
	INT8U tmp=0;
	INT16U Runtimes=0;					//�������еĴ�����ͬʱ�����ڼ���ʱ��
	INT8U  tmpArr[30];					//����һ����ʱ�����飬��Ҫ��Ӧ��24C16�Ķ�д���κεط����ɸ���
	pdata = pdata;
	OSTimeDlyHMSM(0,0,5,0);				   //2S��ʱ 
 while(1)
 {	   
	OSTimeDlyHMSM(0,0,0,10);  
	//ModbusCommunication();										//modbusͨѶ����  
  StatusLinkPageId();
	gUserDB.m485CommAlarmCnt=(gUserDB.m485CommAlarmCnt>=100)?100:(++gUserDB.m485CommAlarmCnt);
	if(gUserDB.m485CommAlarmCnt>=100)
	  gUserDB.m485comLinkStatus=ALARM;	

 }	//end while
}


/*********************************************************************************************************
��������: 	CRC16 
��������:  CRCУ��

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
INT16U CRC16(INT8U *crcdata, INT8U datalen)
{
  INT8U crc16h = 0xFF ;    
  INT8U crc16l = 0xFF ;    
  INT8U savehi,savelo;
  INT8U i,j;
  for(i=0;i<datalen;i++)
   {  
    crc16l=crc16l^ *(crcdata++);
    for(j=0;j<8;j++)
	  {
		savehi = crc16h;
		savelo = crc16l;
        crc16h = crc16h>>1;
        crc16l = crc16l>>1;
        if ((savehi & 0x01) == 0x01)	 crc16l = crc16l|0x80;
        if ((savelo & 0x01) == 0x01) 
		  {
        	crc16h = crc16h ^ 0xA0 ;
            crc16l = crc16l ^ 0x01;
		  }	      
      }
   }
  return ((crc16h << 8) | crc16l) ;
}



/*********************************************************************************************************
��������: 	UartSendArr 
��������:  ���ڷ���

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
void UartSendArr(INT8U *ptrSend,INT8U length) 
{
	INT8U tmp=0;
	//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
	SET_RS485_1_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UartSendByte(*(ptrSend++));							//��������
	}
	//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	SET_RS485_1_REC;
}
 
/*********************************************************************************************************
��������: 	Modbus_Function_3 
��������:  ������3 ����Ĵ�����

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
void Modbus_Function_3(INT8U *Rev_buff) 
{    
    INT8U  RegAddr;
	INT8U  tmp=0;
	INT8U  SendBuff[MBREGNUM*2+5]={0};
	INT8U  SendNum=0;
	INT16U CRCResult=0;    
    RegAddr = Rev_buff[3];  								//�õ��Ĵ����ĵ�ַ

	modbusReadCallBack();                                   //�Ĵ���ת��
    if(RegAddr + Rev_buff[5] <= MBREGNUM)					//�����ʵĵ�ַ�ں���ķ�Χ֮��
    {
        SendNum = 2 * Rev_buff[5] + 3;  					//�����豸��ַ�������룬������3���ֽ�
        for(tmp=3; tmp < SendNum; )							//˳��Ϊ�ȸ��ֽں���ֽ�
    	{
    	   SendBuff[tmp] = (ModbusREGDat[RegAddr] >> 8) & 0xff;           
           tmp++;
           SendBuff[tmp] = ModbusREGDat[RegAddr] & 0xff;
           tmp++;	
           RegAddr++;  										//��һ���Ĵ���
        }
	    SendBuff[0] = MODBUS_DEVICEID;
        SendBuff[1] = 0x03; 
        SendBuff[2] = 2 * Rev_buff[5]; 						//���͵����ݵĸ���
        CRCResult = CRC16(SendBuff,SendNum);  				//���CRCУ����
        SendBuff[SendNum] = CRCResult & 0xff;  				//�ȵ�λ
        SendBuff[SendNum+1] = (CRCResult >> 8) & 0xff;		//���λ
        //UartSendArr(SendBuff,SendNum+2);  					//��������
		uart_write(MID_MODBUS_COM,SendBuff,SendNum+2);  					//��������
    }     	        
} 
/*********************************************************************************************************
��������: 	Modbus_Function_6 
��������:  ������6 �����Ĵ���д

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
void Modbus_Function_6(INT8U *RecBuff) 
{ 
    INT8U lRegAddr; 
    lRegAddr = (RecBuff[2]<<8) + RecBuff[3];  				//�Ĵ����ĵ�ַ
    if(lRegAddr < MBREGNUM)							   		//�����ʵĵ�ַ�ں���ķ�Χ֮��
    {
       ModbusREGDat[lRegAddr] = (RecBuff[4]<<8) + RecBuff[5]; //�޸�ָ���Ĵ����е�����
	   //UartSendArr(RecBuff,8);  							  //�����յ�������ԭ�ⲻ���ķ��� .
	   uart_write(MID_MODBUS_COM,RecBuff,8);  					//��������
	   modbusWriteCallBack( lRegAddr,1);	   		          //����MODBUS���յ�������
    }
} 
/**********************************************************
	�Խ��յ���modbus���ݵĴ���������Ӧ������
	���ݴ�����ƣ�
		���κ��ϴΣ����20ms��������յ������ݸ�����ͬ
		�ҽ��յ������ݴ��ڵ���7
		����֮�������������������
		����˶Ե�ַ��CRCУ�鹦��
***********************************************************/ 
void ModbusCommunication(void) 
{    
    static INT8U lastRecLen=0;															//�������ݳ��ȵı���

	if(gUartInfo[MID_MODBUS_COM].UartRLen == 0) 		                               				//û�н��յ����ݷ���
		return ;							

	if(gUartInfo[MID_MODBUS_COM].UartRLen != lastRecLen)					   						//���β���ͬ���������ڽ������ݣ�����δ���
	{
	   lastRecLen	= gUartInfo[MID_MODBUS_COM].UartRLen;				   							//���汸�����ݣ����´ζԱ�
	   return ;
	}
	else
	{
		if((gUartInfo[MID_MODBUS_COM].UartRLen < 7) && (gUartInfo[MID_MODBUS_COM].UartRLen >=1))			//С��7˵���Ǵ���֡������
		{																				//ȥ��=0ʱͻȻ���յ�����ȴ���㵼�´���
			gUartInfo[MID_MODBUS_COM].UartRLen=0;							   						//���½�������
			lastRecLen=0;
			return ;
		}					
	}
	if(gUartInfo[MID_MODBUS_COM].UartRLen > 8 )													//˵���Ǵ���֡������
	{													
		gUartInfo[MID_MODBUS_COM].UartRLen=0;							   							//���½�������
		lastRecLen=0;
		return ;
	}	
	if((gUartInfo[MID_MODBUS_COM].UartRBuf[0] == MODBUS_DEVICEID))								// ͨ�ŵ�ַ
    {						  
    	if((gUartInfo[MID_MODBUS_COM].UartRLen == 8)&&( CRC16(gUartInfo[MID_MODBUS_COM].UartRBuf,8)== 0))	//CRCУ����ȷ
		{ 
			switch(gUartInfo[MID_MODBUS_COM].UartRBuf[1])   										//������
			{ 
            	case 0x03: 	Modbus_Function_3(gUartInfo[MID_MODBUS_COM].UartRBuf); 						
        					break;  
                case 0x06: 	Modbus_Function_6(gUartInfo[MID_MODBUS_COM].UartRBuf); 
        			 		break; 
        		default:  	break;			 
            } 
    	}
	}
	gUartInfo[MID_MODBUS_COM].UartRLen=0;							   							//���½�������
	lastRecLen=0;    																//���㱸�ݼĴ���
}

/**********************************************************
	�������ƺ���
	����������Сֵ֮��ʱ����0�����򷵻�1 2
**********************************************************/
INT8U DataLimit(INT16U *Dat,INT16U MaxDat,INT16U MinDat)	
{
	if(*Dat<MinDat)	{*Dat = MinDat;	return 1;}	
	if(*Dat>MaxDat)	{*Dat = MaxDat; return 2;}
	return 0;
}
