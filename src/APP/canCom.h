#ifndef __CAN_H
#define __CAN_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "stm32f10x.h"
#include "define.h"
#include "can_cfg.h"
/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
/*====================================供外部调用结构体和变量====================================*/
typedef struct {
	INT32U 	BMR_1 ;// pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;		//通讯协议版本号
	INT8U 	BMR_4 ;//  pu8Mesg[3];					//电池类型
	FP32 	BMR_5 ;//  (FP32)(pu8Mesg[4] + pu8Mesg[5]<<8)/10;			//整车电池额定容量
	FP32 	BMR_7 ;//  (FP32)(pu8Mesg[6] + pu8Mesg[7]<<8)/10;			//整车额定总电压
	INT8U 	BMR_9[4] ;//  pu8Mesg[8];				//电池厂商名字 4	
	INT8U 	BMR_13[4] ;//  pu8Mesg[12];				//电池组序号 4	
	INT16U 	BMR_17 ;//  pu8Mesg[16]+1985;			//电池组生产日期年
	INT8U 	BMR_18 ;//  pu8Mesg[17];				//电池组生产日期  月
	INT8U 	BMR_19 ;//  pu8Mesg[18];				//日
	INT32U 	BMR_20 ;// pu8Mesg[19] + pu8Mesg[20]<<8 + pu8Mesg[21]<<16;//充电次数 
	INT8U 	BMR_23 ;//  pu8Mesg[22];				//产权标识
	INT8U 	BMR_24 ;//  pu8Mesg[23];				//预留
	INT8U 	BMR_25[25] ;//  pu8Mesg[24+u8AdrCnt];		//车辆识别信息 25byte			
	INT32U 	VBI_1 ;//   pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;	//通讯协议版本号;
	INT8U 	VBI_4[17] ;//  pu8Mesg[3+u8AdrCnt];				
	INT8U 	VBI_21[4] ;//  pu8Mesg[20+u8AdrCnt];				
	INT8U 	VBI_25[8] ;//  pu8Mesg[24+u8AdrCnt];						
	INT8U 	VBI_33[8] ;//  pu8Mesg[32+u8AdrCnt];				
	FP32 	BCP_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/100;			//蓄电池模块最高允许充电电压			
	FP32 	BCP_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10;			//最高允许充电电流 
	INT16U 	BCP_5 ;// pu8Mesg[4]*10;									//最大允许充电容量 
	FP32 	BCP_6 ;// (FP32)(pu8Mesg[5] + pu8Mesg[6]<<8)/10;			//最高允许充电总电压 
	INT16S	BCP_8 ;// pu8Mesg[7]-50;									//最高允许温度 
	INT16U	BCP_9 ;// (pu8Mesg[8] + pu8Mesg[9]<<8);						//整车蓄电池估计剩余容量 
	FP32 	BCP_11 ;// (FP32)(pu8Mesg[10] + pu8Mesg[11]<<8)/10;			//整车蓄电池总电压 
	INT8U 	BCP_13 ;// pu8Mesg[12];			
	INT8U 	BRO_1 ;// pu8Mesg[0];//	
	FP32 	BCL_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//电压需求
	FP32 	BCL_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10-800;		//电流需求
	INT8U 	BCL_5 ;// pu8Mesg[4]; 	
	FP32 	BCS_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//充电电压测量值（V） 
	FP32 	BCS_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10 - 800;		//充电电流测量值（A）
	INT16S	BCS_5 ;// pu8Mesg[4]-50;	//电池组最低温度（oC）  
	INT16S	BCS_6 ;// pu8Mesg[5]-50;	//电池组最高温度（oC）  
	INT8U 	BCS_7 ;// pu8Mesg[6];		//当前荷电状态  SOC（%）  
	INT8U 	BCS_8 ;// pu8Mesg[7];		//估算剩余时间（min）
	INT16U	BSM_1 ;// (pu8Mesg[0] + pu8Mesg[1]<<8);
	INT8U 	BSM_2 ;
	INT8U 	BSM_3 ;// pu8Mesg[2];
	INT8U 	BSM_4 ;// pu8Mesg[3];
	INT8U 	BSM_5 ;// pu8Mesg[4];
	INT8U 	BSM_6 ;// pu8Mesg[5];					//和下面的同时解码，方便其他程序调用
	INT8U 	BSM_7 ;// pu8Mesg[6];	
	INT8U 	BSM_6_1;// (pu8Mesg[5] >> 0) &0x01;
	INT8U 	BSM_6_2 ;// (pu8Mesg[5] >> 1) &0x01;
	INT8U 	BSM_6_3 ;// (pu8Mesg[5] >> 2) &0x01;
	INT8U 	BSM_6_4 ;// (pu8Mesg[5] >> 3) &0x01;
	INT8U 	BSM_6_5 ;// (pu8Mesg[5] >> 4) &0x01;
	INT8U 	BSM_6_6 ;// (pu8Mesg[5] >> 5) &0x01;
	INT8U 	BSM_6_7 ;// (pu8Mesg[5] >> 6) &0x01;
	INT8U 	BSM_6_8 ;// (pu8Mesg[5] >> 7) &0x01;	
	INT8U 	BSM_7_1 ;// (pu8Mesg[6] >> 0) &0x01;
	INT8U 	BSM_7_2 ;// (pu8Mesg[6] >> 1) &0x01;	
	INT8U 	BST_1 ;// pu8Mesg[0];
	INT8U 	BST_2 ;// pu8Mesg[1];
	INT8U 	BST_3 ;// pu8Mesg[2];	
	INT8U 	BSD_1 ;// pu8Mesg[0];										//中止荷电状态  SOC（%）  
	FP32 	BSD_2 ;// (FP32)(pu8Mesg[1] + pu8Mesg[2]<<8)/1000;			//蓄电池单体最低电压（V
	FP32 	BSD_4 ;// (FP32)(pu8Mesg[3] + pu8Mesg[4]<<8)/1000;			//蓄电池单体最高电压（V
	INT16S 	BSD_6 ;// pu8Mesg[5]-50;									//蓄电池组最低温度（oC） 
	INT16S	BSD_7 ;// pu8Mesg[6]-50;									//蓄电池组最高温度（oC）
	
	
	INT8U	CRM_1;
	INT8U	CRM_2;
	INT8U	CRM_3;
	INT8U	CRM_4;
	INT32U	CRM_5;
	INT8U	CTS_1;
	INT8U	CTS_2;
	INT8U	CTS_3;
	INT8U	CTS_4;
	INT8U	CTS_5;
	INT16U	CTS_6;
	FP32	CCS_1;
	FP32 	CCS_2;
	INT16U	CCS_3;
	INT8U	CST_1;//中止充电原因
	INT8U	CST_2;//中止充电故障原因			
	INT8U	CST_3;//中止充电错误原因	
	INT16U	CSD_1;			//累计充电时间 1min
	FP32 	CSD_2;//输出能量	0.1kWh 0~500kWh	
	INT8U	CSD_3;			//
	
} _T_BMS_DAT;


