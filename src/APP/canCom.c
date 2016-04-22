/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "includes.h"




#define SET_LED_1_ON	//	GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define SET_LED_1_OFF	//	GPIO_ResetBits(GPIOB, GPIO_Pin_15);

CANRECMSGBUFF CanMessageBuff;			//定义CAN接收缓冲区
CANRECMSGBUFF MCM_CanPackage;	//建立另外一个can接收缓冲区

_T_BMS_DAT gtBMS_Dat;

CanTxMsg TxMessage;
CANPACKAGE	CanPackage;


CANSENDPACKAGE	CanSendPackage;

INT8U  TransmitMailbox;
INT8U  gCommStation = 0x00;				//记录各个通讯阶段
INT16U ModbusREGDat[200];

struct 
{
	INT8U OutTime;						//通讯超时,0-不超时，1表示超时
	INT8U NoCharge;						//BMS不需要充电，1-不需要充电
	INT8U FullCharge;					//BMS已充满 ，1-已经充满
	INT8U Charging;						//充电中标志位，1-充电中
	INT8U Stop;							//表示上位机发送的停止，1-是因为上位机要求才停止充电的
}CAN_ChargeFlag;						//充电过程中的标志
/**********************************************************
	读取消息缓冲区数据
	返回：
		0   -无数据
		>0  -缓冲区消息的个数
***********************************************************/
INT8U CAN_ReadBuffNum()
{
 	INT16S tmp16;
	tmp16 = CanMessageBuff.InLen- CanMessageBuff.OutLen;
	if(tmp16 <= 0x00)	return 0;							//缓冲区空,一般不会小于0
	return 	(tmp16 & (CANMESSAGEBUFFLEN-1)); 				//有消息就返回缓冲区消息的个数
	
}

