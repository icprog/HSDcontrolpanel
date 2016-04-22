#ifndef  __UARTCOM_H__
#define  __UARTCOM_H__
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/

#include "includes.h"

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/

#define USART1_BPS				9600						//���ڲ�����
#define USART2_BPS				1200						//���ڲ�����
#define USART3_BPS				2400						//���ڲ�����
#define USART4_BPS				2400						//���ڲ�����
#define USART5_BPS				115200						//���ڲ�����

#define  SET_RS485_1_REC	   GPIO_SetBits(GPIOA, GPIO_Pin_8);		//����rs485����״̬	
#define  SET_RS485_1_SEND	   GPIO_ResetBits(GPIOA, GPIO_Pin_8);		//����rs485����״̬

#define  SET_RS485_2_REC	    GPIO_SetBits(GPIOA, GPIO_Pin_1);		//����rs485����״̬	
#define  SET_RS485_2_SEND		GPIO_ResetBits(GPIOA, GPIO_Pin_1);		//����rs485����״̬


#define  SET_RS485_3_REC 	     GPIO_SetBits(GPIOB, GPIO_Pin_9);		//����rs485����״̬	
#define SET_RS485_3_SEND		 GPIO_ResetBits(GPIOB, GPIO_Pin_9);		//����rs485����״̬

#define  SET_RS485_4_REC	    GPIO_SetBits(GPIOC, GPIO_Pin_9);		//����rs485����״̬	
#define  SET_RS485_4_SEND		GPIO_ResetBits(GPIOC, GPIO_Pin_9);		//����rs485����״̬

#define  SET_RS485_5_REC	    GPIO_SetBits(GPIOD, GPIO_Pin_3);		//����rs485����״̬	
#define SET_RS485_5_SEND		GPIO_ResetBits(GPIOD, GPIO_Pin_3);		//����rs485����״̬






extern INT8U  gModbusRecArr[100];									//���ڽ������ݵ���ʱ�Ĵ���
//extern INT16U  gModbusRecNum;										//���յ������ݸ��������50
extern void uart2_write(INT8U *ptrSend,INT16U length); 
extern void uart3_write(INT8U *ptrSend,INT16U length); 
extern void GPIO_UART_Configuration_RS485(void);
extern void UART2_Configuration(void);
extern void UART_Configuration(void);
extern void UART3_Configuration(void);
extern void UART4_Configuration(void);
extern void UART5_Configuration(void);


#endif