typedef struct {
	FP32 ActivePower;					//输出有功功率kW
	FP32 AH;							//本次充电累计AH
	FP32 kWh;							//本次充电累计kWh
	FP32 ReactivePower;					//输出无功功率kvar
	FP32 ACAllPower	;					//交流输入总功率
	INT8U Wave_A_Vol;					//A相电压谐波
	INT8U Wave_B_Vol;					//B相电压谐波
	INT8U Wave_C_Vol;					//C相电压谐波
	INT8U Wave_A_Cur;					//A相电流谐波
	INT8U Wave_B_Cur;					//B相电流谐波
	INT8U Wave_C_Cur;					//C相电流谐波		
}_T_METER_DAT;	 


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
	INT8U 	Switch_on;						//开关机信号，0-停止，1-启动
	INT8U 	Switch_off;						//开关机信号，0-停止，1-启动	
	INT8U 	ChargeNum;					//充电阶段数；
	INT8U 	ChargerID;					//充电机编号
	INT8U 	NowStage;					//当前工作的充电阶段
	FP32 	Power;						//输出功率	
	INT8U	BMSStage;					//和bms通讯处于哪个阶段
	_T_MCT_SINGLE 	MCT_Single;			//单体控制参数	
	_T_MCT_STAGE  	MCT_Stage[6];		//各个阶段的值
	
}_T_MCT_DAT;  	                        //充电终端结构体


