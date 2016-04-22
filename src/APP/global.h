#ifndef __GLOBAL_H
#define __GLOBAL_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
 #include "define.h"
 #include "includes.h"
#include "ICCARD.h"
/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define COM_PORT_NUM       5
#define COM_LEN_MAX        2048
#define COM1               0
#define COM2               1
#define COM3               2
#define COM4               3
#define COM5               4

#define MID_MODBUS_COM       COM1

#define  MODULE_SUM        10

#define DCDC_START_ADR      5

#define  COM_BUF_RN   1024   //接收数据的长度
#define  COM_BUF_SN   1024   //发送数据的长度

/*********************************************************************************************************************
*                                            定义包含
*********************************************************************************************************************/

typedef struct
{
  INT16U year;
  INT8U month;
  INT8U day;
  INT8U hour;
  INT8U min;
  INT8U sec;
   
}_TIME;

typedef struct
{ 
  INT16U user_indicating;               //用户指示命令  000待机 0001充电前刷卡请求  0002启动充电  003充电结束刷卡请求 004打印请求 
  INT8U inputOverVoltAlrm;		        //输入过压告警
  INT8U inputLowVoltAlrm;		        //输入欠压告警
  INT8U CCS_outOverVoltAlarm ;		    //输出过压告警
  INT8U CCS_outOverCurrAlarm;			//输出过流告警

  INT32U C_CCSFaultCode;               //充电机故障代码
  INT16U B_BmsFaultCode;               //BMS故障代码
  INT8U B_BatFaultCode[5];             //电池故障代码	
  
  INT8U m485comLinkStatus;              //主485通信状态
  INT16U m485CommAlarmCnt;               //错误计数

  INT8U DcPduComAlarm;                         //电表状态 0--正常  1--告警	
  INT8U insulationAlarm;                       //绝缘检测告警
  INT8U ILB_comAlarm;                          //绝缘检测板通信告警
  INT8U beepFlag;                              //锋鸣标志 0--停止锋鸣   1--锋鸣器响	 
  
  INT8U CCS_chargeEnable;					   //充电允许
  //INT8U CCS_DischargeEnable; 				   //放电允许
  
  INT16U activeILB_value;		             //正向绝缘电阻阻值
  INT16U inactiveILB_value; 		         //负向绝缘电阻阻值 
  INT16U ILB_alarm_value_set; 		         //绝缘电阻告警值   

  INT16U systemAlarmCode;                    //系统状态代码

  FP32   cellOverProtect;                    //单体过压保护
  FP32   cellBatVoltMax;					 //单体电压最大值

  FP32   cellLowProtect;                    //单体低压保护值
  FP32   cellBatVoltMin;					 //单体电压最大值

  FP32   BMS_needVolt;                  //bms需求电压
  FP32   BMS_needCurr;					 //BMS需求电流

  FP32   BMS_Volt;                  //bms检测电压
  FP32   BMS_Curr;					 //BMS检测电流

  FP32 manualSetCurr;  
  FP32 manualSetVolt;

  FP32 CCS_setOutVolt;
  FP32 CCS_setOutCurr;
  FP32 CCS_outCurrMax;
  FP32 CCS_outVoltMin;
  FP32 CCS_outVoltMax;
  FP32 moduleCurrMax;

  INT8U  moduleNormalTable[MODULE_SUM];      //模块启动控制列表
  INT8U startModuleSum;
  INT8U moduleCurrResetFlag;
  INT8U manualRunCtlFlag;
  INT8U autoRunCtlFlag;

  INT8U M_carLinkState;
  INT8U moduleNornalSum;


  INT8U BatAlarm;                        //电池告警	

  INT8U   BatTempMAx;							 //电池组温度
  INT8U   BatTempProtect;						 //电池组温度保护
  INT8U   cellSoc;							 //电池组荷电

  FP32 C_outVolt ;						    //
  FP32 C_outCurr ;							//

  INT8U CCS_chargeRunStatus;
  INT8U CCS_runCtlFlag;

  INT8U autoCtlFlag;
  INT8U bms_readyStatus;

  INT8U BMS_canCommAlarm;      //CAN通信状态
  INT8U BMS_canCommAlarmCnt;  //CAN通信故障计数

  INT8U ccu1_CommAlarm;      //ccu通信状态
  INT8U ccu1_CommAlarmCnt;  //ccu通信故障计数

  
  INT8U systemCtl;                //系统控制  0X00 待机  0X01 充电  0X02 放电 0X03 充放电
  INT8U CCS_adr;					   //充电机地址
  INT32U CCS_charge_time;			  //充电机充电时间
  INT32U CCS_charge_power;			  //充电机充电时间
  INT8U sumContactor_ctl;             //总控制继电器

  INT8U setCharge_flag;

  INT8U dev_adr;					  //设备地址

  INT8U doubleGunPlugStatus;           //双枪插入状态
  INT8U doubleGun_ID;                  //双枪ID号

  INT8U mainGunBms_ready;		      //双枪BMS通信准备状态
  FP32  doubleGun_disCurrent;		  //双枪分流电流

  INT16U doubleGun_timeout;           //双枪超时

  INT8U send_cur_cnt;                     //下发次数
  INT8U send_vol_cnt;
  INT8U	send_onff_cnt;

  INT8U M_pdu_ID[8];                     //电表ID
  FP32  M_degrees;                    //电表电度数
  INT32U M_pdu_failCode[4];             //电表故障代码

  INT8U ledCtl;
  INT8U sysCtlMode;		                
  INT8U Machine2RunFlag;	
	
	uint32	tickMs;
	uint8		crgStyle;
	_TIME delayStartTime;
	uint32  delayTime; 	
	uint32  passWord;
  uint32	getPayId;
	uint32  systemStartT;
	uint16  nowPrice;
  uint32  delayLeftTime;
  uint16  setPageId;
	
	INT32U leftMoney;
	INT8U userCard[16];
	INT16U crgStatus;
	INT8U cardReader_indicating;
	INT32U user_CMD_timeout;              //用户指示命令超时
	INT8U  userCardState;                       //用户卡状态
	INT8U  userCardState2;                      //用户卡状态2
  INT8U card_cmd_err;                   //0 正常卡   1异常卡
	INT8U  user_CMD_return_status;        //用户命令返回结果  00无效状态   01正常 02失败
	INT8U  user_CMD_ing;                  //用户命令执行状态,防止执行过程中，上位机重新下发,导致再次重复操作
	
	FP32   M_thisPower;                     //本次输出功率
    INT16U M_thisChargeAH;                  //本次充电AH
    FP32   M_thisChargeDegree;              //本次充电充电电量
    INT32U M_thisChargeTime;                //本次充电时间   单位:s
		INT8U  M_thisChargeTime_en;             //充电计时使能
    FP32 M_thisChargeMoney;               //本次充电金额   单位:元
		INT32U M_thisRemainTime;              //估算剩余充电时间

    FP32   M_meterDegree;                   //电表实时读数
    FP32   M_startDegree;                   //开始充电时的电表读数	
    FP32   M_endDegree;                     //结束充电时的电表读数  
		 INT8U userRunRequestFlag;                    //用户显示屏控制启停标志  0--停止  1--启动
		 
	INT32U charge_timeCnt;
	INT8U  charge_temeEn;

	INT8U cardCom_timeCnt;
	INT8U  cardComAlarm;
	INT8U  chargeFull;
	INT8U  cardBeepEn;
} USER_DB; 


