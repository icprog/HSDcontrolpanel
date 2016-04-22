/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "includes.h"




#define SET_LED_1_ON	//	GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define SET_LED_1_OFF	//	GPIO_ResetBits(GPIOB, GPIO_Pin_15);

CANRECMSGBUFF CanMessageBuff;			//����CAN���ջ�����
CANRECMSGBUFF MCM_CanPackage;	//��������һ��can���ջ�����

_T_BMS_DAT gtBMS_Dat;

CanTxMsg TxMessage;
CANPACKAGE	CanPackage;


CANSENDPACKAGE	CanSendPackage;

INT8U  TransmitMailbox;
INT8U  gCommStation = 0x00;				//��¼����ͨѶ�׶�
INT16U ModbusREGDat[200];

struct 
{
	INT8U OutTime;						//ͨѶ��ʱ,0-����ʱ��1��ʾ��ʱ
	INT8U NoCharge;						//BMS����Ҫ��磬1-����Ҫ���
	INT8U FullCharge;					//BMS�ѳ��� ��1-�Ѿ�����
	INT8U Charging;						//����б�־λ��1-�����
	INT8U Stop;							//��ʾ��λ�����͵�ֹͣ��1-����Ϊ��λ��Ҫ���ֹͣ����
}CAN_ChargeFlag;						//�������еı�־
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
	if(tmp16 <= 0x00)	return 0;							//��������,һ�㲻��С��0
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
//  	if(u8Type == CAN_REC_ALL)	{							//������BMS��������������
		CAN_FilterInitStructure.CAN_FilterIdHigh	= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x0000;
//	}
//	if(u8Type == CAN_REC_BMS)	{							//�������е�����
//		CAN_FilterInitStructure.CAN_FilterIdHigh	= (0xE5F4 >> 13);
//	  	CAN_FilterInitStructure.CAN_FilterIdLow		= (0xE5F4 & 0x1FFF)<<3;
//	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0xFFFF>>13);;
//	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x1FFF<<3;
//	}
//
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
//	INT8U u8RegAdr = 0;
	switch(u8ID)
   	{
		case BMS_DEC_BRM:  				//BMS�ͳ�����ʶ����BRM										   
		{
//			for(u8RegAdr = M_BRM_START_ADR; u8AdrCnt < M_BRM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BMR_1 = pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;		//ͨѶЭ��汾��
			gtBMS_Dat.BMR_4 =  pu8Mesg[3];					//�������
			gtBMS_Dat.BMR_5 =  (FP32)(pu8Mesg[4] + pu8Mesg[5]<<8)/10;			//������ض����
			gtBMS_Dat.BMR_7 =  (FP32)(pu8Mesg[6] + pu8Mesg[7]<<8)/10;			//������ܵ�ѹ
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
			gtBMS_Dat.BMR_20 = pu8Mesg[19] + pu8Mesg[20]<<8 + pu8Mesg[21]<<16;//������ 
			gtBMS_Dat.BMR_23 =  pu8Mesg[22];				//��Ȩ��ʶ
			gtBMS_Dat.BMR_24 =  pu8Mesg[23];				//Ԥ��
			for(u8AdrCnt=0; u8AdrCnt<25; u8AdrCnt++){		//����ʶ����Ϣ 25byte
				gtBMS_Dat.BMR_25[u8AdrCnt] =  pu8Mesg[24+u8AdrCnt];				
			}		
			
			break;		
		}
		case BMS_DEC_VBI:  									//�����͵�ر�ʶ����VBI	
		{
//			for(u8RegAdr = M_VBI_START_ADR; u8AdrCnt < M_VBI_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.VBI_1 =   pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;	//ͨѶЭ��汾��;
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
			
			break;		
		}
		case BMS_DEC_BCP:  				//���س�����BCP	
		{
//			for(u8RegAdr = M_BCP_START_ADR; u8AdrCnt < M_BCP_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			
			gtBMS_Dat.BCP_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/100;			//����ģ������������ѹ			
			gtBMS_Dat.BCP_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10;			//������������ 
			gtBMS_Dat.BCP_5 = pu8Mesg[4]*10;									//������������� 
			gtBMS_Dat.BCP_6 = (FP32)(pu8Mesg[5] + pu8Mesg[6]<<8)/10;			//����������ܵ�ѹ 
			gtBMS_Dat.BCP_8 = pu8Mesg[7]-50;									//��������¶� 
			gtBMS_Dat.BCP_9 = (pu8Mesg[8] + pu8Mesg[9]<<8);						//�������ع���ʣ������ 
			gtBMS_Dat.BCP_11 = (FP32)(pu8Mesg[10] + pu8Mesg[11]<<8)/10;			//���������ܵ�ѹ 
			gtBMS_Dat.BCP_13 = pu8Mesg[12];										//���� SOH 
			
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
			
			gtBMS_Dat.BCL_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//��ѹ����
			gtBMS_Dat.BCL_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10-800;		//��������
			gtBMS_Dat.BCL_5 = pu8Mesg[4]; 										//���ģʽ
			
			break; 
		}
		case BMS_DEC_BCS:  				//��س��״̬BCS�����׶�
		{
//			for(u8RegAdr = M_BCS_START_ADR; u8AdrCnt < M_BCS_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BCS_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//����ѹ����ֵ��V�� 
			gtBMS_Dat.BCS_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10 - 800;		//����������ֵ��A��
			gtBMS_Dat.BCS_5 = pu8Mesg[4]-50;	//���������¶ȣ�oC��  
			gtBMS_Dat.BCS_6 = pu8Mesg[5]-50;	//���������¶ȣ�oC��  
			gtBMS_Dat.BCS_7 = pu8Mesg[6];		//��ǰ�ɵ�״̬  SOC��%��  
			gtBMS_Dat.BCS_8 = pu8Mesg[7];		//����ʣ��ʱ�䣨min��
			
			break;         
		}
		case BMS_DEC_BSM:  				//����״̬��ϢBSM�����׶�
		{
//			for(u8RegAdr = M_BSM_START_ADR; u8AdrCnt < M_BSM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BSM_1 = (pu8Mesg[0] + pu8Mesg[1]<<8);
			gtBMS_Dat.BSM_3 = pu8Mesg[2];
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
			gtBMS_Dat.BSD_2 = (FP32)(pu8Mesg[1] + pu8Mesg[2]<<8)/1000;			//���ص�����͵�ѹ��V
			gtBMS_Dat.BSD_4 = (FP32)(pu8Mesg[3] + pu8Mesg[4]<<8)/1000;			//���ص�����ߵ�ѹ��V
			gtBMS_Dat.BSD_6 = pu8Mesg[5]-50;									//����������¶ȣ�oC�� 
			gtBMS_Dat.BSD_7 = pu8Mesg[6]-50;									//����������¶ȣ�oC��
			break;	            
		}
		case BMS_DEC_BEM:  				//BMS������BEM������׶�
		{
//			for(u8RegAdr = M_BEM_START_ADR; u8AdrCnt < M_BEM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			break;
		}
		case BMS_DEC_BFM:  				//BMS���ϱ���BFM�����Ͻ׶�
		{
//			for(u8RegAdr = M_BFM_START_ADR; u8AdrCnt < M_BFM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			break;		
		}		
		default: 	break;
	}
	
}