typedef struct 
{
	INT8U OutTime;						//通讯超时,0-不超时，1表示超时
	INT8U NoCharge;						//BMS不需要充电，1-不需要充电
	INT8U FullCharge;					//BMS已充满 ，1-已经充满
	INT8U Charging;						//充电中标志位，1-充电中
	INT8U Stop;							//表示上位机发送的停止，1-是因为上位机要求才停止充电的
}_CAN_CHARGE_FLAG;                      //充电机过程结构体


/*====================================供内部调用结构体和变量====================================*/
/*
typedef struct	  						//接收到的扩展帧的数据类型
{
  INT32U ExtID;
  INT8U  DLC;
  INT8U  Data[8];
} CANRECMSG;


typedef struct	  						//接收到的扩展帧的数据类型
{
  INT16U OutLen; 						//读取的数据缓冲区位置，采用16位的数据基本满足要求
  INT16U InLen;							//进入缓冲区的位置
  CANRECMSG  Buff[16];					//对应的消息
} CANRECMSGBUFF;
*/
typedef struct	  						//接收到的扩展帧的数据类型
{
  INT8U  PGN;							//参数群编号，只取中间的字节
  INT8U  RxFlag;						//接收标志 =1 正在接收 =2 接收完成
  INT16U DataLen; 						//读取的数据缓冲区位置，采用16位的数据基本满足要求
  INT8U  FrameNum;						//数据包的数目
  INT8U	 FramePointer;					//已经接收的数据帧的位置
  INT8U  Buff[200];						//对应的消息，其实100个已经足够了
} CANPACKAGE;							//CAN多消息传输的临时寄存器

typedef struct	  						//发送的扩展帧的数据类型
{
  INT8U  PGN;							//参数群编号，只取中间的字节
  INT8U  SendFlag;						//发送标志 =1 正在发送 =2 发送完成
  INT16U DataLen; 						//发送的数据缓冲区位置，采用16位的数据基本满足要求
  INT8U  FrameNum;						//数据包的数目
  INT8U	 FramePointer;					//已经发送的数据帧的位置，从1开始
  INT8U  Buff[100];						//对应的消息，其实100个已经足够了
} CANSENDPACKAGE;


#define CANMESSAGEBUFFLEN	16			//定义接收缓冲区的长度

#define CAN_REC_BMS	0x00
#define CAN_REC_ALL 0x01
//extern void 		 CAN_Initial(INT8U u8Type);
extern INT16U 		 ModbusREGDat[300];

extern _T_BMS_DAT gtBMS_Dat;
extern _T_METER_DAT gtMeter_Dat;
extern _T_MCT_DAT gtMCT_Dat; 
extern _CAN_CHARGE_FLAG CAN_ChargeFlag;						//充电过程中的标志
										//默认发送的无效数据
//#define INVALID_CANDATA   0xFF 

//BMS报文解码不同处理阶段值PGN
#define	BMS_DEC_BRM				0x02	//BMS和车辆辨识报文BRM，250ms
#define	BMS_DEC_VBI				0x30	//车辆和电池标识报文VBI，250ms
#define	BMS_DEC_BCP				0x06	//蓄电池充电参数BCP，500ms，实际是13个字节
#define	BMS_DEC_BRO				0x09	//电池充电准备就绪状态BRO，250ms
#define	BMS_DEC_BCL				0x10	//电池充电需求报文BCL，20ms
#define	BMS_DEC_BCS				0x11	//电池充电总状态报文BCS，250ms
#define	BMS_DEC_BSM    			0x13	//蓄电池状态信息BSM，250ms
#define	BMS_DEC_BMV     		0x15	//蓄电池最小测量单元电压BMV，1000ms
#define	BMS_DEC_BMT     		0x16	//蓄电池组温度BMT，1000ms
#define	BMS_DEC_BSOC      		0x17	//蓄电池组荷电容量SOC ,BSOC，1000ms
#define	BMS_DEC_BST       		0x19	//BMS中止充电报文BST，10ms
#define	BMS_DEC_BSD      		0x1C	//BMS统计数据BSD，250ms
#define	BMS_DEC_BEM				0x1E	//BMS错误报文BEM，250ms
#define	BMS_DEC_BFM				0x33	//BMS故障报文BFM，250ms

#define BMS_DOUBLE_STATUS			0XF8    //
#define BMS_DOUBLE_OUT              0XF9	//

#define CCS_DOUBLE_STATUS			0XF8    //
#define CCS_DOUBLE_OUT              0XF9	//

