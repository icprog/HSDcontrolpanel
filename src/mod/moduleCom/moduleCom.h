#ifndef __MODULECOM_H
#define __MODULECOM_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "../../app/global.h"
#include "../../app/includes.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define REG_ON_CTL_ADR	  				(0X1000)  //开机控制寄存器
#define REG_OFF_CTL_ADR	  				(0X1001)  //关机控制寄存器
#define REG_AVER_CTL_ADR	    		(0X1002)  //均充控制寄存器
#define REG_FLOAT_CTL_ADR	    		(0X1003)  //浮充控制寄存器

#define REG_AVER_VOLT_SET_ADR        	(0X0000)    //均充电压设置
#define REG_FLOAT_VOLT_SET_ADR       	(0X0001)    //浮充电压设置
#define REG_OVER_VOLT_SET_ADR        	(0X0002)    //过压值设置
#define REG_LOW_VOLT_SET_ADR         	(0X0003)    //欠压值设置
#define REG_OUT_VOLT_SET_ADR         	(0X0004)    //输出电压设置
#define REG_OUT_CURR_SET_ADR         	(0X0005)    //输出电流设置
#define REG_OUT_VOLT_DISP_ADJ_ADR       (0X0006)    //电压测量校准
#define REG_OUT_VOLT_SET_ADJ_ADR        (0X0007)    //电压控制校准
#define REG_OUT_CURR_DISP_ADJ_ADR       (0X0008)    //电流测量校准
#define REG_OUT_CURR_SET_ADJ_ADR        (0X0009)    //电流控制校准



#define GET_ANALOG_CMD            0X41
#define GET_STATUS_CMD            0X43
#define GET_ALARM_CMD             0X44
#define CTL_CMD                   0X45
#define REMOTE_CMD                0X80

#define DC_MODULE                 0
#define AC_MODULE                 1



/*********************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************/

uint8 readModuleStatus(INT8U devAdr);
uint8 readModuleAnalogDat(INT8U devAdr);
uint8 readModuleConfigDat(INT8U devAdr);
uint8 writeModuleConfigDat(INT8U devAdr,INT16U regAdr,INT16U dat);
uint8 writeModuleCtl(INT8U devAdr,INT16U regAdr,INT16U dat);











#endif