typedef struct {
	INT8U 	FlagValid;					//0,�ý׶���Ч��1-�ý׶���Ч����δ��ɾ��
	INT8U 	WorkType;					//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
	FP32 	SetValue;					//�������ֵ
	FP32 	LimitValue;					//�������ֵ
	FP32 	StopValue;					//���ֵֹͣ
}_T_MCT_STAGE;

typedef struct {
	INT8U 	FlagValid;					//������Ч��־λ��=1����Ч
	FP32	MaxCellVoltage;				//��������������ѹ��ѹ
	INT16S	MaxTemperature;				//����������¶�
}_T_MCT_SINGLE;

typedef struct {
	INT8U 	Switch;						//���ػ��źţ�0-ֹͣ��1-����	
	INT8U 	ChargeNum;					//���׶�����
	INT8U 	ChargerID;					//�������
	INT8U 	NowStage;					//��ǰ�����ĳ��׶�
	FP32 	Power;						//�������	
	INT8U	BMSStage;					//��bmsͨѶ�����ĸ��׶�
	_T_MCT_SINGLE 	MCT_Single;			//������Ʋ���	
	_T_MCT_STAGE  	MCT_Stage[6];		//�����׶ε�ֵ
	
}_T_MCT_DAT;

_T_MCT_DAT gtMCT_Dat;//����һ���ṹ�����
_T_METER_DAT gtMeter_Dat;
//gtMCT_Dat.NowStage = 1;//�ñ���һ��Ҫ��ʼ��

//����ն� MCT
//����   CCM
//����0��ʾû�з�������֡�����߸�ID��Ч��һ����ǰ��
INT8U DecodeCCMMessage(INT32U ID)
{
	CanTxMsg ltTxMsg;
	INT16U tmp;

	switch(ID)
	{
		case MCM_RETURNSET_STAGE_ALL_SINGLE	:			//��վ����ȫ���׶ε����������Ʋ���,�������������û�����þͣ�����
		case MCM_RETURNSET_SINGLE:						//��վ���ص����������Ʋ���
		{
			  if(gtMCT_Dat.MCT_Single.FlagValid == 0) return 0;
		}
		case MCM_STATE2	:								//���׮/��״̬֡2		
		{					
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;		//���׶���
			ltTxMsg.Data[1] = gtMCT_Dat.NowStage;	
			if(ID == MCM_RETURNSET_STAGE_ALL_SINGLE) {
				ltTxMsg.Data[1] = 0x00;			//�������������
			}
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Single.MaxCellVoltage*1000)%256;	//������ߵ�ѹ
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Single.MaxCellVoltage*1000)/256;
			ltTxMsg.Data[4] = (INT8U)(gtMCT_Dat.MCT_Single.MaxTemperature + 50);
			ltTxMsg.Data[5] = 0x00;		
			ltTxMsg.Data[6] = 0x00;
			ltTxMsg.Data[7] = 0x00;			
			break;
		}
		case MCM_RETURNSET_STAGE_ALL_1	:				//��վ�������н׶γ����Ʋ���
		case MCM_RETURNSET_STAGE_ALL_2	:
		case MCM_RETURNSET_STAGE_ALL_3	:
		case MCM_RETURNSET_STAGE_ALL_4	:
		case MCM_RETURNSET_STAGE_ALL_5	:
		case MCM_RETURNSET_STAGE_ALL_6	:
		{			
			tmp = ((ID>>16)&0xFF) - 0x4D;
			if(tmp > 5)  return 0;
			if(gtMCT_Dat.MCT_Stage[tmp].FlagValid == 0x00) return 0;
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;								//���׶���
			ltTxMsg.Data[1] = 0xA0 + gtMCT_Dat.MCT_Stage[tmp].WorkType;			//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) %256;					//�������ֵ,���ڻ���ȷ��������С����
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) %256;				//�������ֵ
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) %256	;				//���ֵֹͣ	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) /256	;
			break;
		}
		case MCM_RETURNSET_SPECIAL_STAGE_1	:			//��վ����ָ���׶γ����Ʋ�������6���׶Σ�
		case MCM_RETURNSET_SPECIAL_STAGE_2	:
		case MCM_RETURNSET_SPECIAL_STAGE_3	:
		case MCM_RETURNSET_SPECIAL_STAGE_4	:
		case MCM_RETURNSET_SPECIAL_STAGE_5	:
		case MCM_RETURNSET_SPECIAL_STAGE_6	:		
		{			
			tmp = ((ID>>16)&0xFF) - 0x54;
			if(tmp > 5)  return 0;
			if(gtMCT_Dat.MCT_Stage[tmp].FlagValid == 0x00) return 0;
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;								//���׶���
			ltTxMsg.Data[1] = 0xA0 + gtMCT_Dat.MCT_Stage[tmp].WorkType;			//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) %256;					//�������ֵ,���ڻ���ȷ��������С����
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) %256;				//�������ֵ
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) %256	;				//���ֵֹͣ	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) /256	;
			break;
		}		
		case MCM_STATE0	:						//���׮/��״̬֡0
		{	
			static INT8U LifeTime ;
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMCT_Dat.NowStage&0x0F ;
			if((tmp >6) ||(tmp <1)) tmp = 1;
			ltTxMsg.Data[0] = tmp +(gtMCT_Dat.MCT_Stage[tmp-1].WorkType&0x0F)<<4; //��ǰ��緽ʽ�͵�ǰ�����׶�
			ltTxMsg.Data[1] = gtMCT_Dat.BMSStage;		//��ǰ״̬���ǳ��׶Σ����ֽ׶Σ����ý׶Σ����׶Σ������׶Σ�������׶�
			ltTxMsg.Data[2] = 0xFF;		//����
			ltTxMsg.Data[3] = 0xFF;		//����
			ltTxMsg.Data[4] = 0xFF;		//����			
			ltTxMsg.Data[5] = 0xFF;		//����	
			ltTxMsg.Data[6] = 0xFF;		//����					
			ltTxMsg.Data[7] = LifeTime;	//����ֵ
			if(LifeTime >= 15)	{
				LifeTime = 0;			//����ֵ��1-15��������0-15����������Ҫ ++
			}
			LifeTime ++;		
			break;
		}
		case MCM_STATE1	:						//���׮/��״̬֡1
		{
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMeter_Dat.ActivePower ;
			ltTxMsg.Data[0] = tmp % 256;						//������ѹ�ܹ���
			ltTxMsg.Data[1] = tmp /256;
			
			tmp = gtMeter_Dat.AH ;			
			ltTxMsg.Data[2] = tmp % 256;						
			ltTxMsg.Data[3] = tmp /256;

			tmp = gtMeter_Dat.kWh ;			
			ltTxMsg.Data[4] = tmp % 256;						
			ltTxMsg.Data[5] = tmp /256;

			tmp = gtMeter_Dat.ReactivePower ;			
			ltTxMsg.Data[6] = tmp % 256;						//����޹�����
			ltTxMsg.Data[7] = tmp /256;			
			
			break;
		}													//״̬֡2�ڿ�ͷ����
		case MCM_STATE3	:									//���׮/��״̬֡3
		{			
			tmp = gtMCT_Dat.NowStage&0x0F ;
			if((tmp >6) ||(tmp <1)) tmp = 1;				//��ֹ�������
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.NowStage&0x0F;		//���׶���
			ltTxMsg.Data[1] = gtMCT_Dat.MCT_Stage[tmp-1].WorkType;									//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].SetValue *10) %256;				//�������ֵ,���ڻ���ȷ��������С����
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].LimitValue *10) %256;				//�������ֵ
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].StopValue *10) %256;				//���ֵֹͣ	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].StopValue *10) /256 ;
			break;
		}

		case MCM_STATE4	:										//���׮/��״̬֡4
		{
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMeter_Dat.ACAllPower;	 
			ltTxMsg.Data[0] = tmp % 256;						//������ѹ�ܹ���
			ltTxMsg.Data[1] = tmp /256;
			ltTxMsg.Data[2] = gtMeter_Dat.Wave_A_Vol;			//��ѹг��ABC
			ltTxMsg.Data[3] = gtMeter_Dat.Wave_B_Vol;	
			ltTxMsg.Data[4] = gtMeter_Dat.Wave_C_Vol;	
			ltTxMsg.Data[5] = gtMeter_Dat.Wave_A_Cur;			//����г��ABC
			ltTxMsg.Data[6] = gtMeter_Dat.Wave_B_Cur;
			ltTxMsg.Data[7] = gtMeter_Dat.Wave_C_Cur;			
			break;
		}
		default:break;
	}
	ltTxMsg.ExtId  = ID;	
	ltTxMsg.IDE = CAN_ID_EXT;
	ltTxMsg.RTR = CAN_RTR_DATA;
	tmp = CAN_Transmit(CAN1,&ltTxMsg);	//������Ϣ
	if(CAN_TransmitStatus(CAN1,tmp) == CAN_NO_MB)	  //���û�п�����͵ȴ�һ���ٷ���
	{
		OSTimeDlyHMSM(0,0,0,2);	
		CAN_Transmit(CAN1,&ltTxMsg);				
	}
 	return 1;
}

