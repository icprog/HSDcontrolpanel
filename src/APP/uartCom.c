/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "uartcom.h"


/********************************************************************************************************************
*                                            ������������������
*********************************************************************************************************************/
//INT8U  gModbusRecArr[20];				//���ڽ������ݵ���ʱ�Ĵ���
//INT16U  gModbusRecNum;					//���յ������ݸ��������50
void UartSendByte(INT8U lSend);			//�������͵����ֽں���
void ModbusCommunication(void);			//����MODBUSͨѶ������
void UART_Configuration(void);
void UART2_Configuration(void);
void UART3_Configuration(void);

void UartSendArr(INT8U *ptrSend,INT16U length) ;
void GPIO_UART_Configuration_RS485EN(void);

void UART2_SendByte(INT8U lSend); 

extern uint16  pdu_rxdata_len;

/*********************************************************************************************************
��������: 	GPIO_UART_Configuration_RS485 
��������:  485����ʹ�� ��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void GPIO_UART_Configuration_RS485EN(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						//��GPIOAʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_1 | GPIO_Pin_8 ;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//�������-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9 ;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//�������-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9 ;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//�������-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3 ;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//�������-TX
    GPIO_Init(GPIOD, &GPIO_InitStructure);


	SET_RS485_1_REC	;	    //��ʼ��Ϊ����״̬
	SET_RS485_2_REC	;	    //��ʼ��Ϊ����״̬
	SET_RS485_3_REC	;	    //��ʼ��Ϊ����״̬
	SET_RS485_4_REC	;	    //��ʼ��Ϊ����״̬
	SET_RS485_5_REC	;		 //��ʼ��Ϊ����״̬
}
/*********************************************************************************************************
��������: 	UART_Configuration 
��������:  ����1��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void UART_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 						//�򿪴���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//��GPIOAʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//���սӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART1_BPS;	 						//���ò�����
	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//��������λ
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);													//ʹ�ܴ���
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		   						//���ж�
}

/*********************************************************************************************************
��������: 	UART2_Configuration 
��������:  ����2��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/	  
void UART2_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 						//�򿪴���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//��GPIOAʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3;							//���սӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART2_BPS;	 						//���ò�����
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//��������λ ��żУ������λ9,��У������λΪ8
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);													//ʹ�ܴ���
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		   						//���ж�
}

/*********************************************************************************************************
��������: 	UART3_Configuration 
��������:  ����3��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/	
void UART3_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 						//�򿪴���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						//��GPIOAʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_11;							//���սӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART3_BPS;	 						//���ò�����
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//��������λ
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);													//ʹ�ܴ���
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		   						//���ж�
}

/*********************************************************************************************************
��������: 	UART4_Configuration 
��������:  ����4��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/	
void UART4_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 						//�򿪴���4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 						//��GPIOCʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_11;							//���սӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART4_BPS;	 						//���ò�����
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//��������λ
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
    USART_Init(UART4, &USART_InitStructure);

    USART_Cmd(UART4, ENABLE);													//ʹ�ܴ���
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);		   						//���ж�
}


/*********************************************************************************************************
��������: 	UART5_Configuration 
��������:  ����5��ʼ��

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/	
void UART5_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= UART5_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 						//�򿪴���5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 						//��GPIOCʱ��
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_12;							//���ͽӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//���սӿڶ�Ӧ����
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART5_BPS;	 						//���ò�����
	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//��������λ
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
    USART_Init(UART5, &USART_InitStructure);

    USART_Cmd(UART5, ENABLE);													//ʹ�ܴ���
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);		   						    //���ж�
}


/*********************************************************************************************************
��������: 	UartSendByte 
��������:  ����1����һ���ֽ�

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/

void UartSendByte(INT8U lSend)  
{
    USART_SendData(USART1, lSend); 												//���ͺ���
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );				//�ȴ��������
}
/*********************************************************************************************************
��������: 	UART2_SendByte 
��������:  ����2����һ���ֽ�

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void UART2_SendByte(INT8U lSend)  
{
    USART_SendData(USART2, lSend); 												//���ͺ���
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );				//�ȴ�������� USART_FLAG_TXE
}

/*********************************************************************************************************
��������: 	UART3_SendByte 
��������:  ����3����һ���ֽ�

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void UART3_SendByte(INT8U lSend)  
{
    USART_SendData(USART3, lSend); 												//���ͺ���
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );				//�ȴ��������
}

/*********************************************************************************************************
��������: 	UART3_SendByte 
��������:  ����3����һ���ֽ�

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void UART4_SendByte(INT8U lSend)  
{
    USART_SendData(UART4, lSend); 												//���ͺ���
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );				//�ȴ��������
}

/**********************************************************
* ���ڷ���һ������
* ����UartSendByte����
***********************************************************/
/*
void UartSendArr(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	//SET_RS485_1_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UartSendByte(*(ptrSend++));							//��������
	}
	//SET_RS485_1_REC;
}
*/
/*********************************************************************************************************
��������: 	UartSendArr 
��������:  ����2����

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void uart2_write(INT8U *ptrSend,INT16U length) 
//void UART2_SendArr(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	SET_RS485_2_SEND;
//	SET_LED_2_ON
	for(tmp = 0; tmp < length; tmp++)
	{
		UART2_SendByte(*(ptrSend++));							//��������
	}
	SET_RS485_2_REC;
//	SET_LED_2_OFF
}

/*********************************************************************************************************
��������: 	UART3_SendArr 
��������:  ����3����

�䡡��:      ��			

�䡡��:   	 ��										     
*********************************************************************************************************/
void uart3_write(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	SET_RS485_3_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UART3_SendByte(*(ptrSend++));							//��������
	}
	SET_RS485_3_REC;
}

