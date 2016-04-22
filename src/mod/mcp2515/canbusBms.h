//modbus头文件
#ifndef	__MODBUS_H__
#define	__MODBUS_H__



//485片选IO口
#define MAX485SL    P2_2   //定义485通信的片选信号----

//从机地址
#define  CAN_485_DEVICE_ID  100  //默认地址 100

//系统定时器及定时值
#define CCAPXL_MODBUS  CCAP2L  //Modbus T3.5字节
#define CCAPXH_MODBUS  CCAP2H
#define CCAPMX_MODBUS  CCAPM2
#define CCFX_MODBUS    CCF2
#define MODBUS_T_ON    {CCAPMX_MODBUS=0x49;}  //打开定时器
#define MODBUS_T_OFF   {CCAPMX_MODBUS=0x00;}  //关闭定时器
#define PCA_T35mS_L     0x00
#define PCA_T35mS_H     0x6C

//数组长度
//#define  COM_BUF_RN   200   //接收数据的长度
//#define  COM_BUF_SN   200   //发送数据的长度
#define  END_REG_LEN  107  //寄存器个数

//最后的一个寄存器地址
#define END_REG_ADR  108    

//全局变量
//extern uint8 Device_ID;
//extern uint8 RevComflag,ModbusT_CloseFlag;
//extern uint8 RevComCount,CrcCounter;
//extern  uint8 RevComBuff[COM_BUF_RN];  //串口接收的buff
//
////全局函数
//extern void UartSendStr(uint8 *p,uint8 i);  //发送一串数据
//extern void UARTInit(void);  //串口初始化
//extern void ModbusCommunication(uint16 *ptr);  //modbus通信

#endif


#ifndef  CAN_H
#define  CAN_H

//======操作系统时间调度=====//
#define OSDELAYTIME10MS     10   //延时10ms
#define OSDELAYTIME20MS     20
#define OSDELAYTIME50MS     50
#define OSDELAYTIME100MS    100  //延时100ms
#define OSDELAYTIME150MS    150
#define OSDELAYTIME200MS    200
#define OSDELAYTIME250MS    250  //延时250ms
#define OSDELAYTIME300MS    300  //延时300ms
#define OSDELAYTIME500MS    500
#define OSDELAYTIME1000MS   1000
#define OSDELAYTIME1500MS   1500
#define OSDELAYTIME2000MS   2000
#define OSDELAYTIME2500MS   2500  //延时2.5s 

//======定时值===========//
#define TIMER5S      500    //1S数值100， 定时器10ms，总定时5s,超时错误5s
#define TIMERRESS    1500    //15s钟内没有通信成功则自动重启

//======PCA定时器设置，采用PCA定时器,PCA0,PCA2
#define CCAPXL_TC      CCAP0L  //定时10ms
#define CCAPXH_TC      CCAP0H
#define CCAPMX_TC      CCAPM0
#define CCFX_TC        CCF0
#define PCA_10mS_L     0x00
#define PCA_10mS_H     0x48

//=====标准帧，扩展帧===========//
#define  STANDARD_FRAME_TYPE    1   //标准帧
#define  EXTEND_FRAME_TYPE      2   //扩展帧

//======CAN收发数据buf长度
#define  CAN_BUF_RN   8    //接收数据的长度,8个数据
#define  CAN_BUF_SN   8    //发送数据的长度，8个数据

//=====默认发送的无效数据===========//
#define  INVALID_CANDATA   0xFF 

