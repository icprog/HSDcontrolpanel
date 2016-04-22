 /*********************************************************************************************************************
*                                            ͷ�ļ�����
***********************************************************************************************************************/
//#include "cancom.h" 
#include "ucos_ii.h"
#include "stm32f10x.h"
#include "global.h"
#include "includes.h"
//#include "controller.h"
//�޸����ݣ�
//���״̬0������if(gtMCT_Dat.BMSStage != 0x03)  ltTxMsg.Data[0] = 0x00;//�ڷǳ��׶ξ���Ϊֹͣ״̬�������ֶ����޷���ʾ���״̬����Ҫ�ⲿ�Ƿ�������־λ
//�ڳ��׶Σ���ԭ while(CAN_ReadBuffNum())			
//	����if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID)Ϊ if(ptrMsg.Data[1] == gtMCT_Dat.ChargerID)�����ڻ���ע��
/********************************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************************/
//CanTxMsg TxMessage;
//_BMSFlow gBmsFlow;
INT16U gu8StartLink = 0;

//void sysPareInit(void);
//void pcRequireProcess(void);
//void timeSendProcess(void);
INT8U   tmp_y=0;	
 
#define SET_LED_1_ON	//	GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define SET_LED_1_OFF	//	GPIO_ResetBits(GPIOB, GPIO_Pin_15);

CANRECMSGBUFF CanMessageBuff;			//����CAN���ջ�����
CANRECMSGBUFF MCM_CanPackage;	//��������һ��can���ջ�����

_T_BMS_DAT gtBMS_Dat;

CanTxMsg TxMessage;
CANRECMSG rxMessage;
CANPACKAGE	CanPackage;


CANSENDPACKAGE	CanSendPackage;

INT8U  TransmitMailbox;
INT8U  gCommStation = 0x00;				//��¼����ͨѶ�׶�
INT16U ModbusREGDat[300];
_T_MCT_DAT gtMCT_Dat;//����һ���ṹ�����
_T_METER_DAT gtMeter_Dat;

_CAN_CHARGE_FLAG CAN_ChargeFlag;						//�������еı�־
void rec_secondGun_can();


/**********************************************************
	��ȡ��Ϣ����������
	���أ�
		0   -������
		>0  -��������Ϣ�ĸ���
***********************************************************/
INT8U CAN_ReadBuffNum()
{
 	INT16S tmp16;
	tmp16 = CanMessageBuff.InLen- CanMessageBuff.OutLen;
	if(tmp16 <= 0x00)	
		return 0;							//��������,һ�㲻��С��0
	
	return 	(tmp16 & (CANMESSAGEBUFFLEN-1)); 				//����Ϣ�ͷ��ػ�������Ϣ�ĸ���
	
}

/**********************************************************
	��ʼ��CANͨѶ
	 ��Type=1ʱ���Խ������е�����
***********************************************************/
void CAN_Initial(INT8U u8Type)
{

  		CAN_InitTypeDef        	CAN_InitStructure;
  	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	GPIO_InitTypeDef 		GPIO_InitStructure; 			//���� GPIO �Ľṹ
	NVIC_InitTypeDef 		NVIC_InitStructure;				//NVIC����

  	CAN_ITConfig(CAN1,CAN_IT_FMP0, DISABLE); 	  			//ʹ��FIFO0��Ϣ�Һ��ж�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//������ʱ��.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 	//������ʱ��.

  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;				//Configure CAN pin: RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;				//Configure CAN pin: TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	CAN_DeInit(CAN1);
  	CAN_StructInit(&CAN_InitStructure);
  	CAN_InitStructure.CAN_TTCM = DISABLE;					// CAN cell init 
	//CAN_InitStructure.CAN_ABOM = DISABLE;					//����ʹ�ܻ���ʧ���Զ����߹��� 	
	CAN_InitStructure.CAN_ABOM = ENABLE;					//����ʹ�ܻ���ʧ���Զ����߹���
  	CAN_InitStructure.CAN_AWUM = DISABLE;
  	CAN_InitStructure.CAN_NART = DISABLE;
  	CAN_InitStructure.CAN_RFLM = DISABLE;
  	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  	CAN_InitStructure.CAN_SJW  = CAN_SJW_2tq;
  	CAN_InitStructure.CAN_BS1  = CAN_BS1_8tq;
  	CAN_InitStructure.CAN_BS2  = CAN_BS2_7tq;
  	CAN_InitStructure.CAN_Prescaler = 9;
  	CAN_Init(CAN1,&CAN_InitStructure);
						   									//����������BMS���͸�����������
  	CAN_FilterInitStructure.CAN_FilterNumber	= 0;
  	CAN_FilterInitStructure.CAN_FilterMode		= CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale		= CAN_FilterScale_32bit;
  	if(u8Type == CAN_REC_ALL)	{							//������BMS��������������
		CAN_FilterInitStructure.CAN_FilterIdHigh	= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x0000;
	}
	if(u8Type == CAN_REC_BMS)	{							//�������е�����
		CAN_FilterInitStructure.CAN_FilterIdHigh	= (0x56F4 >> 13);
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= (0x56F4 & 0x1FFF)<<3;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0xFFFF>>13);;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x1FFF<<3;
	}

  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;
  	CAN_FilterInit(&CAN_FilterInitStructure);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   		//��FLASH������
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); 	  			//ʹ��FIFO0��Ϣ�Һ��ж�
	//MessagePending = CAN_MessagePending(CANFIFO0);		//���عҺŵ���Ϣ���� 

}

