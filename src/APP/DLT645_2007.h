#ifndef  _DLT645_H_
#define  _DLT645_H_
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "includes.h"
#include "define.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define PDU_STX		0X68
#define PDU_ETX		0X16

//---------------------------数据标识-------------------------------------
#define POSITIVE_POWER_TOTAL_F		0x00010000	//正向有功总电能标识
#define A_VOLT_F					0x02010100	//A相电压标识
#define B_VOLT_F					0X02010200	//B相电压标识
#define C_VOLT_F					0X02010300	//C相电压标识
#define A_CURR_F					0X02020100	//A相电压标识
#define B_CURR_F					0X02020200	//B相电流标识
#define C_CURR_F					0X02020300	//C相电流标识
#define ADDR_F            0x04000401  //地址

//--------------------------控制码-----------------------------------------
#define M_R_DATA_CODE					0X11				//主机读数据控制码
#define M_RC_DATA_CODE					0X12				//主机读后续数据控制码
#define M_W_DATA_CODE					0X14				//主机写数据控制码
#define M_R_ADR_CODE					0X13				//主机读通信地址控制码
#define M_W_ADR_CODE					0X15				//主机写通信地址控制码
#define M_RADIO_TIME_CODE				0X08				//主机广播校时控制码

#define S_R_DATA_CODE					0X91				//从机读数据帧控制码
#define S_RC_DATA_CODE					0XB1				//从机读有后续数据帧控制码
#define S_ERR_DATA_CODE					0XD1				//从机异常应答控制码
#define S_R_DATA2_CODE					0X92				//从机读数据帧控制码
#define S_RC_DATA2_CODE					0XB2				//从机读有后续数据帧控制码
#define S_R_ADR_CODE					0X93				//从机读通信地址控制码

//--------------------------命令--------------------------------------------
#define POWER_TOTAL_CMD					0X00				//读总功率命令
#define A_VOLT_CMD						0X01				//读A相电压命令
#define B_VOLT_CMD						0X02				//读B相电压命令
#define C_VOLT_CMD						0X03				//读C相电压命令
#define A_CURR_CMD						0X04				//读A相电流命令
#define B_CURR_CMD						0X05				//读B相电流命令
#define C_CURR_CMD						0X06				//读C相电流命令
#define R_ADDR_CMD						0X07				//读电表地址命令


/********************************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************************/
#pragma pack(1)
typedef struct
{
	uint8	stx1;			//开始字节1
	uint8   padr[6];		//地址
	uint8   stx2;			//开始字节2
	uint8   ctl;			//控制码
	uint8   len;			//数据长度
	uint8   pdata[150];		//数据域
	uint8	cs;				//校验码
	uint8   etx;			//结束字节
}PDU_FRAME;


typedef  struct
{
	fp32 power_total;		//总功率
	fp32 A_volt;			//A相电压
	fp32 B_volt;			//B相电压
	fp32 C_volt;			//C相电压
	fp32 A_curr;			//A相电流
	fp32 B_curr;			//B相电流
	fp32 C_curr;			//C相电流
	uint8 addr[6];			//电表地址

}PDU_INFO;
#pragma pack()


#endif