//=========BMS报文解码不同处理阶段值PGN，因为CAN传输过程中是先传高位再传低位，所以接收时的数据与协议中的数据高低位相反==========//
//#define	BMS_DEC_BRM						0x02	//BMS和车辆辨识报文BRM，250ms
//#define	BMS_DEC_VBI						0x30	//车辆和电池标识报文VBI，250ms
//#define	BMS_DEC_BCP						0x06	//蓄电池充电参数BCP，500ms，实际是13个字节
//#define	BMS_DEC_BRO						0x09	//电池充电准备就绪状态BRO，250ms
//#define	BMS_DEC_BCL						0x10	//电池充电需求报文BCL，20ms
//#define	BMS_DEC_BCS						0x11	//电池充电总状态报文BCS，250ms
//#define	BMS_DEC_BSM    					0x12	//蓄电池状态信息BSM，250ms
//#define	BMS_DEC_BMV     				0x15	//蓄电池最小测量单元电压BMV，1000ms
//#define	BMS_DEC_BMT     				0x16	//蓄电池组温度BMT，1000ms
//#define	BMS_DEC_BSOC      				0x17	//蓄电池组荷电容量SOC ,BSOC，1000ms
//#define	BMS_DEC_BST       				0x19	//BMS中止充电报文BST，10ms
//#define	BMS_DEC_BSD      				0x1C	//BMS统计数据BSD，250ms
//#define	BMS_DEC_BEM						0x1E	//BMS错误报文BEM，250ms
//#define	BMS_DEC_BFM						0x33	//BMS故障报文BFM，250ms

#define WZL_BMS_DEC_BCP					0X06    //五洲龙BMS蓄电池充电参数BCP，500ms，实际是13个字节

//================BMS报文PGN值，因为CAN传输过程中是先传高位再传低位，所以接收时的数据与协议中的数据高低位相反================//
#define	M_BRM_PGN						0x02	//BMS和车辆辨识报文BRM，250ms
#define	M_VBI_PGN						0x30	//车辆和电池标识报文VBI，250ms
#define	M_BCP_PGN						0x06	//蓄电池充电参数BCP，500ms，实际是13个字节
#define	M_BRO_PGN						0x09	//电池充电准备就绪状态BRO，250ms
#define	M_BCL_PGN						0x10	//电池充电需求报文BCL，20ms
#define	M_BCS_PGN						0x11	//电池充电总状态报文BCS，250ms
#define	M_BSM_PGN    					0x12	//蓄电池状态信息BSM，250ms
#define	M_BMV_PGN     				    0x15	//蓄电池最小测量单元电压BMV，1000ms
#define	M_BMT_PGN     				    0x16	//蓄电池组温度BMT，1000ms
#define	M_BSOC_PGN      				0x17	//蓄电池组荷电容量SOC ,BSOC，1000ms
#define	M_BST_PGN       				0x19	//BMS中止充电报文BST，10ms
#define	M_BSD_PGN      				    0x1C	//BMS统计数据BSD，250ms
#define	M_BEM_PGN						0x1E	//BMS错误报文BEM，250ms
#define	M_BFM_PGN						0x33	//BMS故障报文BFM，250ms

//=========CCS报文解码不同处理阶段值==========//
#define	CCS_DEC_CRM_0					0x01	//充电机辨识报文CRM，250ms
#define	CCS_DEC_CRM_1					0x01	//充电机辨识报文CRM，250ms
#define	CCS_DEC_CTS				        0x07	//充电机发送的时间同步信息CTS，500ms
#define	CCS_DEC_CML				        0x08	//充电机最大输出能力CML，250ms
#define	CCS_DEC_CRO						0x0A	//充电机输出准备就绪状态CRO，250ms
#define	CCS_DEC_CCS						0x14	//充电机充电状态CCS,20ms
#define	CCS_DEC_CST						0x1A	//充电机中止充电CST，10ms
#define	CCS_DEC_CSD						0x1D	//充电机统计数据报文CSD，250ms
#define	CCS_DEC_CEM						0x1F	//充电机错误报文CEM，250ms
#define	CCS_DEC_CFM						0x34	//充电机故障报文CFM，250ms

#define WZL_DEC_CCS                     0xFF    //五洲龙充机充电状态报文

