/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "uartcom.h"


/********************************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************************/
//INT8U  gModbusRecArr[20];				//串口接收数据的临时寄存器
//INT16U  gModbusRecNum;					//接收到的数据个数，最大50
void UartSendByte(INT8U lSend);			//声明发送单个字节函数
void ModbusCommunication(void);			//声明MODBUS通讯处理函数
void UART_Configuration(void);
void UART2_Configuration(void);
void UART3_Configuration(void);

void UartSendArr(INT8U *ptrSend,INT16U length) ;
void GPIO_UART_Configuration_RS485EN(void);

void UART2_SendByte(INT8U lSend); 

extern uint16  pdu_rxdata_len;

/*********************************************************************************************************
函数名称: 	GPIO_UART_Configuration_RS485 
功能描述:  485串口使能 初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void GPIO_UART_Configuration_RS485EN(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//打开GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						//打开GPIOA时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_1 | GPIO_Pin_8 ;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//推挽输出-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9 ;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//推挽输出-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9 ;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//推挽输出-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3 ;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;						//推挽输出-TX
    GPIO_Init(GPIOD, &GPIO_InitStructure);


	SET_RS485_1_REC	;	    //初始化为接收状态
	SET_RS485_2_REC	;	    //初始化为接收状态
	SET_RS485_3_REC	;	    //初始化为接收状态
	SET_RS485_4_REC	;	    //初始化为接收状态
	SET_RS485_5_REC	;		 //初始化为接收状态
}
/*********************************************************************************************************
函数名称: 	UART_Configuration 
功能描述:  串口1初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void UART_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 						//打开串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//打开GPIOA时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//接收接口对应引脚
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART1_BPS;	 						//设置波特率
	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//设置数据位
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);													//使能串口
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		   						//开中断
}

/*********************************************************************************************************
函数名称: 	UART2_Configuration 
功能描述:  串口2初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/	  
void UART2_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 						//打开串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						//打开GPIOA时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3;							//接收接口对应引脚
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART2_BPS;	 						//设置波特率
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//设置数据位 奇偶校验数据位9,无校验数据位为8
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);													//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		   						//开中断
}

/*********************************************************************************************************
函数名称: 	UART3_Configuration 
功能描述:  串口3初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/	
void UART3_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 						//打开串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						//打开GPIOA时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_11;							//接收接口对应引脚
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART3_BPS;	 						//设置波特率
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//设置数据位
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);													//使能串口
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		   						//开中断
}

/*********************************************************************************************************
函数名称: 	UART4_Configuration 
功能描述:  串口4初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/	
void UART4_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 						//打开串口4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 						//打开GPIOC时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_11;							//接收接口对应引脚
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART4_BPS;	 						//设置波特率
	USART_InitStructure.USART_WordLength= USART_WordLength_9b; 					//设置数据位
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
	USART_InitStructure.USART_Parity 	= USART_Parity_Even;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
    USART_Init(UART4, &USART_InitStructure);

    USART_Cmd(UART4, ENABLE);													//使能串口
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);		   						//开中断
}


/*********************************************************************************************************
函数名称: 	UART5_Configuration 
功能描述:  串口5初始化

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/	
void UART5_Configuration(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= UART5_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 						//打开串口5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 						//打开GPIOC时钟
		 
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_12;							//发送接口对应引脚
    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//接收接口对应引脚
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate 	= USART5_BPS;	 						//设置波特率
	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//设置数据位
	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
    USART_Init(UART5, &USART_InitStructure);

    USART_Cmd(UART5, ENABLE);													//使能串口
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);		   						    //开中断
}


/*********************************************************************************************************
函数名称: 	UartSendByte 
功能描述:  串口1发送一个字节

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/

void UartSendByte(INT8U lSend)  
{
    USART_SendData(USART1, lSend); 												//发送函数
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );				//等待发送完毕
}
/*********************************************************************************************************
函数名称: 	UART2_SendByte 
功能描述:  串口2发送一个字节

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void UART2_SendByte(INT8U lSend)  
{
    USART_SendData(USART2, lSend); 												//发送函数
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );				//等待发送完毕 USART_FLAG_TXE
}

/*********************************************************************************************************
函数名称: 	UART3_SendByte 
功能描述:  串口3发送一个字节

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void UART3_SendByte(INT8U lSend)  
{
    USART_SendData(USART3, lSend); 												//发送函数
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );				//等待发送完毕
}

/*********************************************************************************************************
函数名称: 	UART3_SendByte 
功能描述:  串口3发送一个字节

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void UART4_SendByte(INT8U lSend)  
{
    USART_SendData(UART4, lSend); 												//发送函数
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );				//等待发送完毕
}

/**********************************************************
* 串口发送一个数组
* 调用UartSendByte函数
***********************************************************/
/*
void UartSendArr(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	//SET_RS485_1_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UartSendByte(*(ptrSend++));							//发送数据
	}
	//SET_RS485_1_REC;
}
*/
/*********************************************************************************************************
函数名称: 	UartSendArr 
功能描述:  串口2发送

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void uart2_write(INT8U *ptrSend,INT16U length) 
//void UART2_SendArr(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	SET_RS485_2_SEND;
//	SET_LED_2_ON
	for(tmp = 0; tmp < length; tmp++)
	{
		UART2_SendByte(*(ptrSend++));							//发送数据
	}
	SET_RS485_2_REC;
//	SET_LED_2_OFF
}

/*********************************************************************************************************
函数名称: 	UART3_SendArr 
功能描述:  串口3发送

输　入:      无			

输　出:   	 无										     
*********************************************************************************************************/
void uart3_write(INT8U *ptrSend,INT16U length) 
{
	INT16U tmp=0;
	SET_RS485_3_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UART3_SendByte(*(ptrSend++));							//发送数据
	}
	SET_RS485_3_REC;
}