#pragma pack(1)
typedef struct
{
    INT32U logID;                 //日志序号
    INT8U pay_cardID[16];         //支付卡卡号
//	INT8U physics_cardID[16];   //物理卡卡号
//	INT8U phase_pay_flag;       //分段计费标志
    _TIME startTime;              //充电开始时间
    _TIME endTime;                //充电结束时间
    FP32   M_startDegree;                   //开始充电时的电表读数	
    FP32   M_endDegree;                     //结束充电时的电表读数  
/*	FP32 tine_start_degree;      //峰起始值
    FP32 tine_end_power;
    FP32 tartness_start_degree;   //尖起始值
    FP32 tartness_end_power;
    FP32 flat_start_degree;       //平起始值
    FP32 flat_end_power;
    FP32 vale_start_degree;       //谷起始值
    FP32 vale_end_power;	
    
    FP32 tine_price;             //峰单价
    FP32 tine_total_degree;
    FP32 tine_total_money;
    
    FP32 tartness_price;          //尖电价
    FP32 tartness_total_degree;
    FP32 tartness_total_money;
    
    FP32 flat_price;              //平电价
    FP32 flat_total_degree;
    FP32 flat_total_money;
    
    FP32 vale_price;              //谷电价
    FP32 vale_total_degree;	
    FP32 vale_total_money;
*/  
    FP32 charge_degree;             //充电总电量	
    FP32 chargeVoltMax;              //充电电压最大值
    FP32 chargeCurrMax;              //充电电流最大值
    FP32 consum_money;              //消费金额
    INT8U car_id[17];           //车辆标识码

    INT8U trade_flag;           //交易标志  0记录无效 1交易未完成  2交易已正常完成   3交易已通过异常处理完成
    INT8U autoCtlFlag;          //手动/自动控制
		INT8U remote_enable;        //远程使能
		INT8U charge_mod;           //充电模式
		INT32U charge_time;           //充电时间
		INT8U  machineID;             //充电机ID;
		/*add member for portocol*/ //add

		uint8 crg_type;		//1立即充电2预约充电
		uint8 card_type;	//卡类型
		uint8 soc;			//soc 
		uint8 upLoadF;		//是否上传主站
		uint8 Pay_flg;		//是否付费
		uint32 stopReason;//充电结束原因
		uint32 remainTime;//剩余时间
		INT8U checkSum;              //校验码
		
    INT8U bargain_Num[16];        //交易流水号		
		FP32 purseBalance;           //钱包余额
		FP32 price_per_degree;       //消费单价
}CHARGE_LOG_INFO;  //充电日志信息
#pragma pack()
typedef  struct
{
  INT8U UartRBuf[COM_LEN_MAX];
  INT16U UartRLen;

}UART_INFO;

