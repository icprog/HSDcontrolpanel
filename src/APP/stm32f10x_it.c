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
	while(1)	;						//��ѭ��
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
*	����1�����ж�
*******************************************************************************/
void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//�����ж�
	{
		INT8U i;

		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	    //����ж�
		if(gUartInfo[COM1].UartRLen<255)
		{
			i=gUartInfo[COM1].UartRLen;
			gUartInfo[COM1].UartRBuf[i]=USART_ReceiveData(USART1);
			gUartInfo[COM1].UartRLen++;
		}
		
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);				//��SR
		USART_ReceiveData(USART1);							//��DR
	}
}

/******************************************************************************
*	����2�����ж�
*******************************************************************************/

void USART2_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	    //����ж�
		if(gUartInfo[COM2].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM2].UartRLen;
			gUartInfo[COM2].UartRBuf[i]=USART_ReceiveData(USART2);
			gUartInfo[COM2].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);				//��SR
		USART_ReceiveData(USART2);							//��DR
	}
}

/******************************************************************************
*	����3�����ж�
*******************************************************************************/

void USART3_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		INT16U i;

		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	    //����ж�
		if(gUartInfo[COM3].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM3].UartRLen;
			gUartInfo[COM3].UartRBuf[i]=USART_ReceiveData(USART3);
			gUartInfo[COM3].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);				//��SR
		USART_ReceiveData(USART3);							//��DR
	}
}


/******************************************************************************
*	����4�����ж�
*******************************************************************************/

void UART4_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(UART4,USART_IT_RXNE);	    //����ж�
		if(gUartInfo[COM4].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM4].UartRLen;
			gUartInfo[COM4].UartRBuf[i]=USART_ReceiveData(UART4);
			gUartInfo[COM4].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);				//��SR
		USART_ReceiveData(UART4);							//��DR
	}
}

/******************************************************************************
*	����5�����ж�
*******************************************************************************/

void UART5_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		INT8U i;

		USART_ClearITPendingBit(UART5,USART_IT_RXNE);	    //����ж�
		if(gUartInfo[COM5].UartRLen<COM_LEN_MAX)
		{
			i=gUartInfo[COM5].UartRLen;
			gUartInfo[COM5].UartRBuf[i]=USART_ReceiveData(UART5);
			gUartInfo[COM5].UartRLen++;
		}
	}
	 if(USART_GetFlagStatus(UART5,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_ORE);				//��SR
		USART_ReceiveData(UART5);							//��DR
	}
}


/******************************************************************************
*	��ʱ��2�ж�
*	�����ˢ����ʾ
*******************************************************************************/
unsigned char gSegShowArr[8]={0};							//8���������ʾ�ļĴ�������ȫ��ʹ��
INT16U gSysSetCurpwm=6000;										//ϵͳ���õĵ���PMW
INT16U gSysSetVolpwm=6000;										//ϵͳ���õĵ�ѹPMW

void TIM2_IRQHandler(void)
{
  	INT16U Capture = 0;									   	//��ʱ����
	INT8U Temp;												//��ʱ����
	INT8U TempDat;
	static INT8U  SegTurn;									//��¼����ѡ���ĸ������
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)			//�ж��Ƿ񱾴ζ�ʱ������ж�
  	{
//		//CLR_RESET;   	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);			//����жϱ�־λ
    	Capture = TIM_GetCapture1(TIM2);					//��ȡ��ʱ���ĵ�ǰֵ	   	
		TIM_SetCompare1(TIM2, Capture + 4);					//�����´εıȽ�ֵ
     
	  
	}
	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)			//�ж��Ƿ񱾴ζ�ʱ������ж�
  	{
    	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);			//����жϱ�־λ
    	Capture = TIM_GetCapture2(TIM2);					//��ȡ��ʱ���ĵ�ǰֵ
    	TIM_SetCompare2(TIM2, Capture + 49);				//�����´εıȽ�ֵ 10ms
		Capture = TIM_GetCapture3(TIM3);					//��ѹ�趨PWMֵ

		if(Capture == gSysSetVolpwm) 						//��������κθı�
		{
			//TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);		//�ر��ж�
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
			//TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);			//���ж�
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
			//TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);		   	//���ж�
		}
															//��������������
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)			//�ж��Ƿ񱾴ζ�ʱ������ж�
  	{
    	TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);			//����жϱ�־λ
    	Capture = TIM_GetCapture3(TIM2);					//��ȡ��ʱ���ĵ�ǰֵ
    	TIM_SetCompare3(TIM2, Capture + 51);				//�����´εıȽ�ֵ 10ms
	  	Capture = TIM_GetCapture4(TIM3);					//��ѹ�趨PWMֵ
		if(Capture == gSysSetCurpwm) 								//��������κθı�
		{
			//TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);		//�ر��ж�
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
			//TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);			//���ж�
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
			//TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);		   	//���ж�
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
  	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);			   	//��ȡ����
	if( (CanMessageBuff.InLen >= CANMESSAGEBUFFLEN)	//���������
	    ||(CanMessageBuff.InLen <= CanMessageBuff.OutLen ))	//��������
	{
		CanMessageBuff.OutLen = 0x0000;						//��ȡ����������
		CanMessageBuff.InLen  = 0x0000;						//���ջ���������
	}
	tmp =CanMessageBuff.InLen ;	//��ȡҪд���λ��
	CanMessageBuff.Buff[tmp].ExtID = RxMessage.ExtId;		//��ȡID��
	CanMessageBuff.Buff[tmp].DLC   = RxMessage.DLC;			//��ȡ���ݳ���
	CanMessageBuff.Buff[tmp].Data[0] = RxMessage.Data[0];	//��ȡ��������
	CanMessageBuff.Buff[tmp].Data[1] = RxMessage.Data[1];	//Ҳ�����ݲ���8�������ǿ���ȫ����ȡ
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