/************************************************************
*	BMS���Ľ���
*	��BMS�ϴ��ı��ĸ�ֵ��modbus�Ĵ���������������ȡ
*************************************************************/
void DecodeBMSMessage(INT8U *pu8Mesg, INT8U u8ID)
{
	
	INT8U u8AdrCnt = 0;
	INT8U u8RegAdr = 0;
	INT32U tmp;

	switch(u8ID)
   	{
		case BMS_DEC_BRM:  				//BMS�ͳ�����ʶ����BRM										   
		{
//			for(u8RegAdr = M_BRM_START_ADR; u8AdrCnt < M_BRM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			gtBMS_Dat.BMR_1 = pu8Mesg[0] + (pu8Mesg[1]<<8) + (pu8Mesg[2]<<16);		//ͨѶЭ��汾��
			gtBMS_Dat.BMR_4 =  pu8Mesg[3];					//�������
			gtBMS_Dat.BMR_5 =  (FP32)(pu8Mesg[4] + (pu8Mesg[5]<<8))/10;			//������ض����
			gtBMS_Dat.BMR_7 =  (FP32)(pu8Mesg[6] + (pu8Mesg[7]<<8))/10;			//������ܵ�ѹ
			gtBMS_Dat.BMR_9[0] =  pu8Mesg[8];				//��س������� 4
			gtBMS_Dat.BMR_9[1] =  pu8Mesg[9];		
			gtBMS_Dat.BMR_9[2] =  pu8Mesg[10];		
			gtBMS_Dat.BMR_9[3] =  pu8Mesg[11];
			
			gtBMS_Dat.BMR_13[0] =  pu8Mesg[12];				//�������� 4
			gtBMS_Dat.BMR_13[1] =  pu8Mesg[13];		
			gtBMS_Dat.BMR_13[2] =  pu8Mesg[14];		
			gtBMS_Dat.BMR_13[3] =  pu8Mesg[15];	
			
			gtBMS_Dat.BMR_17 =  pu8Mesg[16]+1985;			//���������������
			gtBMS_Dat.BMR_18 =  pu8Mesg[17];				//�������������  ��
			gtBMS_Dat.BMR_19 =  pu8Mesg[18];				//��
			gtBMS_Dat.BMR_20 = pu8Mesg[19] + (pu8Mesg[20]<<8) + (pu8Mesg[21]<<16);//������ 
			gtBMS_Dat.BMR_23 =  pu8Mesg[22];				//��Ȩ��ʶ
			gtBMS_Dat.BMR_24 =  pu8Mesg[23];				//Ԥ��
			for(u8AdrCnt=0; u8AdrCnt<25; u8AdrCnt++){		//����ʶ����Ϣ 25byte
				gtBMS_Dat.BMR_25[u8AdrCnt] =  pu8Mesg[24+u8AdrCnt];				
			}
			
			memcpy((INT8U*)&ModbusREGDat[73],(INT8U*)&gtBMS_Dat.BMR_1,4);	
			ModbusREGDat[75] =gtBMS_Dat.BMR_4;        						                         //�������
			
			tmp =  gtBMS_Dat.BMR_5;       						              
  			memcpy((INT8U*)&ModbusREGDat[76],(INT8U*)&tmp,4);								 //������ض����   

			tmp =  gtBMS_Dat.BMR_7;       						              
  			memcpy((INT8U*)&ModbusREGDat[78],(INT8U*)&tmp,4);								  //������ܵ�ѹ
  		
			break;		
		}
		case BMS_DEC_VBI:  				//�����͵�ر�ʶ����VBI	
		{
//			for(u8RegAdr = M_VBI_START_ADR; u8AdrCnt < M_VBI_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.VBI_1 =   pu8Mesg[0] + (pu8Mesg[1]<<8) + (pu8Mesg[2]<<16);	//ͨѶЭ��汾��;
			for(u8AdrCnt=0; u8AdrCnt<17; u8AdrCnt++){							//VIN 17byte
				gtBMS_Dat.VBI_4[u8AdrCnt] =  pu8Mesg[3+u8AdrCnt];				
			}
			for(u8AdrCnt=0; u8AdrCnt<4; u8AdrCnt++){							//byte
				gtBMS_Dat.VBI_21[u8AdrCnt] =  pu8Mesg[20+u8AdrCnt];				
			}
			for(u8AdrCnt=0; u8AdrCnt<8; u8AdrCnt++){							//byte
				gtBMS_Dat.VBI_25[u8AdrCnt] =  pu8Mesg[24+u8AdrCnt];				
			}			
			for(u8AdrCnt=0; u8AdrCnt<8; u8AdrCnt++){							//byte
				gtBMS_Dat.VBI_33[u8AdrCnt] =  pu8Mesg[32+u8AdrCnt];				
			}

			memcpy((INT8U*)&ModbusREGDat[82],(INT8U*)gtBMS_Dat.VBI_4,9);								   //����VIN
			memcpy((INT8U*)&ModbusREGDat[91],(INT8U*)&gtBMS_Dat.VBI_21,2);							   //
			memcpy((INT8U*)&ModbusREGDat[93],(INT8U*)&gtBMS_Dat.VBI_25,4);							   //��ع���ϵͳID	
			memcpy((INT8U*)&ModbusREGDat[97],(INT8U*)&gtBMS_Dat.VBI_33,4);							   //��������к�

			break;		
		}
		case BMS_DEC_BCP:  				//���س�����BCP	
		{
//			for(u8RegAdr = M_BCP_START_ADR; u8AdrCnt < M_BCP_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			gtBMS_Dat.BCP_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/100.0f;			//����ģ������������ѹ			
			gtBMS_Dat.BCP_3 = (pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f-400;			//������������ 
			gtBMS_Dat.BCP_5 = pu8Mesg[4]*10;									//������������� 
			gtBMS_Dat.BCP_6 = (pu8Mesg[6] + (pu8Mesg[7]<<8))/10.0f;			//����������ܵ�ѹ 
			gtBMS_Dat.BCP_8 = pu8Mesg[8]-50;									//��������¶� 
			gtBMS_Dat.BCP_9 = (pu8Mesg[9] + (pu8Mesg[10]<<8));						//�������ع���ʣ������ 
			gtBMS_Dat.BCP_11 = (pu8Mesg[11] + (pu8Mesg[12]<<8))/10.0f;			//���������ܵ�ѹ 
			gtBMS_Dat.BCP_13 = pu8Mesg[12];										//���� SOH 

			tmp =  gtBMS_Dat.BCP_1;        						              
  			memcpy((INT8U*)&ModbusREGDat[101],(INT8U*)&tmp,4);							   //����ģ������������ѹ	

			tmp =  gtBMS_Dat.BCP_3;        						                  
  			memcpy((INT8U*)&ModbusREGDat[103],(INT8U*)&tmp,4);							   //������������ 

			ModbusREGDat[105] =gtBMS_Dat.BCP_5;        						                          //�������������

			tmp =  gtBMS_Dat.BCP_6;        						              
  			memcpy((INT8U*)&ModbusREGDat[106],(INT8U*)&tmp,4);							   //������������ 

  			ModbusREGDat[108] =gtBMS_Dat.BCP_8;        						                          //��������¶� 
  			ModbusREGDat[109] =gtBMS_Dat.BCP_9;        						                          //�������ع���ʣ������ 

  			//ModbusREGDat[110] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[111] =gUserDB.ILB_alarm_value_set;  
  			tmp =  gtBMS_Dat.BCP_11;        						              
  			memcpy((INT8U*)&ModbusREGDat[110],(INT8U*)&tmp,4);							   //���������ܵ�ѹ 

  			ModbusREGDat[112] =gtBMS_Dat.BCP_13;        						                          //SOH

			break;		
		}
		case BMS_DEC_BRO:  				//��س��׼������״̬BRO	
		{
//			for(u8RegAdr = M_BRO_START_ADR; u8AdrCnt < M_BRO_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BRO_1 = pu8Mesg[0];//

			break;            
		}
		case BMS_DEC_BCL:  				//��س�缶������BCL�����׶�
		{
//			for(u8RegAdr = M_BCL_START_ADR; u8AdrCnt < M_BCL_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BCL_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//��ѹ����
			gtBMS_Dat.BCL_3 = 400-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f;		//��������
			gtBMS_Dat.BCL_5 = pu8Mesg[4]; 										//���ģʽ

			if(gUserDB.doubleGunPlugStatus == NOK)	 //��ǹ���ʱ
			{
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			//��ѹ����			
				gUserDB.BMS_needCurr = gtBMS_Dat.BCL_3;		//��������
				gUserDB.doubleGun_disCurrent=0;
			}
			else  //˫ǹ���ʱ
			{
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			      //��ѹ����			
				gUserDB.BMS_needCurr = gtBMS_Dat.BCL_3 /2;		       //��������/2 
				gUserDB.doubleGun_disCurrent = gUserDB.BMS_needCurr;  //�������� //��ǹ
			}																	 

			tmp =  gtBMS_Dat.BCL_1; 
  			if(tmp<0)
  			tmp=0;      						                  
  			memcpy((INT8U*)&ModbusREGDat[113],(INT8U*)&tmp,4);							   //��ѹ����

  			//ModbusREGDat[115] =gUserDB.inactiveILB_value;        						              
  			//ModbusREGDat[116] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gUserDB.BMS_needCurr;  
  			if(tmp<0)
  				tmp=0;   						              
  			memcpy((INT8U*)&ModbusREGDat[115],(INT8U*)&tmp,4);							   //��������

  			ModbusREGDat[117] =gtBMS_Dat.BCL_5;        						                              //���ģʽ

						
			break; 
		}
		case BMS_DEC_BCS:  				//��س��״̬BCS�����׶�
		{
//			for(u8RegAdr = M_BCS_START_ADR; u8AdrCnt < M_BCS_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BCS_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//����ѹ����ֵ��V�� 
			gtBMS_Dat.BCS_3 = 400-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//����������ֵ��A��
			gtBMS_Dat.BCS_5 = pu8Mesg[4]-50;	//���������¶ȣ�oC��  
			gtBMS_Dat.BCS_6 = pu8Mesg[5]-50;	//���������¶ȣ�oC��  
			gtBMS_Dat.BCS_7 = pu8Mesg[6];		//��ǰ�ɵ�״̬  SOC��%��  
			gtBMS_Dat.BCS_8 = pu8Mesg[7];		//����ʣ��ʱ�䣨min��

			gUserDB.BMS_Volt = gtBMS_Dat.BCS_1;     //(pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//����ѹ����ֵ��V�� 
			gUserDB.BMS_Curr = gtBMS_Dat.BCS_3;     //800-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//����������ֵ��A��

			tmp =  gtBMS_Dat.BCS_1;  
  			if(tmp<0)
  				tmp=0;        						              
  			memcpy((INT8U*)&ModbusREGDat[118],(INT8U*)&tmp,4);							   //����ѹ����ֵ��V�� 

  			//ModbusREGDat[120] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[121] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gtBMS_Dat.BCS_3;  
  			if(tmp<0)
  				tmp=0;          						              
  			memcpy((INT8U*)&ModbusREGDat[120],(INT8U*)&tmp,4);							   //����������ֵ��A��

  			//OSTimeDlyHMSM(0,0,0,20); 
  			//ModbusREGDat[122] =gtBMS_Dat.BCS_5;        						                          //���������¶ȣ�oC�� 
  			//ModbusREGDat[123] =gtBMS_Dat.BCS_6;        						                          //���������¶ȣ�oC��  
  			ModbusREGDat[124] =gtBMS_Dat.BCS_7;        						                          //��ǰ�ɵ�״̬  SOC��%�� 
  			ModbusREGDat[125] =gtBMS_Dat.BCS_8;        						                          //����ʣ��ʱ�䣨min��

		   ModbusREGDat[123] =gUserDB.BatTempMAx +50 ;      						                          //���������¶ȣ�oC��  

			//gUserDB.cellBatVoltMax	= (pu8Mesg[4]+(pu8Mesg[5]<<4))/100.0f;		
			gUserDB.cellBatVoltMax	= ((pu8Mesg[4]+((pu8Mesg[5]&0x0f)<<8)))/100.0;	//��ߵ����ѹ
			ModbusREGDat[133] =gUserDB.cellBatVoltMax *100;	  // gtBMS_Dat.BSM_1;
			break;         
		}
		case BMS_DEC_BSM:  				//����״̬��ϢBSM�����׶�
		{
//			for(u8RegAdr = M_BSM_START_ADR; u8AdrCnt < M_BSM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BSM_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))&0xfff;
			gtBMS_Dat.BSM_2= pu8Mesg[1]-50;
			gtBMS_Dat.BSM_3 = pu8Mesg[2]-50;
			gtBMS_Dat.BSM_4 = pu8Mesg[3];
			gtBMS_Dat.BSM_5 = pu8Mesg[4];
			gtBMS_Dat.BSM_6 = pu8Mesg[5];					//�������ͬʱ���룬���������������
			gtBMS_Dat.BSM_7 = pu8Mesg[6];
			
			gtBMS_Dat.BSM_6_1 = (pu8Mesg[5] >> 0) &0x01;
			gtBMS_Dat.BSM_6_2 = (pu8Mesg[5] >> 1) &0x01;
			gtBMS_Dat.BSM_6_3 = (pu8Mesg[5] >> 2) &0x01;
			gtBMS_Dat.BSM_6_4 = (pu8Mesg[5] >> 3) &0x01;
			gtBMS_Dat.BSM_6_5 = (pu8Mesg[5] >> 4) &0x01;
			gtBMS_Dat.BSM_6_6 = (pu8Mesg[5] >> 5) &0x01;
			gtBMS_Dat.BSM_6_7 = (pu8Mesg[5] >> 6) &0x01;
			gtBMS_Dat.BSM_6_8 = (pu8Mesg[5] >> 7) &0x01;
			
			gtBMS_Dat.BSM_7_1 = (pu8Mesg[6] >> 0) &0x01;
			gtBMS_Dat.BSM_7_2 = (pu8Mesg[6] >> 1) &0x01;

		//	gUserDB.cellBatVoltMax	= gtBMS_Dat.BSM_1/100.0f;
			gUserDB.BatTempMAx	= pu8Mesg[1]-50;

			//ModbusREGDat[133] =gtBMS_Dat.BSM_1;
			ModbusREGDat[134] =gtBMS_Dat.BSM_2+(gtBMS_Dat.BSM_3<<8);
			ModbusREGDat[135] =gtBMS_Dat.BSM_4+(gtBMS_Dat.BSM_5<<8);
			

			break;                      
		}
		case BMS_DEC_BMV:  				//������С������Ԫ��ѹBMV�����׶�
		{
		 	break;       
		}
		case BMS_DEC_BMT:  				//�������¶�BMT�����׶�
		{
		    break;       
		}
		case BMS_DEC_BSOC:  			//������ɵ�����SOC ,BSOC�����׶�
		{
		  	break;
		}
		case BMS_DEC_BST:  				//BMS��ֹ��籨��BST�����׶�
		{
//			for(u8RegAdr = M_BST_START_ADR; u8AdrCnt < M_BST_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//			gUserDB.cellBatVoltMax = (pu8Mesg[3] + (pu8Mesg[4]<<8))/1000.0f;			//���ص�����ߵ�ѹ��V
//			gUserDB.BatTempMAx = pu8Mesg[6]-50;									//����������¶ȣ�oC��
//            }
			gtBMS_Dat.BST_1 = pu8Mesg[0];
			gtBMS_Dat.BST_2 = pu8Mesg[1];
			gtBMS_Dat.BST_3 = pu8Mesg[2];

			break;                   
		}		
		case BMS_DEC_BSD:  				//BMS���ͳ�Ʊ���BSD�������׶�
		{
//			for(u8RegAdr = M_BSD_START_ADR; u8AdrCnt < M_BSD_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BSD_1 = pu8Mesg[0];										//��ֹ�ɵ�״̬  SOC��%��  
			gtBMS_Dat.BSD_2 = (pu8Mesg[1] + (pu8Mesg[2]<<8))/1000.0f;			//���ص�����͵�ѹ��V
			gtBMS_Dat.BSD_4 = (pu8Mesg[3] + (pu8Mesg[4]<<8))/1000.0f;			//���ص�����ߵ�ѹ��V
			gtBMS_Dat.BSD_6 = pu8Mesg[5]-50;									//����������¶ȣ�oC�� 
			gtBMS_Dat.BSD_7 = pu8Mesg[6]-50;									//����������¶ȣ�oC��

			ModbusREGDat[126] =gtBMS_Dat.BSD_1;        						                          //��ֹ�ɵ�״̬  SOC��%�� 
  			//ModbusREGDat[127] =gUserDB.ILB_alarm_value_set;        						             
  			//ModbusREGDat[128] =gUserDB.ILB_alarm_value_set;  
  			tmp =  gtBMS_Dat.BSD_2*100;       						              
  			memcpy((INT8U*)&ModbusREGDat[127],(INT8U*)&tmp,4);							 //���ص�����͵�ѹ��V

  			//ModbusREGDat[129] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[130] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gtBMS_Dat.BSD_4*100;        						              
  			memcpy((INT8U*)&ModbusREGDat[129],(INT8U*)&tmp,4);							   //���ص�����ߵ�ѹ��V	    			

  			ModbusREGDat[131] =gtBMS_Dat.BSD_6;        						                           //����������¶ȣ�oC�� 
  			ModbusREGDat[132] =gtBMS_Dat.BSD_7;        						                           //����������¶ȣ�oC��

			break;	            
		}
		case BMS_DEC_BEM:  				//BMS������BEM������׶�
		{
			for(u8RegAdr = M_BEM_START_ADR; u8AdrCnt < M_BEM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
			{
				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
            }
			break;
		}
		case BMS_DEC_BFM:  				//BMS���ϱ���BFM�����Ͻ׶�
		{
			for(u8RegAdr = M_BFM_START_ADR; u8AdrCnt < M_BFM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
			{
				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
            }
			break;		
		}
		case BMS_DOUBLE_STATUS:   //BMS ˫ǹ���״̬
		{
		    gUserDB.doubleGunPlugStatus= OK;
			gUserDB.doubleGun_timeout=0;
			//gUserDB.mainGunBms_ready=pu8Mesg[0] ;	   //��ǹBMS״̬
			if( (pu8Mesg[0]& 0X01) ==OK && (pu8Mesg[0]>>4)==1 )	//�ӳ��ǹ      //���յ������ǹ����ʱ,��Ϊ�������ǹ,������ִ��
			{
			    gUserDB.bms_readyStatus=OK;
			    gUserDB.doubleGun_ID=2 ;			  //�������ǹID
				gtBMS_Dat.BCS_7=pu8Mesg[1] ;
					   					//SOC
			//	tmp=(pu8Mesg[3]<<8) + pu8Mesg[2];	   
				gtBMS_Dat.BCL_1=(pu8Mesg[3]<<8) + pu8Mesg[2];	 //�����ѹ

				gtBMS_Dat.BCL_3=(pu8Mesg[5]<<8 )+pu8Mesg[4] ;	   //�������	   
				gUserDB.doubleGun_disCurrent=(pu8Mesg[7]<<8 )+pu8Mesg[6] ;	   //��������
				gUserDB.BMS_needCurr = gUserDB.doubleGun_disCurrent	 ;
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			//��ѹ����			
				
				
				ModbusREGDat[124] =gtBMS_Dat.BCS_7;        						   //��ǰ�ɵ�״̬  SOC��%��
				
				tmp =  gtBMS_Dat.BCL_1; 
  				if(tmp<0)
  				tmp=0;      						                  
  				memcpy((INT8U*)&ModbusREGDat[113],(INT8U*)&tmp,4);							   //��ѹ����  
  			
  				tmp =  gUserDB.BMS_needCurr;  
  				if(tmp<0)
  					tmp=0;   						              
  				memcpy((INT8U*)&ModbusREGDat[115],(INT8U*)&tmp,4);							   //�������� 
							
			}
			
			

		   break;					
		}
		case BMS_DOUBLE_OUT:	 //BMS ˫ǹ������
		{								
			gUserDB.doubleGunPlugStatus= OK;
			gUserDB.doubleGun_timeout=0;

			if(gUserDB.doubleGun_ID==2)	 //�������ǹ
			{
				gtBMS_Dat.BCS_1=(pu8Mesg[1]<<8 )+pu8Mesg[0] ;	   //BMS����ѹ����ֵ
				gtBMS_Dat.BCS_3=(pu8Mesg[3]<<8 )+pu8Mesg[2] ;	   //BMS����������ֵ	
                

				gUserDB.BMS_Volt = gtBMS_Dat.BCS_1;     //(pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//����ѹ����ֵ��V�� 
			    gUserDB.BMS_Curr = gtBMS_Dat.BCS_3;     //800-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//����������ֵ��A��

			    tmp =  gtBMS_Dat.BCS_1;  
  			    if(tmp<0)
  				   tmp=0;        						              
  			   memcpy((INT8U*)&ModbusREGDat[118],(INT8U*)&tmp,4);							   //����ѹ����ֵ��V��
  			
  			  tmp =  gtBMS_Dat.BCS_3;  
  			  if(tmp<0)
  				tmp=0;          						              
  			  memcpy((INT8U*)&ModbusREGDat[120],(INT8U*)&tmp,4);							   //����������ֵ��A��

			  if(gUserDB.BMS_Curr >gUserDB.BMS_needCurr*2 +20 )								   //BMS�����������������20A,�رմӳ���
			     gUserDB.autoRunCtlFlag =STOP; 
			}

		   break;
		}
				
		default: 	break;
	}
	
}


/*********************************************************
�������Ҫ��ID�ŵ�����	
�����յ�������û���õ�ȫ������
***********************************************************/
INT8U Can_DeleteMessageEXT(INT8U IDNum,INT8U ID1,INT8U ID2,INT8U ID3,
	                       INT8U ID4,INT8U ID5,INT8U ID6,INT8U ID7,INT8U ID8)
{
	INT8U ID;
	INT8U Addr;
	if(!CAN_ReadBuffNum()) return 0;						//�����ݾͷ���	
	Addr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	//�õ������ڻ�������λ��
	ID =( CanMessageBuff.Buff[Addr].ExtID>>16)&0xFF;
	if(IDNum>0)	{	if(ID == ID1)	return 0x01;	}
	if(IDNum>1)	{	if(ID == ID2)	return 0x02;	}
	if(IDNum>2)	{	if(ID == ID3)	return 0x03;	}
	if(IDNum>3)	{	if(ID == ID4)	return 0x04;	}
	if(IDNum>4)	{	if(ID == ID5)	return 0x05;	}
	if(IDNum>5)	{	if(ID == ID6)	return 0x06;	}
	if(IDNum>6)	{	if(ID == ID7)	return 0x07;	}
	if(IDNum>7)	{	if(ID == ID8)	return 0x08;	}
	if(IDNum>8)	{	return 0xFF;	}
	CanMessageBuff.OutLen++;								//���������Ⱦ�ɾ������
	return 0;
}

/************************************************************
*	����Э��
*	����ģʽ�µ�������
*	����ģʽ�µ�׼������
*	����ģʽ�µ���Ϣ���պͳ�ʱ�жϼ�������
*	��Ϣ����Ӧ��
*	���õ�����֡ȫ�������
*		
*************************************************************/
INT8S CAN_TP_CM(INT8U ID)
{
	static INT16U Runtime;				//���ڶ�Э����������ݼ�ļ�ʱ����λ10ms��1000ms��ʱ
	INT16U tmp16,kk=0;						//��ʱ����
	INT8U lBuffAddr,ret=0;
	Runtime	++;							//��ʱ����
	//Runtime = 0x00;//debug
	if(Runtime > 500)					//��ʱ����1S
	{									//�����ʱ��CanPackage��ʼ��
		tmp16 = CanPackage.PGN;			//���������������ӾͲ�����������
		CanPackage.PGN = 0x00;			//����Ⱥ���
		CanPackage.DataLen = 0x00 ;
		CanPackage.FrameNum = 0x00;
		CanPackage.FramePointer = 0x00;	//λ��ָ��
		Runtime = 0x00;	  
		if(CanPackage.RxFlag == 0x01)						//�������ͨѶ
		{
			CanPackage.RxFlag = 0x00;						//����־λ����
			TxMessage.ExtId   = 0x1CECF456;	  				//��������
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.DLC 	  = 0x08;						
			TxMessage.Data[0] = 0xFF;						
			TxMessage.Data[1] = 0xFF;		
			TxMessage.Data[2] = 0xFF;
			TxMessage.Data[3] = 0xFF;
			TxMessage.Data[4] = 0xFF;
			TxMessage.Data[5] = 0x00;		
			TxMessage.Data[6] = tmp16;//CanPackage.PGN;
			TxMessage.Data[7] = 0x00;
			
	//		can1_write(TxMessage);
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
			tmp16 = 0;										//�ȴ��������
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}
	   	}
	}								

 	//���Ӵ���Э��											
	lBuffAddr = CanMessageBuff.OutLen ;	 //�õ������ڻ�������λ�� 	
	switch(CanMessageBuff.Buff[lBuffAddr].ExtID )
	{
	   case   0x1CEC56F4:
	   {
	   		if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0x10)	//����������
			{													//����Ⱥ���
				CanPackage.PGN = CanMessageBuff.Buff[lBuffAddr].Data[6];
				CanPackage.DataLen = CanMessageBuff.Buff[lBuffAddr].Data[1]	+ CanMessageBuff.Buff[lBuffAddr].Data[2] *256 ;
				CanPackage.FrameNum = CanMessageBuff.Buff[lBuffAddr].Data[3];//0x0F
				CanPackage.RxFlag = 0x01;						//�Ѿ���������
				CanPackage.FramePointer = 0x01;	   				//λ��ָ���1��ʼ
				Runtime = 0x00;

				TxMessage.ExtId   = 0x1CECF456;
				TxMessage.IDE = CAN_ID_EXT;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.DLC 	  = 0x08;						
				TxMessage.Data[0] = 0x11;						//����������
				TxMessage.Data[1] = CanPackage.FrameNum;		//0x0F		
				TxMessage.Data[2] = CanPackage.FramePointer ;
				TxMessage.Data[3] = 0xFF;
				TxMessage.Data[4] = 0xFF;
				TxMessage.Data[5] = 0x00;		
				TxMessage.Data[6] = CanPackage.PGN;
				TxMessage.Data[7] = 0x00;
//				can1_write(TxMessage);
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
				tmp16 = 0;										//�ȴ��������
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
			}
			else if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0xFF)	//��������
			{
				CanPackage.PGN = 0x00;							//����Ⱥ���
				CanPackage.DataLen = 0x00 ;
				CanPackage.FrameNum = 0x00;	
				CanPackage.FramePointer = 0x00;	   				//λ��ָ�� 					
			}
			Runtime = 0x00;//
			
		}
	   	break;
	case 0x1CEB56F4:
		{				
			if((CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CEB56F4) &&(CanPackage.RxFlag == 0x01))//�����ݴ���
			{
				if(CanMessageBuff.Buff[lBuffAddr].Data[0] == CanPackage.FramePointer)
				{
					tmp16 =	(CanPackage.FramePointer-1) *7 ;
					if(tmp16 > 190 )	
						tmp16= 190;				//��ֹ�������
					CanPackage.Buff[tmp16 + 0] = CanMessageBuff.Buff[lBuffAddr].Data[1];
					CanPackage.Buff[tmp16 + 1] = CanMessageBuff.Buff[lBuffAddr].Data[2];
					CanPackage.Buff[tmp16 + 2] = CanMessageBuff.Buff[lBuffAddr].Data[3];
					CanPackage.Buff[tmp16 + 3] = CanMessageBuff.Buff[lBuffAddr].Data[4];
					CanPackage.Buff[tmp16 + 4] = CanMessageBuff.Buff[lBuffAddr].Data[5];
					CanPackage.Buff[tmp16 + 5] = CanMessageBuff.Buff[lBuffAddr].Data[6];
					CanPackage.Buff[tmp16 + 6] = CanMessageBuff.Buff[lBuffAddr].Data[7];
					CanPackage.FramePointer	++;
					Runtime = 0x00;//
				}
				else											//���ݽ��մ������´��䴦��
				{
					TxMessage.ExtId   = 0x1CECF456;
					TxMessage.IDE = CAN_ID_EXT;
					TxMessage.RTR = CAN_RTR_DATA;
					TxMessage.DLC 	  = 0x08;						
					TxMessage.Data[0] = 0x11;					//����������
					TxMessage.Data[1] = CanPackage.FrameNum - CanPackage.FramePointer +1;//������ʣ�������֡		
					TxMessage.Data[2] = CanPackage.FramePointer ;
					TxMessage.Data[3] = 0xFF;
					TxMessage.Data[4] = 0xFF;
					TxMessage.Data[5] = 0x00;		
					TxMessage.Data[6] = CanPackage.PGN;
					TxMessage.Data[7] = 0x00;
//					can1_write(TxMessage);
					TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
					tmp16 = 0;										//�ȴ��������
					
					while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
					{
						tmp16++;
					}
				}

				if(CanPackage.FramePointer == CanPackage.FrameNum + 1)	//����������ϣ���ΪCanPackage.FramePointer����ָ����һ������λ�ã�����Ҫ+1
				{																					
					TxMessage.ExtId   = 0x1CECF456;
					TxMessage.IDE = CAN_ID_EXT;
					TxMessage.RTR = CAN_RTR_DATA;
					TxMessage.DLC 	  = 0x08;						
					TxMessage.Data[0] = 0x13;						//��������
					TxMessage.Data[1] = CanPackage.DataLen % 256;		
					TxMessage.Data[2] = CanPackage.DataLen / 256;
					TxMessage.Data[3] = CanPackage.FrameNum;
					TxMessage.Data[4] = 0xFF;
					TxMessage.Data[5] = 0x00;		
					TxMessage.Data[6] = CanPackage.PGN;
					TxMessage.Data[7] = 0x00;
//					can1_write(TxMessage);
					TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
					tmp16 = 0;										//�ȴ��������
					while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
					{
						tmp16++;
					}
					
					CanPackage.RxFlag = 0x02;//֡�������
					ret=1;
				}
	  		}
						  
		}
		break;
	}

	Runtime = 0x00;//
	//CanMessageBuff.OutLen ++;

	return ret;
}

