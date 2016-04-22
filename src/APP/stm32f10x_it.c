/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "Global.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
//	SegShowData(888,1);
	while(1)	;						//死循环
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
 // SegShowData(888,2);
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  //SegShowData(888,3);
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  //SegShowData(888,4);
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  //SegShowData(888,5);
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR */
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

/******************************************************************************
*	串口1接收中断
*******************************************************************************/
void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//接收中断
	{
		INT8U i;

		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	    //清除中断
		if(gUartInfo[COM1].UartRLen<255)
		{
			i=gUartInfo[COM1].UartRLen;
			gUartInfo[COM1].UartRBuf[i]=USART_ReceiveData(USART1);
			gUartInfo[COM1].UartRLen++;
		}
		
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);				//读SR
		USART_ReceiveData(USART1);							//读DR
	}
}

/******************************************************************************
*	串口2接收中断
*******************************************************************************/

void USART2_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	    //清除中断
		if(gUartInfo[COM2].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM2].UartRLen;
			gUartInfo[COM2].UartRBuf[i]=USART_ReceiveData(USART2);
			gUartInfo[COM2].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);				//读SR
		USART_ReceiveData(USART2);							//读DR
	}
}

/******************************************************************************
*	串口3接收中断
*******************************************************************************/

void USART3_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		INT16U i;

		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	    //清除中断
		if(gUartInfo[COM3].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM3].UartRLen;
			gUartInfo[COM3].UartRBuf[i]=USART_ReceiveData(USART3);
			gUartInfo[COM3].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);				//读SR
		USART_ReceiveData(USART3);							//读DR
	}
}


/******************************************************************************
*	串口4接收中断
*******************************************************************************/

void UART4_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(UART4,USART_IT_RXNE);	    //清除中断
		if(gUartInfo[COM4].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM4].UartRLen;
			gUartInfo[COM4].UartRBuf[i]=USART_ReceiveData(UART4);
			gUartInfo[COM4].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);				//读SR
		USART_ReceiveData(UART4);							//读DR
	}
}

/******************************************************************************
*	串口5接收中断
*******************************************************************************/

void UART5_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(UART5,USART_IT_RXNE);	    //清除中断
		if(gUartInfo[COM5].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM5].UartRLen;
			gUartInfo[COM5].UartRBuf[i]=USART_ReceiveData(UART5);
			gUartInfo[COM5].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(UART5,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_ORE);				//读SR
		USART_ReceiveData(UART5);							//读DR
	}
}


/******************************************************************************
*	定时器2中断
*	数码管刷新显示
*******************************************************************************/
unsigned char gSegShowArr[8]={0};							//8个数码管显示的寄存器，供全局使用
INT16U gSysSetCurpwm=6000;										//系统设置的电流PMW
INT16U gSysSetVolpwm=6000;										//系统设置的电压PMW

void TIM2_IRQHandler(void)
{
  	INT16U Capture = 0;									   	//临时变量
	INT8U Temp;												//临时变量
	INT8U TempDat;
	static INT8U  SegTurn;									//记录本次选择哪个数码管
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)			//判断是否本次定时引起的中断
  	{
//		//CLR_RESET;   	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);			//清除中断标志位
    	Capture = TIM_GetCapture1(TIM2);					//获取定时器的当前值	   	
		TIM_SetCompare1(TIM2, Capture + 4);					//设置下次的比较值
     
	  
	}
	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)			//判断是否本次定时引起的中断
  	{
    	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);			//清除中断标志位
    	Capture = TIM_GetCapture2(TIM2);					//获取定时器的当前值
    	TIM_SetCompare2(TIM2, Capture + 49);				//设置下次的比较值 10ms
		Capture = TIM_GetCapture3(TIM3);					//电压设定PWM值

		if(Capture == gSysSetVolpwm) 						//相等则不作任何改变
		{
			//TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);		//关闭中断
		}
		else if(Capture > gSysSetVolpwm)
		{
			if((Capture - gSysSetVolpwm) >	60) 
			{	
				TIM_SetCompare3(TIM3, Capture -60);
			}
			else	
			{
				TIM_SetCompare3(TIM3, gSysSetVolpwm);
			}
			//TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);			//打开中断
		}
		else
		{
			if((gSysSetVolpwm - Capture) >	60) 
			{	
				TIM_SetCompare3(TIM3, Capture +60);
			}
			else
			{
				TIM_SetCompare3(TIM3, gSysSetVolpwm);
			}
			//TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);		   	//打开中断
		}
															//电流软启动部分
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)			//判断是否本次定时引起的中断
  	{
    	TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);			//清除中断标志位
    	Capture = TIM_GetCapture3(TIM2);					//获取定时器的当前值
    	TIM_SetCompare3(TIM2, Capture + 51);				//设置下次的比较值 10ms
	  	Capture = TIM_GetCapture4(TIM3);					//电压设定PWM值
		if(Capture == gSysSetCurpwm) 								//相等则不作任何改变
		{
			//TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);		//关闭中断
		}
		else if(Capture > gSysSetCurpwm)
		{
			if((Capture - gSysSetCurpwm) >	60) 
			{	
				TIM_SetCompare4(TIM3, Capture -60);
			}
			else	
			{
				TIM_SetCompare4(TIM3, gSysSetCurpwm);
			}
			//TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);			//打开中断
		}
		else
		{
			if((gSysSetCurpwm - Capture) >	60) 
			{	
				TIM_SetCompare4(TIM3, Capture +60);
			}
			else
			{
				TIM_SetCompare4(TIM3, gSysSetCurpwm);
			}
			//TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);		   	//打开中断
		}
	}			 
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    INT8U tmp;
	CanRxMsg RxMessage;
  	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);			   	//读取数据
	if( (CanMessageBuff.InLen >= CANMESSAGEBUFFLEN)	//缓冲区溢出
	    ||(CanMessageBuff.InLen <= CanMessageBuff.OutLen ))	//缓冲区空
	{
		CanMessageBuff.OutLen = 0x0000;						//读取缓冲区置零
		CanMessageBuff.InLen  = 0x0000;						//接收缓冲区置零
	}
	tmp =CanMessageBuff.InLen ;	//获取要写入的位置
	CanMessageBuff.Buff[tmp].ExtID = RxMessage.ExtId;		//读取ID号
	CanMessageBuff.Buff[tmp].DLC   = RxMessage.DLC;			//读取数据长度
	CanMessageBuff.Buff[tmp].Data[0] = RxMessage.Data[0];	//读取数据内容
	CanMessageBuff.Buff[tmp].Data[1] = RxMessage.Data[1];	//也许数据不足8个，但是可以全部读取
	CanMessageBuff.Buff[tmp].Data[2] = RxMessage.Data[2];
	CanMessageBuff.Buff[tmp].Data[3] = RxMessage.Data[3];
	CanMessageBuff.Buff[tmp].Data[4] = RxMessage.Data[4];
	CanMessageBuff.Buff[tmp].Data[5] = RxMessage.Data[5];
	CanMessageBuff.Buff[tmp].Data[6] = RxMessage.Data[6];
	CanMessageBuff.Buff[tmp].Data[7] = RxMessage.Data[7];
	CanMessageBuff.InLen ++	;
}

/*void PPP_IRQHandler(void)
{
}
*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
