#include "stm32f10x.h"
#include  <ucos_ii.h>
#include "BSP.h"
#include"controller.h"
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
volatile TestStatus TestRx;

void CAN_Initial()
{
	GPIO_InitTypeDef GPIO_InitStructure; 					//声明 GPIO 的结构
  	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  	CanTxMsg TxMessage;
  	CanRxMsg RxMessage;
  	u32 i = 0;
  	u8 TransmitMailbox;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//在使用GPIO之前必须允许相应端的时钟.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 	//在使用GPIO之前必须允许相应端的时钟.

  	/* Configure CAN pin: RX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//悬空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  	/* Configure CAN pin: TX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	  //开漏输出
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* CAN register init */
  	CAN_DeInit(CAN1);
  	CAN_StructInit(&CAN_InitStructure);

  	/* CAN cell init */
  	CAN_InitStructure.CAN_TTCM=DISABLE;
  	CAN_InitStructure.CAN_ABOM=DISABLE;
  	CAN_InitStructure.CAN_AWUM=DISABLE;
  	CAN_InitStructure.CAN_NART=DISABLE;
  	CAN_InitStructure.CAN_RFLM=DISABLE;
  	CAN_InitStructure.CAN_TXFP=DISABLE;
  	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;			 //测试使用
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
  	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  	CAN_InitStructure.CAN_Prescaler=9;
  	CAN_Init(CAN1,&CAN_InitStructure);	//36MHz/9/（1+8+7）=250K

  	/* CAN filter0 init */
  	CAN_FilterInitStructure.CAN_FilterNumber=0;
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  	CAN_FilterInit(&CAN_FilterInitStructure);
  	/* CAN filter1 init */
  	CAN_FilterInitStructure.CAN_FilterNumber=1;
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  	CAN_FilterInit(&CAN_FilterInitStructure);
	/* CAN filter2 init */
	//......................
	/* CAN filter13 init */

}

unsigned char CAN_ClearRecBuff(unsigned char Type)//0-标准，1-扩展
{
  TxMessage.StdId=0x11; 
  TxMessage.ExtId=0x00;
  RxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  RxMessage.DLC=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;
  TxMessage.Data[0]=0xCA;
  TxMessage.Data[1]=0xFE;
  TxMessage.Data[2]=0x01;
  TxMessage.Data[3]=0x02;
  TxMessage.Data[4]=0x03;
  TxMessage.Data[5]=0x04;
  TxMessage.Data[6]=0x05;
  TxMessage.Data[7]++;


}
TestStatus CAN_Polling(void)
{

  /* transmit */
  TxMessage.StdId=0x11;
   TxMessage.ExtId=0x2312745;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  //TxMessage.RTR=CAN_RTR_DATA;
  //TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=8;
  TxMessage.Data[0]=0xCA;
  TxMessage.Data[1]=0xFE;
  TxMessage.Data[2]=0x01;
  TxMessage.Data[3]=0x02;
  TxMessage.Data[4]=0x03;
  TxMessage.Data[5]=0x04;
  TxMessage.Data[6]=0x05;
  TxMessage.Data[7]++;

  TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
//  TxMessage.ExtId=0x3127452;
  TxMessage.Data[7]++;
 // TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
//  TxMessage.Data[7]++;
//  TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
  {
    i++;
  }
  /* receive */
  TxMessage.ExtId=0x00;
  RxMessage.IDE=CAN_ID_EXT;
  RxMessage.DLC=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
// TxMessage.ExtId=0x00;
//  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);


//  if (RxMessage.StdId!=0x11) return FAILED;
  if (RxMessage.ExtId!=0x2312745) return FAILED;
  if (RxMessage.IDE!=CAN_ID_STD) return FAILED;

  if (RxMessage.DLC!=2) return FAILED;

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE) return FAILED;
  
  return PASSED; /* Test Passed */
}