//=====CCS向BMS发送数据的字节数========//
#define CCS_NUM_CRM		8  //充电机辨识报文
#define CCS_NUM_CTS 	7  //CCS发送的时间同步信息
#define CCS_NUM_CML		8 //CCS最大输出能力
#define CCS_NUM_CRO 	1 //CCS充电准备报文
#define CCS_NUM_CCS		6 //CCS充电状态
#define CCS_NUM_CST		3  //CCS中止充电CST
#define CCS_NUM_CSD		7  //CCS充电统计
#define CCS_NUM_CEM		3  //转入错误阶段发送的错误统计报文 
#define CCS_NUM_CFM		2  //故障阶段的故障报文

//=============BMS CAN ID前两个字节============================//
#define BMSID_BRM	0xC00A			//0x1802E5F4,BMS辨识报文BRM
#define BMSID_VBI	0xC188			//0x1830E5F4,车辆识别码VBI
#define BMSID_BCP	0xC02A			//0x1806E5F4,BMS蓄电池充电参数BCP
#define BMSID_BRO	0xA049			//0x1409E5F4,BMS充电准备就绪报文BRO
#define BMSID_BCL	0xC088			//0x1810E5F4,电池充电需求BCL
#define BMSID_BCS	0xC089			//0x1811E5F4,电池充电状态BCS
#define BMSID_BSM	0xC08A			//0x1812E5F4,蓄电池状态信息BSM
#define BMSID_BMV	0xC0A9			//0x1815E5F4,蓄电池最小测量单元电压BMV
#define BMSID_BMT	0xC0AA			//0x1816E5F4,蓄电池组温度BMT
#define BMSID_BSOC	0xC0AB			//0x1817E5F4,蓄电池组荷电容量BSOC
#define BMSID_BST	0xA0C9			//0x1419E5F4,BMS中止充电BST
#define BMSID_BSD	0xC0E8			//0x181CE5F4,BMS充电统计BSD
#define BMSID_BEM	0x80EA			//0x101E05F4,转入错误阶段发送的错误统计报文BEM
#define BMSID_BFM	0x818B			//0x103305F4,故障报文BFM

#define WZL_BMSID_BCP 0XC02A       //五洲龙BMS

//==============CCS 充电流程阶段值===========================//
#define CCS_SEND_CRM_0		    0
#define CCS_SEND_CRM_INVALID    1
#define CCS_SEND_CRM_1		    2
#define CCS_SEND_CTS_CML	    3 
#define CCS_SEND_CRO		    4	
#define CCS_SEND_CCS		    5
#define CCS_SEND_CST		    6	
#define CCS_SEND_CSD		    7	

//=========CCS报文解码不同处理阶段值所对应的ModbusREGDat起始地址和数据长度==========//
#define	CCS_CRM_START_ADR		9	//start Adr
#define	CCS_CRM_START_LEN		8
#define CCS_CTS_START_ADR		13	//充电机输出时间同步CTS，配置阶段
#define CCS_CTS_START_LEN		7
#define CCS_CML_START_ADR		17	//充电机最大输出级别CML，配置阶段 
#define CCS_CML_START_LEN		8
#define CCS_CRO_START_ADR		21	//充电机输出准备就绪状态CRO，配置阶段
#define CCS_CRO_START_LEN		1
#define CCS_CCS_START_ADR		22	//充电机充电状态CCS，充电阶段
#define CCS_CCS_START_LEN		6
#define CCS_CST_START_ADR		25	//充电机中止充电CST，充电阶段
#define CCS_CST_START_LEN		3
#define CCS_CSD_START_ADR		27	//充电机统计数据报文CSD，结束阶段
#define CCS_CSD_START_LEN		5
#define CCS_CEM_START_ADR		30	//充电机错误报文CEM  
#define CCS_CEM_START_LEN		3
#define CCS_CFM_START_ADR		32	//充电机故障报文CFM  
#define CCS_CFM_START_LEN		1