//CCS报文解码不同处理阶段值
#define	CCS_DEC_CRM_0			0x01	//充电机辨识报文CRM，250ms
#define	CCS_DEC_CRM_1			0x02	//充电机辨识报文CRM，250ms
#define	CCS_DEC_CTS				0x03	//充电机发送的时间同步信息CTS，500ms
#define	CCS_DEC_CML				0x04	//充电机最大输出能力CML，250ms
#define	CCS_DEC_CRO				0x05	//充电机输出准备就绪状态CRO，250ms
#define	CCS_DEC_CCS				0x06	//充电机充电状态CCS,20ms
#define	CCS_DEC_CST				0x07	//充电机中止充电CST，10ms
#define	CCS_DEC_CSD				0x08	//充电机统计数据报文CSD，250ms
#define	CCS_DEC_CEM				0x09	//充电机错误报文CEM，250ms
#define	CCS_DEC_CFM				0x0A	//充电机故障报文CFM，250ms

//CCS报文解码不同处理阶段值所对应的起始地址和数据长度
#define	M_CRM_START_ADR		10	//start Adr 对于上位机就是11
#define	M_CRM_ADR_LEN		8
#define	M_CTS_START_ADR		(M_CRM_START_ADR + (M_CRM_ADR_LEN+1)/2 + 0)	//10+4
#define	M_CTS_ADR_LEN		7
#define	M_CML_START_ADR		(M_CTS_START_ADR + (M_CTS_ADR_LEN+1)/2 + 0)	//14+4
#define	M_CML_ADR_LEN		8
#define	M_CRO_START_ADR		(M_CML_START_ADR + (M_CML_ADR_LEN+1)/2 + 0)	//18+4
#define	M_CRO_ADR_LEN		1
#define	M_CCS_START_ADR		(M_CRO_START_ADR + (M_CRO_ADR_LEN+1)/2 + 0)	//22+1
#define	M_CCS_ADR_LEN		6
#define	M_CST_START_ADR		(M_CCS_START_ADR + (M_CCS_ADR_LEN+1)/2 + 0)	//23+3
#define	M_CST_ADR_LEN		3
#define	M_CSD_START_ADR		(M_CST_START_ADR + (M_CST_ADR_LEN+1)/2 + 0)	//start Adr
#define	M_CSD_ADR_LEN		7
#define	M_CEM_START_ADR		(M_CSD_START_ADR + (M_CSD_ADR_LEN+1)/2 + 0)	//多字节传送
#define	M_CEM_ADR_LEN		9
#define	M_CFM_START_ADR		(M_CEM_START_ADR + (M_CEM_ADR_LEN+1)/2 + 0)	//start Adr
#define	M_CFM_ADR_LEN		2



//=========BMS报文解码不同处理阶段值所对应的起始地址和数据长度==========//
#define	M_BRM_START_ADR		35	//对于上位机就是11
#define	M_BRM_ADR_LEN		41
#define	M_VBI_START_ADR		(M_BRM_START_ADR + (M_BRM_ADR_LEN+1)/2 + 0)	//35+21
#define	M_VBI_ADR_LEN		40
#define	M_BCP_START_ADR		(M_VBI_START_ADR + (M_VBI_ADR_LEN+1)/2 + 0)	//56+20
#define	M_BCP_ADR_LEN		13
#define	M_BRO_START_ADR		(M_BCP_START_ADR + (M_BCP_ADR_LEN+1)/2 + 0)	//76+7
#define	M_BRO_ADR_LEN		1
#define	M_BCL_START_ADR		(M_BRO_START_ADR + (M_BRO_ADR_LEN+1)/2 + 0)	//83+1
#define	M_BCL_ADR_LEN		5
#define	M_BCS_START_ADR		(M_BCL_START_ADR + (M_BCL_ADR_LEN+1)/2 + 0)	//84+3
#define	M_BCS_ADR_LEN		8
#define	M_BST_START_ADR		(M_BCS_START_ADR + (M_BCS_ADR_LEN+1)/2 + 0)	//87+4
#define	M_BST_ADR_LEN		3
#define	M_BSD_START_ADR		(M_BST_START_ADR + (M_BST_ADR_LEN+1)/2 + 0)	//91+2
#define	M_BSD_ADR_LEN		7
#define	M_BEM_START_ADR		(M_BSD_START_ADR + (M_BSD_ADR_LEN+1)/2 + 0)	//93+4
#define	M_BEM_ADR_LEN		4
#define	M_BFM_START_ADR		(M_BEM_START_ADR + (M_BEM_ADR_LEN+1)/2 + 0)	//97+2
#define	M_BFM_ADR_LEN		7	//99+4=103,即modbusreg[103]
//=====将该部分屏蔽掉======//
#define	M_BSM_START_ADR		103
#define	M_BSM_ADR_LEN		7	//7
//#define	M_BMV_START_ADR		X
//#define	M_BMV_ADR_LEN		X	//0xFF
//#define	M_BMT_START_ADR		X
//#define	M_BMT_ADR_LEN		X	//0xFF
//#define	M_BSOC_START_ADR	X
//#define	M_BSOC_ADR_LEN		X	//0xFF

