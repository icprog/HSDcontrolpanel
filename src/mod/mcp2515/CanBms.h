#pragma once
//#include "stdafx.h"
//#include <afxmt.h>
//#include "datatypeConfig.h"
#include "MCP2515.h"
//#include "canbusBms.h"

// CCanBms


	extern uint8 Can_Receive0(uint8 ID[],uint8 rx_buff[],uint8 *ptr);
	extern uint8 Can_Receive1(uint8 ID[],uint8 rx_buff[],uint8 *ptr);
	extern void DecodeCCSMessage(uint8 *pu8DecodeCCSMesg, uint8 u8CCSID);
	extern void Can_Send(uint8 ID[],uint8 tx_buff[],uint8 num,uint8 Fream);
	extern void DecodeBMSMessage(uint8 *pu8DecodeBMSMesg, uint8 u8ID);
	extern void TaskCanBusRev(void);
	extern void TaskCanBusSend(void);
	extern void SPIInit(void);
	extern void Reset_2515(void);
	extern void CanInit(void);
	extern void MODIFY_2515(uint8 Addr, uint8 MASK, uint8 dat);
	extern uint8 Read_BYTE_2515(uint8 Addr);
	extern void Write_BYTE_2515(uint8 Addr,uint8 dat);
	extern uint8 Read_SPI(void);
	extern uint8 Write_SPI(uint8 Write_Byte);
//	extern HANDLE m_hFileSpi;
//	extern HANDLE m_hFileSpi_Eint;

	extern uint8 Task_Init(void);
	extern uint8 CS_enable(uint8 enable);
	extern void delay(int us);
	extern uint8 waitSpiRecciveIntr(void);
	extern void closeSpi(void);

	//CSemaphore *semaphore_spi;
	extern void BmsComAffairManage(void);
	extern void data_callback(uint8 cmd);
	extern void bms_pdu_format_init(void);
	extern void CCS_info(void);