typedef struct
{
	uint8  uploadFlag;																//是否上传主站         1,上传
	uint8  overFlag;																	//充电是否正常结束     1,正常
	uint8  payFlag;																		//结束后是否正常付费   1,付费 2.补扣
	uint32  recordIndex;															//记录序号，会一直增加
	uint32  recordId;																	//记录索引，一直增加，但不会超过允许存储的最大记录数
	uint8			carNo[8];
	uint8			cardNo[8];
	uint32		startEnergeValue;
	uint32		endEnergeValue;
	uint32		startCardMoney;
	uint32		endCardMoney;
	uint16    chargeV;
	uint16    chargeI;
	uint32		crgLastT;
	uint32  	crgEnergy;
	uint32		crgMoney;
	 _TIME	startTime;
	 _TIME	endTime;
	uint32    leftTime;
	uint8 		crgType;
	uint8			crgMode;
	uint8			cardType;
	uint8			stopReason;//0112充电终止原因
	uint8			currentSOC;
}ChargeRecord;


#define TRUE                              1
#define	FALSE															0
//#define ON                             		1
//#define	OFF																0
#define PWMOFFV                           11
#define PWMLINKV                          8
#define PWMONV                            5


typedef enum
{
	IC_TYPE_NONE										=0,									
	IC_TYPE_USER										=0X01,											//用户卡 
	IC_TYPE_EMPLOYEE								=0X51,											//员工卡 
	IC_TYPE_RECHARGE								=0X61,											//充电卡 
}IC_TYPE;
typedef enum
{
	AC_STATUS_IDLE                      			=0,											//空闲
	AC_STATUS_START_CHECK_CARD_EXIST          =1,   									//充电前检测卡的存在
	AC_STATUS_PRE_DEAL_CARD									  =2,											//卡预处理
	AC_STATUS_PRE_WAIT_VERIFY_PWD							=3,											//充电前等待验证输入密码
	AC_STATUS_PRE_VERIFY_PWD                  =4,											//充电前验证卡密码
	AC_STATUS_CRG_STYLE											  =5,											//选择充电方式
	AC_STATUS_CRG_MODE											 	=6,											//选择充电模式						
	AC_STATUS_PRE_LOCK_CARD									  =7,											//充电前锁卡
	AC_STATUS_WAIT_DELAY_TIME									=8,											//预约充电
	AC_STATUS_CRGING												  =9,											//充电中
	AC_STATUS_OTHER_OVER_CHECK_CARD					  =10,									  //充电结束后等待刷卡
	AC_STATUS_OTHER_OVER_WAIT_PWD     			  =11,										//充电结束等待输入密码
	AC_STATUS_OTHER_OVER_VERIFY_PWD   				=12,										//充电结束等待验证密码
	AC_STATUS_CRGING_EXIST_CARD							  =13,										//充电中检测到卡
	AC_STATUS_CRGING_EXIST_CARD_WAIT_PWD      =14,										//充电中检测到卡等待验证密码
	AC_STATUS_CRGING_EXIST_CARD_VERIFY_PWD    =15,										//充电中检测到卡验证密码
	AC_STATUS_CRGING_EXIST_CARD_OVER_CRG			=16,										//充电中检测到卡停止充电
	AC_STATUS_UNLOCK_CARD                     =17,										//结束加电
	AC_STATUS_CRG_FINISH 										  =18,										//整个充电流程结束
	AC_STATUS_PRINT													  =19,										//打印
	AC_STATUS_INSERT_GUN_AGAIN   							=20,										//重新插枪
	AC_STATUS_OTHER_OVER_EXIST_CARD           =21,   									//充电结束后检测到卡
	AC_STATUS_OTHER_OVER_UNLOCK_CARD					=22,                    //其它方式结束充电结束加电状态
//	AC_STATUS_END_WAIT_VERIFY_PWD						,
//	AC_STATUS_END_VERIFY_PWD							  ,
  AC_STATUS_START_CHECK_CARD								=23,										//充电前检测卡
	AC_STATUS_UNLOCK_PRE_LOCK_CARD						=24,                    //充电前结束加电
	AC_STATUS_UNLOCK_PRE_LOCK_CARD_ADD        =25,										//充电前补扣
	AC_STATUS_SYSTEM_MANAGE                     =26,                //系统管理
}AC_STATUS;