//=========多数据传输协议的PGN的值===========//
/*
#define	M_PGN_BRM		0x02  //只取中间2个字节，其它默认0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17
*/

#define	CCS_DEC_VBI		0x01
#define	CCS_DEC_BRM		0x02
#define	CCS_DEC_BCP		0x03
#define	CCS_DEC_BRO		0x04
#define	CCS_DEC_BCS		0x05
#define	CCS_DEC_BCL		0x06
#define	CCS_DEC_BST		0x07
#define	CCS_DEC_BSD		0x08
#define	CCS_DEC_OVER	0x09

#define	CCS_DEC_INVALID	0x00
#define	COMM_OUTTIME	500	//超时时间值，单位10ms


//充电终端到充电机
#define MCM_STOP_ALL		0x0440E500						//主站令所有充电桩/机停机
#define MCM_START_SINGLE	0x0841E500						//主站令某台充电桩/机充电
#define MCM_STOP_SINGLE		0x0442E500						//主站令某台充电桩/机停止充电
#define MCM_SET_SINGLE		0x0C43E500						//主站设置单体最大充电控制参数
#define MCM_SET_STAGE_1		0x0C44E500 						//主站设置阶段充电控制参数
#define MCM_SET_STAGE_2		0x0C45E500 
#define MCM_SET_STAGE_3		0x0C46E500 
#define MCM_SET_STAGE_4		0x0C47E500 
#define MCM_SET_STAGE_5		0x0C48E500 
#define MCM_SET_STAGE_6		0x0C49E500 
#define MCM_READSET_ALL		0x104AE500						//主站读取充电桩/机所有充电控制参数
#define MCM_READSET_SPECIAL_PARA 0x104BE500					//主站读取充电桩/机特定充电控制参数
#define MCM_SET_POWER		0x0C60E500						//主站设置充电桩/机输出功
//充电机到充电终端  
#define MCM_RETURNSET_STAGE_ALL_SINGLE	0x104C00E5			//从站返回全部阶段单体最大充电控制参数
#define MCM_RETURNSET_STAGE_ALL_1		0x104D00E5			//从站返回所有阶段充电控制参数
#define MCM_RETURNSET_STAGE_ALL_2		0x104E00E5
#define MCM_RETURNSET_STAGE_ALL_3		0x104F00E5
#define MCM_RETURNSET_STAGE_ALL_4		0x105000E5
#define MCM_RETURNSET_STAGE_ALL_5		0x105100E5
#define MCM_RETURNSET_STAGE_ALL_6		0x105200E5
#define MCM_RETURNSET_SINGLE			0x105300E5			//从站返回单体最大充电控制参数
#define MCM_RETURNSET_SPECIAL_STAGE_1	0x105400E5			//从站返回指定阶段充电控制参数（共6个阶段）
#define MCM_RETURNSET_SPECIAL_STAGE_2	0x105500E5
#define MCM_RETURNSET_SPECIAL_STAGE_3	0x105600E5
#define MCM_RETURNSET_SPECIAL_STAGE_4	0x105700E5
#define MCM_RETURNSET_SPECIAL_STAGE_5	0x105800E5
#define MCM_RETURNSET_SPECIAL_STAGE_6	0x105900E5
#define MCM_STATE0			0x185A00E5						//充电桩/机状态帧0
#define MCM_STATE1			0x185B00E5						//充电桩/机状态帧1
#define MCM_STATE2			0x185C00E5						//充电桩/机状态帧2
#define MCM_STATE3			0x185D00E5						//充电桩/机状态帧3
#define MCM_STATE4			0x185E00E5						//充电桩/机状态帧4

#endif






