void DecodeMCTMessage(CANRECMSG ptrMsg)
{
	INT32U tmp;
	switch(ptrMsg.ExtID)
	{
		case  MCM_STOP_ALL	:								//��վ�����г��׮/��ͣ��
		{
			if(ptrMsg.Data[0] == 0x55) {
				gtMCT_Dat.Switch = 0x00;					//����ֹͣ���
			}
			break;
		}
		case MCM_START_SINGLE:								//��վ��ĳ̨���׮/�����
		{
			if(ptrMsg.Data[0] == 0x55) {
				if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID) 
				{	gtMCT_Dat.Switch = 0x01;	}			//�������������Լ��ı�ž���������
			}
			break;
		}
		case MCM_STOP_SINGLE:								//��վ��ĳ̨���׮/��ֹͣ���
		{
			if(ptrMsg.Data[0] == 0x55) {
				if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID) 
				{	gtMCT_Dat.Switch = 0x00;	}			//�������������Լ��ı�ž�����ֹͣ
			}				
			break;
		}

		case MCM_SET_SINGLE	:								//��վ���õ����������Ʋ���
		{
			if((ptrMsg.Data[0]>>5) == 0x000) //
			{
				;
			}
			else if(((ptrMsg.Data[0]>>5) == 0x01)||(ptrMsg.Data[0]>>5) == 0x02)//��ӿ��Ʋ������߸��Ŀ��Ʋ���
			{
				gtMCT_Dat.MCT_Single.FlagValid = 1;
				gtMCT_Dat.ChargeNum =ptrMsg.Data[1];							//���׶���
				gtMCT_Dat.MCT_Single.MaxCellVoltage = (FP32)(ptrMsg.Data[2] +ptrMsg.Data[3]*256)/1000;//������ߵ�ѹ
				gtMCT_Dat.MCT_Single.MaxTemperature =ptrMsg.Data[4] -50;
			}
			else if((ptrMsg.Data[0]>>5) == 0x03)								//ɾ�����Ʋ���
			{
				gtMCT_Dat.MCT_Single.FlagValid = 0;								//������Ʋ�����Ч
				//��������Ҳ�������
			}
			else {;}
			break;
		}
		case MCM_SET_STAGE_1:								//��վ���ý׶γ����Ʋ���
		case MCM_SET_STAGE_2:
		case MCM_SET_STAGE_3:
		case MCM_SET_STAGE_4:
		case MCM_SET_STAGE_5:
		case MCM_SET_STAGE_6:								//����ͨ��ID�жϣ�Ҳ����ͨ���ڲ������ж�
		{
			if((ptrMsg.Data[0]>>5) == 0x000) 
			{
				;
			}
			else if(((ptrMsg.Data[0]>>5) == 0x01)||((ptrMsg.Data[0]>>5) == 0x02))//��ӿ��Ʋ������߸��Ŀ��Ʋ���
			{
				tmp = (ptrMsg.ExtID>>16)&0xFF - 0x44;			//�ڼ����׶�
				if(tmp > 6-1) 	break;							//����0��ʼ���׶��Ǵ�1��ʼ
				gtMCT_Dat.ChargeNum =(ptrMsg.Data[1]>>4)&0x0F;	//���׶���
				gtMCT_Dat.MCT_Stage[tmp].FlagValid = 1;
				gtMCT_Dat.MCT_Stage[tmp].WorkType  = ptrMsg.Data[1]&0x0F;//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
				gtMCT_Dat.MCT_Stage[tmp].SetValue  = (FP32)(ptrMsg.Data[2] +ptrMsg.Data[3]*256)/10;//�������ֵ,���ڻ���ȷ��������С����
				gtMCT_Dat.MCT_Stage[tmp].LimitValue= (FP32)(ptrMsg.Data[4] +ptrMsg.Data[5]*256)/10;//�������ֵ
				gtMCT_Dat.MCT_Stage[tmp].StopValue = (FP32)(ptrMsg.Data[6] +ptrMsg.Data[7]*256)/10;//���ֵֹͣ					
			}
			else if((ptrMsg.Data[0]>>5) == 0x03)			//ɾ�����Ʋ���
			{
				tmp = (ptrMsg.ExtID>>16)&0xFF - 0x44;		//�ڼ����׶�
				if(tmp > 6-1) 	break;							//����0��ʼ���׶��Ǵ�1��ʼ
				gtMCT_Dat.MCT_Stage[tmp].FlagValid = 0;		//�׶ο��Ʋ�����Ч
				gtMCT_Dat.MCT_Stage[tmp].WorkType  = 0x00;
				gtMCT_Dat.MCT_Stage[tmp].SetValue  = 0.0;
				gtMCT_Dat.MCT_Stage[tmp].LimitValue= 0.0; 
				gtMCT_Dat.MCT_Stage[tmp].StopValue = 0.0; 
			}
			else {;}
			break;
		}

		case MCM_READSET_ALL:								//��վ��ȡ���׮/�����г����Ʋ���
		{													//����������ʱ����α�֤bms�ķ��ͼ������Ӱ�죿����ֱ�ӵȴ�����װ�ڵ����ͻ��������棿
			if(ptrMsg.Data[0] == 0x55) {					//����ȫ������������ն�				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_SINGLE))  {			//��վ����ȫ���׶ε����������Ʋ���
					OSTimeDlyHMSM(0,0,0,6);
				}
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_1))  {				//��վ�������н׶γ����Ʋ���
					OSTimeDlyHMSM(0,0,0,6);
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_2))  {				//��վ�������н׶γ����Ʋ���
					OSTimeDlyHMSM(0,0,0,6);				
				}
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_3))  {				//��վ�������н׶γ����Ʋ���
					OSTimeDlyHMSM(0,0,0,6);				
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_4))  {				//��վ�������н׶γ����Ʋ���
					OSTimeDlyHMSM(0,0,0,6);				
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_5))  {				//��վ�������н׶γ����Ʋ���
					OSTimeDlyHMSM(0,0,0,6);		
				}				
				DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_6) ; 					//��վ�������н׶γ����Ʋ���	,���һ֡����Ҫ���			
			}				
			break;
		}
		case MCM_READSET_SPECIAL_PARA:						//��վ��ȡ���׮/���ض������Ʋ���
		{
			if(ptrMsg.Data[0] == 0x55) {
				tmp = ptrMsg.Data[1];						//��ȡҪ��ȡ�Ľ׶�
				if(tmp > 6) break;							//�����ڸù����׶�
				if(tmp == 0x00) tmp = gtMCT_Dat.NowStage;	//=0,Ҫ��ȡ��ǰ�����׶�
				if(tmp == 0x00) tmp = 1;//NowStage����ȷ
				if(DecodeCCMMessage(MCM_RETURNSET_SINGLE))  {//��վ���ص����������Ʋ���
					OSTimeDlyHMSM(0,0,0,6);		
				}
				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)((tmp-1)<<16));//��վ����ָ���׶γ����Ʋ�������6���׶Σ�
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(0<<16));
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(1<<16));
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(2<<16));
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(3<<16));
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(4<<16));
//				OSTimeDlyHMSM(0,0,0,6);
//				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)(5<<16));											
			}				
			break;
		}

		case MCM_SET_POWER	:								//��վ���ó��׮/���������
		{
			gtMCT_Dat.Power = ptrMsg.Data[0] +ptrMsg.Data[1]*256;
			gtMCT_Dat.ChargerID = ptrMsg.Data[3];
			break;
		}		
		default:break;
	}
}
/*******************************************************************************
*	���״̬֡0 ÿ1Sһ��
*	���״̬֡1 ÿ10Sһ��
*	���״̬֡1 ÿ10Sһ��
********************************************************************************/
#define INTERVAL_1000MS	100	//���1s
#define INTERVAL_10S	1000//���10s
#define INTERVAL_60S	6000