//=========BMS报文解码不同处理阶段值所对应的ModbusREGDat起始地址和数据长度==========//
#define	M_BRM_START_ADR		35	//start Adr
#define	M_BRM_ADR_LEN		41
#define	M_VBI_START_ADR		56	//35+21
#define	M_VBI_ADR_LEN		40
#define	M_BCP_START_ADR		76	//56+20
#define	M_BCP_ADR_LEN		13
#define	M_BRO_START_ADR		83	//76+7
#define	M_BRO_ADR_LEN		1
#define	M_BCL_START_ADR		84	//83+1
#define	M_BCL_ADR_LEN		5
#define	M_BCS_START_ADR		87	//84+3
#define	M_BCS_ADR_LEN		8
#define	M_BST_START_ADR		91	//87+4
#define	M_BST_ADR_LEN		3
#define	M_BSD_START_ADR		93	//91+2
#define	M_BSD_ADR_LEN		7
#define	M_BEM_START_ADR		97	//93+4
#define	M_BEM_ADR_LEN		4
#define	M_BFM_START_ADR		99	//97+2
#define	M_BFM_ADR_LEN		7	//99+4=103,即modbusreg[103]
//=====将该部分屏蔽掉======//
#define	M_BSM_START_ADR		103
#define	M_BSM_ADR_LEN		7	//7
#define	M_BMV_START_ADR		X
#define	M_BMV_ADR_LEN		X	//0xFF
#define	M_BMT_START_ADR		X
#define	M_BMT_ADR_LEN		X	//0xFF
#define	M_BSOC_START_ADR	X
#define	M_BSOC_ADR_LEN		X	//0xFF

//=========多数据传输协议的PGN的值===========//
#define	M_PGN_BRM		0x02  //只取中间2个字节，其它默认0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17

#define MUTICAN_CMD0    0xE7
#define MUTICAN_CMD1    0x68
#define MUTICAN_DAT0    0xE7
#define MUTICAN_DAT1    0x4B

//=========多数据传输协议的控制指令值===========//
#define M_TP_CM_RTS_CMD		16
#define M_TP_CM_CTS_CMD 	17
#define M_TP_ENDMSGACK_CMD 	19
#define M_TP_CONN_ABORT_CMD 255     //放弃连接

//========CAN一帧数据默认发送的字节数===========//
#define	M_CAN_DEFAULT_LEN	8

//=====接收帧的高低字节定义=======//
#define BYTE_LOW_8  0   //低地址为低字节
#define BYTE_HIGH_8 8   //高地址为高字节

//==========布尔量==========//
#define  TRUE	1
#define  FALSE	0

//=====CCS向BMS发送数据的CAN ID========//
#define BMS_CCS_STATUS_ADR			0		//BMS-CCS状态寄存器
#define CHARGE_PHASE_ADR			1		//充电阶段寄存器
#define CHARGE_CCS_START_ADR		2		//充电启动寄存器
#define BMS_TRANSMIT_ADR			3		//CAN传输寄存器


//=====CCS向BMS发送数据的CAN ID========//
 uint8 CCSID_CRM_0[4] = {0xC0, 0x09, 0xF4, 0xE5,};  //0x1801F4E5,充电机辨识报文CRM
 uint8 CCSID_CRM_1[4] = {0xC0, 0x09, 0xF4, 0xE5,};  //0x1801F4E5,充电机辨识报文CRM
 uint8 CCSID_CTS[4]   = {0xC0, 0x2B, 0xF4, 0xE5,};  //0x1807F4E5,CCS时间同步信息CTS
 uint8 CCSID_CML[4]   = {0xC0, 0x48, 0xF4, 0xE5,};  //0x1808F4E5,CCS最大输出能力CML
 uint8 CCSID_CRO[4]   = {0xA0, 0x4A, 0xF4, 0xE5,};  //0X140AF4E5,CCS充电准备报文CRO
 uint8 CCSID_CCS[4]   = {0xC0, 0xA8, 0xF4, 0xE5,};  //0x1814F4E5,CCS充电状态CCS
 uint8 CCSID_CST[4]   = {0xA0, 0xCA, 0xF4, 0xE5,};  //0x141AF4E5,CCS中止充电CST
 uint8 CCSID_CSD[4]   = {0xC0, 0xE9, 0xF4, 0xE5,};  //0x181DF4E5,CCS充电统计CSD
 uint8 CCSID_CEM[4]   = {0x80, 0xEB, 0xF4, 0xE5,};  //0x101FF4E5,转入错误阶段发送的错误统计报文CEM 
 uint8 CCSID_CFM[4]   = {0x81, 0xA8, 0xF4, 0xE5,};  //0x1034F4E5,充电机发送的故障报文CFM
 uint8 MUTICAN_CTS_ID[4] = {0xE7, 0x68, 0xF4, 0xE5};//多包数据帧指令