typedef enum
    {
        NoErr,
        TxFault,
        RxFault,
        TimeOut,
    } EnumErrCode;


typedef enum
{
	SWITCH_VALUE_ON=0,
	SWITCH_VALUE_OFF=1,
}SWITCH_VALUE;


typedef struct {

INT8U comAlarm;	                    //通信状态
INT8U comAlarmCnt;	                //通信错误计数

INT8U moduleFail;	                //充电机故障
INT8U runStatus;                    //开关机状态
//INT8U averFloatStatus;              //均浮充状态
INT8U overVoltShut;	                //过压关机
INT8U overVoltAlarm;                //过压
INT8U lowVoltAlarm;	                //欠压

FP32  outVolt;		                //输出电压
FP32  outCurr;	                    //输出电流
FP32  moduleTemp;                   //模块温度
FP32  currPiont;                   //模块电流点
//INT16U  pf;                          //功率因数值
//FP32  setAverVolt;		            //均充电压
//FP32  setFloatVolt;                 //浮充电压
//FP32  setVoltUp;	                //过压值
//FP32  setVoltDown;	                //欠压值
FP32  setVolt;                      //设置电压
FP32  setCurr;		                //设置电流

//FP32  getSetAverVolt;		            //均充电压
//FP32  getSetFloatVolt;                 //浮充电压
//FP32  getSetVoltUp;	                //过压值
//FP32  getSetVoltDown;	                //欠压值
FP32  getSetVolt;                      //设置电压
FP32  getSetCurr;		                //设置电流

INT8U onoffCtl;				        //开关机控制
//INT8U averFloatCtl;                 //均浮充控制

}_MODULE_INFO;


/*====================================供CAN内部调用结构体和变量====================================*/
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

extern INT8U  gModbusRecArr[100];				//串口接收数据的临时寄存器
extern INT8U  gModbusRecNum;					//接收到的数据个数，最大50
//extern INT16U gModbusREG[MBREGNUM]={0};		//定义Modbus 寄存器，也是全局变量的桥梁								
										




#define MBREGNUM 	150					//modbus通讯允许访问的寄存器个数




extern INT8U gModbusRecNum;
extern INT8U gSystemCommFlag;
extern INT16U gModbusREG[MBREGNUM];
extern unsigned char  gChargeMode;									//bit0-0/1-auto/manual,bit1-0/1-autoV/C,bit2-0/1-manualV/C
extern unsigned char  gKeyValue;									//按键值
extern unsigned char  gKeyTimes;									//连续按键的次数或时间
extern unsigned char  gSegShowStatus;								//电压电流设置所处的状态
extern unsigned short gSystemERR;									//各种故障
extern INT16U gSysSetCurpwm;										//系统设置的电流PMW
extern INT16U gSysSetVolpwm;										//系统设置的电压PMW

extern char PWM_SetVoltage(INT16U lSetDat);							//设置电压
extern char PWM_SetCurrent(INT16U lSetDat);							//设置电流
extern INT16U abs_int16u(INT16U data1,INT16U data2);				//求两个无符号整形的绝对值

extern void SetVolMode(INT8U Mode);
extern INT8U GetVolMode(void);	

extern USER_DB gUserDB;

extern UART_INFO gUartInfo[COM_PORT_NUM];                     		//串口接收缓冲信息
extern INT8U uart_write(INT8U porID,INT8U * buf,INT8U length);
//extern INT8U uart_read(INT8U porID,INT8U * buf,INT8U timeout);
INT16U uart_read(INT8U porID,INT8U * buf,INT16U timeout);
extern _MODULE_INFO gModuleInfo[30];


extern CANRECMSGBUFF CanMessageBuff;								//定义CAN接收缓冲区
extern CANRECMSGBUFF MCM_CanPackage;								//建立另外一个can接收缓冲区

extern uint8 dataExchange(uint8 * pdat, uint8 len); 				//数据高低字节交换

#endif