void MCMDeal(INT8U stage)
{
	static INT16U times[3] = {0,1,2};	//������Ҫȫ����0����ֹcan����������
	INT8U tmp;
	times[0]++;							//ʱ������
	times[1]++;
	times[2]++;
	if(times[0] >= INTERVAL_1000MS) {	//ÿ1s����һ�Σ�������û�й�ϵ
		DecodeCCMMessage(MCM_STATE0);
		times[0] = 0x00;
	}
	
	if(times[1] == INTERVAL_10S) {		//ÿ10s����һ�Σ������ڳ��׶�
		if(gtMCT_Dat.BMSStage == 0x03) 
		{
			DecodeCCMMessage(MCM_STATE1);
		}
	}	
	if(times[1] >= INTERVAL_10S+1) {	//ÿ10s����һ�Σ������ڳ��׶�
		if(gtMCT_Dat.BMSStage == 0x03) 
		{
			DecodeCCMMessage(MCM_STATE4);
		}
		times[1] = 0x00;
	}
	
	if( (stage == CCS_DEC_INVALID)) {	//���׶Σ�0-�ǳ��׶Σ�1-���֣�2-���ã�3-��磬4-������5-������ ||(stage == )
		tmp = 0x00;
	}
	else if((stage == CCS_DEC_VBI)||(stage == CCS_DEC_BRM)) {
		tmp = 0x01;
	}
	else if((stage == CCS_DEC_BCP)||(stage == CCS_DEC_BRO)) {
		tmp = 0x02;
	}	
	else if((stage == CCS_DEC_BCS)||(stage == CCS_DEC_BCL)) {
		tmp = 0x03;
	}	
	else if((stage == CCS_DEC_BST)||(stage == CCS_DEC_BSD)) {
		tmp = 0x04;
	}
	else {												//CCS_DEC_OVER	0x09 
		tmp = 0x05;
	}
	
	if(gtMCT_Dat.BMSStage != tmp) {		//�϶������׶η����˱仯
		gtMCT_Dat.BMSStage = tmp;		//���浱ǰ�����׶�
		times[2] = INTERVAL_60S;		//�������ݾͻ�������ʼ����
	}									
	if(times[2] == INTERVAL_60S) {		//ÿ60s����һ�Σ������׶η����仯ʱ����
		DecodeCCMMessage(MCM_STATE2);
	}	
	if(times[2] >= INTERVAL_60S+1) {	//ÿ60s����һ�Σ�
		DecodeCCMMessage(MCM_STATE3);
		times[2] = 0x00;
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
	
	if((CanMessageBuff.Buff[Addr].ExtID &0xFFFF) == 0x5600) {//�ǳ���ն˷��͵�����
		DecodeMCTMessage(CanMessageBuff.Buff[Addr]);
//		DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_SINGLE);
	 	CanMessageBuff.OutLen++;							//���������Ⱦ�ɾ������
	}
	if(!CAN_ReadBuffNum()) return 0;						//�����ݾͷ���	
	
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
	INT16U tmp16;						//��ʱ����
	INT8U lBuffAddr;
	Runtime	++;							//��ʱ����
	//Runtime = 0x00;//debug
	if(Runtime > 100)					//��ʱ����1S
	{									//�����ʱ��canpackage��ʼ��
		tmp16 = CanPackage.PGN;			//���������������ӾͲ�����������
		CanPackage.PGN = 0x00;			//����Ⱥ���
		CanPackage.DataLen = 0x00 ;
		CanPackage.FrameNum = 0x00;
		CanPackage.FramePointer = 0x00;	//λ��ָ��
		Runtime = 0x00;
		if(CanPackage.RxFlag == 0x01)						//�������ͨѶ
		{
			CanPackage.RxFlag = 0x00;						//����־λ����
			TxMessage.ExtId   = 0x1CECF4E5;	  				//��������
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
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
			tmp16 = 0;										//�ȴ��������
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}
	   	}
	}
	while(CAN_ReadBuffNum())  								//ֱ���ҵ���Чֵ
	{
		if(Can_DeleteMessageEXT(2,0xEC,0xEB,0x00,0x00,0x00,0x00,0x00,0x00) > 0)//������õ�����֡
		{	  break ;	 }								
	}
	if(!CAN_ReadBuffNum())	return 0;								

 	//���Ӵ���Э��											//�õ������ڻ�������λ��
	lBuffAddr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	 
	//tmp16 = (CanMessageBuff.Buff[tmp16].ExtID>>16)&0xFF;
	if(CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CECE5F4)	//�����ݴ���
	{
	 	if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0x10)	//����������
		{													//����Ⱥ���
			CanPackage.PGN = CanMessageBuff.Buff[lBuffAddr].Data[6];
			CanPackage.DataLen = CanMessageBuff.Buff[lBuffAddr].Data[1]	+ CanMessageBuff.Buff[lBuffAddr].Data[2] *256 ;
			CanPackage.FrameNum = CanMessageBuff.Buff[lBuffAddr].Data[3];//0x0F
			CanPackage.RxFlag = 0x01;						//�Ѿ���������
			CanPackage.FramePointer = 0x01;	   				//λ��ָ���1��ʼ
			Runtime = 0x00;

			TxMessage.ExtId   = 0x1CECF4E5;
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
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
			tmp16 = 0;										//�ȴ��������
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}	
		}

		if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0xFF)	//��������
		{
			CanPackage.PGN = 0x00;							//����Ⱥ���
			CanPackage.DataLen = 0x00 ;
			CanPackage.FrameNum = 0x00;	
			CanPackage.FramePointer = 0x00;	   				//λ��ָ��
			Runtime = 0x00;//	
		}
		CanMessageBuff.OutLen ++;
	}
	while(CAN_ReadBuffNum())
	{				   										//���ֽڴ�������
		if(Can_DeleteMessageEXT(2,0xEC,0xEB,0x00,0x00,0x00,0x00,0x00,0x00) == 0x01)
		{	  break ;	 }									//�õ������ڻ�������λ��	
		lBuffAddr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	
		if((CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CEBE5F4) &&(CanPackage.RxFlag == 0x01))//�����ݴ���
		{
			if(CanMessageBuff.Buff[lBuffAddr].Data[0] == CanPackage.FramePointer)
			{
				tmp16 =	(CanPackage.FramePointer-1) *7 ;
				if(tmp16 > 190 )	tmp16= 190;				//��ֹ�������
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
				TxMessage.ExtId   = 0x1CECF4E5;
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
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
				tmp16 = 0;										//�ȴ��������
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
			}
			if(CanPackage.FramePointer == CanPackage.FrameNum + 1)	//����������ϣ���ΪCanPackage.FramePointer����ָ����һ������λ�ã�����Ҫ+1
			{																					
				TxMessage.ExtId   = 0x1CECF4E5;
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
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//������Ϣ
				tmp16 = 0;										//�ȴ��������
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
				Runtime = 0x00;//
				CanPackage.RxFlag = 0x02;//֡�������
			}
	  	}
		CanMessageBuff.OutLen ++;	//���������ٶ�ȡ���ݣ���ΪͨѶ�ٶȱȽ�����Ӧ�ò��Ḳ��					
	}
	return 1;
}