/**********************************************************
	初始化CAN通讯
	 当Type=1时可以接收所有的数据
***********************************************************/
void CAN_Initial(INT8U u8Type)
{

  	CAN_InitTypeDef        	CAN_InitStructure;
  	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	GPIO_InitTypeDef 		GPIO_InitStructure; 			//声明 GPIO 的结构
	NVIC_InitTypeDef 		NVIC_InitStructure;				//NVIC设置

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//打开外设时钟.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 	//打开外设时钟.

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
	//CAN_InitStructure.CAN_ABOM = DISABLE;					//用来使能或者失能自动离线管理 	
	CAN_InitStructure.CAN_ABOM = ENABLE;					//用来使能或者失能自动离线管理
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
						   									//将接收所有BMS发送给充电机的数据
  	CAN_FilterInitStructure.CAN_FilterNumber	= 0;
  	CAN_FilterInitStructure.CAN_FilterMode		= CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale		= CAN_FilterScale_32bit;
//  	if(u8Type == CAN_REC_ALL)	{							//仅接受BMS发给充电机的数据
		CAN_FilterInitStructure.CAN_FilterIdHigh	= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x0000;
//	}
//	if(u8Type == CAN_REC_BMS)	{							//接收所有的数据
//		CAN_FilterInitStructure.CAN_FilterIdHigh	= (0xE5F4 >> 13);
//	  	CAN_FilterInitStructure.CAN_FilterIdLow		= (0xE5F4 & 0x1FFF)<<3;
//	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0xFFFF>>13);;
//	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x1FFF<<3;
//	}
//
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  	CAN_FilterInitStructure.CAN_FilterActivation= ENABLE;
  	CAN_FilterInit(&CAN_FilterInitStructure);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   		//在FLASH中运行
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); 	  			//使能FIFO0消息挂号中断
	//MessagePending = CAN_MessagePending(CANFIFO0);		//返回挂号的信息数量 
}
/************************************************************
*	BMS报文解码
*	将BMS上传的报文赋值给modbus寄存器，共触摸屏读取
*************************************************************/
void DecodeBMSMessage(INT8U *pu8Mesg, INT8U u8ID)
{
	
	INT8U u8AdrCnt = 0;
//	INT8U u8RegAdr = 0;
	switch(u8ID)
   	{
		case BMS_DEC_BRM:  				//BMS和车辆辨识报文BRM										   
		{
//			for(u8RegAdr = M_BRM_START_ADR; u8AdrCnt < M_BRM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BMR_1 = pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;		//通讯协议版本号
			gtBMS_Dat.BMR_4 =  pu8Mesg[3];					//电池类型
			gtBMS_Dat.BMR_5 =  (FP32)(pu8Mesg[4] + pu8Mesg[5]<<8)/10;			//整车电池额定容量
			gtBMS_Dat.BMR_7 =  (FP32)(pu8Mesg[6] + pu8Mesg[7]<<8)/10;			//整车额定总电压
			gtBMS_Dat.BMR_9[0] =  pu8Mesg[8];				//电池厂商名字 4
			gtBMS_Dat.BMR_9[1] =  pu8Mesg[9];		
			gtBMS_Dat.BMR_9[2] =  pu8Mesg[10];		
			gtBMS_Dat.BMR_9[3] =  pu8Mesg[11];
			
			gtBMS_Dat.BMR_13[0] =  pu8Mesg[12];				//电池组序号 4
			gtBMS_Dat.BMR_13[1] =  pu8Mesg[13];		
			gtBMS_Dat.BMR_13[2] =  pu8Mesg[14];		
			gtBMS_Dat.BMR_13[3] =  pu8Mesg[15];	
			
			gtBMS_Dat.BMR_17 =  pu8Mesg[16]+1985;			//电池组生产日期年
			gtBMS_Dat.BMR_18 =  pu8Mesg[17];				//电池组生产日期  月
			gtBMS_Dat.BMR_19 =  pu8Mesg[18];				//日
			gtBMS_Dat.BMR_20 = pu8Mesg[19] + pu8Mesg[20]<<8 + pu8Mesg[21]<<16;//充电次数 
			gtBMS_Dat.BMR_23 =  pu8Mesg[22];				//产权标识
			gtBMS_Dat.BMR_24 =  pu8Mesg[23];				//预留
			for(u8AdrCnt=0; u8AdrCnt<25; u8AdrCnt++){		//车辆识别信息 25byte
				gtBMS_Dat.BMR_25[u8AdrCnt] =  pu8Mesg[24+u8AdrCnt];				
			}		
			
			break;		
		}
		case BMS_DEC_VBI:  									//车辆和电池标识报文VBI	
		{
//			for(u8RegAdr = M_VBI_START_ADR; u8AdrCnt < M_VBI_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.VBI_1 =   pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;	//通讯协议版本号;
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
		case BMS_DEC_BCP:  				//蓄电池充电参数BCP	
		{
//			for(u8RegAdr = M_BCP_START_ADR; u8AdrCnt < M_BCP_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			
			gtBMS_Dat.BCP_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/100;			//蓄电池模块最高允许充电电压			
			gtBMS_Dat.BCP_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10;			//最高允许充电电流 
			gtBMS_Dat.BCP_5 = pu8Mesg[4]*10;									//最大允许充电容量 
			gtBMS_Dat.BCP_6 = (FP32)(pu8Mesg[5] + pu8Mesg[6]<<8)/10;			//最高允许充电总电压 
			gtBMS_Dat.BCP_8 = pu8Mesg[7]-50;									//最高允许温度 
			gtBMS_Dat.BCP_9 = (pu8Mesg[8] + pu8Mesg[9]<<8);						//整车蓄电池估计剩余容量 
			gtBMS_Dat.BCP_11 = (FP32)(pu8Mesg[10] + pu8Mesg[11]<<8)/10;			//整车蓄电池总电压 
			gtBMS_Dat.BCP_13 = pu8Mesg[12];										//整车 SOH 
			
			break;		
		}
		case BMS_DEC_BRO:  				//电池充电准备就绪状态BRO	
		{
//			for(u8RegAdr = M_BRO_START_ADR; u8AdrCnt < M_BRO_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BRO_1 = pu8Mesg[0];//
			
			break;            
		}
		case BMS_DEC_BCL:  				//电池充电级别需求BCL，充电阶段
		{
//			for(u8RegAdr = M_BCL_START_ADR; u8AdrCnt < M_BCL_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BCL_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//电压需求
			gtBMS_Dat.BCL_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10-800;		//电流需求
			gtBMS_Dat.BCL_5 = pu8Mesg[4]; 										//充电模式
			
			break; 
		}
		case BMS_DEC_BCS:  				//电池充电状态BCS，充电阶段
		{
//			for(u8RegAdr = M_BCS_START_ADR; u8AdrCnt < M_BCS_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BCS_1 = (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//充电电压测量值（V） 
			gtBMS_Dat.BCS_3 = (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10 - 800;		//充电电流测量值（A）
			gtBMS_Dat.BCS_5 = pu8Mesg[4]-50;	//电池组最低温度（oC）  
			gtBMS_Dat.BCS_6 = pu8Mesg[5]-50;	//电池组最高温度（oC）  
			gtBMS_Dat.BCS_7 = pu8Mesg[6];		//当前荷电状态  SOC（%）  
			gtBMS_Dat.BCS_8 = pu8Mesg[7];		//估算剩余时间（min）
			
			break;         
		}
		case BMS_DEC_BSM:  				//蓄电池状态信息BSM，充电阶段
		{
//			for(u8RegAdr = M_BSM_START_ADR; u8AdrCnt < M_BSM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.BSM_1 = (pu8Mesg[0] + pu8Mesg[1]<<8);
			gtBMS_Dat.BSM_3 = pu8Mesg[2];
			gtBMS_Dat.BSM_4 = pu8Mesg[3];
			gtBMS_Dat.BSM_5 = pu8Mesg[4];
			gtBMS_Dat.BSM_6 = pu8Mesg[5];					//和下面的同时解码，方便其他程序调用
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
		case BMS_DEC_BMV:  				//蓄电池最小测量单元电压BMV，充电阶段
		{
		 	
			break;       
		}
		case BMS_DEC_BMT:  				//蓄电池组温度BMT，充电阶段
		{
		    
			break;       
		}
		case BMS_DEC_BSOC:  			//蓄电池组荷电容量SOC ,BSOC，充电阶段
		{
		  	break;
		}
		case BMS_DEC_BST:  				//BMS中止充电报文BST，充电阶段
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
		case BMS_DEC_BSD:  				//BMS充电统计报文BSD，结束阶段
		{
//			for(u8RegAdr = M_BSD_START_ADR; u8AdrCnt < M_BSD_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BSD_1 = pu8Mesg[0];										//中止荷电状态  SOC（%）  
			gtBMS_Dat.BSD_2 = (FP32)(pu8Mesg[1] + pu8Mesg[2]<<8)/1000;			//蓄电池单体最低电压（V
			gtBMS_Dat.BSD_4 = (FP32)(pu8Mesg[3] + pu8Mesg[4]<<8)/1000;			//蓄电池单体最高电压（V
			gtBMS_Dat.BSD_6 = pu8Mesg[5]-50;									//蓄电池组最低温度（oC） 
			gtBMS_Dat.BSD_7 = pu8Mesg[6]-50;									//蓄电池组最高温度（oC）
			break;	            
		}
		case BMS_DEC_BEM:  				//BMS错误报文BEM，错误阶段
		{
//			for(u8RegAdr = M_BEM_START_ADR; u8AdrCnt < M_BEM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			break;
		}
		case BMS_DEC_BFM:  				//BMS故障报文BFM，故障阶段
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
	INT8U 	FlagValid;					//0,该阶段无效，1-该阶段有效或者未被删除
	INT8U 	WorkType;					//工作方式，0-停止，1-恒流限压，2-恒压限流，3-恒流定时，4-恒压定时
	FP32 	SetValue;					//输出给定值
	FP32 	LimitValue;					//输出限制值
	FP32 	StopValue;					//输出停止值
}_T_MCT_STAGE;

typedef struct {
	INT8U 	FlagValid;					//数据有效标志位，=1，有效
	FP32	MaxCellVoltage;				//单体最高允许充电电压电压
	INT16S	MaxTemperature;				//最高允许充电温度
}_T_MCT_SINGLE;

typedef struct {
	INT8U 	Switch;						//开关机信号，0-停止，1-启动	
	INT8U 	ChargeNum;					//充电阶段数；
	INT8U 	ChargerID;					//充电机编号
	INT8U 	NowStage;					//当前工作的充电阶段
	FP32 	Power;						//输出功率	
	INT8U	BMSStage;					//和bms通讯处于哪个阶段
	_T_MCT_SINGLE 	MCT_Single;			//单体控制参数	
	_T_MCT_STAGE  	MCT_Stage[6];		//各个阶段的值
	
}_T_MCT_DAT;

_T_MCT_DAT gtMCT_Dat;//定义一个结构体变量
_T_METER_DAT gtMeter_Dat;
//gtMCT_Dat.NowStage = 1;//该变量一定要初始化

//充电终端 MCT
//充电机   CCM
//返回0表示没有发送数据帧，或者该ID无效，一般是前者
INT8U DecodeCCMMessage(INT32U ID)
{
	CanTxMsg ltTxMsg;
	INT16U tmp;

	switch(ID)
	{
		case MCM_RETURNSET_STAGE_ALL_SINGLE	:			//从站返回全部阶段单体最大充电控制参数,折两个命令如果没有设置就，返回
		case MCM_RETURNSET_SINGLE:						//从站返回单体最大充电控制参数
		{
			  if(gtMCT_Dat.MCT_Single.FlagValid == 0) return 0;
		}
		case MCM_STATE2	:								//充电桩/机状态帧2		
		{					
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;		//充电阶段数
			ltTxMsg.Data[1] = gtMCT_Dat.NowStage;	
			if(ID == MCM_RETURNSET_STAGE_ALL_SINGLE) {
				ltTxMsg.Data[1] = 0x00;			//如果读所有数据
			}
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Single.MaxCellVoltage*1000)%256;	//单体最高电压
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Single.MaxCellVoltage*1000)/256;
			ltTxMsg.Data[4] = (INT8U)(gtMCT_Dat.MCT_Single.MaxTemperature + 50);
			ltTxMsg.Data[5] = 0x00;		
			ltTxMsg.Data[6] = 0x00;
			ltTxMsg.Data[7] = 0x00;			
			break;
		}
		case MCM_RETURNSET_STAGE_ALL_1	:				//从站返回所有阶段充电控制参数
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
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;								//充电阶段数
			ltTxMsg.Data[1] = 0xA0 + gtMCT_Dat.MCT_Stage[tmp].WorkType;			//工作方式，0-停止，1-恒流限压，2-恒压限流，3-恒流定时，4-恒压定时
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) %256;					//输出给定值,现在还不确定带不带小数点
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) %256;				//输出限制值
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) %256	;				//输出停止值	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) /256	;
			break;
		}
		case MCM_RETURNSET_SPECIAL_STAGE_1	:			//从站返回指定阶段充电控制参数（共6个阶段）
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
			ltTxMsg.Data[0] = gtMCT_Dat.ChargeNum;								//充电阶段数
			ltTxMsg.Data[1] = 0xA0 + gtMCT_Dat.MCT_Stage[tmp].WorkType;			//工作方式，0-停止，1-恒流限压，2-恒压限流，3-恒流定时，4-恒压定时
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) %256;					//输出给定值,现在还不确定带不带小数点
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) %256;				//输出限制值
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) %256	;				//输出停止值	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp].StopValue *10) /256	;
			break;
		}		
		case MCM_STATE0	:						//充电桩/机状态帧0
		{	
			static INT8U LifeTime ;
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMCT_Dat.NowStage&0x0F ;
			if((tmp >6) ||(tmp <1)) tmp = 1;
			ltTxMsg.Data[0] = tmp +(gtMCT_Dat.MCT_Stage[tmp-1].WorkType&0x0F)<<4; //当前充电方式和当前工作阶段
			ltTxMsg.Data[1] = gtMCT_Dat.BMSStage;		//当前状态，非充电阶段，握手阶段，配置阶段，充电阶段，结束阶段，错误处理阶段
			ltTxMsg.Data[2] = 0xFF;		//保留
			ltTxMsg.Data[3] = 0xFF;		//保留
			ltTxMsg.Data[4] = 0xFF;		//保留			
			ltTxMsg.Data[5] = 0xFF;		//保留	
			ltTxMsg.Data[6] = 0xFF;		//保留					
			ltTxMsg.Data[7] = LifeTime;	//生命值
			if(LifeTime >= 15)	{
				LifeTime = 0;			//生命值是1-15，而不是0-15，所以下面要 ++
			}
			LifeTime ++;		
			break;
		}
		case MCM_STATE1	:						//充电桩/机状态帧1
		{
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMeter_Dat.ActivePower ;
			ltTxMsg.Data[0] = tmp % 256;						//交流电压总功率
			ltTxMsg.Data[1] = tmp /256;
			
			tmp = gtMeter_Dat.AH ;			
			ltTxMsg.Data[2] = tmp % 256;						
			ltTxMsg.Data[3] = tmp /256;

			tmp = gtMeter_Dat.kWh ;			
			ltTxMsg.Data[4] = tmp % 256;						
			ltTxMsg.Data[5] = tmp /256;

			tmp = gtMeter_Dat.ReactivePower ;			
			ltTxMsg.Data[6] = tmp % 256;						//输出无功功率
			ltTxMsg.Data[7] = tmp /256;			
			
			break;
		}													//状态帧2在开头部分
		case MCM_STATE3	:									//充电桩/机状态帧3
		{			
			tmp = gtMCT_Dat.NowStage&0x0F ;
			if((tmp >6) ||(tmp <1)) tmp = 1;				//防止数据溢出
			ltTxMsg.DLC 	= 0x08;
			ltTxMsg.Data[0] = gtMCT_Dat.NowStage&0x0F;		//充电阶段数
			ltTxMsg.Data[1] = gtMCT_Dat.MCT_Stage[tmp-1].WorkType;									//工作方式，0-停止，1-恒流限压，2-恒压限流，3-恒流定时，4-恒压定时
			ltTxMsg.Data[2] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].SetValue *10) %256;				//输出给定值,现在还不确定带不带小数点
			ltTxMsg.Data[3] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].SetValue *10) /256;
			ltTxMsg.Data[4] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].LimitValue *10) %256;				//输出限制值
			ltTxMsg.Data[5] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].LimitValue *10) /256;		
			ltTxMsg.Data[6] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].StopValue *10) %256;				//输出停止值	
			ltTxMsg.Data[7] = (INT16U)(gtMCT_Dat.MCT_Stage[tmp-1].StopValue *10) /256 ;
			break;
		}

		case MCM_STATE4	:										//充电桩/机状态帧4
		{
			ltTxMsg.DLC 	= 0x08;
			tmp = gtMeter_Dat.ACAllPower;	 
			ltTxMsg.Data[0] = tmp % 256;						//交流电压总功率
			ltTxMsg.Data[1] = tmp /256;
			ltTxMsg.Data[2] = gtMeter_Dat.Wave_A_Vol;			//电压谐波ABC
			ltTxMsg.Data[3] = gtMeter_Dat.Wave_B_Vol;	
			ltTxMsg.Data[4] = gtMeter_Dat.Wave_C_Vol;	
			ltTxMsg.Data[5] = gtMeter_Dat.Wave_A_Cur;			//电流谐波ABC
			ltTxMsg.Data[6] = gtMeter_Dat.Wave_B_Cur;
			ltTxMsg.Data[7] = gtMeter_Dat.Wave_C_Cur;			
			break;
		}
		default:break;
	}
	ltTxMsg.ExtId  = ID;	
	ltTxMsg.IDE = CAN_ID_EXT;
	ltTxMsg.RTR = CAN_RTR_DATA;
	tmp = CAN_Transmit(CAN1,&ltTxMsg);	//发送消息
	if(CAN_TransmitStatus(CAN1,tmp) == CAN_NO_MB)	  //如果没有空邮箱就等待一下再发送
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
		case  MCM_STOP_ALL	:								//主站令所有充电桩/机停机
		{
			if(ptrMsg.Data[0] == 0x55) {
				gtMCT_Dat.Switch = 0x00;					//立即停止充电
			}
			break;
		}
		case MCM_START_SINGLE:								//主站令某台充电桩/机充电
		{
			if(ptrMsg.Data[0] == 0x55) {
				if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID) 
				{	gtMCT_Dat.Switch = 0x01;	}			//如果充电机编号是自己的编号就立即启动
			}
			break;
		}
		case MCM_STOP_SINGLE:								//主站令某台充电桩/机停止充电
		{
			if(ptrMsg.Data[0] == 0x55) {
				if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID) 
				{	gtMCT_Dat.Switch = 0x00;	}			//如果充电机编号是自己的编号就立即停止
			}				
			break;
		}

		case MCM_SET_SINGLE	:								//主站设置单体最大充电控制参数
		{
			if((ptrMsg.Data[0]>>5) == 0x000) //
			{
				;
			}
			else if(((ptrMsg.Data[0]>>5) == 0x01)||(ptrMsg.Data[0]>>5) == 0x02)//添加控制参数或者更改控制参数
			{
				gtMCT_Dat.MCT_Single.FlagValid = 1;
				gtMCT_Dat.ChargeNum =ptrMsg.Data[1];							//充电阶段数
				gtMCT_Dat.MCT_Single.MaxCellVoltage = (FP32)(ptrMsg.Data[2] +ptrMsg.Data[3]*256)/1000;//单体最高电压
				gtMCT_Dat.MCT_Single.MaxTemperature =ptrMsg.Data[4] -50;
			}
			else if((ptrMsg.Data[0]>>5) == 0x03)								//删除控制参数
			{
				gtMCT_Dat.MCT_Single.FlagValid = 0;								//单体控制参数无效
				//其他参数也可以清除
			}
			else {;}
			break;
		}
		case MCM_SET_STAGE_1:								//主站设置阶段充电控制参数
		case MCM_SET_STAGE_2:
		case MCM_SET_STAGE_3:
		case MCM_SET_STAGE_4:
		case MCM_SET_STAGE_5:
		case MCM_SET_STAGE_6:								//可以通过ID判断，也可以通过内部数据判断
		{
			if((ptrMsg.Data[0]>>5) == 0x000) 
			{
				;
			}
			else if(((ptrMsg.Data[0]>>5) == 0x01)||((ptrMsg.Data[0]>>5) == 0x02))//添加控制参数或者更改控制参数
			{
				tmp = (ptrMsg.ExtID>>16)&0xFF - 0x44;			//第几个阶段
				if(tmp > 6-1) 	break;							//数据0开始，阶段是从1开始
				gtMCT_Dat.ChargeNum =(ptrMsg.Data[1]>>4)&0x0F;	//充电阶段数
				gtMCT_Dat.MCT_Stage[tmp].FlagValid = 1;
				gtMCT_Dat.MCT_Stage[tmp].WorkType  = ptrMsg.Data[1]&0x0F;//工作方式，0-停止，1-恒流限压，2-恒压限流，3-恒流定时，4-恒压定时
				gtMCT_Dat.MCT_Stage[tmp].SetValue  = (FP32)(ptrMsg.Data[2] +ptrMsg.Data[3]*256)/10;//输出给定值,现在还不确定带不带小数点
				gtMCT_Dat.MCT_Stage[tmp].LimitValue= (FP32)(ptrMsg.Data[4] +ptrMsg.Data[5]*256)/10;//输出限制值
				gtMCT_Dat.MCT_Stage[tmp].StopValue = (FP32)(ptrMsg.Data[6] +ptrMsg.Data[7]*256)/10;//输出停止值					
			}
			else if((ptrMsg.Data[0]>>5) == 0x03)			//删除控制参数
			{
				tmp = (ptrMsg.ExtID>>16)&0xFF - 0x44;		//第几个阶段
				if(tmp > 6-1) 	break;							//数据0开始，阶段是从1开始
				gtMCT_Dat.MCT_Stage[tmp].FlagValid = 0;		//阶段控制参数无效
				gtMCT_Dat.MCT_Stage[tmp].WorkType  = 0x00;
				gtMCT_Dat.MCT_Stage[tmp].SetValue  = 0.0;
				gtMCT_Dat.MCT_Stage[tmp].LimitValue= 0.0; 
				gtMCT_Dat.MCT_Stage[tmp].StopValue = 0.0; 
			}
			else {;}
			break;
		}

		case MCM_READSET_ALL:								//主站读取充电桩/机所有充电控制参数
		{													//当发送数据时，如何保证bms的发送间隔不受影响？采用直接等待还是装在到发送缓冲区里面？
			if(ptrMsg.Data[0] == 0x55) {					//发送全部参数到充电终端				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_SINGLE))  {			//从站返回全部阶段单体最大充电控制参数
					OSTimeDlyHMSM(0,0,0,6);
				}
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_1))  {				//从站返回所有阶段充电控制参数
					OSTimeDlyHMSM(0,0,0,6);
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_2))  {				//从站返回所有阶段充电控制参数
					OSTimeDlyHMSM(0,0,0,6);				
				}
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_3))  {				//从站返回所有阶段充电控制参数
					OSTimeDlyHMSM(0,0,0,6);				
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_4))  {				//从站返回所有阶段充电控制参数
					OSTimeDlyHMSM(0,0,0,6);				
				}				
				if(DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_5))  {				//从站返回所有阶段充电控制参数
					OSTimeDlyHMSM(0,0,0,6);		
				}				
				DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_6) ; 					//从站返回所有阶段充电控制参数	,最后一帧不需要间隔			
			}				
			break;
		}
		case MCM_READSET_SPECIAL_PARA:						//主站读取充电桩/机特定充电控制参数
		{
			if(ptrMsg.Data[0] == 0x55) {
				tmp = ptrMsg.Data[1];						//获取要读取的阶段
				if(tmp > 6) break;							//不存在该工作阶段
				if(tmp == 0x00) tmp = gtMCT_Dat.NowStage;	//=0,要读取当前工作阶段
				if(tmp == 0x00) tmp = 1;//NowStage不正确
				if(DecodeCCMMessage(MCM_RETURNSET_SINGLE))  {//从站返回单体最大充电控制参数
					OSTimeDlyHMSM(0,0,0,6);		
				}
				DecodeCCMMessage(MCM_RETURNSET_SPECIAL_STAGE_1 + (INT32U)((tmp-1)<<16));//从站返回指定阶段充电控制参数（共6个阶段）
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

		case MCM_SET_POWER	:								//主站设置充电桩/机输出功率
		{
			gtMCT_Dat.Power = ptrMsg.Data[0] +ptrMsg.Data[1]*256;
			gtMCT_Dat.ChargerID = ptrMsg.Data[3];
			break;
		}		
		default:break;
	}
}
/*******************************************************************************
*	充电状态帧0 每1S一次
*	充电状态帧1 每10S一次
*	充电状态帧1 每10S一次
********************************************************************************/
#define INTERVAL_1000MS	100	//间隔1s
#define INTERVAL_10S	1000//间隔10s
#define INTERVAL_60S	6000

