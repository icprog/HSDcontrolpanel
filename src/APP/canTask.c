 /*********************************************************************************************************************
*                                            头文件包含
***********************************************************************************************************************/
//#include "cancom.h" 
#include "ucos_ii.h"
#include "stm32f10x.h"
#include "global.h"
#include "includes.h"
//#include "controller.h"
//修改内容：
//充电状态0，增加if(gtMCT_Dat.BMSStage != 0x03)  ltTxMsg.Data[0] = 0x00;//在非充电阶段就设为停止状态，但是手动会无法显示充电状态，需要外部是否启动标志位
//在充电阶段，还原 while(CAN_ReadBuffNum())			
//	更改if(ptrMsg.Data[0] == gtMCT_Dat.ChargerID)为 if(ptrMsg.Data[1] == gtMCT_Dat.ChargerID)，现在还是注释
/********************************************************************************************************************
*                                            全局变量定义
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

CANRECMSGBUFF CanMessageBuff;			//定义CAN接收缓冲区
CANRECMSGBUFF MCM_CanPackage;	//建立另外一个can接收缓冲区

_T_BMS_DAT gtBMS_Dat;

CanTxMsg TxMessage;
CANRECMSG rxMessage;
CANPACKAGE	CanPackage;


CANSENDPACKAGE	CanSendPackage;

INT8U  TransmitMailbox;
INT8U  gCommStation = 0x00;				//记录各个通讯阶段
INT16U ModbusREGDat[300];
_T_MCT_DAT gtMCT_Dat;//定义一个结构体变量
_T_METER_DAT gtMeter_Dat;

_CAN_CHARGE_FLAG CAN_ChargeFlag;						//充电过程中的标志
void rec_secondGun_can();


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
	if(tmp16 <= 0x00)	
		return 0;							//缓冲区空,一般不会小于0
	
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

  	CAN_ITConfig(CAN1,CAN_IT_FMP0, DISABLE); 	  			//使能FIFO0消息挂号中断
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
  	if(u8Type == CAN_REC_ALL)	{							//仅接受BMS发给充电机的数据
		CAN_FilterInitStructure.CAN_FilterIdHigh	= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x0000;
	}
	if(u8Type == CAN_REC_BMS)	{							//接收所有的数据
		CAN_FilterInitStructure.CAN_FilterIdHigh	= (0x56F4 >> 13);
	  	CAN_FilterInitStructure.CAN_FilterIdLow		= (0x56F4 & 0x1FFF)<<3;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0xFFFF>>13);;
	  	CAN_FilterInitStructure.CAN_FilterMaskIdLow	= 0x1FFF<<3;
	}

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
	INT8U u8RegAdr = 0;
	INT32U tmp;

	switch(u8ID)
   	{
		case BMS_DEC_BRM:  				//BMS和车辆辨识报文BRM										   
		{
//			for(u8RegAdr = M_BRM_START_ADR; u8AdrCnt < M_BRM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			gtBMS_Dat.BMR_1 = pu8Mesg[0] + (pu8Mesg[1]<<8) + (pu8Mesg[2]<<16);		//通讯协议版本号
			gtBMS_Dat.BMR_4 =  pu8Mesg[3];					//电池类型
			gtBMS_Dat.BMR_5 =  (FP32)(pu8Mesg[4] + (pu8Mesg[5]<<8))/10;			//整车电池额定容量
			gtBMS_Dat.BMR_7 =  (FP32)(pu8Mesg[6] + (pu8Mesg[7]<<8))/10;			//整车额定总电压
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
			gtBMS_Dat.BMR_20 = pu8Mesg[19] + (pu8Mesg[20]<<8) + (pu8Mesg[21]<<16);//充电次数 
			gtBMS_Dat.BMR_23 =  pu8Mesg[22];				//产权标识
			gtBMS_Dat.BMR_24 =  pu8Mesg[23];				//预留
			for(u8AdrCnt=0; u8AdrCnt<25; u8AdrCnt++){		//车辆识别信息 25byte
				gtBMS_Dat.BMR_25[u8AdrCnt] =  pu8Mesg[24+u8AdrCnt];				
			}
			
			memcpy((INT8U*)&ModbusREGDat[73],(INT8U*)&gtBMS_Dat.BMR_1,4);	
			ModbusREGDat[75] =gtBMS_Dat.BMR_4;        						                         //电池类型
			
			tmp =  gtBMS_Dat.BMR_5;       						              
  			memcpy((INT8U*)&ModbusREGDat[76],(INT8U*)&tmp,4);								 //整车电池额定容量   

			tmp =  gtBMS_Dat.BMR_7;       						              
  			memcpy((INT8U*)&ModbusREGDat[78],(INT8U*)&tmp,4);								  //整车额定总电压
  		
			break;		
		}
		case BMS_DEC_VBI:  				//车辆和电池标识报文VBI	
		{
//			for(u8RegAdr = M_VBI_START_ADR; u8AdrCnt < M_VBI_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt]) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			
			gtBMS_Dat.VBI_1 =   pu8Mesg[0] + (pu8Mesg[1]<<8) + (pu8Mesg[2]<<16);	//通讯协议版本号;
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

			memcpy((INT8U*)&ModbusREGDat[82],(INT8U*)gtBMS_Dat.VBI_4,9);								   //车辆VIN
			memcpy((INT8U*)&ModbusREGDat[91],(INT8U*)&gtBMS_Dat.VBI_21,2);							   //
			memcpy((INT8U*)&ModbusREGDat[93],(INT8U*)&gtBMS_Dat.VBI_25,4);							   //电池管理系统ID	
			memcpy((INT8U*)&ModbusREGDat[97],(INT8U*)&gtBMS_Dat.VBI_33,4);							   //电池箱序列号

			break;		
		}
		case BMS_DEC_BCP:  				//蓄电池充电参数BCP	
		{
//			for(u8RegAdr = M_BCP_START_ADR; u8AdrCnt < M_BCP_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            } 
			gtBMS_Dat.BCP_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/100.0f;			//蓄电池模块最高允许充电电压			
			gtBMS_Dat.BCP_3 = (pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f-400;			//最高允许充电电流 
			gtBMS_Dat.BCP_5 = pu8Mesg[4]*10;									//最大允许充电容量 
			gtBMS_Dat.BCP_6 = (pu8Mesg[6] + (pu8Mesg[7]<<8))/10.0f;			//最高允许充电总电压 
			gtBMS_Dat.BCP_8 = pu8Mesg[8]-50;									//最高允许温度 
			gtBMS_Dat.BCP_9 = (pu8Mesg[9] + (pu8Mesg[10]<<8));						//整车蓄电池估计剩余容量 
			gtBMS_Dat.BCP_11 = (pu8Mesg[11] + (pu8Mesg[12]<<8))/10.0f;			//整车蓄电池总电压 
			gtBMS_Dat.BCP_13 = pu8Mesg[12];										//整车 SOH 

			tmp =  gtBMS_Dat.BCP_1;        						              
  			memcpy((INT8U*)&ModbusREGDat[101],(INT8U*)&tmp,4);							   //蓄电池模块最高允许充电电压	

			tmp =  gtBMS_Dat.BCP_3;        						                  
  			memcpy((INT8U*)&ModbusREGDat[103],(INT8U*)&tmp,4);							   //最高允许充电电流 

			ModbusREGDat[105] =gtBMS_Dat.BCP_5;        						                          //最大允许充电容量

			tmp =  gtBMS_Dat.BCP_6;        						              
  			memcpy((INT8U*)&ModbusREGDat[106],(INT8U*)&tmp,4);							   //最高允许充电电流 

  			ModbusREGDat[108] =gtBMS_Dat.BCP_8;        						                          //最高允许温度 
  			ModbusREGDat[109] =gtBMS_Dat.BCP_9;        						                          //整车蓄电池估计剩余容量 

  			//ModbusREGDat[110] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[111] =gUserDB.ILB_alarm_value_set;  
  			tmp =  gtBMS_Dat.BCP_11;        						              
  			memcpy((INT8U*)&ModbusREGDat[110],(INT8U*)&tmp,4);							   //整车蓄电池总电压 

  			ModbusREGDat[112] =gtBMS_Dat.BCP_13;        						                          //SOH

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
			gtBMS_Dat.BCL_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//电压需求
			gtBMS_Dat.BCL_3 = 400-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f;		//电流需求
			gtBMS_Dat.BCL_5 = pu8Mesg[4]; 										//充电模式

			if(gUserDB.doubleGunPlugStatus == NOK)	 //单枪充电时
			{
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			//电压需求			
				gUserDB.BMS_needCurr = gtBMS_Dat.BCL_3;		//电流需求
				gUserDB.doubleGun_disCurrent=0;
			}
			else  //双枪充电时
			{
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			      //电压需求			
				gUserDB.BMS_needCurr = gtBMS_Dat.BCL_3 /2;		       //电流需求/2 
				gUserDB.doubleGun_disCurrent = gUserDB.BMS_needCurr;  //分流电流 //单枪
			}																	 

			tmp =  gtBMS_Dat.BCL_1; 
  			if(tmp<0)
  			tmp=0;      						                  
  			memcpy((INT8U*)&ModbusREGDat[113],(INT8U*)&tmp,4);							   //电压需求

  			//ModbusREGDat[115] =gUserDB.inactiveILB_value;        						              
  			//ModbusREGDat[116] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gUserDB.BMS_needCurr;  
  			if(tmp<0)
  				tmp=0;   						              
  			memcpy((INT8U*)&ModbusREGDat[115],(INT8U*)&tmp,4);							   //电流需求

  			ModbusREGDat[117] =gtBMS_Dat.BCL_5;        						                              //充电模式

						
			break; 
		}
		case BMS_DEC_BCS:  				//电池充电状态BCS，充电阶段
		{
//			for(u8RegAdr = M_BCS_START_ADR; u8AdrCnt < M_BCS_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
//			{
//				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
//            }
			gtBMS_Dat.BCS_1 = (pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//充电电压测量值（V） 
			gtBMS_Dat.BCS_3 = 400-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//充电电流测量值（A）
			gtBMS_Dat.BCS_5 = pu8Mesg[4]-50;	//电池组最低温度（oC）  
			gtBMS_Dat.BCS_6 = pu8Mesg[5]-50;	//电池组最高温度（oC）  
			gtBMS_Dat.BCS_7 = pu8Mesg[6];		//当前荷电状态  SOC（%）  
			gtBMS_Dat.BCS_8 = pu8Mesg[7];		//估算剩余时间（min）

			gUserDB.BMS_Volt = gtBMS_Dat.BCS_1;     //(pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//充电电压测量值（V） 
			gUserDB.BMS_Curr = gtBMS_Dat.BCS_3;     //800-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//充电电流测量值（A）

			tmp =  gtBMS_Dat.BCS_1;  
  			if(tmp<0)
  				tmp=0;        						              
  			memcpy((INT8U*)&ModbusREGDat[118],(INT8U*)&tmp,4);							   //充电电压测量值（V） 

  			//ModbusREGDat[120] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[121] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gtBMS_Dat.BCS_3;  
  			if(tmp<0)
  				tmp=0;          						              
  			memcpy((INT8U*)&ModbusREGDat[120],(INT8U*)&tmp,4);							   //充电电流测量值（A）

  			//OSTimeDlyHMSM(0,0,0,20); 
  			//ModbusREGDat[122] =gtBMS_Dat.BCS_5;        						                          //电池组最低温度（oC） 
  			//ModbusREGDat[123] =gtBMS_Dat.BCS_6;        						                          //电池组最高温度（oC）  
  			ModbusREGDat[124] =gtBMS_Dat.BCS_7;        						                          //当前荷电状态  SOC（%） 
  			ModbusREGDat[125] =gtBMS_Dat.BCS_8;        						                          //估算剩余时间（min）

		   ModbusREGDat[123] =gUserDB.BatTempMAx +50 ;      						                          //电池组最高温度（oC）  

			//gUserDB.cellBatVoltMax	= (pu8Mesg[4]+(pu8Mesg[5]<<4))/100.0f;		
			gUserDB.cellBatVoltMax	= ((pu8Mesg[4]+((pu8Mesg[5]&0x0f)<<8)))/100.0;	//最高单体电压
			ModbusREGDat[133] =gUserDB.cellBatVoltMax *100;	  // gtBMS_Dat.BSM_1;
			break;         
		}
		case BMS_DEC_BSM:  				//蓄电池状态信息BSM，充电阶段
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

		//	gUserDB.cellBatVoltMax	= gtBMS_Dat.BSM_1/100.0f;
			gUserDB.BatTempMAx	= pu8Mesg[1]-50;

			//ModbusREGDat[133] =gtBMS_Dat.BSM_1;
			ModbusREGDat[134] =gtBMS_Dat.BSM_2+(gtBMS_Dat.BSM_3<<8);
			ModbusREGDat[135] =gtBMS_Dat.BSM_4+(gtBMS_Dat.BSM_5<<8);
			

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
//			gUserDB.cellBatVoltMax = (pu8Mesg[3] + (pu8Mesg[4]<<8))/1000.0f;			//蓄电池单体最高电压（V
//			gUserDB.BatTempMAx = pu8Mesg[6]-50;									//蓄电池组最高温度（oC）
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
			gtBMS_Dat.BSD_2 = (pu8Mesg[1] + (pu8Mesg[2]<<8))/1000.0f;			//蓄电池单体最低电压（V
			gtBMS_Dat.BSD_4 = (pu8Mesg[3] + (pu8Mesg[4]<<8))/1000.0f;			//蓄电池单体最高电压（V
			gtBMS_Dat.BSD_6 = pu8Mesg[5]-50;									//蓄电池组最低温度（oC） 
			gtBMS_Dat.BSD_7 = pu8Mesg[6]-50;									//蓄电池组最高温度（oC）

			ModbusREGDat[126] =gtBMS_Dat.BSD_1;        						                          //中止荷电状态  SOC（%） 
  			//ModbusREGDat[127] =gUserDB.ILB_alarm_value_set;        						             
  			//ModbusREGDat[128] =gUserDB.ILB_alarm_value_set;  
  			tmp =  gtBMS_Dat.BSD_2*100;       						              
  			memcpy((INT8U*)&ModbusREGDat[127],(INT8U*)&tmp,4);							 //蓄电池单体最低电压（V

  			//ModbusREGDat[129] =gUserDB.ILB_alarm_value_set;        						              
  			//ModbusREGDat[130] =gUserDB.ILB_alarm_value_set; 
  			tmp =  gtBMS_Dat.BSD_4*100;        						              
  			memcpy((INT8U*)&ModbusREGDat[129],(INT8U*)&tmp,4);							   //蓄电池单体最高电压（V	    			

  			ModbusREGDat[131] =gtBMS_Dat.BSD_6;        						                           //蓄电池组最低温度（oC） 
  			ModbusREGDat[132] =gtBMS_Dat.BSD_7;        						                           //蓄电池组最高温度（oC）

			break;	            
		}
		case BMS_DEC_BEM:  				//BMS错误报文BEM，错误阶段
		{
			for(u8RegAdr = M_BEM_START_ADR; u8AdrCnt < M_BEM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
			{
				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
            }
			break;
		}
		case BMS_DEC_BFM:  				//BMS故障报文BFM，故障阶段
		{
			for(u8RegAdr = M_BFM_START_ADR; u8AdrCnt < M_BFM_ADR_LEN; u8AdrCnt += 2, u8RegAdr++)
			{
				ModbusREGDat[u8RegAdr] = (pu8Mesg[u8AdrCnt] ) + (pu8Mesg[u8AdrCnt + 1] << 8);		 	
            }
			break;		
		}
		case BMS_DOUBLE_STATUS:   //BMS 双枪充电状态
		{
		    gUserDB.doubleGunPlugStatus= OK;
			gUserDB.doubleGun_timeout=0;
			//gUserDB.mainGunBms_ready=pu8Mesg[0] ;	   //主枪BMS状态
			if( (pu8Mesg[0]& 0X01) ==OK && (pu8Mesg[0]>>4)==1 )	//从充电枪      //接收到主充电枪数据时,则为辅助充电枪,则往下执行
			{
			    gUserDB.bms_readyStatus=OK;
			    gUserDB.doubleGun_ID=2 ;			  //辅助充电枪ID
				gtBMS_Dat.BCS_7=pu8Mesg[1] ;
					   					//SOC
			//	tmp=(pu8Mesg[3]<<8) + pu8Mesg[2];	   
				gtBMS_Dat.BCL_1=(pu8Mesg[3]<<8) + pu8Mesg[2];	 //需求电压

				gtBMS_Dat.BCL_3=(pu8Mesg[5]<<8 )+pu8Mesg[4] ;	   //需求电流	   
				gUserDB.doubleGun_disCurrent=(pu8Mesg[7]<<8 )+pu8Mesg[6] ;	   //分流电流
				gUserDB.BMS_needCurr = gUserDB.doubleGun_disCurrent	 ;
				gUserDB.BMS_needVolt = gtBMS_Dat.BCL_1;			//电压需求			
				
				
				ModbusREGDat[124] =gtBMS_Dat.BCS_7;        						   //当前荷电状态  SOC（%）
				
				tmp =  gtBMS_Dat.BCL_1; 
  				if(tmp<0)
  				tmp=0;      						                  
  				memcpy((INT8U*)&ModbusREGDat[113],(INT8U*)&tmp,4);							   //电压需求  
  			
  				tmp =  gUserDB.BMS_needCurr;  
  				if(tmp<0)
  					tmp=0;   						              
  				memcpy((INT8U*)&ModbusREGDat[115],(INT8U*)&tmp,4);							   //电流需求 
							
			}
			
			

		   break;					
		}
		case BMS_DOUBLE_OUT:	 //BMS 双枪充电输出
		{								
			gUserDB.doubleGunPlugStatus= OK;
			gUserDB.doubleGun_timeout=0;

			if(gUserDB.doubleGun_ID==2)	 //辅助充电枪
			{
				gtBMS_Dat.BCS_1=(pu8Mesg[1]<<8 )+pu8Mesg[0] ;	   //BMS充电电压测量值
				gtBMS_Dat.BCS_3=(pu8Mesg[3]<<8 )+pu8Mesg[2] ;	   //BMS充电电流测量值	
                

				gUserDB.BMS_Volt = gtBMS_Dat.BCS_1;     //(pu8Mesg[0] + (pu8Mesg[1]<<8))/10.0f;			//充电电压测量值（V） 
			    gUserDB.BMS_Curr = gtBMS_Dat.BCS_3;     //800-(pu8Mesg[2] + (pu8Mesg[3]<<8))/10.0f ;		//充电电流测量值（A）

			    tmp =  gtBMS_Dat.BCS_1;  
  			    if(tmp<0)
  				   tmp=0;        						              
  			   memcpy((INT8U*)&ModbusREGDat[118],(INT8U*)&tmp,4);							   //充电电压测量值（V）
  			
  			  tmp =  gtBMS_Dat.BCS_3;  
  			  if(tmp<0)
  				tmp=0;          						              
  			  memcpy((INT8U*)&ModbusREGDat[120],(INT8U*)&tmp,4);							   //充电电流测量值（A）

			  if(gUserDB.BMS_Curr >gUserDB.BMS_needCurr*2 +20 )								   //BMS检测电流大于需求电流20A,关闭从充电机
			     gUserDB.autoRunCtlFlag =STOP; 
			}

		   break;
		}
				
		default: 	break;
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
	INT16U tmp16,kk=0;						//临时变量
	INT8U lBuffAddr,ret=0;
	Runtime	++;							//计时增加
	//Runtime = 0x00;//debug
	if(Runtime > 500)					//超时处理1S
	{									//如果超时则将CanPackage初始化
		tmp16 = CanPackage.PGN;			//如果不保存放弃连接就不能正常工作
		CanPackage.PGN = 0x00;			//参数群编号
		CanPackage.DataLen = 0x00 ;
		CanPackage.FrameNum = 0x00;
		CanPackage.FramePointer = 0x00;	//位置指针
		Runtime = 0x00;	  
		if(CanPackage.RxFlag == 0x01)						//如果正在通讯
		{
			CanPackage.RxFlag = 0x00;						//将标志位清零
			TxMessage.ExtId   = 0x1CECF456;	  				//放弃连接
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
			TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
			tmp16 = 0;										//等待发送完毕
			while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
			{
				tmp16++;
			}
	   	}
	}								

 	//连接传输协议											
	lBuffAddr = CanMessageBuff.OutLen ;	 //得到数据在缓冲区的位置 	
	switch(CanMessageBuff.Buff[lBuffAddr].ExtID )
	{
	   case   0x1CEC56F4:
	   {
	   		if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0x10)	//请求建立连接
			{													//参数群编号
				CanPackage.PGN = CanMessageBuff.Buff[lBuffAddr].Data[6];
				CanPackage.DataLen = CanMessageBuff.Buff[lBuffAddr].Data[1]	+ CanMessageBuff.Buff[lBuffAddr].Data[2] *256 ;
				CanPackage.FrameNum = CanMessageBuff.Buff[lBuffAddr].Data[3];//0x0F
				CanPackage.RxFlag = 0x01;						//已经建立连接
				CanPackage.FramePointer = 0x01;	   				//位置指针从1开始
				Runtime = 0x00;

				TxMessage.ExtId   = 0x1CECF456;
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
//				can1_write(TxMessage);
				TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
				tmp16 = 0;										//等待发送完毕
				while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
				{
					tmp16++;
				}
			}
			else if(CanMessageBuff.Buff[lBuffAddr].Data[0] == 0xFF)	//放弃连接
			{
				CanPackage.PGN = 0x00;							//参数群编号
				CanPackage.DataLen = 0x00 ;
				CanPackage.FrameNum = 0x00;	
				CanPackage.FramePointer = 0x00;	   				//位置指针 					
			}
			Runtime = 0x00;//
			
		}
	   	break;
	case 0x1CEB56F4:
		{				
			if((CanMessageBuff.Buff[lBuffAddr].ExtID == 0x1CEB56F4) &&(CanPackage.RxFlag == 0x01))//多数据传输
			{
				if(CanMessageBuff.Buff[lBuffAddr].Data[0] == CanPackage.FramePointer)
				{
					tmp16 =	(CanPackage.FramePointer-1) *7 ;
					if(tmp16 > 190 )	
						tmp16= 190;				//防止数据溢出
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
					TxMessage.ExtId   = 0x1CECF456;
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
//					can1_write(TxMessage);
					TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
					tmp16 = 0;										//等待发送完毕
					
					while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
					{
						tmp16++;
					}
				}

				if(CanPackage.FramePointer == CanPackage.FrameNum + 1)	//表明接收完毕，因为CanPackage.FramePointer总是指向下一个接收位置，所以要+1
				{																					
					TxMessage.ExtId   = 0x1CECF456;
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
//					can1_write(TxMessage);
					TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);	//发送消息
					tmp16 = 0;										//等待发送完毕
					while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 != 0x0FFF))
					{
						tmp16++;
					}
					
					CanPackage.RxFlag = 0x02;//帧接收完成
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
* 初始化所有参数
* 多协议数据帧初始化
* 开始充电阶段初始化
*	 参数：
		1-初始化为可以重新连接
		2-初始为不可以重新连接，比如已充满或BMS不需要充电	
*******************************************************************************/
void ParemeterInitial(INT8U u8Select)
{
	INT8U tmp;
	for(tmp=10;tmp<100;tmp++){ModbusREGDat[tmp] = 0;}
	CanPackage.PGN 		= 0x00;			//将多数据接收包清零
	CanPackage.DataLen 	= 0x00 ;
	CanPackage.RxFlag 	= 0x00;
	CanPackage.FrameNum = 0x00;
	CanPackage.FramePointer = 0x00;
	gCommStation = CCS_DEC_BRM;	   	//初始化为无效的状态
	CanMessageBuff.OutLen = 0x0000;		//读取缓冲区置零
	CanMessageBuff.InLen  = 0x0000;		//接收缓冲区置零
	if(u8Select == 0x01)				//允许重新连接
	{
		CAN_ChargeFlag.NoCharge = 0;	//BMS需要充电
		CAN_ChargeFlag.FullCharge = 0;	//BMS未充满
		CAN_ChargeFlag.Stop = 0;
		gCommStation = CCS_DEC_BRM;	   	//初始化为的状态
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
void DecodeCCSMessage(INT8U u8CCSID)
{
	INT16U tmp16 = 0x00;
	switch(u8CCSID)
	{
		case CCS_DEC_CRM_0:   	//充电机辨识报文CRM，握手阶段，BMS不能辨识
		{
			TxMessage.ExtId   = 0x1801F456;
			TxMessage.DLC 	  = 0x08;
			TxMessage.Data[0] = 0;	//充电机编号
			TxMessage.Data[1] = 0;	//充电插头编号		
			TxMessage.Data[2] = 0;	//充电机位置00-户外，01-户内
			TxMessage.Data[3] = 0x00;	//BMS辨识结果，00-不能辨识，01-能辨识
			TxMessage.Data[4] =  0xFF;//充电机所在区域编号，4字节ascii
			TxMessage.Data[5] = 0xFF;			
			TxMessage.Data[6] =  0xFF;
			TxMessage.Data[7] =  0xFF;						
			break;		
		}		
		case CCS_DEC_CRM_1:   	//充电机辨识报文CRM，握手阶段，BMS能辨识
		{
			TxMessage.ExtId   = 0x1801F456;
			TxMessage.DLC 	  = 0x08;
			TxMessage.Data[0] =  0xAA;	//BMS辨识结果，00-不能辨识，0AA-能辨识
			TxMessage.Data[1] = 0;			
			TxMessage.Data[2] = 0;
			TxMessage.Data[3] = 0x01;
			TxMessage.Data[4] =  0xFF;
			TxMessage.Data[5] =  0xFF;			
			TxMessage.Data[6] =  0xFF;
			TxMessage.Data[7] =  0xFF;						
			break;		
		}
		case CCS_DEC_CTS:		//充电机输出时间同步CTS，配置阶段，压缩BCD码 
		{						//可以不配置分秒，仅配置大的日期即可

			TxMessage.ExtId   = 0x1807F456;
			TxMessage.DLC 	  = 0x07;						
			TxMessage.Data[0] = 0;//(ModbusREGDat[13] >> 0) & 0xFF; //秒
			TxMessage.Data[1] = 0x20;//(ModbusREGDat[13] >> 8) & 0xFF;//分			
			TxMessage.Data[2] = 0x17;//(ModbusREGDat[14] >> 0) & 0xFF;//时
			TxMessage.Data[3] = 0x25;//(ModbusREGDat[14] >> 8) & 0xFF;//日
			TxMessage.Data[4] = 0x03;//(ModbusREGDat[15] >> 0) & 0xFF;//月
			TxMessage.Data[5] = 0x14;//(ModbusREGDat[15] >> 8) & 0xFF;//年 低位			
			TxMessage.Data[6] = 0x20;//(ModbusREGDat[16] >> 0) & 0xFF;//年高位	
	
//			TxMessage.Data[2] = (ModbusREGDat[14] >> 0) & 0xFF;//时
//			TxMessage.Data[3] = (ModbusREGDat[14] >> 8) & 0xFF;//日
//			TxMessage.Data[4] = (ModbusREGDat[15] >> 0) & 0xFF;//月
//			TxMessage.Data[5] = (ModbusREGDat[15] >> 8) & 0xFF;//年 低位			
//			TxMessage.Data[6] = (ModbusREGDat[16] >> 0) & 0xFF;//年高位
			break;						
		}
		case CCS_DEC_CML:		//充电机最大输出级别CML，配置阶段  
		{
			#define CD40017_MAX_OUTVOL	7500
			#define CD40017_MIN_OUTVOL	3000
			#define CD40017_MAX_OUTCUR	2000
			TxMessage.ExtId   = 0x1808F456;
			TxMessage.DLC 	  = 0x06;												
			TxMessage.Data[0] = CD40017_MAX_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
			TxMessage.Data[1] = CD40017_MAX_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;
			TxMessage.Data[2] = CD40017_MIN_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
			TxMessage.Data[3] = CD40017_MIN_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;						
			TxMessage.Data[4] = (4000 - CD40017_MAX_OUTCUR) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//电流输出值 0.1A  -800A ~ 500A
			TxMessage.Data[5] = (4000 - CD40017_MAX_OUTCUR) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
			TxMessage.Data[6] = 0;
			TxMessage.Data[7] = 0;
			break;						
		}
		case CCS_DEC_CRO:		//充电机输出准备就绪状态CRO，配置阶段  
		{						//0010-充电机未完成充电准备，0100B-充电机已完成充电准备
			TxMessage.ExtId   = 0x100AF456;
			TxMessage.DLC 	  = 0x01;									
			TxMessage.Data[0] = 0xAA;
			break;
		}
		case CCS_DEC_CCS:		//充电机充电状态CCS，充电阶段  
		{
			#define CD40017_OUTVOL		3600	
			#define CD40017_OUTCUR		0
			#define CD40017_CHARGETIME	10

			TxMessage.ExtId   = 0x1812F456;
			TxMessage.DLC 	  = 0x06;												
//			TxMessage.Data[0] = CD40017_OUTVOL % 256;//(ModbusREGDat[22] >> 0) & 0xFF;//电压输出值，0.1V
//			TxMessage.Data[1] = CD40017_OUTVOL / 256;//(ModbusREGDat[22] >> 8) & 0xFF;			
//			TxMessage.Data[2] = (8000 - CD40017_OUTCUR) % 256;//(ModbusREGDat[23] >> 0) & 0xFF;//电流输出值 0.1A  -800A ~ 500A
//			TxMessage.Data[3] = (8000 - CD40017_OUTCUR) / 256;//(ModbusREGDat[23] >> 8) & 0xFF;
//			TxMessage.Data[4] = CD40017_CHARGETIME % 256;//(ModbusREGDat[24] >> 0) & 0xFF;//累计充电时间 1min 0-6000min
//			TxMessage.Data[5] = CD40017_CHARGETIME / 256;//(ModbusREGDat[24] >> 8) & 0xFF;
			TxMessage.Data[0] = ((INT32U)gUserDB.C_outVolt*10 >> 0) & 0xFF;//电压输出值，0.1V
			TxMessage.Data[1] = ((INT32U)gUserDB.C_outVolt*10 >> 8) & 0xFF;			
			TxMessage.Data[2] = ((INT32U)gUserDB.C_outCurr*10 >> 0) & 0xFF;//电流输出值 0.1A  -800A ~ 500A
			TxMessage.Data[3] = ((INT32U)gUserDB.C_outCurr*10 >> 8) & 0xFF;
			TxMessage.Data[4] = (gUserDB.CCS_charge_time >> 0) & 0xFF;//累计充电时间 1min 0-6000min
			TxMessage.Data[5] = (gUserDB.CCS_charge_time >> 8) & 0xFF;


			break;						
		}
		case CCS_DEC_CST:			//充电机中止充电CST，充电阶段  
		{
			TxMessage.ExtId   = 0x101AF456;
			TxMessage.DLC 	  = 0x03;															
			TxMessage.Data[0] = 0x03;//(ModbusREGDat[25] >> 0) & 0xFF;//中止充电原因
			TxMessage.Data[1] = 0x00;//(ModbusREGDat[25] >> 8) & 0xFF;//中止充电故障原因			
			TxMessage.Data[2] = 0x00;//(ModbusREGDat[26] >> 0) & 0xFF;//中止充电错误原因
			break;						
		}
		case CCS_DEC_CSD:  				//充电机统计数据报文CSD，结束阶段 
		{
			TxMessage.ExtId   = 0x181DF456;
			TxMessage.DLC 	  = 0x05;																	
			TxMessage.Data[0] = (gUserDB.CCS_charge_time >> 0) & 0xFF;//累计充电时间 1min
			TxMessage.Data[1] = (gUserDB.CCS_charge_time >> 8) & 0xFF;	
			TxMessage.Data[2] = (gUserDB.CCS_charge_power/10 >> 0) & 0xFF;//输出能量	0.1kWh 0~500kWh	
			TxMessage.Data[3] = (gUserDB.CCS_charge_power >> 8) & 0xFF;
			TxMessage.Data[4] = (gUserDB.CCS_adr >> 0) & 0xFF;//充电机编号
			break;						
		}
		case CCS_DEC_CEM:  				//充电机错误报文CEM  
		{
			TxMessage.ExtId   = 0x101FF456;
			TxMessage.DLC 	  = 0x03;																			
			TxMessage.Data[0] = 0;
			TxMessage.Data[1] = 0;			
			TxMessage.Data[2] = 0;
			break;						
		}
		case CCS_DEC_CFM:  		//充电机故障报文CFM  
		{
			TxMessage.ExtId   = 0x1434F456;
			TxMessage.DLC 	  = 0x02;																						
			TxMessage.Data[0] = 0;
			TxMessage.Data[1] = 0;			
			break;						
		}
		case CCS_DOUBLE_STATUS:   //充电机 双枪充电状态
		{
		    TxMessage.ExtId   = 0x10F856F4;
			TxMessage.DLC 	  = 0x08;										
						
			if(	gUserDB.doubleGun_ID==1)       //主充电枪
			{
				TxMessage.Data[0] = 0x10+gUserDB.bms_readyStatus;
				TxMessage.Data[1]=gtBMS_Dat.BCS_7;             //SOC
				TxMessage.Data[2]=(INT32U)gtBMS_Dat.BCL_1;	   //需求电压
				TxMessage.Data[3]=(INT32U)gtBMS_Dat.BCL_1>>8;
				TxMessage.Data[4]=(INT32U)gtBMS_Dat.BCL_3;
				TxMessage.Data[5]=(INT32U)gtBMS_Dat.BCL_3>>8;  //需求电流
				TxMessage.Data[6]=(INT32U)gUserDB.doubleGun_disCurrent;	  //分流电流
				TxMessage.Data[7]=(INT32U)gUserDB.doubleGun_disCurrent>>8;
			}
			else if( gUserDB.doubleGun_ID==2) //从充电枪
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
		case CCS_DOUBLE_OUT:	 //双枪充电输出
		{
			TxMessage.ExtId   = 0x10F956F4;
			TxMessage.DLC 	  = 0x08;
																									
			TxMessage.Data[0] = (INT32U)gUserDB.BMS_Volt &0xff;		   //bms检测电压
			TxMessage.Data[1]=(INT32U)gUserDB.BMS_Volt>>8;             
			TxMessage.Data[2]=(INT32U)gUserDB.BMS_Curr &0xff;	       //bms检测电流
			TxMessage.Data[3]=(INT32U)gUserDB.BMS_Curr>>8;
			TxMessage.Data[4]=(INT32U)gUserDB.C_outVolt &0xff;
			TxMessage.Data[5]=(INT32U)gUserDB.C_outVolt>>8;             //需求电流
			TxMessage.Data[6]=(INT32U)gUserDB.C_outCurr &0xff;	        //分流电流
			TxMessage.Data[7]=(INT32U)gUserDB.C_outCurr >>8;

			gUserDB.doubleGunPlugStatus= OK;

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




/*********************************************************************************************************
函数名称: 	CAN_ClearBuff 
功能描述:  清空缓冲

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void CAN_ClearBuff()
{
	CanMessageBuff.OutLen = 0x0000;		//读取缓冲区置零
	CanMessageBuff.InLen  = 0x0000;		//接收缓冲区置零
}
/*********************************************************************************************************
函数名称: 	can_read 
功能描述:  从指定CAN口读取数据,读到相应的数据后函数返回，注：在CAN接收中断中只接收扩展帧

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    len     接收的数据包长度
									     
*********************************************************************************************************/
INT8U can_read(CANRECMSG* pRxMessage) 
{      
	INT8U len=0;
	INT8U  lBuffAddr;

	len=CAN_ReadBuffNum();
	if(len>0)
	{				   										
		lBuffAddr = (CanMessageBuff.OutLen );	//得到数据在缓冲区的位置
		*pRxMessage = CanMessageBuff.Buff[lBuffAddr]; 		

		CanMessageBuff.OutLen++;
		if(CanMessageBuff.OutLen ==CanMessageBuff.InLen && CanMessageBuff.OutLen !=0)
		{	
			CAN_ClearBuff();   //清零
		}		
	} 	 

 return  len;
}


/*********************************************************************************************************
函数名称: 	can_write 
功能描述:  CAN发送

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
INT8U can_write(CanTxMsg * pTxMessage) 
{	 
	INT16U tmp16=0;
	INT8U ret=0;

	TransmitMailbox=CAN_Transmit(CAN1,pTxMessage);									//发送消息
																					//等待发送完毕
	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (tmp16 < 500))   //500ms内，判断CAN是否发送成功，否则超时退出循环
	{
			tmp16++;
			if(	tmp16>20)
				OSTimeDlyHMSM(0,0,0,1);				//休眠1MS
	}
	if(tmp16>=500)		 
	 	ret=0;				//发送超时
	else
	 	ret=1;  			//发送成功

	return ret ;
}


/*********************************************************************************************************
函数名称: 	Scu_Send 
功能描述:  SCU发送数据

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
INT8U Scu_Send(INT8U id) 
{
   
 return 0;
}


/*********************************************************************************************************
函数名称: 	Scu_Read_callback 
功能描述:  SCU接收数据

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
INT8U Scu_Read_callback(void ) 
{    

   
  return 1;
}
/*********************************************************************************************************
函数名称: 	task_canProcess 
功能描述:  CAN通信线程，普天协议

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
void task_canProcess(void *pdata)
{	
    INT8U timeCnt=0;

	INT16U RunTime = 0x0000;			//运行时间计时，单位10ms,
	INT16U OutTime = 0x0000;			//超时时间计时，单位10ms
	INT16U tmp16;
//	INT16U *ptrCTRLRegister;			//整个流程的控制寄存器，其实就是ModbusREGDat[2]，为了方便修改就重新定义
	pdata = pdata;
	CAN_Initial(CAN_REC_BMS);  			//仅能接收BMS数据，因为开始就进行连接
	ParemeterInitial(0x01);
	//ptrCTRLRegister = &ModbusREGDat[1];

     while(1)
{
	OSTimeDlyHMSM(0,0,0,10);
//	gCommStation = CCS_DEC_BCL;			//调试使用
	RunTime ++;			  				//运行时间，只用于循环，不用于其他功能
	OutTime ++;			  				//仅用于超时检测，不能和运行时间混合在一起
										//当允许连接且还没有进行连接的情况下开始连接
	ModbusREGDat[0] = gCommStation; 	//上报协议转换器状态
//	ModbusREGDat[0] = ( ((1-CAN_ChargeFlag.OutTime) << 0)	//在上位机看来，1-不超时，0表示超时
//					   |(CAN_ChargeFlag.NoCharge<< 1)
//					   |(CAN_ChargeFlag.Charging << 2)
//					   |(CAN_ChargeFlag.Stop<< 3));						
//	if(GetBitStatus(*ptrCTRLRegister,1))					//有启动连接指令
//	{ 
//	  	ClrBitStatus(ptrCTRLRegister,1);					//将第bit1位清零 ，指针类型
//		if((gCommStation == CCS_DEC_INVALID)  	   			//处于不连接阶段或者是结束阶段都可以重新初始化
//		   ||(gCommStation == CCS_DEC_OVER)) 				//保证可以在手动接收后可以重新开始充电
//		{													//其实就是在上个车已经充满，下个车开始的时候才有用			
//			ClrBitStatus(ptrCTRLRegister,0);				//清除上位机控制停机标志
//			ParemeterInitial(0x01);							//开始连接
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

	if(gUserDB.doubleGun_timeout<3000)	     //20秒超时
	{
		gUserDB.doubleGun_timeout++;
	}
	else //双枪检测超时
	{
		gUserDB.doubleGunPlugStatus=NOK;
		gUserDB.doubleGun_ID=0;
		gUserDB.mainGunBms_ready=NOK;
		gUserDB.doubleGun_disCurrent=0;
	}


//-----------------------------------------------------------
//****************以下为充电的各个阶段*********************//
	
	if(gCommStation == CCS_DEC_BCL) 	//充电中判断 不再充电阶段就清零充电中标志
	{  	
		CAN_ChargeFlag.Charging = 1;
		gUserDB.bms_readyStatus=OK;
	}
	else 
	{	
		CAN_ChargeFlag.Charging = 0;
		
	    rec_secondGun_can();						    //接收双枪CAN数据

		if(	(gUserDB.doubleGunPlugStatus ==OK && gUserDB.doubleGun_ID==2))  //从充电枪时
		{
		   continue;
		}
		else
		{
		  gUserDB.bms_readyStatus=NOK; 
		}
	}
	
	
	if(MODBUS_DEVICEID ==2 && (gUserDB.sysCtlMode ==2 || gUserDB.sysCtlMode ==0 ))  //并机或单枪模式时,从充电枪时
	{	  
		 if(CAN_ReadBuffNum())
	   {	
			  CanMessageBuff.OutLen ++;
	   }
		  continue;
	}

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

			

			CAN_TP_CM(1) ;
			if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x30))	//VBI
			{
			   DecodeBMSMessage(CanPackage.Buff, BMS_DEC_VBI);
			   gCommStation = CCS_DEC_BRM;
			   OutTime = 0x00;
			} 
			
			 

			break ;
		}

		case CCS_DEC_BRM:					   									//已经接收到VBI，等待接受BRM
		{	
			if(OutTime>COMM_OUTTIME) 
			{
				OutTime=0;
				ParemeterInitial(0x01);					//5S超时错误
			}
			
			while(CAN_ReadBuffNum())
			{
				
			  if(CAN_TP_CM(1)) 
				{	 
          CanMessageBuff.OutLen ++;					
					//OutTime = 0x00;									//多数据接受处理
					if((CanPackage.RxFlag == 0x02) &&(CanPackage.PGN == 0x02))			//BRM
					{
						DecodeCCSMessage(CCS_DEC_CRM_1);									//当接收到完整的握手数据后立即发送可以识别命令
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
				DecodeCCSMessage(CCS_DEC_CRM_0);			//因为不能一次握手成功，怀疑是接收成功后不能立即回复信息 
			break ;													 			//所以将此行放在本阶段的末尾，接收成功后将不发送不能识别数据
		}

		case CCS_DEC_BCP:					  									//已经接收到BRM，等待接受电池充电参数
		{																		//250ms发送一次充电机能辨识报文
			if((RunTime%25) == 0x01) 
				DecodeCCSMessage(CCS_DEC_CRM_1);	
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);					//5S超时错误
				OutTime = 0x00;
			}
			
			while(CAN_ReadBuffNum())
			{
				//OutTime = 0x00;	
				if(CAN_TP_CM(1)) 
				{
					CanMessageBuff.OutLen ++;
				  OutTime = 0x00;	   								//多数据处理函数
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
		case CCS_DEC_BRO:					  									//等待BMS充电参数准备报文
		{
			if((RunTime%50) == 0x01) 
				DecodeCCSMessage(CCS_DEC_CTS);				//500ms时间同步和最大输出能力报文
			if((RunTime%25) == 0x02) 
				DecodeCCSMessage(CCS_DEC_CML);				//250ms
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);					//5S超时错误
				OutTime = 0x00;	
			}
			while(CAN_ReadBuffNum())
			{				
				Can_DeleteMessageEXT(1,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//清除无用的数据帧						
				if(CAN_ReadBuffNum())
				{	
					tmp16 = CanMessageBuff.OutLen ; 		//获取缓冲区的地址
				  if((CanMessageBuff.Buff[tmp16].ExtID & 0XFFFFFF) == 0x0956F4 )	 		//确认收到BRO报文 国标优先级为4 0x100956F4 ， 普天为5	 0x140956F4

				//	if(CanMessageBuff.Buff[tmp16].ExtID == 0x140956F4 )	 		//确认收到BRO报文
					{	
						if(CanMessageBuff.Buff[tmp16].Data[0] == 0xaa)
							{ 		//报文内容为完成充电准备，否则超时退出						
							DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BRO);
				   			gCommStation = CCS_DEC_BCS;
				   			OutTime = 0x00;
						}
						else 
						{										//两种情况，1-BMS已满，2-配置错误
							 ParemeterInitial(0x01);		 	//初始化为不需要连接
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
//			while(CAN_ReadBuffNum()){
//				Can_DeleteMessageEXT(1,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
//				if(CAN_ReadBuffNum()) {
//					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); //获取缓冲区的地址			
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
				  OutTime = 0x00;	   								//多数据处理函数
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
		case CCS_DEC_BCL:						  			//充电阶段报文
		{													//20ms
		    if( gUserDB.doubleGunPlugStatus == OK)
			{				
				gUserDB.mainGunBms_ready =OK;
				gUserDB.doubleGun_ID=1 ;			  //主充电枪ID

				if((RunTime%50) == 0x01)	{DecodeCCSMessage(CCS_DOUBLE_STATUS);  }
				if((RunTime%80) == 0x01)	{DecodeCCSMessage(CCS_DOUBLE_OUT);  }
			}

			if((RunTime%2) == 0x01)	{DecodeCCSMessage(CCS_DEC_CCS);  }
			if(OutTime>COMM_OUTTIME*10) ParemeterInitial(0x01);//5S超时错误
//			if(GetBitStatus(*ptrCTRLRegister,0)) 			//上位机发出充电结束指令
//			{												//或者是上位机要求停止
//				ClrBitStatus(ptrCTRLRegister,0);			//将第0位清零 ，指针类型
//				gCommStation = CCS_DEC_BST;
//				CAN_ChargeFlag.Stop = 1;					//设置上位机要求停止标志
//				OutTime = 0x00;
//			}							
			
			while(CAN_ReadBuffNum())
			{
			//	ModbusREGDat[2] = (CAN_ChargeFlag.Charging ) ;			 //充电状态
	    // ModbusREGDat[3] = (CAN_ChargeFlag.Charging ) ;			 //充电状态
				//Can_DeleteMessageEXT(5,0x10,0x11,0x19,0x14,0x12,0x00,0x00,0x00);	
//				Can_DeleteMessageEXT(5,0xec,0x10,0x19,0x14,0x12,0xf8,0xf9,0x00);
//				if(CAN_ReadBuffNum())
//					{											//执行删除处理后可能已经没有可用的数据，所以要做判断
//					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//获取缓冲区的地址
//					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181056F4){
//						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
//			   			OutTime = 0x00;											//电池充电需求
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
					{											//执行删除处理后可能已经没有可用的数据，所以要做判断
					  tmp16 = CanMessageBuff.OutLen ; 		//获取缓冲区的地址
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x181056F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCL);
			   			OutTime = 0x00;											//电池充电需求
					}
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1156F4){
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x181156F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BCS);
			   			OutTime = 0x00;											//电池充电总状态
					}

					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1356F4){
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x181156F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSM);
			   			OutTime = 0x00;											//电池充电总状态
					}
					
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x10f856F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_STATUS);
			   			//OutTime = 0x00;											//双枪充电状态
					}
					if(CanMessageBuff.Buff[tmp16].ExtID == 0x10f956F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_OUT);
			   			//OutTime = 0x00;											//双枪输出
					}
					
					//if(CanMessageBuff.Buff[tmp16].ExtID == 0x141956F4){
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF) == 0x1956F4 )
						{
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BST);
			   			
						if((CanMessageBuff.Buff[tmp16].Data[0] > 0) 			//如果充满就设置充满标志
						   ||(CanMessageBuff.Buff[tmp16].Data[1] > 0)
						   ||(CanMessageBuff.Buff[tmp16].Data[2] > 0))
						{	
							CAN_ChargeFlag.FullCharge = 1;	
							gCommStation = CCS_DEC_BST;							//根据数据的状态来判断是否要进行下一步
							gUserDB.autoRunCtlFlag =STOP; 	   //自动开关
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
				ParemeterInitial(0x01);						//5S超时错误
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

		case CCS_DEC_BSD:						  			//充电结束阶段报文,BMS统计数据
		{										  			//500ms时间同步和最大输出能力报文
			if((RunTime%25) == 0x01)	{ DecodeCCSMessage(CCS_DEC_CST);  }
			if(OutTime>COMM_OUTTIME) 
			{
				ParemeterInitial(0x01);						//5S超时错误
			   	OutTime = 0x00;						
			}						
			while(CAN_ReadBuffNum()){
				Can_DeleteMessageEXT(1,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00);						
				if(CAN_ReadBuffNum()){	
					tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1);
					if((CanMessageBuff.Buff[tmp16].ExtID &0XFFFFFF)== 0x1C56F4){
						DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DEC_BSD);
			   			gCommStation = CCS_DEC_OVER;
						CAN_Initial(CAN_REC_BMS);							//允许接收所有的数据
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
					ParemeterInitial(0x01);			//5S超时错误，说明插头已经拔掉了，为下次连接做准备
					//CAN_Initial(CAN_REC_ALL);		//
//				}									//下面进入INVALID状态
			}		
			break;
		}
		default:break;
	}		

}

}