/*******************************************************************************************
函数名称: uart_write
描    述: 串口发送
输入参数: porID 串口号  buf 发送缓冲   length发送字符长度
输出参数: 无
返    回: 无
********************************************************************************************/
INT8U uart_write(INT8U porID,INT8U * buf,INT8U length)
{
	INT8U tmp=0;

	switch(	porID)
	{
	case  COM1: //串口1 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_1_SEND;
		gUartInfo[COM1].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART1, *buf++); 												//发送函数
        	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );				    //等待发送完毕
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_1_REC;
		break;
	case COM2:	 //串口2 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_2_SEND;
		gUartInfo[COM2].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART2, *buf++); 												//发送函数
        	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );				    //等待发送完毕
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_2_REC;
		break;
	case COM3:	 //串口3 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_3_SEND ;
		gUartInfo[COM3].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(USART3, *buf++); 												//发送函数
        	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );				    //等待发送完毕

			if(tmp>255)
			  break;
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_3_REC	;
		break;
	case COM4:	 //串口4 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_4_SEND ;
		gUartInfo[COM4].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(UART4, *buf++); 												//发送函数
        	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );				    //等待发送完毕
		}
		//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		SET_RS485_4_REC	;
		break;
	case COM5:	 //串口4 

		//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
		SET_RS485_5_SEND ;
		gUartInfo[COM5].UartRLen=0;
		for(tmp = 0; tmp < length; tmp++)
		{
			USART_SendData(UART5, *buf++); 												//发送函数
        	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );				    //等待发送完毕
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
函数名称: uart_read
描    述: 串口发送
输入参数: 	porID 串口号   timeout接收超时时间(ms)
输出参数: 	buf输出缓冲 
返    回: 	接收数据长度
********************************************************************************************/
INT16U uart_read(INT8U porID,INT8U * buf,INT16U timeout)
{  	
	 INT16U len=0;
//	INT8U i;
	static INT16U lastCom1Len=0,lastCom2Len=0,lastCom3Len=0 ,lastCom4Len=0,lastCom5Len=0;  //上次接收数据的备份
						
   timeout =timeout/20;
   

   switch(porID)
   {
   case COM1: //串口1
		while(timeout--)  //超时或接收到数据则退出
		{ 										
			if(gUartInfo[COM1].UartRLen != lastCom1Len && gUartInfo[COM1].UartRLen!=0 )					//两次不相同，表明正在接收数据，接收未完成
			{
	   			lastCom1Len	= gUartInfo[COM1].UartRLen;				   	                                //保存备份数据，供下次对比
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM1].UartRLen!=0)                    //数据长度不为零
	      		{
					len=gUartInfo[COM1].UartRLen;                    //接收到数据
					memcpy(buf,gUartInfo[COM1].UartRBuf,len);
					lastCom1Len=0;                                    //清零
					gUartInfo[COM1].UartRLen=0;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,20);
		}
		break;
	case COM2:	 //串口2
		while(timeout--)  //超时或接收到数据则退出
		{ 										
			if(gUartInfo[COM2].UartRLen != lastCom2Len && gUartInfo[COM2].UartRLen!=0 )					    //两次不相同，表明正在接收数据，接收未完成
			{
	   			lastCom2Len	= gUartInfo[COM2].UartRLen;				   	                                //保存备份数据，供下次对比
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM2].UartRLen!=0)                  //数据长度不为零
	      		{
					len=gUartInfo[COM2].UartRLen;                 //接收到数据
					memcpy(buf,gUartInfo[COM2].UartRBuf,len);
					lastCom2Len=0;                                 //清零
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
	case COM3:	//串口3
		while(timeout--)  //超时或接收到数据则退出
		{ 										
			if(gUartInfo[COM3].UartRLen != lastCom3Len && gUartInfo[COM3].UartRLen!=0 )					//两次不相同，表明正在接收数据，接收未完成
			{
	   			lastCom3Len	= gUartInfo[COM3].UartRLen;				   	                                //保存备份数据，供下次对比
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM3].UartRLen!=0)                  //数据长度不为零
	      		{
					len=gUartInfo[COM3].UartRLen;                 //接收到数据
					memcpy(buf,gUartInfo[COM3].UartRBuf,len);
					lastCom3Len=0;                                 //清零
					gUartInfo[COM3].UartRLen=0;
					break;
				}
	   			else
	      			;
			}
			OSTimeDlyHMSM(0,0,0,100);
		}
		break;
	case COM4:	//串口3
		while(timeout--)  //超时或接收到数据则退出
		{ 										
			if(gUartInfo[COM4].UartRLen != lastCom4Len && gUartInfo[COM4].UartRLen!=0 )					//两次不相同，表明正在接收数据，接收未完成
			{
	   			lastCom4Len	= gUartInfo[COM4].UartRLen;				   	                                //保存备份数据，供下次对比
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM4].UartRLen!=0)                  //数据长度不为零
	      		{
					len=gUartInfo[COM4].UartRLen;                 //接收到数据
					memcpy(buf,gUartInfo[COM4].UartRBuf,len);
					lastCom4Len=0;                                 //清零
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
	case COM5:	//串口3
		while(timeout--)  //超时或接收到数据则退出
		{ 										
			if(gUartInfo[COM5].UartRLen != lastCom5Len && gUartInfo[COM5].UartRLen!=0 )					//两次不相同，表明正在接收数据，接收未完成
			{
	   			lastCom5Len	= gUartInfo[COM5].UartRLen;				   	                                //保存备份数据，供下次对比
	   			//return ;
			}
			else
			{
	   			if( gUartInfo[COM5].UartRLen!=0)                  //数据长度不为零
	      		{
					len=gUartInfo[COM5].UartRLen;                 //接收到数据
					memcpy(buf,gUartInfo[COM5].UartRBuf,len);
					lastCom5Len=0;                                 //清零
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