void MCMDeal(INT8U stage)
{
	static INT16U times[3] = {0,1,2};	//尽量不要全部是0，防止can发送邮箱满
	INT8U tmp;
	times[0]++;							//时间增加
	times[1]++;
	times[2]++;
	if(times[0] >= INTERVAL_1000MS) {	//每1s发送一次，和其他没有关系
		DecodeCCMMessage(MCM_STATE0);
		times[0] = 0x00;
	}
	
	if(times[1] == INTERVAL_10S) {		//每10s发送一次，必须在充电阶段
		if(gtMCT_Dat.BMSStage == 0x03) 
		{
			DecodeCCMMessage(MCM_STATE1);
		}
	}	
	if(times[1] >= INTERVAL_10S+1) {	//每10s发送一次，必须在充电阶段
		if(gtMCT_Dat.BMSStage == 0x03) 
		{
			DecodeCCMMessage(MCM_STATE4);
		}
		times[1] = 0x00;
	}
	
	if( (stage == CCS_DEC_INVALID)) {	//充电阶段，0-非充电阶段，1-握手，2-配置，3-充电，4-结束，5-错误处理 ||(stage == )
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
	
	if(gtMCT_Dat.BMSStage != tmp) {		//肯定工作阶段发生了变化
		gtMCT_Dat.BMSStage = tmp;		//保存当前工作阶段
		times[2] = INTERVAL_60S;		//这样数据就会立即开始发送
	}									
	if(times[2] == INTERVAL_60S) {		//每60s发送一次，公共阶段发生变化时发送
		DecodeCCMMessage(MCM_STATE2);
	}	
	if(times[2] >= INTERVAL_60S+1) {	//每60s发送一次，
		DecodeCCMMessage(MCM_STATE3);
		times[2] = 0x00;
	}
	
}


/*********************************************************
清除不需要的ID号的数据	
将接收到的数据没有用的全部清零
***********************************************************/
INT8U Can_DeleteMessageEXT(INT8U IDNum,INT8U ID1,INT8U ID2,INT8U ID3,
	                       INT8U ID4,INT8U ID5,INT8U ID6,INT8U ID7,INT8U ID8)
{
	INT8U ID;
	INT8U Addr;
	if(!CAN_ReadBuffNum()) return 0;						//无数据就返回	
	Addr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	//得到数据在缓冲区的位置
	
	if((CanMessageBuff.Buff[Addr].ExtID &0xFFFF) == 0x5600) {//是充电终端发送的数据
		DecodeMCTMessage(CanMessageBuff.Buff[Addr]);
//		DecodeCCMMessage(MCM_RETURNSET_STAGE_ALL_SINGLE);
	 	CanMessageBuff.OutLen++;							//如果都不相等就删除数据
	}
	if(!CAN_ReadBuffNum()) return 0;						//无数据就返回	
	
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
	CanMessageBuff.OutLen++;								//如果都不相等就删除数据
	return 0;
}

/************************************************************
*	传输协议
*	连接模式下的请求发送
*	连接模式下的准备发送
*	连接模式下的消息接收和超时判断及错误处理
*	消息结束应答
*	无用的数据帧全部清除掉
*		
*************************************************************/
INT8S CAN_TP_CM(INT8U ID)
{
	static INT16U Runtime;				//用于多协议的两个数据间的计时，单位10ms，1000ms超时
	INT16U tmp16;						//临时变量
	INT8U lBuffAddr;
	Runtime	++;							//计时增加
	//Runtime = 0x00;//debug
	if(Runtime > 100)					//超时处理1S
	{									//如果超时则将canpackage初始化
		tmp16 = CanPackage.PGN;			//如果不保存放弃连接就不能正常工作
		CanPackage.PGN = 0x00;			//参数群编号
		CanPackage.DataLen = 0x00 ;
		CanPackage.FrameNum = 0x00;
		CanPackage.FramePointer = 0x00;	//位置指针
		Runtime = 0x00;
		if(CanPackage.RxFlag == 0x01)						//如果正在通讯
		{
			CanPackage.RxFlag = 0x00;						//将标志位清零
			TxMessage.ExtId   = 0x1CECF4E5;	  				//放弃连接
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
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
			tmp16 = 0;										//等待发送完毕
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}
	   	}
	}
	while(CAN_ReadBuffNum())  								//直到找到有效值
	{
		if(Can_DeleteMessageEXT(2,0xEC,0xEB,0x00,0x00,0x00,0x00,0x00,0x00) > 0)//清除无用的数据帧
		{	  break ;	 }								
	}
	if(!CAN_ReadBuffNum())	return 0;								

 	//连接传输协议											//得到数据在缓冲区的位置
	lBuffAddr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	 
	//tmp16 = (CanMessageBuff.Buff[tmp16].ExtID>>16)&0xFF;
	if(CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CECE5F4)	//多数据传输
	{
	 	if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0x10)	//请求建立连接
		{													//参数群编号
			CanPackage.PGN = CanMessageBuff.Buff[lBuffAddr].Data[6];
			CanPackage.DataLen = CanMessageBuff.Buff[lBuffAddr].Data[1]	+ CanMessageBuff.Buff[lBuffAddr].Data[2] *256 ;
			CanPackage.FrameNum = CanMessageBuff.Buff[lBuffAddr].Data[3];//0x0F
			CanPackage.RxFlag = 0x01;						//已经建立连接
			CanPackage.FramePointer = 0x01;	   				//位置指针从1开始
			Runtime = 0x00;

			TxMessage.ExtId   = 0x1CECF4E5;
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.DLC 	  = 0x08;						
			TxMessage.Data[0] = 0x11;						//允许建立连接
			TxMessage.Data[1] = CanPackage.FrameNum;		//0x0F		
			TxMessage.Data[2] = CanPackage.FramePointer ;
			TxMessage.Data[3] = 0xFF;
			TxMessage.Data[4] = 0xFF;
			TxMessage.Data[5] = 0x00;		
			TxMessage.Data[6] = CanPackage.PGN;
			TxMessage.Data[7] = 0x00;
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
			tmp16 = 0;										//等待发送完毕
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}	
		}

		if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0xFF)	//放弃连接
		{
			CanPackage.PGN = 0x00;							//参数群编号
			CanPackage.DataLen = 0x00 ;
			CanPackage.FrameNum = 0x00;	
			CanPackage.FramePointer = 0x00;	   				//位置指针
			Runtime = 0x00;//	
		}
		CanMessageBuff.OutLen ++;
	}
	while(CAN_ReadBuffNum())
	{				   										//多字节传输重组
		if(Can_DeleteMessageEXT(2,0xEC,0xEB,0x00,0x00,0x00,0x00,0x00,0x00) == 0x01)
		{	  break ;	 }									//得到数据在缓冲区的位置	
		lBuffAddr = (CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1));	
		if((CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CEBE5F4) &&(CanPackage.RxFlag == 0x01))//多数据传输
		{
			if(CanMessageBuff.Buff[lBuffAddr].Data[0] == CanPackage.FramePointer)
			{
				tmp16 =	(CanPackage.FramePointer-1) *7 ;
				if(tmp16 > 190 )	tmp16= 190;				//防止数据溢出
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
			else											//数据接收错误重新传输处理
			{
				TxMessage.ExtId   = 0x1CECF4E5;
				TxMessage.IDE = CAN_ID_EXT;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.DLC 	  = 0x08;						
				TxMessage.Data[0] = 0x11;					//允许建立连接
				TxMessage.Data[1] = CanPackage.FrameNum - CanPackage.FramePointer +1;//请求传输剩余的数据帧		
				TxMessage.Data[2] = CanPackage.FramePointer ;
				TxMessage.Data[3] = 0xFF;
				TxMessage.Data[4] = 0xFF;
				TxMessage.Data[5] = 0x00;		
				TxMessage.Data[6] = CanPackage.PGN;
				TxMessage.Data[7] = 0x00;
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
				tmp16 = 0;										//等待发送完毕
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
			}
			if(CanPackage.FramePointer == CanPackage.FrameNum + 1)	//表明接收完毕，因为CanPackage.FramePointer总是指向下一个接收位置，所以要+1
			{																					
				TxMessage.ExtId   = 0x1CECF4E5;
				TxMessage.IDE = CAN_ID_EXT;
				TxMessage.RTR = CAN_RTR_DATA;
				TxMessage.DLC 	  = 0x08;						
				TxMessage.Data[0] = 0x13;						//结束连接
				TxMessage.Data[1] = CanPackage.DataLen % 256;		
				TxMessage.Data[2] = CanPackage.DataLen / 256;
				TxMessage.Data[3] = CanPackage.FrameNum;
				TxMessage.Data[4] = 0xFF;
				TxMessage.Data[5] = 0x00;		
				TxMessage.Data[6] = CanPackage.PGN;
				TxMessage.Data[7] = 0x00;
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
				tmp16 = 0;										//等待发送完毕
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
				Runtime = 0x00;//
				CanPackage.RxFlag = 0x02;//帧接收完成
			}
	  	}
		CanMessageBuff.OutLen ++;	//可先增加再读取数据，因为通讯速度比较慢，应该不会覆盖					
	}
	return 1;
}


