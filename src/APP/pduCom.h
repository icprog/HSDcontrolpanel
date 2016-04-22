#ifndef __PDUCOM_H
#define __PDUCOM_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "DLT645_2007.h"
#include "define.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define PDU_READ_A_VOLT   0X01       //读电压值
#define PDU_READ_B_VOLT   0X04       //读电压值
#define PDU_READ_C_VOLT   0X07      //读电压值

#define PDU_READ_A_CURR   0X02       //读电流值
#define PDU_READ_B_CURR   0X05       //读电流值
#define PDU_READ_C_CURR   0X06       //读电流值

#define PDU_READ_POWER  0X03       //读总功率
#define PDU_READ_ADDR   0X00       //读电表地址


/********************************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************************/
extern uint8 pdu_Write(uint8 * sbuf, uint16 * len , uint8  cmd);
extern uint8 pdu_makePackage(uint8 * sbuf, uint16 *len , PDU_FRAME * pFrame);
extern uint8 get_dataCheck(uint8 * pbuf, uint8 len);
extern uint8 add33H(uint8* pbuf, uint8 datalen);
extern uint8 dec33H(uint8* pbuf, uint8 datalen);
extern uint8 pdu_releasePackage(uint8* rbuf, uint16 len, PDU_FRAME * pFrame);
extern uint8 pdu_Read(uint8* rbuf, uint16 len, uint8* cmd);
extern uint8 pdu_readCallback(PDU_FRAME pdu_frame,uint8 * cmd);
extern PDU_INFO pdu_info;
extern uint8 pdu_data_separate(uint8 datalen , uint8 * pdata,uint8 * cmd);
extern uint8 pdu_Manage(uint8 CMD);

#endif