/*******************************************************************************
* ��ʼ�����в���
* ��Э������֡��ʼ��
* ��ʼ���׶γ�ʼ��
*	 ������
		1-��ʼ��Ϊ������������
		2-��ʼΪ�������������ӣ������ѳ�����BMS����Ҫ���	
*******************************************************************************/
void ParemeterInitial(INT8U u8Select)
{
	INT8U tmp;
	for(tmp=10;tmp<100;tmp++){ModbusREGDat[tmp] = 0;}
	CanPackage.PGN 		= 0x00;			//�������ݽ��հ�����
	CanPackage.DataLen 	= 0x00 ;
	CanPackage.RxFlag 	= 0x00;
	CanPackage.FrameNum = 0x00;
	CanPackage.FramePointer = 0x00;
	gCommStation = CCS_DEC_BRM;	   	//��ʼ��Ϊ��Ч��״̬
	CanMessageBuff.OutLen = 0x0000;		//��ȡ����������
	CanMessageBuff.InLen  = 0x0000;		//���ջ���������
	if(u8Select == 0x01)				//������������
	{
		CAN_ChargeFlag.NoCharge = 0;	//BMS��Ҫ���
		CAN_ChargeFlag.FullCharge = 0;	//BMSδ����
		CAN_ChargeFlag.Stop = 0;
		gCommStation = CCS_DEC_BRM;	   	//��ʼ��Ϊ��״̬
		CAN_Initial(CAN_REC_BMS);		//������BMS���͸����������� 
	}
	if(u8Select == 0x02)				//���չ㲥���ݣ�����������������
	{
	 	CAN_Initial(CAN_REC_ALL);		//������������
	}
}