void DecodeCCSMessage(INT8U u8CCSID)
{
	INT16U tmp16 = 0x00;
	switch(u8CCSID)
	{
		case CCS_DEC_CRM_0:   	//充电机辨识报文CRM，握手阶段，BMS不能辨识
		{

		
			TxMessage.ExtId   = 0x1801F4E5;
			TxMessage.DLC 	  = 0x08;
//			TxMessage.Data[0] = (ModbusREGDat[9] >> 0) & 0xFF;	//充电机编号
//			TxMessage.Data[1] = (ModbusREGDat[9] >> 8) & 0xFF;	//充电插头编号		
//			TxMessage.Data[2] = (ModbusREGDat[10] >> 0) & 0xFF;	//充电机位置00-户外，01-户内
//			TxMessage.Data[3] = 0x00;	//BMS辨识结果，00-不能辨识，01-能辨识
//			TxMessage.Data[4] = (ModbusREGDat[11] >> 0) & 0xFF;//充电机所在区域编号，4字节ascii
//			TxMessage.Data[5] = (ModbusREGDat[11] >> 8) & 0xFF;			
//			TxMessage.Data[6] = (ModbusREGDat[12] >> 0) & 0xFF;
//			TxMessage.Data[7] = (ModbusREGDat[12] >> 8) & 0xFF;	

			TxMessage.Data[0] = gtBMS_Dat.CRM_1;	//充电机编号
			TxMessage.Data[1] = gtBMS_Dat.CRM_2;	//充电插头编号		
			TxMessage.Data[2] = gtBMS_Dat.CRM_3;	//充电机位置00-户外，01-户内
			TxMessage.Data[3] = 0x00;	//gtBMS_Dat.CRM_4   //BMS辨识结果，00-不能辨识，01-能辨识 
			TxMessage.Data[4] = gtBMS_Dat.CRM_5 & 0xFF;//充电机所在区域编号，4字节ascii
			TxMessage.Data[5] = (gtBMS_Dat.CRM_5 >> 8) & 0xFF;			
			TxMessage.Data[6] = (gtBMS_Dat.CRM_5 >> 16) & 0xFF;
			TxMessage.Data[7] = (gtBMS_Dat.CRM_5 >> 24) & 0xFF;	
			
			break;		
		}		
		case CCS_DEC_CRM_1:   	//充电机辨识报文CRM，握手阶段，BMS能辨识
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

			TxMessage.Data[0] = gtBMS_Dat.CRM_1;	//充电机编号
			TxMessage.Data[1] = gtBMS_Dat.CRM_2;	//充电插头编号		
			TxMessage.Data[2] = gtBMS_Dat.CRM_3;	//充电机位置00-户外，01-户内
			TxMessage.Data[3] = 0x01;	//gtBMS_Dat.CRM_4   //BMS辨识结果，00-不能辨识，01-能辨识 
			TxMessage.Data[4] = gtBMS_Dat.CRM_5 & 0xFF;//充电机所在区域编号，4字节ascii
			TxMessage.Data[5] = (gtBMS_Dat.CRM_5 >> 8) & 0xFF;			
			TxMessage.Data[6] = (gtBMS_Dat.CRM_5 >> 16) & 0xFF;
			TxMessage.Data[7] = (gtBMS_Dat.CRM_5 >> 24) & 0xFF;				
			break;		
		}
		case CCS_DEC_CTS:		//充电机输出时间同步CTS，配置阶段，压缩BCD码 
		{						//可以不配置分秒，仅配置大的日期即可

			TxMessage.ExtId   = 0x1807F4E5;
			TxMessage.DLC 	  = 0x07;						
//			TxMessage.Data[0] = 0;//(ModbusREGDat[13] >> 0) & 0xFF; //秒
//			TxMessage.Data[1] = 0x20;//(ModbusREGDat[13] >> 8) & 0xFF;//分			
//			TxMessage.Data[2] = (ModbusREGDat[14] >> 0) & 0xFF;//时
//			TxMessage.Data[3] = (ModbusREGDat[14] >> 8) & 0xFF;//日
//			TxMessage.Data[4] = (ModbusREGDat[15] >> 0) & 0xFF;//月
//			TxMessage.Data[5] = (ModbusREGDat[15] >> 8) & 0xFF;//年 低位			
//			TxMessage.Data[6] = (ModbusREGDat[16] >> 0) & 0xFF;//年高位
			
			TxMessage.Data[0] = (gtBMS_Dat.CTS_1/10)*16 + (gtBMS_Dat.CTS_1%10);//(ModbusREGDat[13] >> 0) & 0xFF; //秒
			TxMessage.Data[1] = (gtBMS_Dat.CTS_2/10)*16 + (gtBMS_Dat.CTS_2%10);//(ModbusREGDat[13] >> 8) & 0xFF;//分			
			TxMessage.Data[2] = (gtBMS_Dat.CTS_3/10)*16 + (gtBMS_Dat.CTS_3%10);//时
			TxMessage.Data[3] = (gtBMS_Dat.CTS_4/10)*16 + (gtBMS_Dat.CTS_4%10);//日
			TxMessage.Data[4] = (gtBMS_Dat.CTS_5/10)*16 + (gtBMS_Dat.CTS_5%10);//月
			tmp16 = (gtBMS_Dat.CTS_6%100);
			TxMessage.Data[5] = (tmp16/10)*16 + (tmp16%10);//年 低位
			tmp16 = (gtBMS_Dat.CTS_6/100);
			TxMessage.Data[6] = (tmp16/10)*16 + (tmp16%10);//年高位			
			break;						
		}
		case CCS_DEC_CML:		//充电机最大输出级别CML，配置阶段  
		{
			#define CD40017_MAX_OUTVOL	650				//参数固定，不同的充电机要不同，充电机最大输出电压
			#define CD40017_MIN_OUTVOL	250				//充电机最低输出电压
			#define CD40017_MAX_OUTCUR	250				//充电机最大输出电流
			TxMessage.ExtId   = 0x1808F4E5;
			TxMessage.DLC 	  = 0x06;												
			TxMessage.Data[0] = CD40017_MAX_OUTVOL*10 % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
			TxMessage.Data[1] = CD40017_MAX_OUTVOL*10 / 256;//(ModbusREGDat[22] >> 8) & 0xFF;
			TxMessage.Data[2] = CD40017_MIN_OUTVOL*10 % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
			TxMessage.Data[3] = CD40017_MIN_OUTVOL*10 / 256;//(ModbusREGDat[22] >> 8) & 0xFF;						
			TxMessage.Data[4] = (8000 - CD40017_MAX_OUTCUR*10) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//电流输出值 0.1A  -800A ~ 500A
			TxMessage.Data[5] = (8000 - CD40017_MAX_OUTCUR*10) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
			TxMessage.Data[6] = 0;
			TxMessage.Data[7] = 0;
			break;						
		}
		case CCS_DEC_CRO:		//充电机输出准备就绪状态CRO，配置阶段  
		{						//0010-充电机未完成充电准备，0100B-充电机已完成充电准备
			TxMessage.ExtId   = 0x140AF4E5;
			TxMessage.DLC 	  = 0x01;									
			TxMessage.Data[0] = 0x04;
			break;
		}
		case CCS_DEC_CCS:		//充电机充电状态CCS，充电阶段  
		{
			#define CD40017_OUTVOL		3600	//这是最初的定义，主要是调试使用，
			#define CD40017_OUTCUR		0
			#define CD40017_CHARGETIME	10

			TxMessage.ExtId   = 0x1814F4E5;
			TxMessage.DLC 	  = 0x06;												
//			TxMessage.Data[0] = (ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
//			TxMessage.Data[1] = (ModbusREGDat[22] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (ModbusREGDat[23] >> 0) & 0xFF;//电流输出值 0.1A  -800A ~ 500A
//			TxMessage.Data[3] = (ModbusREGDat[23] >> 8) & 0xFF;
//			TxMessage.Data[4] = (ModbusREGDat[24] >> 0) & 0xFF;//累计充电时间 1min 0-6000min
//			TxMessage.Data[5] = (ModbusREGDat[24] >> 8) & 0xFF;
			
			TxMessage.Data[0] = (INT16U)(gtBMS_Dat.CCS_1*10)%256;//电压输出值，0.1V
			TxMessage.Data[1] = (INT16U)(gtBMS_Dat.CCS_1*10)/256;			
			TxMessage.Data[2] = (INT16U)(gtBMS_Dat.CCS_2*10)%256;//电流输出值 0.1A  -800A ~ 500A
			TxMessage.Data[3] = (INT16U)(gtBMS_Dat.CCS_2*10)/256;
			TxMessage.Data[4] = gtBMS_Dat.CCS_3%256;			//累计充电时间 1min 0-6000min
			TxMessage.Data[5] = gtBMS_Dat.CCS_3/256;			


			break;						
		}
		case CCS_DEC_CST:			//充电机中止充电CST，充电阶段  
		{
			TxMessage.ExtId   = 0x141AF4E5;
			TxMessage.DLC 	  = 0x03;															
//			TxMessage.Data[0] = 0x03;//(ModbusREGDat[25] >> 0) & 0xFF;//中止充电原因
//			TxMessage.Data[1] = 0x00;//(ModbusREGDat[25] >> 8) & 0xFF;//中止充电故障原因			
//			TxMessage.Data[2] = 0x00;//(ModbusREGDat[26] >> 0) & 0xFF;//中止充电错误原因
			
			TxMessage.Data[0] = gtBMS_Dat.CST_1;//中止充电原因
			TxMessage.Data[1] = gtBMS_Dat.CST_2;//中止充电故障原因			
			TxMessage.Data[2] = gtBMS_Dat.CST_3;//中止充电错误原因			
			break;						
		}
		case CCS_DEC_CSD:  				//充电机统计数据报文CSD，结束阶段 
		{
			TxMessage.ExtId   = 0x181DF4E5;
			TxMessage.DLC 	  = 0x05;																	
//			TxMessage.Data[0] = (ModbusREGDat[27] >> 0) & 0xFF;//累计充电时间 1min
//			TxMessage.Data[1] = (ModbusREGDat[27] >> 8) & 0xFF;	
//			TxMessage.Data[2] = (ModbusREGDat[28] >> 0) & 0xFF;//输出能量	0.1kWh 0~500kWh	
//			TxMessage.Data[3] = (ModbusREGDat[28] >> 8) & 0xFF;
//			TxMessage.Data[4] = (ModbusREGDat[29] >> 0) & 0xFF;//充电机编号
			
			TxMessage.Data[0] = gtBMS_Dat.CSD_1%256;			//累计充电时间 1min
			TxMessage.Data[1] = gtBMS_Dat.CSD_1/256;	
			TxMessage.Data[2] = (INT16U)(gtBMS_Dat.CSD_2*10)%256;//输出能量	0.1kWh 0~500kWh	
			TxMessage.Data[3] = (INT16U)(gtBMS_Dat.CSD_2*10)/256;
			TxMessage.Data[4] = gtBMS_Dat.CSD_3;				//充电机编号			
			
			break;						
		}
		case CCS_DEC_CEM:  				//充电机错误报文CEM  
		{
			TxMessage.ExtId   = 0x141FF4E5;
			TxMessage.DLC 	  = 0x03;																			
//			TxMessage.Data[0] = (ModbusREGDat[30] >> 0) & 0xFF;
//			TxMessage.Data[1] = (ModbusREGDat[30] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (ModbusREGDat[31] >> 0) & 0xFF;
			break;						
		}
		case CCS_DEC_CFM:  		//充电机故障报文CFM  
		{
			TxMessage.ExtId   = 0x1434F4E5;
			TxMessage.DLC 	  = 0x02;																						
//			TxMessage.Data[0] = (ModbusREGDat[32] >> 0) & 0xFF;
//			TxMessage.Data[1] = (ModbusREGDat[32] >> 8) & 0xFF;			
			break;						
		}
		default:   tmp16 = 0x01;	break;	
	}
	if(tmp16 != 0x00)	return ;					//如果不存在该定义就退出
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;
  	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
  	tmp16 = 0;										//等待发送完毕
  	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x00FF))
  	{
    	tmp16++;
  	}

}