void DecodeCCSMessage(INT8U u8CCSID)
{
	INT16U tmp16 = 0x00;
	switch(u8CCSID)
	{
		case CCS_DEC_CRM_0:   	//������ʶ����CRM�����ֽ׶Σ�BMS���ܱ�ʶ
		{

		
			TxMessage.ExtId   = 0x1801F4E5;
			TxMessage.DLC 	  = 0x08;
//			TxMessage.Data[0] = (ModbusREGDat[9] >> 0) & 0xFF;	//�������
//			TxMessage.Data[1] = (ModbusREGDat[9] >> 8) & 0xFF;	//����ͷ���		
//			TxMessage.Data[2] = (ModbusREGDat[10] >> 0) & 0xFF;	//����λ��00-���⣬01-����
//			TxMessage.Data[3] = 0x00;	//BMS��ʶ�����00-���ܱ�ʶ��01-�ܱ�ʶ
//			TxMessage.Data[4] = (ModbusREGDat[11] >> 0) & 0xFF;//�������������ţ�4�ֽ�ascii
//			TxMessage.Data[5] = (ModbusREGDat[11] >> 8) & 0xFF;			
//			TxMessage.Data[6] = (ModbusREGDat[12] >> 0) & 0xFF;
//			TxMessage.Data[7] = (ModbusREGDat[12] >> 8) & 0xFF;	

			TxMessage.Data[0] = gtBMS_Dat.CRM_1;	//�������
			TxMessage.Data[1] = gtBMS_Dat.CRM_2;	//����ͷ���		
			TxMessage.Data[2] = gtBMS_Dat.CRM_3;	//����λ��00-���⣬01-����
			TxMessage.Data[3] = 0x00;	//gtBMS_Dat.CRM_4   //BMS��ʶ�����00-���ܱ�ʶ��01-�ܱ�ʶ 
			TxMessage.Data[4] = gtBMS_Dat.CRM_5 & 0xFF;//�������������ţ�4�ֽ�ascii
			TxMessage.Data[5] = (gtBMS_Dat.CRM_5 >> 8) & 0xFF;			
			TxMessage.Data[6] = (gtBMS_Dat.CRM_5 >> 16) & 0xFF;
			TxMessage.Data[7] = (gtBMS_Dat.CRM_5 >> 24) & 0xFF;	
			
			break;		
		}		
		case CCS_DEC_CRM_1:   	//������ʶ����CRM�����ֽ׶Σ�BMS�ܱ�ʶ
		{
			TxMessage.ExtId   = 0x1801F4E5;
			TxMessage.DLC 	  = 0x08;
//			TxMessage.Data[0] = (ModbusREGDat[9] >> 0) & 0xFF;
//			TxMessage.Data[1] = (ModbusREGDat[9] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (ModbusREGDat[10] >> 0) & 0xFF;
//			TxMessage.Data[3] = 0x01;
//			TxMessage.Data[4] = (ModbusREGDat[11] >> 0) & 0xFF;
//			TxMessage.Data[5] = (ModbusREGDat[11] >> 8) & 0xFF;			
//			TxMessage.Data[6] = (ModbusREGDat[12] >> 0) & 0xFF;
//			TxMessage.Data[7] = (ModbusREGDat[12] >> 8) & 0xFF;		

			TxMessage.Data[0] = gtBMS_Dat.CRM_1;	//�������
			TxMessage.Data[1] = gtBMS_Dat.CRM_2;	//����ͷ���		
			TxMessage.Data[2] = gtBMS_Dat.CRM_3;	//����λ��00-���⣬01-����
			TxMessage.Data[3] = 0x01;	//gtBMS_Dat.CRM_4   //BMS��ʶ�����00-���ܱ�ʶ��01-�ܱ�ʶ 
			TxMessage.Data[4] = gtBMS_Dat.CRM_5 & 0xFF;//�������������ţ�4�ֽ�ascii
			TxMessage.Data[5] = (gtBMS_Dat.CRM_5 >> 8) & 0xFF;			
			TxMessage.Data[6] = (gtBMS_Dat.CRM_5 >> 16) & 0xFF;
			TxMessage.Data[7] = (gtBMS_Dat.CRM_5 >> 24) & 0xFF;				
			break;		
		}
		case CCS_DEC_CTS:		//�������ʱ��ͬ��CTS�����ý׶Σ�ѹ��BCD�� 
		{						//���Բ����÷��룬�����ô�����ڼ���

			TxMessage.ExtId   = 0x1807F4E5;
			TxMessage.DLC 	  = 0x07;						
//			TxMessage.Data[0] = 0;//(ModbusREGDat[13] >> 0) & 0xFF; //��
//			TxMessage.Data[1] = 0x20;//(ModbusREGDat[13] >> 8) & 0xFF;//��			
//			TxMessage.Data[2] = (ModbusREGDat[14] >> 0) & 0xFF;//ʱ
//			TxMessage.Data[3] = (ModbusREGDat[14] >> 8) & 0xFF;//��
//			TxMessage.Data[4] = (ModbusREGDat[15] >> 0) & 0xFF;//��
//			TxMessage.Data[5] = (ModbusREGDat[15] >> 8) & 0xFF;//�� ��λ			
//			TxMessage.Data[6] = (ModbusREGDat[16] >> 0) & 0xFF;//���λ
			
			TxMessage.Data[0] = (gtBMS_Dat.CTS_1/10)*16 + (gtBMS_Dat.CTS_1%10);//(ModbusREGDat[13] >> 0) & 0xFF; //��
			TxMessage.Data[1] = (gtBMS_Dat.CTS_2/10)*16 + (gtBMS_Dat.CTS_2%10);//(ModbusREGDat[13] >> 8) & 0xFF;//��			
			TxMessage.Data[2] = (gtBMS_Dat.CTS_3/10)*16 + (gtBMS_Dat.CTS_3%10);//ʱ
			TxMessage.Data[3] = (gtBMS_Dat.CTS_4/10)*16 + (gtBMS_Dat.CTS_4%10);//��
			TxMessage.Data[4] = (gtBMS_Dat.CTS_5/10)*16 + (gtBMS_Dat.CTS_5%10);//��
			tmp16 = (gtBMS_Dat.CTS_6%100);
			TxMessage.Data[5] = (tmp16/10)*16 + (tmp16%10);//�� ��λ
			tmp16 = (gtBMS_Dat.CTS_6/100);
			TxMessage.Data[6] = (tmp16/10)*16 + (tmp16%10);//���λ			
			break;						
		}
		case CCS_DEC_CML:		//��������������CML�����ý׶�  
		{
			#define CD40017_MAX_OUTVOL	650				//�����̶�����ͬ�ĳ���Ҫ��ͬ��������������ѹ
			#define CD40017_MIN_OUTVOL	250				//������������ѹ
			#define CD40017_MAX_OUTCUR	250				//��������������
			TxMessage.ExtId   = 0x1808F4E5;
			TxMessage.DLC 	  = 0x06;												
			TxMessage.Data[0] = CD40017_MAX_OUTVOL*10 % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
			TxMessage.Data[1] = CD40017_MAX_OUTVOL*10 / 256;//(ModbusREGDat[22] >> 8) & 0xFF;
			TxMessage.Data[2] = CD40017_MIN_OUTVOL*10 % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
			TxMessage.Data[3] = CD40017_MIN_OUTVOL*10 / 256;//(ModbusREGDat[22] >> 8) & 0xFF;						
			TxMessage.Data[4] = (8000 - CD40017_MAX_OUTCUR*10) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//�������ֵ 0.1A  -800A ~ 500A
			TxMessage.Data[5] = (8000 - CD40017_MAX_OUTCUR*10) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
			TxMessage.Data[6] = 0;
			TxMessage.Data[7] = 0;
			break;						
		}
		case CCS_DEC_CRO:		//�������׼������״̬CRO�����ý׶�  
		{						//0010-����δ��ɳ��׼����0100B-��������ɳ��׼��
			TxMessage.ExtId   = 0x140AF4E5;
			TxMessage.DLC 	  = 0x01;									
			TxMessage.Data[0] = 0x04;
			break;
		}
		case CCS_DEC_CCS:		//�������״̬CCS�����׶�  
		{
			#define CD40017_OUTVOL		3600	//��������Ķ��壬��Ҫ�ǵ���ʹ�ã�
			#define CD40017_OUTCUR		0
			#define CD40017_CHARGETIME	10

			TxMessage.ExtId   = 0x1814F4E5;
			TxMessage.DLC 	  = 0x06;												
//			TxMessage.Data[0] = (ModbusREGDat[22] >> 0) & 0xFF;//��ѹ���ֵ��0.1V
//			TxMessage.Data[1] = (ModbusREGDat[22] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (ModbusREGDat[23] >> 0) & 0xFF;//�������ֵ 0.1A  -800A ~ 500A
//			TxMessage.Data[3] = (ModbusREGDat[23] >> 8) & 0xFF;
//			TxMessage.Data[4] = (ModbusREGDat[24] >> 0) & 0xFF;//�ۼƳ��ʱ�� 1min 0-6000min
//			TxMessage.Data[5] = (ModbusREGDat[24] >> 8) & 0xFF;
			
			TxMessage.Data[0] = (INT16U)(gtBMS_Dat.CCS_1*10)%256;//��ѹ���ֵ��0.1V
			TxMessage.Data[1] = (INT16U)(gtBMS_Dat.CCS_1*10)/256;			
			TxMessage.Data[2] = (INT16U)(gtBMS_Dat.CCS_2*10)%256;//�������ֵ 0.1A  -800A ~ 500A
			TxMessage.Data[3] = (INT16U)(gtBMS_Dat.CCS_2*10)/256;
			TxMessage.Data[4] = gtBMS_Dat.CCS_3%256;			//�ۼƳ��ʱ�� 1min 0-6000min
			TxMessage.Data[5] = gtBMS_Dat.CCS_3/256;			


			break;						
		}
		case CCS_DEC_CST:			//������ֹ���CST�����׶�  
		{
			TxMessage.ExtId   = 0x141AF4E5;
			TxMessage.DLC 	  = 0x03;															
//			TxMessage.Data[0] = 0x03;//(ModbusREGDat[25] >> 0) & 0xFF;//��ֹ���ԭ��
//			TxMessage.Data[1] = 0x00;//(ModbusREGDat[25] >> 8) & 0xFF;//��ֹ������ԭ��			
//			TxMessage.Data[2] = 0x00;//(ModbusREGDat[26] >> 0) & 0xFF;//��ֹ������ԭ��
			
			TxMessage.Data[0] = gtBMS_Dat.CST_1;//��ֹ���ԭ��
			TxMessage.Data[1] = gtBMS_Dat.CST_2;//��ֹ������ԭ��			
			TxMessage.Data[2] = gtBMS_Dat.CST_3;//��ֹ������ԭ��			
			break;						
		}
		case CCS_DEC_CSD:  				//����ͳ�����ݱ���CSD�������׶� 
		{
			TxMessage.ExtId   = 0x181DF4E5;
			TxMessage.DLC 	  = 0x05;																	
//			TxMessage.Data[0] = (ModbusREGDat[27] >> 0) & 0xFF;//�ۼƳ��ʱ�� 1min
//			TxMessage.Data[1] = (ModbusREGDat[27] >> 8) & 0xFF;	
//			TxMessage.Data[2] = (ModbusREGDat[28] >> 0) & 0xFF;//�������	0.1kWh 0~500kWh	
//			TxMessage.Data[3] = (ModbusREGDat[28] >> 8) & 0xFF;
//			TxMessage.Data[4] = (ModbusREGDat[29] >> 0) & 0xFF;//�������
			
			TxMessage.Data[0] = gtBMS_Dat.CSD_1%256;			//�ۼƳ��ʱ�� 1min
			TxMessage.Data[1] = gtBMS_Dat.CSD_1/256;	
			TxMessage.Data[2] = (INT16U)(gtBMS_Dat.CSD_2*10)%256;//�������	0.1kWh 0~500kWh	
			TxMessage.Data[3] = (INT16U)(gtBMS_Dat.CSD_2*10)/256;
			TxMessage.Data[4] = gtBMS_Dat.CSD_3;				//�������			
			
			break;						
		}
		case CCS_DEC_CEM:  				//����������CEM  
		{
			TxMessage.ExtId   = 0x141FF4E5;
			TxMessage.DLC 	  = 0x03;																			
//			TxMessage.Data[0] = (ModbusREGDat[30] >> 0) & 0xFF;
//			TxMessage.Data[1] = (ModbusREGDat[30] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (ModbusREGDat[31] >> 0) & 0xFF;
			break;						
		}
		case CCS_DEC_CFM:  		//�������ϱ���CFM  
		{
			TxMessage.ExtId   = 0x1434F4E5;
			TxMessage.DLC 	  = 0x02;																						
//			TxMessage.Data[0] = (ModbusREGDat[32] >> 0) & 0xFF;
//			TxMessage.Data[1] = (ModbusREGDat[32] >> 8) & 0xFF;			
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
//	INT8U tmp;
// for(tmp=10;tmp<100;tmp++){ModbusREGDat[tmp] = 0;}
	CanPackage.PGN 		= 0x00;			//�������ݽ��հ�����
	CanPackage.DataLen 	= 0x00 ;
	CanPackage.RxFlag 	= 0x00;
	CanPackage.FrameNum = 0x00;
	CanPackage.FramePointer = 0x00;
	gCommStation = CCS_DEC_INVALID;	   	//��ʼ��Ϊ��Ч��״̬
	CanMessageBuff.OutLen = 0x0000;		//��ȡ����������
	CanMessageBuff.InLen  = 0x0000;		//���ջ���������
	if(u8Select == 0x01)				//������������
	{
		CAN_ChargeFlag.NoCharge = 0;	//BMS��Ҫ���
		CAN_ChargeFlag.FullCharge = 0;	//BMSδ����
		CAN_ChargeFlag.Stop = 0;
		gCommStation = CCS_DEC_VBI;	   	//��ʼ��Ϊ��״̬
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


/*********************************************************
��Ҫ���ʱ����ơ����̿��ƣ����ݵķ��ͺͽ��ܣ�������������������ݴ���

***********************************************************/

void task2_core(void *pdata)
{
	INT16U RunTime = 0x0000;			//����ʱ���ʱ����λ10ms,
	INT16U OutTime = 0x0000;			//��ʱʱ���ʱ����λ10ms
	INT16U tmp16;
	INT16U *ptrCTRLRegister;			//�������̵Ŀ��ƼĴ�������ʵ����ModbusREGDat[2]��Ϊ�˷����޸ľ����¶���
	pdata = pdata;
	CAN_Initial(CAN_REC_BMS);  			//���ܽ���BMS���ݣ���Ϊ��ʼ�ͽ�������
	ParemeterInitial(0x01);
//	ptrCTRLRegister = &ModbusREGDat[2];
while(1)
{
	OSTimeDlyHMSM(0,0,0,10);
	gCommStation = CCS_DEC_BCL;			//����ʹ��
	RunTime ++;			  				//����ʱ�䣬ֻ����ѭ������������������
	OutTime ++;			  				//�����ڳ�ʱ��⣬���ܺ�����ʱ������һ��
										//�����������һ�û�н������ӵ�����¿�ʼ����
//	ModbusREGDat[1] = gCommStation; 	//�ϱ�Э��ת����״̬
//	ModbusREGDat[0] = ( ((1-CAN_ChargeFlag.OutTime) << 0)	//����λ��������1-����ʱ��0��ʾ��ʱ
//					   |(CAN_ChargeFlag.NoCharge<< 1)
//					   |(CAN_ChargeFlag.Charging << 2)
//					   |(CAN_ChargeFlag.Stop<< 3));						
	if(GetBitStatus(*ptrCTRLRegister,1))					//����������ָ��
	{ 
	  	ClrBitStatus(ptrCTRLRegister,1);					//����bit1λ���� ��ָ������
		if((gCommStation == CCS_DEC_INVALID)  	   			//���ڲ����ӽ׶λ����ǽ����׶ζ��������³�ʼ��
		   ||(gCommStation == CCS_DEC_OVER)) 				//��֤�������ֶ����պ�������¿�ʼ���
		{													//��ʵ�������ϸ����Ѿ��������¸�����ʼ��ʱ�������			
			ClrBitStatus(ptrCTRLRegister,0);				//�����λ������ͣ����־
			ParemeterInitial(0x01);							//��ʼ����
			OutTime = 0x00;
		}
	}
//****************����Ϊ���ĸ����׶�*********************//
	
	if(gCommStation == CCS_DEC_BCL) 	//������ж� ���ٳ��׶ξ��������б�־
	{  	CAN_ChargeFlag.Charging = 1; }
	else 
	{	CAN_ChargeFlag.Charging = 0; }

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
//==============================================================================
	MCMDeal(gCommStation);	//�����ն˵���ش���
//==============================================================================
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

			if(CAN_TP_CM(1)) OutTime = 0x00;				//�����ݽ��ܴ���
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x30))	//VBI
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_VBI);
			   gCommStation = CCS_DEC_BRM;
			   OutTime = 0x00;
			}
			break ;
		}

		case CCS_DEC_BRM:					   									//�Ѿ����յ�VBI���ȴ�����BRM
		{																		//250ms����һ�γ������ܱ�ʶ����
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S��ʱ����
			if(CAN_TP_CM(1)) OutTime = 0x00;									//�����ݽ��ܴ���
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x02))			//BRM
			{
			   DecodeCCSMessage(CCS_DEC_CRM_1);									//�����յ��������������ݺ��������Ϳ���ʶ������
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BRM);
			   gCommStation = CCS_DEC_BCP;
			   OutTime = 0x00;										 
			}
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_0);			//��Ϊ����һ�����ֳɹ��������ǽ��ճɹ����������ظ���Ϣ 
			break ;													 			//���Խ����з��ڱ��׶ε�ĩβ�����ճɹ��󽫲����Ͳ���ʶ������
		}

		case CCS_DEC_BCP:					  									//�Ѿ����յ�BRM���ȴ����ܵ�س�����
		{																		//250ms����һ�γ����ܱ�ʶ����
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_1);	
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S��ʱ����
			if(CAN_TP_CM(1)) OutTime = 0x00;	   								//�����ݴ�����
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x06))			//BCP
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BCP);
			   gCommStation = CCS_DEC_BRO;
			   OutTime = 0x00;
			}
			break ;
		}
		case CCS_DEC_BRO:					  									//�ȴ�BMS������׼������
		{
			if((RunTime%50) == 0x01) DecodeCCSMessage(CCS_DEC_CTS);				//500msʱ��ͬ������������������
			if((RunTime%25) == 0x02) DecodeCCSMessage(CCS_DEC_CML);				//250ms
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S��ʱ����
			while(CAN_ReadBuffNum())
			{
				Can_DeleteMessageEXT(1,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//������õ�����֡						
				if(CAN_ReadBuffNum())
				{	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//��ȡ�������ĵ�ַ
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1409E5F4 )	 		//ȷ���յ�BRO����
					{	
						if(CanMessageBuff.Buff[tmp16].Data[0] == 0x04){ 		//��������Ϊ��ɳ��׼��������ʱ�˳�						
							DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BRO);
				   			gCommStation = CCS_DEC_BCS;
				   			OutTime = 0x00;
						}
						else 
						{										//���������1-BMS������2-���ô���
							 ParemeterInitial(0x02);		 	//��ʼ��Ϊ����Ҫ����
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
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()) {
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); //��ȡ�������ĵ�ַ			
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1811E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
			   			gCommStation = CCS_DEC_BCL;
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}
		case CCS_DEC_BCL:						  			//���׶α���
		{													//20ms
			if((RunTime%2) == 0x01)	{DecodeCCSMessage(CCS_DEC_CCS);  }
			//1s����һ��CSD
//			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);//5S��ʱ����
			if(GetBitStatus(*ptrCTRLRegister,0)) 			//��λ������������ָ��
			{												//��������λ��Ҫ��ֹͣ
				ClrBitStatus(ptrCTRLRegister,0);			//����0λ���� ��ָ������
				gCommStation = CCS_DEC_BST;
				CAN_ChargeFlag.Stop = 1;					//������λ��Ҫ��ֹͣ��־
				OutTime = 0x00;
			}							
			while(CAN_ReadBuffNum())
			{
				Can_DeleteMessageEXT(5,0x10,0x11,0x19,0x14,0x12,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){											//ִ��ɾ�����������Ѿ�û�п��õ����ݣ�����Ҫ���ж�
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//��ȡ�������ĵ�ַ
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1810E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
			   			OutTime = 0x00;											//��س������
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1811E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
			   			OutTime = 0x00;											//��س����״̬
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1419E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BST);
			   			
						if((CanMessageBuff.Buff[tmp16].Data[0] > 0) 			//������������ó�����־
						   ||(CanMessageBuff.Buff[tmp16].Data[1] > 0)
						   ||(CanMessageBuff.Buff[tmp16].Data[2] > 0))
						{	
							CAN_ChargeFlag.FullCharge = 1;	
							gCommStation = CCS_DEC_BST;							//�������ݵ�״̬���ж��Ƿ�Ҫ������һ��
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
				ParemeterInitial(0x02);						//5S��ʱ����
			   	OutTime = 0x00;						
			}										
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1419E5F4){
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
				ParemeterInitial(0x02);						//5S��ʱ����
			   	OutTime = 0x00;						
			}						
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181CE5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSD);
			   			gCommStation = CCS_DEC_OVER;
						CAN_Initial(CAN_REC_ALL);							//����������е�����
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
					ParemeterInitial(0x02);			//5S��ʱ����˵����ͷ�Ѿ��ε��ˣ�Ϊ�´�������׼��
					CAN_Initial(CAN_REC_ALL);		//
//				}									//�������INVALID״̬
			}		
			break;
		}
		default:break;
	}
}			

}