/*******************************************************************************
*  	��ȡ���ݵ�λֵ
* 
*******************************************************************************/
INT8U GetBitStatus(INT16U DatX,INT8U Shift)
{
	INT8U tmp;
	if(Shift > 15)	return (0); 
	tmp = (DatX >> Shift) & 0x01;
	return tmp; 
}
/*******************************************************************************
*  	�����ݵ�ָ��λ��1
* 
*******************************************************************************/
void SetBitStatus(INT16U *DatX,INT8U Shift)
{
	if(Shift > 15)	return ;
	*DatX |= 1 << Shift;
}
/*******************************************************************************
*  	�����ݵ�ָ��λ��0
* 
*******************************************************************************/
void ClrBitStatus(INT16U *DatX,INT8U Shift)
{
	if(Shift > 15)	return;
	*DatX &= ~(1 << Shift);
}
void DecodeCCSMessage(INT8U u8CCSID)
{
	INT16U tmp16 = 0x00;
	switch(u8CCSID)
	{
		case CCS_DEC_CRM_0:   	//������ʶ����CRM�����ֽ׶Σ�BMS���ܱ�ʶ
		{
			TxMessage.ExtId   = 0x1801F456;
			TxMessage.DLC 	  = 0x08;
			TxMessage.Data[0] = 0;	//�������
			TxMessage.Data[1] = 0;	//����ͷ���		
			TxMessage.Data[2] = 0;	//����λ��00-���⣬01-����
			TxMessage.Data[3] = 0x00;	//BMS��ʶ�����00-���ܱ�ʶ��01-�ܱ�ʶ
			TxMessage.Data[4] =  0xFF;//�������������ţ�4�ֽ�ascii
			TxMessage.Data[5] = 0xFF;			
			TxMessage.Data[6] =  0xFF;
			TxMessage.Data[7] =  0xFF;						
			break;		
		}		
		case CCS_DEC_CRM_1:   	//������ʶ����CRM�����ֽ׶Σ�BMS�ܱ�ʶ
		{
			TxMessage.ExtId   = 0x1801F456;
			TxMessage.DLC 	  = 0x08;
			TxMessage.Data[0] =  0xAA;	//BMS��ʶ�����00-���ܱ�ʶ��0AA-�ܱ�ʶ
			TxMessage.Data[1] = 0;			
			TxMessage.Data[2] = 0;
			TxMessage.Data[3] = 0x01;
			TxMessage.Data[4] =  0xFF;
			TxMessage.Data[5] =  0xFF;			
			TxMessage.Data[6] =  0xFF;
			TxMessage.Data[7] =  0xFF;						
			break;		
		}
		case CCS_DEC_CTS:		//�������ʱ��ͬ��CTS�����ý׶Σ�ѹ��BCD�� 
		{						//���Բ����÷��룬�����ô�����ڼ���

			TxMessage.ExtId   = 0x1807F456;
			TxMessage.DLC 	  = 0x07;						
			TxMessage.Data[0] = 0;//(ModbusREGDat[13] >> 0) & 0xFF; //��
			TxMessage.Data[1] = 0x20;//(ModbusREGDat[13] >> 8) & 0xFF;//��			
			TxMessage.Data[2] = 0x17;//(ModbusREGDat[14] >> 0) & 0xFF;//ʱ
			TxMessage.Data[3] = 0x25;//(ModbusREGDat[14] >> 8) & 0xFF;//��
			TxMessage.Data[4] = 0x03;//(ModbusREGDat[15] >> 0) & 0xFF;//��
			TxMessage.Data[5] = 0x14;//(ModbusREGDat[15] >> 8) & 0xFF;//�� ��λ			
			TxMessage.Data[6] = 0x20;//(ModbusREGDat[16] >> 0) & 0xFF;//���λ	
	
//			TxMessage.Data[2] = (ModbusREGDat[14] >> 0) & 0xFF;//ʱ
//			TxMessage.Data[3] = (ModbusREGDat[14] >> 8) & 0xFF;//��
//			TxMessage.Data[4] = (ModbusREGDat[15] >> 0) & 0xFF;//��
//			TxMessage.Data[5] = (ModbusREGDat[15] >> 8) & 0xFF;//�� ��λ			
//			TxMessage.Data[6] = (ModbusREGDat[16] >> 0) & 0xFF;//���λ
			break;						
		}
		case CCS_DEC_CML:		//��������������CML�����ý׶�  
		{
			#define CD40017_MAX_OUTVOL	7500
			#define CD40017_MIN_OUTVOL	3000
			#define CD40017_MAX_OUTCUR	2000
			TxMessage.ExtId   = 0x1808F456;
			TxMessage.DLC 	  = 0x06;												
			TxMessage.Data[0] = CD40017_MAX_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
			TxMessage.Data[1] = CD40017_MAX_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;
			TxMessage.Data[2] = CD40017_MIN_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
			TxMessage.Data[3] = CD40017_MIN_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;						
			TxMessage.Data[4] = (4000 - CD40017_MAX_OUTCUR) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//�������ֵ 0.1A  -800A ~ 500A
			TxMessage.Data[5] = (4000 - CD40017_MAX_OUTCUR) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
			TxMessage.Data[6] = 0;
			TxMessage.Data[7] = 0;
			break;						
		}
		case CCS_DEC_CRO:		//�������׼������״̬CRO�����ý׶�  
		{						//0010-����δ��ɳ��׼����0100B-��������ɳ��׼��
			TxMessage.ExtId   = 0x100AF456;
			TxMessage.DLC 	  = 0x01;									
			TxMessage.Data[0] = 0xAA;
			break;
		}
		case CCS_DEC_CCS:		//�������״̬CCS�����׶�  
		{
			#define CD40017_OUTVOL		3600	
			#define CD40017_OUTCUR		0
			#define CD40017_CHARGETIME	10

			TxMessage.ExtId   = 0x1812F456;
			TxMessage.DLC 	  = 0x06;												
//			TxMessage.Data[0] = CD40017_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
//			TxMessage.Data[1] = CD40017_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (8000 - CD40017_OUTCUR) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//�������ֵ 0.1A  -800A ~ 500A
//			TxMessage.Data[3] = (8000 - CD40017_OUTCUR) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
//			TxMessage.Data[4] = CD40017_CHARGETIME % 256;//(ModbusREGDat[24] >> 0) & 0xFF;//�ۼƳ��ʱ�� 1min 0-6000min
//			TxMessage.Data[5] = CD40017_CHARGETIME / 256;//(ModbusREGDat[24] >> 8) & 0xFF;
			TxMessage.Data[0] = ((INT32U)gUserDB.C_outVolt*10 >> 0) & 0xFF;//��ѹ���ֵ��0.1V
			TxMessage.Data[1] = ((INT32U)gUserDB.C_outVolt*10 >> 8) & 0xFF;			
			TxMessage.Data[2] = ((INT32U)gUserDB.C_outCurr*10 >> 0) & 0xFF;//�������ֵ 0.1A  -800A ~ 500A
			TxMessage.Data[3] = ((INT32U)gUserDB.C_outCurr*10 >> 8) & 0xFF;
			TxMessage.Data[4] = (gUserDB.CCS_charge_time >> 0) & 0xFF;//�ۼƳ��ʱ�� 1min 0-6000min
			TxMessage.Data[5] = (gUserDB.CCS_charge_time >> 8) & 0xFF;


			break;						
		}
		case CCS_DEC_CST:			//������ֹ���CST�����׶�  
		{
			TxMessage.ExtId   = 0x101AF456;
			TxMessage.DLC 	  = 0x03;															
			TxMessage.Data[0] = 0x03;//(ModbusREGDat[25] >> 0) & 0xFF;//��ֹ���ԭ��
			TxMessage.Data[1] = 0x00;//(ModbusREGDat[25] >> 8) & 0xFF;//��ֹ������ԭ��			
			TxMessage.Data[2] = 0x00;//(ModbusREGDat[26] >> 0) & 0xFF;//��ֹ������ԭ��
			break;						
		}
		case CCS_DEC_CSD:  				//����ͳ�����ݱ���CSD�������׶� 
		{
			TxMessage.ExtId   = 0x181DF456;
			TxMessage.DLC 	  = 0x05;																	
			TxMessage.Data[0] = (gUserDB.CCS_charge_time >> 0) & 0xFF;//�ۼƳ��ʱ�� 1min
			TxMessage.Data[1] = (gUserDB.CCS_charge_time >> 8) & 0xFF;	
			TxMessage.Data[2] = (gUserDB.CCS_charge_power/10 >> 0) & 0xFF;//�������	0.1kWh 0~500kWh	
			TxMessage.Data[3] = (gUserDB.CCS_charge_power >> 8) & 0xFF;
			TxMessage.Data[4] = (gUserDB.CCS_adr >> 0) & 0xFF;//�������
			break;						
		}
		case CCS_DEC_CEM:  				//����������CEM  
		{
			TxMessage.ExtId   = 0x101FF456;
			TxMessage.DLC 	  = 0x03;																			
			TxMessage.Data[0] = 0;
			TxMessage.Data[1] = 0;			
			TxMessage.Data[2] = 0;
			break;						
		}
		case CCS_DEC_CFM:  		//�������ϱ���CFM  
		{
			TxMessage.ExtId   = 0x1434F456;
			TxMessage.DLC 	  = 0x02;																						
			TxMessage.Data[0] = 0;
			TxMessage.Data[1] = 0;			
			break;						
		}
		case CCS_DOUBLE_STATUS:   //���� ˫ǹ���״̬
		{
		    TxMessage.ExtId   = 0x10F856F4;
			TxMessage.DLC 	  = 0x08;										
						
			if(	gUserDB.doubleGun_ID==1)       //�����ǹ
			{
				TxMessage.Data[0] = 0x10+gUserDB.bms_readyStatus;
				TxMessage.Data[1]=gtBMS_Dat.BCS_7;             //SOC
				TxMessage.Data[2]=(INT32U)gtBMS_Dat.BCL_1;	   //�����ѹ
				TxMessage.Data[3]=(INT32U)gtBMS_Dat.BCL_1>>8;
				TxMessage.Data[4]=(INT32U)gtBMS_Dat.BCL_3;
				TxMessage.Data[5]=(INT32U)gtBMS_Dat.BCL_3>>8;  //�������
				TxMessage.Data[6]=(INT32U)gUserDB.doubleGun_disCurrent;	  //��������
				TxMessage.Data[7]=(INT32U)gUserDB.doubleGun_disCurrent>>8;
			}
			else if( gUserDB.doubleGun_ID==2) //�ӳ��ǹ
			{
				TxMessage.Data[0] = 0x20+gUserDB.bms_readyStatus;
				TxMessage.Data[1]=0;      //SOC
				TxMessage.Data[2]=0;
				TxMessage.Data[3]=0;
				TxMessage.Data[4]=0;
				TxMessage.Data[5]=0;
				TxMessage.Data[6]=0;
				TxMessage.Data[7]=0;
			}
			else
			{
				TxMessage.Data[0]= 0;
				TxMessage.Data[1]=0;      //SOC
				TxMessage.Data[2]=0;
				TxMessage.Data[3]=0;
				TxMessage.Data[4]=0;
				TxMessage.Data[5]=0;
				TxMessage.Data[6]=0;
				TxMessage.Data[7]=0;
			}    

		   break;					
		}
		case CCS_DOUBLE_OUT:	 //˫ǹ������
		{
			TxMessage.ExtId   = 0x10F956F4;
			TxMessage.DLC 	  = 0x08;
																									
			TxMessage.Data[0] = (INT32U)gUserDB.BMS_Volt &0xff;		   //bms����ѹ
			TxMessage.Data[1]=(INT32U)gUserDB.BMS_Volt>>8;             
			TxMessage.Data[2]=(INT32U)gUserDB.BMS_Curr &0xff;	       //bms������
			TxMessage.Data[3]=(INT32U)gUserDB.BMS_Curr>>8;
			TxMessage.Data[4]=(INT32U)gUserDB.C_outVolt &0xff;
			TxMessage.Data[5]=(INT32U)gUserDB.C_outVolt>>8;             //�������
			TxMessage.Data[6]=(INT32U)gUserDB.C_outCurr &0xff;	        //��������
			TxMessage.Data[7]=(INT32U)gUserDB.C_outCurr >>8;

			gUserDB.doubleGunPlugStatus= OK;

		   break;
		}

		default:   tmp16 = 0x01;	break;	
	}
	if(tmp16 != 0x00)	return ;					//��������ڸö�����˳�
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;
  	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
  	tmp16 = 0;										//�ȴ��������
  	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x00FF))
  	{
    	tmp16++;
  	}

}