/*******************************************************************************
* 初始化所有参数
* 多协议数据帧初始化
* 开始充电阶段初始化
*	 参数：
		1-初始化为可以重新连接
		2-初始为不可以重新连接，比如已充满或BMS不需要充电	
*******************************************************************************/
void ParemeterInitial(INT8U u8Select)
{
//	INT8U tmp;
// for(tmp=10;tmp<100;tmp++){ModbusREGDat[tmp] = 0;}
	CanPackage.PGN 		= 0x00;			//将多数据接收包清零
	CanPackage.DataLen 	= 0x00 ;
	CanPackage.RxFlag 	= 0x00;
	CanPackage.FrameNum = 0x00;
	CanPackage.FramePointer = 0x00;
	gCommStation = CCS_DEC_INVALID;	   	//初始化为无效的状态
	CanMessageBuff.OutLen = 0x0000;		//读取缓冲区置零
	CanMessageBuff.InLen  = 0x0000;		//接收缓冲区置零
	if(u8Select == 0x01)				//允许重新连接
	{
		CAN_ChargeFlag.NoCharge = 0;	//BMS需要充电
		CAN_ChargeFlag.FullCharge = 0;	//BMS未充满
		CAN_ChargeFlag.Stop = 0;
		gCommStation = CCS_DEC_VBI;	   	//初始化为的状态
		CAN_Initial(CAN_REC_BMS);		//仅接受BMS发送给充电机的数据 
	}
	if(u8Select == 0x02)				//接收广播数据，但不允许重新连接
	{
	 	CAN_Initial(CAN_REC_ALL);		//接受所有数据
	}
}


