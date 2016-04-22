#ifndef __PDUCOM_H
#define __PDUCOM_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "DLT645_2007.h"
#include "define.h"

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define PDU_READ_A_VOLT   0X01       //����ѹֵ
#define PDU_READ_B_VOLT   0X04       //����ѹֵ
#define PDU_READ_C_VOLT   0X07      //����ѹֵ

#define PDU_READ_A_CURR   0X02       //������ֵ
#define PDU_READ_B_CURR   0X05       //������ֵ
#define PDU_READ_C_CURR   0X06       //������ֵ

#define PDU_READ_POWER  0X03       //���ܹ���
#define PDU_READ_ADDR   0X00       //������ַ


/********************************************************************************************************************
*                                            ������������������
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