/*********************************************************************************************************
��������: 	CAN_ClearBuff 
��������:  ��ջ���

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void CAN_ClearBuff()
{
	CanMessageBuff.OutLen = 0x0000;		//��ȡ����������
	CanMessageBuff.InLen  = 0x0000;		//���ջ���������
}
/*********************************************************************************************************
��������: 	can_read 
��������:  ��ָ��CAN�ڶ�ȡ����,������Ӧ�����ݺ������أ�ע����CAN�����ж���ֻ������չ֡

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    len     ���յ����ݰ�����
									     
*********************************************************************************************************/
INT8U can_read(CANRECMSG* pRxMessage) 
{      
	INT8U len=0;
	INT8U  lBuffAddr;

	len=CAN_ReadBuffNum();
	if(len>0)
	{				   										
		lBuffAddr = (CanMessageBuff.OutLen );	//�õ������ڻ�������λ��
		*pRxMessage = CanMessageBuff.Buff[lBuffAddr]; 		

		CanMessageBuff.OutLen++;
		if(CanMessageBuff.OutLen ==CanMessageBuff.InLen && CanMessageBuff.OutLen !=0)
		{	
			CAN_ClearBuff();   //����
		}		
	} 	 

 return  len;
}


/*********************************************************************************************************
��������: 	can_write 
��������:  CAN����

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
INT8U can_write(CanTxMsg * pTxMessage) 
{	 
	INT16U tmp16=0;
	INT8U ret=0;

	TransmitMailbox=CAN_Transmit(CAN1,pTxMessage);									//������Ϣ
																					//�ȴ��������
	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 < 500))   //500ms�ڣ��ж�CAN�Ƿ��ͳɹ�������ʱ�˳�ѭ��
	{
			tmp16++;
			if(	tmp16>20)
				OSTimeDlyHMSM(0,0,0,1);				//����1MS
	}
	if(tmp16>=500)		 
	 	ret=0;				//���ͳ�ʱ
	else
	 	ret=1;  			//���ͳɹ�

	return ret ;
}


/*********************************************************************************************************
��������: 	Scu_Send 
��������:  SCU��������

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
INT8U Scu_Send(INT8U id) 
{
   
 return 0;
}


/*********************************************************************************************************
��������: 	Scu_Read_callback 
��������:  SCU��������

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
INT8U Scu_Read_callback(void ) 
{    

   
  return 1;
}
/*********************************************************************************************************
��������: 	task_canProcess 
��������:  CANͨ���̣߳�����Э��

�䡡��:      buf    ���ջ�����
			len     ���յ����ݳ���

 �䡡��:    0		����ʧ��
			>0		���ճɹ�						     
*********************************************************************************************************/
void task_canProcess(void *pdata)
{	
    INT8U timeCnt=0;

	INT16U RunTime = 0x0000;			//����ʱ���ʱ����λ10ms,
	INT16U OutTime = 0x0000;			//��ʱʱ���ʱ����λ10ms
	INT16U tmp16;
//	INT16U *ptrCTRLRegister;			//�������̵Ŀ��ƼĴ�������ʵ����ModbusREGDat[2]��Ϊ�˷����޸ľ����¶���
	pdata = pdata;
	CAN_Initial(CAN_REC_BMS);  			//���ܽ���BMS���ݣ���Ϊ��ʼ�ͽ�������
	ParemeterInitial(0x01);
	//ptrCTRLRegister = &ModbusREGDat[1];

     while(1)
{
	OSTimeDlyHMSM(0,0,0,10);
//	gCommStation = CCS_DEC_BCL;			//����ʹ��
	RunTime ++;			  				//����ʱ�䣬ֻ����ѭ������������������
	OutTime ++;			  				//�����ڳ�ʱ��⣬���ܺ�����ʱ������һ��
										//�����������һ�û�н������ӵ�����¿�ʼ����
	ModbusREGDat[0] = gCommStation; 	//�ϱ�Э��ת����״̬
//	ModbusREGDat[0] = ( ((1-CAN_ChargeFlag.OutTime) << 0)	//����λ��������1-����ʱ��0��ʾ��ʱ
//					   |(CAN_ChargeFlag.NoCharge<< 1)
//					   |(CAN_ChargeFlag.Charging << 2)
//					   |(CAN_ChargeFlag.Stop<< 3));						
//	if(GetBitStatus(*ptrCTRLRegister,1))					//����������ָ��
//	{ 
//	  	ClrBitStatus(ptrCTRLRegister,1);					//����bit1λ���� ��ָ������
//		if((gCommStation == CCS_DEC_INVALID)  	   			//���ڲ����ӽ׶λ����ǽ����׶ζ��������³�ʼ��
//		   ||(gCommStation == CCS_DEC_OVER)) 				//��֤�������ֶ����պ�������¿�ʼ���
//		{													//��ʵ�������ϸ����Ѿ��������¸�����ʼ��ʱ�������			
//			ClrBitStatus(ptrCTRLRegister,0);				//�����λ������ͣ����־
//			ParemeterInitial(0x01);							//��ʼ����
//			OutTime = 0x00;
//		}
//	}
//---------------------------------------------------------
//	if((ModbusREGDat[3] & 0xFF00)==0x5500)
//	{
//	 	gCommStation = 	ModbusREGDat[3] & 0x000F;
//		ModbusREGDat[3]	= 0x0000;
//		OutTime = 0x00;
//	}

	 if((RunTime%40) == 0x01) 
	 {
	 	DecodeCCSMessage(CCS_DOUBLE_STATUS);
		ModbusREGDat[13]=gUserDB.doubleGun_ID;
	 }
	 if((RunTime%70) == 0x01) 
	 {
	 	DecodeCCSMessage(CCS_DOUBLE_OUT);
	 }

	if(gUserDB.doubleGun_timeout<3000)	     //20�볬ʱ
	{
		gUserDB.doubleGun_timeout++;
	}
	else //˫ǹ��ⳬʱ
	{
		gUserDB.doubleGunPlugStatus=NOK;
		gUserDB.doubleGun_ID=0;
		gUserDB.mainGunBms_ready=NOK;
		gUserDB.doubleGun_disCurrent=0;
	}


//-----------------------------------------------------------
//****************����Ϊ���ĸ����׶�*********************//
	
	if(gCommStation == CCS_DEC_BCL) 	//������ж� ���ٳ��׶ξ��������б�־
	{  	
		CAN_ChargeFlag.Charging = 1;
		gUserDB.bms_readyStatus=OK;
	}
	else 
	{	
		CAN_ChargeFlag.Charging = 0;
		
	    rec_secondGun_can();						    //����˫ǹCAN����

		if(	(gUserDB.doubleGunPlugStatus ==OK && gUserDB.doubleGun_ID==2))  //�ӳ��ǹʱ
		{
		   continue;
		}
		else
		{
		  gUserDB.bms_readyStatus=NOK; 
		}
	}
	
	
	if(MODBUS_DEVICEID ==2 && (gUserDB.sysCtlMode ==2 || gUserDB.sysCtlMode ==0 ))  //������ǹģʽʱ,�ӳ��ǹʱ
	{	  
		 if(CAN_ReadBuffNum())
	   {	
			  CanMessageBuff.OutLen ++;
	   }
		  continue;
	}

	if(OutTime > COMM_OUTTIME) 			//��ʱ�ж�  
	{	
		CAN_ChargeFlag.OutTime = 1; 
		SET_LED_1_OFF; 					//ͨѶ��ʱ
	}
	else 
	{	
		CAN_ChargeFlag.OutTime = 0;
		SET_LED_1_ON	; 				//ͨѶ����ʱ 
	}

	switch(gCommStation)
	{
		case CCS_DEC_INVALID:			//��Ч��״̬������Ϊ��ʱ�ж���
		{										 
			while(CAN_ReadBuffNum())
			{						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
			   		OutTime = 0x00;						
				}
				CanMessageBuff.OutLen ++;
			}												//5S���㲥���ݶ�û�н��յ������ܲ�ͷ�Ѿ��ε���
			if(OutTime == COMM_OUTTIME) ParemeterInitial(0x01);//Ҫ���м���Ƿ����µ�BMS������		
			break;
		}
		case CCS_DEC_VBI:									//ʲô��û�н��յ�,�ڵȴ�����VBI
		{													//250ms����һ�γ������ܱ�ʶ����  
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_0);
			if(OutTime > 6000) 								//һ���ӳ�ʱ���󣬵�λ10ms
			{
				CAN_Initial(CAN_REC_BMS);					//Ҫ��ʼ��CAN����ֹCAN��ʼ��ʧ�ܻ��м�ֹͣ����
				OutTime = COMM_OUTTIME+1;					//��һ�׶�������ӳ�ʱ��������λ����ʱ�����ǲ����˳�
				ParemeterInitial(0x01);
			}												//+1 ��ΪͨѶ��ʱ�ж�׼��

			

			CAN_TP_CM(1) ;
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x30))	//VBI
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_VBI);
			   gCommStation = CCS_DEC_BRM;
			   OutTime = 0x00;
			} 
			
			 

			break ;
		}

		case CCS_DEC_BRM:					   									//�Ѿ����յ�VBI���ȴ�����BRM
		{	
			if(OutTime>COMM_OUTTIME) 
			{
				OutTime=0;
				ParemeterInitial(0x01);					//5S��ʱ����
			}
			
			while(CAN_ReadBuffNum())
			{
				
			  if(CAN_TP_CM(1)) 
				{	 
          CanMessageBuff.OutLen ++;					
					//OutTime = 0x00;									//�����ݽ��ܴ���
					if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x02))			//BRM
					{
						DecodeCCSMessage(CCS_DEC_CRM_1);									//�����յ��������������ݺ��������Ϳ���ʶ������
						DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BRM);
						gCommStation = CCS_DEC_BCP;
						OutTime = 0x00;		
            break ;						
					}					
				}
				else
				{
					CanMessageBuff.OutLen ++;
				}
				//OutTime = 0x00;	
			}
			if((RunTime%25) == 0x01) 
				DecodeCCSMessage(CCS_DEC_CRM_0);			//��Ϊ����һ�����ֳɹ��������ǽ��ճɹ����������ظ���Ϣ 
			break ;													 			//���Խ����з��ڱ��׶ε�ĩβ�����ճɹ��󽫲����Ͳ���ʶ������
		}

		case CCS_DEC_BCP:					  									//�Ѿ����յ�BRM���ȴ����ܵ�س�����
		{																		//250ms����һ�γ����ܱ�ʶ����
			if((RunTime%25) == 0x01) 
				DecodeCCSMessage(CCS_DEC_CRM_1);	
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);					//5S��ʱ����
				OutTime = 0x00;
			}
			
			while(CAN_ReadBuffNum())
			{
				//OutTime = 0x00;	
				if(CAN_TP_CM(1)) 
				{
					CanMessageBuff.OutLen ++;
				  OutTime = 0x00;	   								//�����ݴ�����
					if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x06))			//BCP
			    {
			     DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BCP);
			     gCommStation = CCS_DEC_BRO;
			     OutTime = 0x00;
						break ;
			    }
				}
				else
				{
					CanMessageBuff.OutLen ++;
				}
			}			
			
			break ;
		}
		case CCS_DEC_BRO:					  									//�ȴ�BMS������׼������
		{
			if((RunTime%50) == 0x01) 
				DecodeCCSMessage(CCS_DEC_CTS);				//500msʱ��ͬ������������������
			if((RunTime%25) == 0x02) 
				DecodeCCSMessage(CCS_DEC_CML);				//250ms
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);					//5S��ʱ����
				OutTime = 0x00;	
			}
			while(CAN_ReadBuffNum())
			{				
				Can_DeleteMessageEXT(1,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//������õ�����֡						
				if(CAN_ReadBuffNum())
				{	
					tmp16 = CanMessageBuff.OutLen ; 		//��ȡ�������ĵ�ַ
				  if((CanMessageBuff.Buff[tmp16].ExtID & 0XFFFFFF) == 0x0956F4 )	 		//ȷ���յ�BRO���� �������ȼ�Ϊ4 0x100956F4 �� ����Ϊ5	 0x140956F4

				//	if(CanMessageBuff.Buff[tmp16].ExtID == 0x140956F4 )	 		//ȷ���յ�BRO����
					{	
						if(CanMessageBuff.Buff[tmp16].Data[0] == 0xaa)
							{ 		//��������Ϊ��ɳ��׼��������ʱ�˳�						
							DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BRO);
				   			gCommStation = CCS_DEC_BCS;
				   			OutTime = 0x00;
						}
						else 
						{										//���������1-BMS������2-���ô���
							 ParemeterInitial(0x01);		 	//��ʼ��Ϊ����Ҫ����
							 CAN_ChargeFlag.NoCharge = 1;		//BMS����Ҫ���
						}
					}
					CanMessageBuff.OutLen ++;	  				//ɾ���Ѿ�������ı���
				}			
			}
			break ;
		}
		case CCS_DEC_BCS:						  				//���׶α���
		{														//250ms���׼������
			if((RunTime%25) == 0x01)DecodeCCSMessage(CCS_DEC_CRO); 	
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);	//5S��ʱ����
//			while(CAN_ReadBuffNum()){
//				Can_DeleteMessageEXT(1,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
//				if(CAN_ReadBuffNum()) {
//					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); //��ȡ�������ĵ�ַ			
//					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181156F4){
//						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
//			   			gCommStation = CCS_DEC_BCL;
//			   			OutTime = 0x00;						
//					}
//					CanMessageBuff.OutLen ++;
//				}
//			}			

      while(CAN_ReadBuffNum())
			{
				//OutTime = 0x00;	
				if(CAN_TP_CM(1)) 
				{
					CanMessageBuff.OutLen ++;
				  OutTime = 0x00;	   								//�����ݴ�����
					if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x11))			//BCP
			    {
			     DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BCS);
			     gCommStation = CCS_DEC_BCL;
			     OutTime = 0x00;
						break ;
			    }
				}
				else
				{
					CanMessageBuff.OutLen ++;
				}
			}	
			

			break;
		}
		case CCS_DEC_BCL:						  			//���׶α���
		{													//20ms
		    if( gUserDB.doubleGunPlugStatus == OK)
			{				
				gUserDB.mainGunBms_ready =OK;
				gUserDB.doubleGun_ID=1 ;			  //�����ǹID

				if((RunTime%50) == 0x01)	{DecodeCCSMessage(CCS_DOUBLE_STATUS);  }
				if((RunTime%80) == 0x01)	{DecodeCCSMessage(CCS_DOUBLE_OUT);  }
			}

			if((RunTime%2) == 0x01)	{DecodeCCSMessage(CCS_DEC_CCS);  }
			if(OutTime>COMM_OUTTIME*10) ParemeterInitial(0x01);//5S��ʱ����
//			if(GetBitStatus(*ptrCTRLRegister,0)) 			//��λ������������ָ��
//			{												//��������λ��Ҫ��ֹͣ
//				ClrBitStatus(ptrCTRLRegister,0);			//����0λ���� ��ָ������
//				gCommStation = CCS_DEC_BST;
//				CAN_ChargeFlag.Stop = 1;					//������λ��Ҫ��ֹͣ��־
//				OutTime = 0x00;
//			}							
			
			while(CAN_ReadBuffNum())
			{
			//	ModbusREGDat[2] = (CAN_ChargeFlag.Charging ) ;			 //���״̬
	    // ModbusREGDat[3] = (CAN_ChargeFlag.Charging ) ;			 //���״̬
				//Can_DeleteMessageEXT(5,0x10,0x11,0x19,0x14,0x12,0x00,0x00,0x00);	
//				Can_DeleteMessageEXT(5,0xec,0x10,0x19,0x14,0x12,0xf8,0xf9,0x00);
//				if(CAN_ReadBuffNum())
//					{											//ִ��ɾ�����������Ѿ�û�п��õ����ݣ�����Ҫ���ж�
//					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//��ȡ�������ĵ�ַ
//					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181056F4){
//						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
//			   			OutTime = 0x00;											//��س������
//						//gTest=1;
//						//ModbusREGDat[12]=gTest++;
//					}
//					}
				
				if(CAN_TP_CM(1)) 
				{	 
          CanMessageBuff.OutLen ++;		
			   if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x11))	//BCS
			   {
			      DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BCS);
			      gCommStation = CCS_DEC_BCL;
			      OutTime = 0x00;	
			    } 
				}
					
				//if(CAN_ReadBuffNum())
					{											//ִ��ɾ�����������Ѿ�û�п��õ����ݣ�����Ҫ���ж�
					  tmp16 = CanMessageBuff.OutLen ; 		//��ȡ�������ĵ�ַ
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181056F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
			   			OutTime = 0x00;											//��س������
					}
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1156F4){
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x181156F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
			   			OutTime = 0x00;											//��س����״̬
					}

					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1356F4){
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x181156F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSM);
			   			OutTime = 0x00;											//��س����״̬
					}
					
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x10f856F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_STATUS);
			   			//OutTime = 0x00;											//˫ǹ���״̬
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x10f956F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_OUT);
			   			//OutTime = 0x00;											//˫ǹ���
					}
					
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x141956F4){
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF) == 0x1956F4 )
						{
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BST);
			   			
						if((CanMessageBuff.Buff[tmp16].Data[0] > 0) 			//������������ó�����־
						   ||(CanMessageBuff.Buff[tmp16].Data[1] > 0)
						   ||(CanMessageBuff.Buff[tmp16].Data[2] > 0))
						{	
							CAN_ChargeFlag.FullCharge = 1;	
							gCommStation = CCS_DEC_BST;							//�������ݵ�״̬���ж��Ƿ�Ҫ������һ��
							gUserDB.autoRunCtlFlag =STOP; 	   //�Զ�����
						}
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}
		case CCS_DEC_BST:						  			//���׶α���,BMS��ֹ��籨��
		{										  			//
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CST);  }
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);						//5S��ʱ����
			   	OutTime = 0x00;						
			}										
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1956F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BST);
			   			gCommStation = CCS_DEC_BSD;
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}

		case CCS_DEC_BSD:						  			//�������׶α���,BMSͳ������
		{										  			//500msʱ��ͬ������������������
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CST);  }
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);						//5S��ʱ����
			   	OutTime = 0x00;						
			}						
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1C56F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSD);
			   			gCommStation = CCS_DEC_OVER;
						CAN_Initial(CAN_REC_BMS);							//����������е�����
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}
		case CCS_DEC_OVER:						  //���׶α���,BMS��ֹ��籨��
		{										  //250ms�����׶�ʹ��OutTime��ʱ�����Բ������
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CSD);  }
			if(OutTime>COMM_OUTTIME - 1)  		//�����-1��������ͨѶ��ʱ��־λ�жϴ���
			{
				OutTime = 0x00;
//				if(CAN_ChargeFlag.Stop == 1)	//������ֶ�ֹͣ����5S֮��Ҫ��������	
//				{
//					ParemeterInitial(0x01);			//Ϊ�´�������׼��
//					CAN_Initial(CAN_REC_BMS);		//������BMS���͸�����������
//				}
//				else
//				{									//���ۺ���״̬���ﱾ�׶ξ���������
					ParemeterInitial(0x01);			//5S��ʱ����˵����ͷ�Ѿ��ε��ˣ�Ϊ�´�������׼��
					//CAN_Initial(CAN_REC_ALL);		//
//				}									//�������INVALID״̬
			}		
			break;
		}
		default:break;
	}		

}

}


//CANPACKAGE	CanPackage;



/*********************************************************************************************************************
��������: 	secondGun_task          
��������: 	���ó�����ѹ����
�䡡��:   	��

�䡡��:   	��                 
*********************************************************************************************************************/
void rec_secondGun_can()
{
   INT8U  tmp16;
										
	if(CAN_ReadBuffNum())
	{	
		tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//��ȡ�������ĵ�ַ
		if(CanMessageBuff.Buff[tmp16].ExtID == 0x10F856F4 )	 		//ȷ���յ�BRO����
		{								
			DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_STATUS);		
			CanMessageBuff.OutLen ++;	  				//ɾ���Ѿ�������ı���	
		}
		else if(CanMessageBuff.Buff[tmp16].ExtID == 0x10F956F4 )	 		//ȷ���յ�BRO����
		{
			DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_OUT);
			CanMessageBuff.OutLen ++;	  				//ɾ���Ѿ�������ı���
		}
		else if( gUserDB.doubleGunPlugStatus ==OK && gUserDB.doubleGun_ID==2)  //�ӳ��ǹʱ,���������������
		{
		   CanMessageBuff.OutLen ++;
		}
		//CanMessageBuff.OutLen ++;	  				//ɾ���Ѿ�������ı���						
	}		 
}


