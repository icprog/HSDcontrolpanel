#ifndef  __UARTCOM_H__
#define  __UARTCOM_H__
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/

#include "includes.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/

#define USART1_BPS				9600						//串口波特率
#define USART2_BPS				1200						//串口波特率
#define USART3_BPS				2400						//串口波特率
#define USART4_BPS				2400						//串口波特率
#define USART5_BPS				115200						//串口波特率

#define  SET_RS485_1_REC	   GPIO_SetBits(GPIOA, GPIO_Pin_8);		//设置rs485发送状态	
#define  SET_RS485_1_SEND	   GPIO_ResetBits(GPIOA, GPIO_Pin_8);		//设置rs485接收状态

#define  SET_RS485_2_REC	    GPIO_SetBits(GPIOA, GPIO_Pin_1);		//设置rs485发送状态	
#define  SET_RS485_2_SEND		GPIO_ResetBits(GPIOA, GPIO_Pin_1);		//设置rs485接收状态


#define  SET_RS485_3_REC 	     GPIO_SetBits(GPIOB, GPIO_Pin_9);		//设置rs485发送状态	
#define SET_RS485_3_SEND		 GPIO_ResetBits(GPIOB, GPIO_Pin_9);		//设置rs485接收状态

#define  SET_RS485_4_REC	    GPIO_SetBits(GPIOC, GPIO_Pin_9);		//设置rs485发送状态	
#define  SET_RS485_4_SEND		GPIO_ResetBits(GPIOC, GPIO_Pin_9);		//设置rs485接收状态

#define  SET_RS485_5_REC	    GPIO_SetBits(GPIOD, GPIO_Pin_3);		//设置rs485发送状态	
#define SET_RS485_5_SEND		GPIO_ResetBits(GPIOD, GPIO_Pin_3);		//设置rs485接收状态






extern INT8U  gModbusRecArr[100];									//串口接收数据的临时寄存器
//extern INT16U  gModbusRecNum;										//接收到的数据个数，最大50
extern void uart2_write(INT8U *ptrSend,INT16U length); 
extern void uart3_write(INT8U *ptrSend,INT16U length); 
extern void GPIO_UART_Configuration_RS485(void);
extern void UART2_Configuration(void);
extern void UART_Configuration(void);
extern void UART3_Configuration(void);
extern void UART4_Configuration(void);
extern void UART5_Configuration(void);


#endif