/*******************************************************************************************
��������: uart_write
��    ��: ���ڷ���
�������: porID ���ں�  buf ���ͻ���   length�����ַ�����
�������: ��
��    ��: ��
********************************************************************************************/
INT8U uart_write(INT8U porID,INT8U * buf,INT8U length)
{
	INT8U tmp=0;

	switch(	porID)
	{
	case  COM1: //����1 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_1_SEND;
		gUartInfo[COM1].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART1, *buf++); 												//���ͺ���
        	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );				    //�ȴ��������
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_1_REC;
		break;
	case COM2:	 //����2 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_2_SEND;
		gUartInfo[COM2].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART2, *buf++); 												//���ͺ���
        	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );				    //�ȴ��������
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_2_REC;
		break;
	case COM3:	 //����3 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_3_SEND ;
		gUartInfo[COM3].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART3, *buf++); 												//���ͺ���
        	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );				    //�ȴ��������

			if(tmp>255)
			  break;
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_3_REC	;
		break;
	case COM4:	 //����4 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_4_SEND ;
		gUartInfo[COM4].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(UART4, *buf++); 												//���ͺ���
        	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );				    //�ȴ��������
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_4_REC	;
		break;
	case COM5:	 //����4 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_5_SEND ;
		gUartInfo[COM5].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(UART5, *buf++); 												//���ͺ���
        	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );				    //�ȴ��������
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_5_REC	;
		break;
	default:
		return 0;

	}

	return 1;
}