//======PGN多包协议CCS向BMS发送的数据CAN ID============//
 uint8 CCS_ID_PGN_BRM[4] = {0xC0, 0x0A, 0xF4, 0xE5,};  //BRM
 uint8 CCS_ID_PGN_VBI[4] = {0xC1, 0x88, 0xF4, 0xE5,};  //VBI
 uint8 CCS_ID_PGN_BCP[4] = {0xC0, 0x2A, 0xF4, 0xE5,};  //BCP
 uint8 CCS_ID_PGN_BMV[4] = {0xC0, 0xA9, 0xF4, 0xE5,};  //BMV
 uint8 CCS_ID_PGN_BMT[4] = {0xC0, 0xAA, 0xF4, 0xE5,};  //BMT
 uint8 CCS_ID_PGN_BSOC[4] ={0xC0, 0xAB, 0xF4, 0xE5,};  //BSOC

//==========全局变量===========//
//uint16  ModbusREGDat[END_REG_LEN];  //定义Modbus寄存器  
uint8 g_bCANBUSTimeoutFlag;   //1--表示canbus通信成功，0--表示canbus通信失败。默认值为1，这与昆仑通态的触摸屏的特性有关。
uint8 g_bChargeFullFlag;	//1--表示充满电，0--表示未充满电
uint8 g_bChargeStartFlag;	//1--表示可以连接，0--表示不能进行连接
uint8 g_bChargingTimeoutFlag; //充电过程中是否出现超时错误
uint8 g_bChargeEndFlag; //充电结束标志
uint8 g_bSoftRestStartFlag;
uint16 g_u16Run5SCount,g_u16RunResSCount;

struct _BMSFlow
{
	uint8 u8BMSStep;
	uint8 btransmitingFlag;
	uint8 noChargeFlag;
	uint8 u8ChargingFlag;
	uint8 u8ChargingTimeoutCnt;
} gs_BMSFlow;

uint8  byte_IR;
#define bit_RI_0    (byte_IR & 0x01)        //位0
#define  bit_RI_1  ((byte_IR & 0x02) >>1) //位1
#define  bit_TI    ((byte_IR & 0x04) >>2)   //位2

#define  bit_MERRF  ((byte_IR & 128) >>7)   //位7
#define  bit_WAKIF  ((byte_IR & 64) >>6)   //位6
#define  bit_ERRIF  ((byte_IR & 32) >>5)   //位5

uint8  byte_RXBnSIDL,byte_RXBnDLC;
#define bit_IDE  ((byte_RXBnSIDL & 0x08) >>3) //位3
#define bit_SRR  ((byte_RXBnSIDL & 16) >>4)   //位4
#define bit_RTR  ((byte_RXBnDLC & 64) >>6)    //位6

uint8 byte_EFLG;
#define bit_RX0OVR	((byte_EFLG & 64) >>6)   //位6
#define bit_RX1OVR	((byte_EFLG & 128) >>7)   //位7