/*******************************************************************************
*  	获取数据的位值
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
*  	将数据的指定位置1
* 
*******************************************************************************/
void SetBitStatus(INT16U *DatX,INT8U Shift)
{
	if(Shift > 15)	return ;
	*DatX |= 1 << Shift;
}
/*******************************************************************************
*  	将数据的指定位清0
* 
*******************************************************************************/
void ClrBitStatus(INT16U *DatX,INT8U Shift)
{
	if(Shift > 15)	return;
	*DatX &= ~(1 << Shift);
}


/*********************************************************
主要完成时间控制、流程控制，数据的发送和接受，调用其他函数完成数据处理

***********************************************************/

void task2_core(void *pdata)
{
	INT16U RunTime = 0x0000;			//运行时间计时，单位10ms,
	INT16U OutTime = 0x0000;			//超时时间计时，单位10ms
	INT16U tmp16;
	INT16U *ptrCTRLRegister;			//整个流程的控制寄存器，其实就是ModbusREGDat[2]，为了方便修改就重新定义
	pdata = pdata;
	CAN_Initial(CAN_REC_BMS);  			//仅能接收BMS数据，因为开始就进行连接
	ParemeterInitial(0x01);
//	ptrCTRLRegister = &ModbusREGDat[2];
while(1)
{
	OSTimeDlyHMSM(0,0,0,10);
	gCommStation = CCS_DEC_BCL;			//调试使用
	RunTime ++;			  				//运行时间，只用于循环，不用于其他功能
	OutTime ++;			  				//仅用于超时检测，不能和运行时间混合在一起
										//当允许连接且还没有进行连接的情况下开始连接
//	ModbusREGDat[1] = gCommStation; 	//上报协议转换器状态
//	ModbusREGDat[0] = ( ((1-CAN_ChargeFlag.OutTime) << 0)	//在上位机看来，1-不超时，0表示超时
//					   |(CAN_ChargeFlag.NoCharge<< 1)
//					   |(CAN_ChargeFlag.Charging << 2)
//					   |(CAN_ChargeFlag.Stop<< 3));						
	if(GetBitStatus(*ptrCTRLRegister,1))					//有启动连接指令
	{ 
	  	ClrBitStatus(ptrCTRLRegister,1);					//将第bit1位清零 ，指针类型
		if((gCommStation == CCS_DEC_INVALID)  	   			//处于不连接阶段或者是结束阶段都可以重新初始化
		   ||(gCommStation == CCS_DEC_OVER)) 				//保证可以在手动接收后可以重新开始充电
		{													//其实就是在上个车已经充满，下个车开始的时候才有用			
			ClrBitStatus(ptrCTRLRegister,0);				//清除上位机控制停机标志
			ParemeterInitial(0x01);							//开始连接
			OutTime = 0x00;
		}
	}
//****************以下为充电的各个阶段*********************//
	
	if(gCommStation == CCS_DEC_BCL) 	//充电中判断 不再充电阶段就清零充电中标志
	{  	CAN_ChargeFlag.Charging = 1; }
	else 
	{	CAN_ChargeFlag.Charging = 0; }

	if(OutTime > COMM_OUTTIME) 			//超时判断  
	{	
		CAN_ChargeFlag.OutTime = 1; 
		SET_LED_1_OFF; 					//通讯超时
	}
	else 
	{	
		CAN_ChargeFlag.OutTime = 0;
		SET_LED_1_ON	; 				//通讯不超时 
	}
//==============================================================================
	MCMDeal(gCommStation);	//与充电终端的相关处理
//==============================================================================
	switch(gCommStation)
	{
		case CCS_DEC_INVALID:			//无效的状态，还是为超时判断做
		{										 
			while(CAN_ReadBuffNum())
			{						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
			   		OutTime = 0x00;						
				}
				CanMessageBuff.OutLen ++;
			}												//5S连广播数据都没有接收到，可能插头已经拔掉了
			if(OutTime == COMM_OUTTIME) ParemeterInitial(0x01);//要进行检测是否有新的BMS连接上		
			break;
		}
		case CCS_DEC_VBI:									//什么都没有接收到,在等待接受VBI
		{													//250ms发送一次充电机不能辨识报文  
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_0);
			if(OutTime > 6000) 								//一分钟超时错误，单位10ms
			{
				CAN_Initial(CAN_REC_BMS);					//要初始化CAN，防止CAN初始化失败或中间停止工作
				OutTime = COMM_OUTTIME+1;					//第一阶段如果连接超时仅报告上位机超时，但是不会退出
				ParemeterInitial(0x01);
			}												//+1 是为通讯超时判断准备

			if(CAN_TP_CM(1)) OutTime = 0x00;				//多数据接受处理
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x30))	//VBI
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_VBI);
			   gCommStation = CCS_DEC_BRM;
			   OutTime = 0x00;
			}
			break ;
		}

		case CCS_DEC_BRM:					   									//已经接收到VBI，等待接受BRM
		{																		//250ms发送一次充电机不能辨识报文
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S超时错误
			if(CAN_TP_CM(1)) OutTime = 0x00;									//多数据接受处理
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x02))			//BRM
			{
			   DecodeCCSMessage(CCS_DEC_CRM_1);									//当接收到完整的握手数据后立即发送可以识别命令
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BRM);
			   gCommStation = CCS_DEC_BCP;
			   OutTime = 0x00;										 
			}
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_0);			//因为不能一次握手成功，怀疑是接收成功后不能立即回复信息 
			break ;													 			//所以将此行放在本阶段的末尾，接收成功后将不发送不能识别数据
		}

		case CCS_DEC_BCP:					  									//已经接收到BRM，等待接受电池充电参数
		{																		//250ms发送一次充电机能辨识报文
			if((RunTime%25) == 0x01) DecodeCCSMessage(CCS_DEC_CRM_1);	
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S超时错误
			if(CAN_TP_CM(1)) OutTime = 0x00;	   								//多数据处理函数
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x06))			//BCP
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_BCP);
			   gCommStation = CCS_DEC_BRO;
			   OutTime = 0x00;
			}
			break ;
		}
		case CCS_DEC_BRO:					  									//等待BMS充电参数准备报文
		{
			if((RunTime%50) == 0x01) DecodeCCSMessage(CCS_DEC_CTS);				//500ms时间同步和最大输出能力报文
			if((RunTime%25) == 0x02) DecodeCCSMessage(CCS_DEC_CML);				//250ms
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);					//5S超时错误
			while(CAN_ReadBuffNum())
			{
				Can_DeleteMessageEXT(1,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//清除无用的数据帧						
				if(CAN_ReadBuffNum())
				{	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//获取缓冲区的地址
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1409E5F4 )	 		//确认收到BRO报文
					{	
						if(CanMessageBuff.Buff[tmp16].Data[0] == 0x04){ 		//报文内容为完成充电准备，否则超时退出						
							DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BRO);
				   			gCommStation = CCS_DEC_BCS;
				   			OutTime = 0x00;
						}
						else 
						{										//两种情况，1-BMS已满，2-配置错误
							 ParemeterInitial(0x02);		 	//初始化为不需要连接
							 CAN_ChargeFlag.NoCharge = 1;		//BMS不需要充电
						}
					}
					CanMessageBuff.OutLen ++;	  				//删除已经处理过的报文
				}			
			}
			break ;
		}
		case CCS_DEC_BCS:						  				//充电阶段报文
		{														//250ms充电准备就绪
			if((RunTime%25) == 0x01)DecodeCCSMessage(CCS_DEC_CRO); 	
			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);	//5S超时错误
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()) {
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); //获取缓冲区的地址			
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
		case CCS_DEC_BCL:						  			//充电阶段报文
		{													//20ms
			if((RunTime%2) == 0x01)	{DecodeCCSMessage(CCS_DEC_CCS);  }
			//1s发送一次CSD
//			if(OutTime>COMM_OUTTIME) ParemeterInitial(0x01);//5S超时错误
			if(GetBitStatus(*ptrCTRLRegister,0)) 			//上位机发出充电结束指令
			{												//或者是上位机要求停止
				ClrBitStatus(ptrCTRLRegister,0);			//将第0位清零 ，指针类型
				gCommStation = CCS_DEC_BST;
				CAN_ChargeFlag.Stop = 1;					//设置上位机要求停止标志
				OutTime = 0x00;
			}							
			while(CAN_ReadBuffNum())
			{
				Can_DeleteMessageEXT(5,0x10,0x11,0x19,0x14,0x12,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){											//执行删除处理后可能已经没有可用的数据，所以要做判断
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//获取缓冲区的地址
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1810E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
			   			OutTime = 0x00;											//电池充电需求
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1811E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
			   			OutTime = 0x00;											//电池充电总状态
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x1419E5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BST);
			   			
						if((CanMessageBuff.Buff[tmp16].Data[0] > 0) 			//如果充满就设置充满标志
						   ||(CanMessageBuff.Buff[tmp16].Data[1] > 0)
						   ||(CanMessageBuff.Buff[tmp16].Data[2] > 0))
						{	
							CAN_ChargeFlag.FullCharge = 1;	
							gCommStation = CCS_DEC_BST;							//根据数据的状态来判断是否要进行下一步
						}
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}
		case CCS_DEC_BST:						  			//充电阶段报文,BMS中止充电报文
		{										  			//
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CST);  }
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x02);						//5S超时错误
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

		case CCS_DEC_BSD:						  			//充电结束阶段报文,BMS统计数据
		{										  			//500ms时间同步和最大输出能力报文
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CST);  }
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x02);						//5S超时错误
			   	OutTime = 0x00;						
			}						
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181CE5F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSD);
			   			gCommStation = CCS_DEC_OVER;
						CAN_Initial(CAN_REC_ALL);							//允许接收所有的数据
			   			OutTime = 0x00;						
					}
					CanMessageBuff.OutLen ++;
				}
			}
			break;
		}
		case CCS_DEC_OVER:						  //充电阶段报文,BMS中止充电报文
		{										  //250ms，本阶段使用OutTime计时，所以不能清除
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CSD);  }
			if(OutTime>COMM_OUTTIME - 1)  		//如果不-1，则会造成通讯超时标志位判断错误
			{
				OutTime = 0x00;
//				if(CAN_ChargeFlag.Stop == 1)	//如果是手动停止，则5S之后还要重新连接	
//				{
//					ParemeterInitial(0x01);			//为下次连接做准备
//					CAN_Initial(CAN_REC_BMS);		//仅接受BMS发送给充电机的数据
//				}
//				else
//				{									//不论何种状态到达本阶段均不在连接
					ParemeterInitial(0x02);			//5S超时错误，说明插头已经拔掉了，为下次连接做准备
					CAN_Initial(CAN_REC_ALL);		//
//				}									//下面进入INVALID状态
			}		
			break;
		}
		default:break;
	}
}			

}