/*******************************************************************************************
��������: uart_read
��    ��: ���ڷ���
�������: 	porID ���ں�   timeout���ճ�ʱʱ��(ms)
�������: 	buf������� 
��    ��: 	�������ݳ���
********************************************************************************************/
INT16U uart_read(INT8U porID,INT8U * buf,INT16U timeout)
{  	
	 INT16U len=0;
//	INT8U i;
	static INT16U lastCom1Len=0,lastCom2Len=0,lastCom3Len=0 ,lastCom4Len=0,lastCom5Len=0;  //�ϴν������ݵı���
						
   timeout =timeout/20;
   

   switch(porID)
   {
   case COM1: //����1
		while(timeout--)  //��ʱ����յ��������˳�
		{ 										
			if(gUartInfo[COM1].UartRLen != lastCom1Len && gUartInfo[COM1].UartRLen!=0 )					//���β���ͬ���������ڽ������ݣ�����δ���
			{
	   			lastCom1Len	= gUartInfo[COM1].UartRLen;				   	                                //���汸�����ݣ����´ζԱ�
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM1].UartRLen!=0)                    //���ݳ��Ȳ�Ϊ��
	      		{
					len=gUartInfo[COM1].UartRLen;                    //���յ�����
					memcpy(buf,gUartInfo[COM1].UartRBuf,len);
					lastCom1Len=0;                                    //����
					gUartInfo[COM1].UartRLen=0;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,20);
		}
		break;
	case COM2:	 //����2
		while(timeout--)  //��ʱ����յ��������˳�
		{ 										
			if(gUartInfo[COM2].UartRLen != lastCom2Len && gUartInfo[COM2].UartRLen!=0 )					    //���β���ͬ���������ڽ������ݣ�����δ���
			{
	   			lastCom2Len	= gUartInfo[COM2].UartRLen;				   	                                //���汸�����ݣ����´ζԱ�
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM2].UartRLen!=0)                  //���ݳ��Ȳ�Ϊ��
	      		{
					len=gUartInfo[COM2].UartRLen;                 //���յ�����
					memcpy(buf,gUartInfo[COM2].UartRBuf,len);
					lastCom2Len=0;                                 //����
					gUartInfo[COM2].UartRLen=0;
					pdu_rxdata_len=len;
					break;
				}
	   			else
	      			;
			}

			OSTimeDlyHMSM(0,0,0,100);
		}
		break;	
	case COM3:	//����3
		while(timeout--)  //��ʱ����յ��������˳�
		{ 										
			if(gUartInfo[COM3].UartRLen != lastCom3Len && gUartInfo[COM3].UartRLen!=0 )					//���β���ͬ���������ڽ������ݣ�����δ���
			{
	   			lastCom3Len	= gUartInfo[COM3].UartRLen;				   	                                //���汸�����ݣ����´ζԱ�
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM3].UartRLen!=0)                  //���ݳ��Ȳ�Ϊ��
	      		{
					len=gUartInfo[COM3].UartRLen;                 //���յ�����
					memcpy(buf,gUartInfo[COM3].UartRBuf,len);
					lastCom3Len=0;                                 //����
					gUartInfo[COM3].UartRLen=0;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,100);
		}
		break;
	case COM4:	//����3
		while(timeout--)  //��ʱ����յ��������˳�
		{ 										
			if(gUartInfo[COM4].UartRLen != lastCom4Len && gUartInfo[COM4].UartRLen!=0 )					//���β���ͬ���������ڽ������ݣ�����δ���
			{
	   			lastCom4Len	= gUartInfo[COM4].UartRLen;				   	                                //���汸�����ݣ����´ζԱ�
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM4].UartRLen!=0)                  //���ݳ��Ȳ�Ϊ��
	      		{
					len=gUartInfo[COM4].UartRLen;                 //���յ�����
					memcpy(buf,gUartInfo[COM4].UartRBuf,len);
					lastCom4Len=0;                                 //����
					gUartInfo[COM4].UartRLen=0;
					pdu_rxdata_len=len;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,100);
		}
		break;
	case COM5:	//����3
		while(timeout--)  //��ʱ����յ��������˳�
		{ 										
			if(gUartInfo[COM5].UartRLen != lastCom5Len && gUartInfo[COM5].UartRLen!=0 )					//���β���ͬ���������ڽ������ݣ�����δ���
			{
	   			lastCom5Len	= gUartInfo[COM5].UartRLen;				   	                                //���汸�����ݣ����´ζԱ�
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM5].UartRLen!=0)                  //���ݳ��Ȳ�Ϊ��
	      		{
					len=gUartInfo[COM5].UartRLen;                 //���յ�����
					memcpy(buf,gUartInfo[COM5].UartRBuf,len);
					lastCom5Len=0;                                 //����
					gUartInfo[COM5].UartRLen=0;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,20);
		}
		break;
	}	


	return len;
}