///////////////////////握手阶段/////////////////////////
typedef struct 
{
	uint8 chargeMachine_code;		//充电机编号
	uint8 plug_code;				//插头编号
	uint8 chargeMachine_station;	//充电机位置 0X00 户外 0X01 户内
	uint8 descern_result;			//辨识结果  0X00 BMS不能识别 0X01能识别
	uint32 chargeMachine_area;		//充电机所在区域编码

}_CRM;								//充电机辨识报文

typedef struct
{
	uint8 bms_ver[3];			//BMS版本号
	uint8 bat_type;				//电池类型
	uint16 vated_soc;			//额定容量
	uint16 vated_volt;			//额定电压
	uint32 bat_manufacturer;	//电池生产厂商
	uint32 bat_serial_num;		//电池组序号
	uint8 produce_date[3];		//电池组生产日期
	uint8 charge_time[3];		//充电次数
	uint8 bat_flag;				//电池产权标识
	uint8 reserve;				//保留
	uint8 descern_info[8];		//辨识信息

}_BRM;							//BMS辨识报文


///////////////////////充电参数配置阶段/////////////////////////

typedef struct
{
	uint16 permitHighModuleCharge_volt;			//电池模块最高允许充电电压  分辨率：0.01V/位， 0V偏移量 ；范围： 0~600V
	uint16 permitCharge_curr;					//最高允许充电电流	分辨率：0.1A/位， -800V偏移量 ；范围： -800~0A
	uint8  max_charge_soc;						//最大允许充电电量	分辨率：1%/位，0%偏移量 ；范围： 0~100%
	uint16 highChargeSum_volt;					//最高允许充电总电压	分辨率：0.1A/位， -800V偏移量 ；范围：0~800V
	uint8  PermitHigh_temper;					//最高允许温度	分辨率：1度/位， -40度偏移量 ；范围： -40~210度
	uint16 bat_soc;								//电池剩余容量	分辨率：1AH/位， 0AH偏移量 ；范围： 0~1000AH
	uint16 batSum_volt;							//电池总电压	分辨率：0.1V/位， 0V偏移量 ；范围：0~800V

}_BCP;											//蓄电池充电参数报文

typedef struct
{
	uint8 time[7];								//充电时间，年月日时分秒	

}_CTS;											//充电机发送时间同步信息报文

typedef struct 
{
	uint16 outVolt_H;							//最高输出电压	分辨率：0.1V/位， 0V偏移量 ；范围：0~800V
	uint16 outVolt_L;							//最低输出电压  分辨率：0.1V/位， 0V偏移量 ；范围：0~800V
	uint16 outCurr_H;							//最高输出电流  分辨率：0.1V/位， -800A偏移量 ；范围：-800~0A
	uint8  pulse_curr;							//脉冲放电电流	分辨率：2A/位， 0A偏移量 ；范围：0~500A
	uint8 pulse_time;							//脉冲时间		分辨率：1S/位， 0S偏移量 ；范围：0~255S

}_CML;											//充电机最大输出能力报文


///////////////////////充电准备阶段/////////////////////////
typedef struct
{
	uint8 bsm_prepare_flag;						//BMS充电准备标志  0000B 不能充电   0010B BMS未准备好  0100B BMS完成充电准备

}_BRO;											//BMS充电准备就绪报文

typedef struct
{
	uint8 chargeMachine_prepare_flag;			//充电机充电准备标志   0010B 未准备好  0100B 完成充电准备

}_CRO;											//充电机充电准备就绪报文


///////////////////////充电阶段/////////////////////////
typedef struct 
{
	uint16 needVolt;							//最高输出电压	分辨率：0.1V/位， 0V偏移量 ；范围：0~800V
	uint16 needCurr;							//最低输出电压  分辨率：0.AV/位，-800V偏移量 ；范围：-800~500A
	uint16 chargeMode;							//充电模式 001B 恒压充电   010B恒流充电

}_BCL;											//电池充电需求报文