//CANPACKAGE	CanPackage;



/*********************************************************************************************************************
函数名称: 	secondGun_task          
功能描述: 	设置充电机电压电流
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/
void rec_secondGun_can()
{
   INT8U  tmp16;
										
	if(CAN_ReadBuffNum())
	{	
		tmp16 = CanMessageBuff.OutLen & (CANMESSAGEBUFFLEN-1); 		//获取缓冲区的地址
		if(CanMessageBuff.Buff[tmp16].ExtID == 0x10F856F4 )	 		//确认收到BRO报文
		{								
			DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_STATUS);		
			CanMessageBuff.OutLen ++;	  				//删除已经处理过的报文	
		}
		else if(CanMessageBuff.Buff[tmp16].ExtID == 0x10F956F4 )	 		//确认收到BRO报文
		{
			DecodeBMSMessage(CanMessageBuff.Buff[tmp16].Data, BMS_DOUBLE_OUT);
			CanMessageBuff.OutLen ++;	  				//删除已经处理过的报文
		}
		else if( gUserDB.doubleGunPlugStatus ==OK && gUserDB.doubleGun_ID==2)  //从充电枪时,清除其它无用数据
		{
		   CanMessageBuff.OutLen ++;
		}
		//CanMessageBuff.OutLen ++;	  				//删除已经处理过的报文						
	}		 
}