typedef struct 
{
	uint16 chargeMeasure_volt;					//充电电压测量值	分辨率：0.1V/位， 0V偏移量 ；范围：0~800V
	uint16 chargeMeasure_curr;					//充电电流测量值	分辨率：0.AV/位，-800V偏移量 ；范围：-800~500A
	uint8  bat_temper_L;						//电池最低温度		分辨率：1度/位 ，-50度偏移量 ；范围：150~200度
	uint8  bat_temper_H;						//电池最高温度		分辨率：1度/位 ，-50度偏移量 ；范围：150~200度
	uint8  soc;									//电池容量			分辨率：1%/位，0%偏移量 ；范围： 0~100%
	uint8  remain_time;							//估计剩余时间		分辨率：1min/位，0min偏移量 ；范围： 0~250min  

}_BCS;//电池充电总状态报文


typedef struct 
{
	uint16 out_volt;					//输出电压	分辨率：0.1V/位， 0V偏移量 ；范围：0~800V
	uint16 out_curr;					//输出电流	分辨率：0.AV/位，-800V偏移量 ；范围：-800~500A
	uint8  remain_time;					//充电时间		分辨率：1min/位，0min偏移量 ；范围： 0~250min  

}_CCS;//充电机充电状态报文



typedef struct 
{	
	uint16  highBatVolt_group;		
	uint8   highBatVolt_addrCode;
	uint8   highBatTemper;
	uint8   highBatTestCode;
	uint8   batCell_highVolt;
	uint8   batCell_lowVolt;
	uint8   baSOC_high;
	uint8   baSOC_low;
	uint8   bat_highCurr;
	uint8   bat_highTemper;
	uint8   isolationState;
	uint8   batConnectState;
	uint8   wachdogState;
	uint8   chargePermit;

}_BSM;//BMS发送蓄电池状态信息报文

typedef struct 
{	
			
	uint16   batVolt[16];	

}_BMV;//蓄电池最小单元电压报文

typedef struct 
{	
			
	uint8   battemper[20];	

}_BMT;//蓄电池最小单元温度报文


typedef struct 
{	
			
	uint8   bSOC[20];	

}_BSOC;//蓄电池最小单元温度报文


typedef struct 
{	
			
	uint8   stopCause;				//BMS中止充电原因
	uint8   failureCause;			//BMS中止充电故障原因
	uint8   errCause;				//BMS中止充电错误原因

}_BST;//BMS中止充电原因  

typedef struct 
{	
			
	uint8   stopCause;				//中止充电原因
	uint8   failureCause;			//中止充电故障原因
	uint8   errCause;				//中止充电错误原因

}_CST;//充电机中止充电原因报文


///////////////////////充电结束阶段/////////////////////////

typedef struct 
{	
			
	uint8   stop_SOC;				//中止状态的SOC
	uint16   cell_lowVolt;			//最低单体电压
	uint16   cell_highVolt;			//最高单体电压
	uint16   bat_lowTemper;			//最低温度
	uint16	 bat_highTemper;		//最高温度

}_BSD;//BMS统计数据报文


typedef struct 
{	
			
	uint16   chargeTime;				//充电时间
	uint16   outPower;					//输出电量
	uint8    mechineCode;			    //充电机编号
	

}_CSD;//充电机统计数据报文


///////////////////////错误报文/////////////////////////

typedef struct 
{			
	uint8   assistant_power_err;		//辅助电源是否正常?
	uint8   CRM0_timeOut;				//接收SPN2563=0充电机辨识报文超时
	uint8   CRM1_timeOut;				//接收SPN2563=1充电机辨识报文超时
	uint8   CTS_CML_timeOut;			//接收充电机同步时间和最大输出能力报文超时
	uint8   CRO_timeOut;				//接收充电机完成充电准备报文超时
	uint8   CCS_timeOut;				//接收充电机状态报文超时
	uint8   CST_timeOut;				//接收充电机中止充电报文超时
	uint8   CSD_timeOut;				//接收充电机统计报文超时

}_BEM;//BMS错误报文


typedef struct 
{	
			
	uint8   BRM_timeOut;				//接收BMS辨识报文超时
	uint8   BCP_timeOut;				//接收电池充电参数报文超时
	uint8   BRO_timeOut;				//接收BMS充电准备报文超时
	uint8   BSM_timeOut;				//接收BMS充电状态报文超时
	uint8   BCL_timeOut;				//接收BMS充电要求报文超时
	uint8   BST_timeOut;				//接收BMS中止充电报文超时
	uint8   BSD_timeOut;				//接收BMS统计数据报文超时

}_CEM;//充电机错误报文



typedef struct
{
	_BRM brm_cmd;		//BMS辨识报文
	_BCP bcp_cmd;		//蓄电池充电参数报文
	_BRO bro_cmd;		//BMS充电准备就绪报文
	_BCL bcl_cmd;		//电池充电需求报文
	_BCS bcs_cmd;		//电池充电总状态报文
	_BSM bsm_cmd;		//BMS发送蓄电池状态信息报文
	_BMV bmv_cmd;		//蓄电池最小单元电压报文
	_BMT bmt_cmd;		//蓄电池最小单元温度报文
	_BSOC bsoc_cmd;		//蓄电池最小单元温度报文
	_BST bst_cmd;		//BMS中止充电原因  
	_BSD bsd_cmd;		//BMS统计数据报文
	_BEM bem_cmd;		//BMS错误报文

}BMS_MSG;


typedef struct
{
	_CRM crm_cmd;   //充电机辨识报文
	_CTS cts_cmd;	//充电机发送时间同步信息报文
	_CML cml_cmd;   //充电机最大输出能力报文
	_CRO cro_cmd;   //充电机充电准备就绪报文
	_CCS ccs_cmd;   //充电机充电状态报文
	_CST cst_cmd;	//充电机中止充电原因报文
	_CSD csd_cmd;	//充电机统计数据报文
	_CEM cem_cmd;	//充电机错误报文


}CCS_MSG;



/****************************************沃特玛，五洲龙BMS协议*********************************************/

#define  VOL_SET_H						0
#define  VOL_SET_L						1
#define  CUR_SET_H						2
#define  CUR_SET_L						3
#define  CONTROL_FLG					4

#define  AH_marker_H					8
#define  AH_marker_L					9
#define  AH_actual_H					10
#define  AH_actual_L					11
#define  VOL_CELL_OVER_protect_H		12
#define  VOL_CELL_OVER_protect_L		13
#define  BAT_NUM						14

#define  VOL_CELL_MAX_H					16
#define  VOL_CELL_MAX_L					17
#define  VOL_CELL_MINH					18
#define  VOL_CELL_MIN_L					19
#define  VOL_CELL_ULV_protect_H			20
#define  VOL_CELL_ULV_protect_L			21
#define  BAT_STATE						22

#define VOL_BAT_H						24
#define VOL_BAT_L						25
#define CUR_CHARGE_H					26
#define CUR_CHARGE_L					27
#define SOC								28
#define TEMP_MAX						29
#define TEMP_MIN						30

#define OUT_VOL_H						32
#define OUT_VOL_L						33
#define OUT_CUR_H						34
#define OUT_CUR_L						35
#define STATUS							36


#define BMS_CTL_CMD						0



typedef struct
{
	fp32 volt_limit;
	fp32 curr_limit;
	uint8 control_flg; 
	fp32  bat_volt;
	fp32  bat_charge_cur;
	uint8 soc;

}WZL_BMS_INFO;  //五洲龙BMS信息


typedef struct
{
	fp32 out_volt;
	fp32 out_cur;
	uint8 status; 

}WZL_CCS_INFO;//充电机信息

//////////////////////////////////////////////////////////////////////////////////

#endif