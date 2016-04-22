/**
  ******************************************************************************
  * File Name          : USART.c
  * Date               : 25/08/2014 11:19:37
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#define READCARDThread_C
/* Includes ------------------------------------------------------------------*/
#include "ICCARD.h"
#include "global.h"
#include <stdlib.h>
#include <string.h>
/*define -----------------------------------------------------------------------*/
#define ReadCardTxFaultMaxCnt                 3
#define ReadCardRxFaultMaxCnt                 3
#define ReadCardTimeOutFaultMaxCnt        		 3
#define ReadCardRxDataTimeout                 100
#define ReadCardTxDataTimeout                 1000

#define icCard_UART  UART_5
/*variable---------------------------------------------------------------------*/

//static ReadCardEnumCommStatus ReadCardCommStatus;
//static uint8_t RxBuff[100];

#define RxBuffMaxLength 100
static uint8_t RxBuff[RxBuffMaxLength];

#define RXDATATIME 10

static uint16_t sTimeOut=0;
static uint8_t RxDataTimeOut=0;
static uint8_t RxDataReady=0; 
static uint8_t RxData[50];
static uint8_t RxBuffLength=0;
ReadCardDataStr ReadCardData;
EnumReaderCardCommCommand uReaderCardCommCommand;

static EnumReaderCardCommCommand uReadCardCommand;
/*function---------------------------------------------------------------------*/

#define ReadCardExcursionVariable              0x30

#define READCARD_SelectCardRxBuffLength  10
#define READCARD_SelectCardTxBuffLength  7
static uint8_t READCARD_SelectCardBuff[READCARD_SelectCardRxBuffLength]={0x02,0x00,0x02,0x51,0x37,0x03,0x65};

#define READCARD_AdvanceHandleTxBuffLength  17
#define READCARD_AdvanceHandleRxBuffLength  28
//static uint8_t READCARD_AdvanceHandleBuff[READCARD_AdvanceHandleTxBuffLength] = {0x02,0x00,0x09,0x51,0x36,0x20,0x10,0x10,0x08,0x10,0x10,0x08,0x03,0x4F};// Ԥ����
static uint8_t READCARD_AdvanceHandleBuff[READCARD_AdvanceHandleTxBuffLength] = {0x02,0x00,0x0C,0x51,0x36,0x20,0x10,0x10,0x08,0x10,0x10,0x08,0x56,0x34,0x12,0x03,0x4F};// Ԥ����


#define READCARD_StartChargeTxBuffLength 14
#define READCARD_StartChargeRxBuffLength 12
static uint8_t READCARD_StartChargeBuff[READCARD_StartChargeTxBuffLength] = {0x02,0x00,0x09,0x51,0x30,0x20,0x10,0x10,0x08,0x10,0x10,0x08,0x03,0x49}; // ��ʼ�ӵ�

#define READCARD_EndChargeTxBuffLength 18
#define READCARD_EndChargeRxBuffLength 12
static uint8_t  READCARD_EndChargeBuff[READCARD_EndChargeTxBuffLength] = { 0x02,0x00,0x0D,0x51,0x34,0x00,0x00,0x00,0x00,0x20,0x10,0x10,0x08,0x10,0x10,0x08,0x03,0x49};// �����ӵ�

#define READCARD_AddBusinessTxBuffLength 7
#define READCARD_AddBusinessRxBuffLength 50
static uint8_t READCARD_AddBusinessBuff[READCARD_AddBusinessTxBuffLength] = {0x02,0x00,0x02,0x51,0x35,0x03,0x67};// ���佻��

#define READCARD_ReturnCardTxBuffLength 7
#define READCARD_ReturnCardRxBuffLength 8
static uint8_t READCARD_ReturnCardBuff[READCARD_ReturnCardTxBuffLength] = {0x02,0x00,0x02,0x32,0x40,0x03,0x71};// ���佻��


////// ��ƵM1�������ַ���
#define READCARD_SelectM1CardRxBuffLength  8
#define READCARD_SelectM1CardTxBuffLength  7
static uint8_t READCARD_SelectM1CardBuff[READCARD_SelectM1CardRxBuffLength]={0x02,0x00,0x02,0x34,0x30,0x03,0x07};  // Ѱ��Ƶ��

#define READCARD_GetSerialNumRxBuffLength  12
#define READCARD_GetSerialNumTxBuffLength  7
static uint8_t READCARD_GetSerialNumBuff[READCARD_GetSerialNumRxBuffLength]={0x02,0x00,0x02,0x34,0x31,0x03,0x65};  // ��ȡ�����к�

#define READCARD_VerifyKeyARxBuffLength  9
#define READCARD_VerifyKeyATxBuffLength  14
static uint8_t READCARD_VerifyKeyABuff[READCARD_VerifyKeyATxBuffLength]={0x02,0x00,0x09,0x34,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // ��֤keyA����

#define READCARD_VerifyKeyBRxBuffLength  9
#define READCARD_VerifyKeyBTxBuffLength  14
static uint8_t READCARD_VerifyKeyBBuff[READCARD_VerifyKeyBTxBuffLength]={0x02,0x00,0x09,0x34,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // ��֤keyB����

#define READCARD_ReadSectionDataRxBuffLength  26
#define READCARD_ReadSectionDataTxBuffLength  9
static uint8_t READCARD_ReadSectionDataBuff[READCARD_ReadSectionDataRxBuffLength]={0x02,0x00,0x04,0x34,0x33,0x00,0x00,0x03,0x65};  // ������������

#define READCARD_WriteSectionDataRxBuffLength  26
#define READCARD_WriteSectionDataTxBuffLength  25
static uint8_t READCARD_WriteSectionDataBuff[READCARD_WriteSectionDataRxBuffLength]={0x02,0x00,0x14,0x34,0x34,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // д����������

#define READCARD_ModifyPSWRxBuffLength  9
#define READCARD_ModifyPSWTxBuffLength  14
static uint8_t READCARD_ModifyPSWBuff[READCARD_ModifyPSWTxBuffLength]={0x02,0x00,0x09,0x34,0x35,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // ��������

#define READCARD_InitValueRxBuffLength  10
#define READCARD_InitValueTxBuffLength  13
static uint8_t READCARD_InitValueBuff[READCARD_InitValueTxBuffLength]={0x02,0x00,0x08,0x34,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // ��ʼ��ֵ

#define READCARD_AddValueRxBuffLength  10
#define READCARD_AddValueTxBuffLength  13
static uint8_t READCARD_AddValueBuff[READCARD_AddValueTxBuffLength]={0x02,0x00,0x08,0x34,0x37,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  // ��ֵ����

#define READCARD_ReduceValueRxBuffLength  10
#define READCARD_ReduceValueTxBuffLength  13
static uint8_t READCARD_ReduceValueBuff[READCARD_ReduceValueTxBuffLength]={0x02,0x00,0x08,0x34,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x65};  //  ��ֵ����

////// end

//static EnumCommStatus CommStatus; 

void READCARDDataInit(void)
{ 
//    CommStatus = Standy_Status;
    uReaderCardCommCommand = READCARD_NothingDo;
    uReadCardCommand = READCARD_NothingDo;
    memset((uint8_t *)&ReadCardData.CardStatus.value,0,sizeof(ReadCardDataStr));
//    memset((uint8_t *)&ChargeData.Status.value,0,sizeof(ChargeDataStr));
    //PSAM������Ҫ���³�ʼ��
}

//// ���У��
static uint8_t CalXodCheck(uint8_t *buff, uint8_t length)
{
    uint8_t i=0;
    uint8_t retVal = 0;
    for (i=0;i<=length;i++)
    {
        retVal ^= buff[i];
    }
    return retVal;
}

//void ReadCard_read_callback(void)
//{
//    if (RxDataTimeOut==0)
//    {
//        RxBuffLength = 0;
//    }
//    RxBuff[RxBuffLength++] = RxData[0];
//    if (RxBuffLength >= RxBuffMaxLength)
//    {
//        RxBuffLength = 0;
//    }
//    RxDataTimeOut = RXDATATIME;  // 50ms����һ������
//    RxDataReady = 1;
//    HAL_UART_Receive_IT(&huart3,(uint8_t *)RxData,1);
//}

uint8_t READCARDReaderCommand(uint8_t* iCommand,uint8_t length,uint8_t rxDataLength)
{
	INT8U ret=0;
	INT16U len=0;
		
	gUserDB.cardCom_timeCnt++; 
	if(gUserDB.cardCom_timeCnt>10)
	{
	  gUserDB.cardCom_timeCnt =10;
	  gUserDB.cardComAlarm =ALARM;
	}

    ret=uart_write(UART_5,iCommand,length);
	
	  len=uart_read(icCard_UART,RxBuff,400);
		if(len>0)
		{
			      uint8_t buffCheck =0;
			      //ReadCardComm.TimeOutCnt = 0;
            RxDataReady = 0;
            //CommStatus = Standy_Status;
             
			        buffCheck=CalXodCheck(RxBuff,rxDataLength-2);
            if (buffCheck == RxBuff[rxDataLength-1] )
            {
                //ReadCardComm.RxFaultCnt = 0;
				gUserDB.cardCom_timeCnt =0;
				gUserDB.cardComAlarm =NOALARM;
                return true;
				
            }
            else
            {
                //ReadCardComm.RxFaultCnt ++; 
                return false;
            }
		}
		
		return false;		
}


static void GetDateTimeToTxBuff(uint8_t *buff)
{ 
    buff[0] = 0x20;
	  buff[1] = 0x16; 
    buff[2] = 0x01; 
	  buff[3] = 0x01; 
    buff[4] = 0x01; 
	  buff[5] =0x01; 
    buff[6] = 0x01; 
}

static void TimeOutCalc(void)
{
    if (RxDataTimeOut)
    {
        RxDataTimeOut--;
    }
    if (sTimeOut)
    {
        sTimeOut--;
    }
}
void LoadNo(uint8_t *dByte,uint8_t *sByte, uint8_t length )
{
	 uint8_t i;
    for ( i=0; i<length; i++)
    {
        *dByte =( (*sByte)>>4) + '0';
        *(dByte+1) =( (*sByte)&0x0F ) + '0';
        dByte+= 2;
        sByte+=1;
    }
}
void TaskIcCard(void  *argument)
{  
    uint32_t temp32;
    uint16_t temp16;
//  uint8_t buff[8];
    argument = argument;
	//SystemConfigerData.ICCardType = SZ_CARD; // test
	
    while (1)
    {
        OSTimeDlyHMSM(0,0,0,50);  
       // TimeOutCalc();
			
        //if ( CommStatus == Standy_Status && uReadCardCommand == READCARD_NothingDo)
        {
						if (gUserDB.cardReader_indicating==0x01 )
            {
                // if(uReadCardCommand != READCARD_AdvanceHandle)
//							  if(SystemConfigerData.ICCardType == SZ_CARD)  // ������
//								{//
//									uReadCardCommand = READCARD_SelectM1Card;
//								}
//								else
								{
									uReadCardCommand = READCARD_SelectCard;
                }
								gUserDB.cardReader_indicating = 0x11;
            }
            else if (gUserDB.cardReader_indicating==0x02 )
            {
                uReadCardCommand = READCARD_StartCharge;
                gUserDB.cardReader_indicating = 0x12;
            }
            else if (gUserDB.cardReader_indicating==0x03  )
            {
                uReadCardCommand = READCARD_EndCharge;
                gUserDB.cardReader_indicating = 0x13;
            }
            else if (gUserDB.cardReader_indicating==0x05  )
            {
							  ReadCardData.CardStatus.value=1;
                gUserDB.cardReader_indicating = 0x15;
                uReadCardCommand = READCARD_RetrunCard;
            }
            else if (gUserDB.cardReader_indicating==0x04  )
            {
                gUserDB.cardReader_indicating = 0x14;
                uReadCardCommand = READCARD_AddBusiness;
            }
        }
				
				
        switch (uReadCardCommand)
        {
        case READCARD_NoComm: // ��ʼ������ͨѶ
            {
//                if(SystemConfigerData.ICCardType == SZ_CARD)  // ������
//								{
//									uReadCardCommand = READCARD_SelectM1Card;
//								}
//								else
								{
									uReadCardCommand = READCARD_SelectCard;
                }
                break;
            }
        case READCARD_SelectCard: //Ѱ������
            {
							  ReadCardData.CardStatus.value =1; 
                if (READCARDReaderCommand(READCARD_SelectCardBuff,READCARD_SelectCardTxBuffLength,
                                          READCARD_SelectCardRxBuffLength)==true)
                {
                    ReadCardData.CardStatus.Bit.SelectCard = RxBuff[6]- ReadCardExcursionVariable;
                    ReadCardData.CardStatus.Bit.SystemCard = RxBuff[7]- ReadCardExcursionVariable; 
                    if (ReadCardData.CardStatus.Bit.SelectCard || ReadCardData.CardStatus.Bit.SystemCard)
                    {
                        gUserDB.cardReader_indicating = 0x21;
                        uReadCardCommand = READCARD_NothingDo;
                    }
                    else
                    {
                        uReadCardCommand = READCARD_AdvanceHandle;
											  //gUserDB.cardReader_indicating = 0x21;
                    }
                }
                break;
            }
        case READCARD_AdvanceHandle:// Ԥ����
            {
                GetDateTimeToTxBuff(&READCARD_AdvanceHandleBuff[5]);
                READCARD_AdvanceHandleBuff[READCARD_AdvanceHandleTxBuffLength-1] = 
                CalXodCheck(READCARD_AdvanceHandleBuff,READCARD_AdvanceHandleTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_AdvanceHandleBuff,READCARD_AdvanceHandleTxBuffLength,
                                          READCARD_AdvanceHandleRxBuffLength)==true)
                {
									  uint8_t i;
                    gUserDB.cardReader_indicating = 0x21;
                    uReadCardCommand = READCARD_NothingDo;
                    switch (RxBuff[5])
                    {
                    case 0x59:
                        ReadCardData.CardStatus.Bit.AdvanceHandle=0;
                        ReadCardData.CaseType = RxBuff[6];
                        ReadCardData.CardStatus.Bit.BusinessStatus = RxBuff[7] - ReadCardExcursionVariable;
//                      memcpy(ReadCardData.CardNo, &RxBuff[8], 8);
//                      memcpy(buff,&RxBuff[8],8);
                        LoadNo(ReadCardData.CardNo,&RxBuff[8],8);
                        temp32 = RxBuff[16];
                        for ( i=1;i<4;i++)
                        {
                            temp32<<=8;
                            temp32+=RxBuff[i+16];
                        }
                        ReadCardData.RemainMoney = temp32;

                        temp16 = RxBuff[20];
                        for ( i=1;i<2;i++)
                        {
                            temp16<<=8;
                            temp16+=RxBuff[i+20];
                        }
                        ReadCardData.TradeOffline = temp16; 

                        temp16 = RxBuff[22];
                        for ( i=1;i<2;i++)
                        {
                            temp16<<=8;
                            temp16+=RxBuff[i+22];
                        }
                        ReadCardData.TradeOnline = temp16;  
                        //	if(gUserDB.cardReader_indicating==0x11)
                        //	  gUserDB.cardReader_indicating = 0x21;
                        //	else if(gUserDB.cardReader_indicating==0x31)
                        //		gUserDB.cardReader_indicating = 0x23;

                        break;
                    case 0x4e:
                        ReadCardData.CardStatus.Bit.AdvanceHandle=1;
                        break;
                    case 0x47:
                        ReadCardData.CardStatus.Bit.ResetPSAM=1;
                        break;
                    case 0x48:
                        ReadCardData.CardStatus.Bit.ResetUserCard=1;
                        break;
                    case 0x57:
                    default:
                        ReadCardData.CardStatus.Bit.SelectCard = 0;
                        ReadCardData.CardStatus.Bit.SystemCard = 0;
                        break;
                    }
                }
                break;
            }
        case READCARD_StartCharge: // ��ʼ�ӵ�
            {
                GetDateTimeToTxBuff(&READCARD_StartChargeBuff[5]);
                READCARD_StartChargeBuff[READCARD_StartChargeTxBuffLength-1] = 
                CalXodCheck(READCARD_StartChargeBuff,READCARD_StartChargeTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_StartChargeBuff,READCARD_StartChargeTxBuffLength
                                          ,READCARD_StartChargeRxBuffLength)==true)
                {
									  uint8_t i;
                    gUserDB.cardReader_indicating = 0x22;
                    uReadCardCommand = READCARD_NothingDo;
                    switch (RxBuff[5])
                    {
                    case 0x59:
                        ReadCardData.CardStatus.Bit.ChargeStatus = 0;
//                        ChargeData.Status.Bit.ChargeFlag = 1;//��ʼ���
//                        ChargeData.ChargeStartTime = timestr2long(&READCARD_StartChargeBuff[6]);
//                        ChargeData.ChargeStartkWh = 100;

                        temp32 = RxBuff[6];
                        for ( i=1;i<4;i++)
                        {
                            temp32<<=8;
                            temp32+=RxBuff[i+16];
                        }
                        ReadCardData.RemainMoney = temp32;
////                      memcpy((uint8_t *)& ReadCardData.RemainMoney,&RxBuff[6],4);
                        break;
                    case 0x4e:
                        ReadCardData.CardStatus.Bit.ChargeStatus = 1;
                        break;
                    case 0x57:
                    default:
                        ReadCardData.CardStatus.Bit.SelectCard = 1;
                        break;
                    }
                }
                break;
            }
        case READCARD_EndCharge: // �����ӵ�
            {               

                READCARD_EndChargeBuff[5] = ReadCardData.ExpenseMoney>>24;
                READCARD_EndChargeBuff[6] = (ReadCardData.ExpenseMoney>>16)&0x000000ff;
                READCARD_EndChargeBuff[7] = (ReadCardData.ExpenseMoney>>8)&0x000000ff;
                READCARD_EndChargeBuff[8] = (ReadCardData.ExpenseMoney)&0x000000ff;
                GetDateTimeToTxBuff(&READCARD_EndChargeBuff[9]);

                READCARD_EndChargeBuff[READCARD_EndChargeTxBuffLength-1] = 
                CalXodCheck(READCARD_EndChargeBuff,READCARD_EndChargeTxBuffLength-2);

                if (READCARDReaderCommand(READCARD_EndChargeBuff,READCARD_EndChargeTxBuffLength
                                          ,READCARD_EndChargeRxBuffLength)==true)
                {
									  uint8_t i;
                    gUserDB.cardReader_indicating = 0x23;
                    uReadCardCommand = READCARD_NothingDo;
                    switch (RxBuff[5])
                    {
                    case 0x59:
                        uReadCardCommand = READCARD_AdvanceHandle;
                        ReadCardData.CardStatus.Bit.EndChargeStatus = 0;
//                        ChargeData.Status.Bit.ChargeFlag = 0;//�������
//                        ChargeData.ChargeEndTime = timestr2long(&READCARD_EndChargeBuff[10]);
//                        ChargeData.ChargeEndkWh = 200;
//                        if (ChargeData.ChargeEndTime >= ChargeData.ChargeStartTime)
//                        {
//                            ChargeData.ChargeTimeLength = (ChargeData.ChargeEndTime - ChargeData.ChargeStartTime)%60;
//                        }
//                        if (ChargeData.ChargeEndkWh >= ChargeData.ChargeStartkWh)
//                        {
//                            ChargeData.ChargekWh = ChargeData.ChargeEndkWh - ChargeData.ChargeStartkWh;
//                        }

                        temp32 = RxBuff[6];
                        for ( i=1;i<4;i++)
                        {
                            temp32<<=8;
                            temp32+=RxBuff[i+16];
                        }
                        ReadCardData.RemainMoney = temp32;
//                      memcpy((uint8_t *)& ReadCardData.RemainMoney,&RxBuff[6],4);
                        break;
                    case 0x4e:
                        ReadCardData.CardStatus.Bit.EndChargeStatus = 1;
                        break;
                    case 0x43:
                        ReadCardData.CardStatus.Bit.Unlock = 1;
                        break;
                    case 0x57:
                    default:
                        ReadCardData.CardStatus.Bit.SelectCard = 1;
                        break;
                    }
                    //gUserDB.cardReader_indicating = 0x23;
                }
                break;
            }
        case READCARD_AddBusiness:// ���佻��            
            {
                if (READCARDReaderCommand(READCARD_AddBusinessBuff,READCARD_AddBusinessTxBuffLength
                                          ,READCARD_AddBusinessRxBuffLength)==true)
                {
									  uint8_t i;
                    //gUserDB.cardReader_indicating = 0x10; 
                    uReadCardCommand = READCARD_NothingDo;
                    switch (RxBuff[5])
                    {
                    case 0x59:
//                      memcpy(ReadCardData.CardNo, &RxBuff[6], 8);

                        LoadNo(ReadCardData.CardNo,&RxBuff[6],8);
                        memcpy(ReadCardData.ICC, &RxBuff[14], 4);

                        temp16 = RxBuff[18];
                        for ( i=1;i<2;i++)
                        {
                            temp16<<=8;
                            temp16+=RxBuff[i+18];
                        }
                        ReadCardData.Serial = temp16;  

                        temp32 = RxBuff[20];
                        for ( i=1;i<4;i++)
                        {
                            temp32<<=8;
                            temp32+=RxBuff[i+20];
                        }
                        ReadCardData.RemainMoney = temp32;

                        temp32 =RxBuff[24];
                        for ( i=1;i<4;i++)
                        {
                            temp32<<=8;
                            temp32+=RxBuff[i+24];
                        }
                        ReadCardData.ExpenseMoney = temp32;

                        ReadCardData.BusinessType = RxBuff[28];
//                      memcpy((uint8_t *)& ReadCardData.EtermNo,&RxBuff[29],6);

                        LoadNo(ReadCardData.EtermNo,&RxBuff[29],6);

                        memcpy((uint8_t *)& ReadCardData.UnlockTime,&RxBuff[35],7);

                        temp16 = RxBuff[22];
                        for ( i=1;i<2;i++)
                        {
                            temp16<<=8;
                            temp16+=RxBuff[i+42];
                        }
                        ReadCardData.tserial = temp16;   
                        break;
                    case 0x4e:
                        break;
                    case 0x57:
                        break;
                    }
                    gUserDB.cardReader_indicating = 0x24;
                }
                break;
            }
        case READCARD_RetrunCard:
						{
								if (READCARDReaderCommand(READCARD_ReturnCardBuff,READCARD_ReturnCardTxBuffLength
																					,READCARD_ReturnCardRxBuffLength)==true)
								{
										uReadCardCommand = READCARD_NothingDo;
										gUserDB.cardReader_indicating = 0x25;
										switch (RxBuff[5])
										{
										case 0x30:
												ReadCardData.CardStatus.Bit.PopupCard=0;
												break;
										case 0x31:
												ReadCardData.CardStatus.Bit.PopupCard=1;
												break;
										}
										//gUserDB.cardReader_indicating = 0x25;
								}
						}
            break;
        case READCARD_NothingDo:       // ��Ѱ��
            break;
				/// ����Ϊ��ƵM1������
				case READCARD_SelectM1Card:    // Ѱ��Ƶ��
						{
							  ReadCardData.CardStatus.value =1; 
								if (READCARDReaderCommand(READCARD_SelectM1CardBuff,READCARD_SelectM1CardTxBuffLength,
																					READCARD_SelectM1CardRxBuffLength)==true)
								{
										if(RxBuff[5] == 0x59) // ��Y' �п�
										{
											ReadCardData.CardStatus.Bit.SelectCard = 0;
											ReadCardData.CardStatus.Bit.SystemCard = 0;
										}
										else
										{
											ReadCardData.CardStatus.Bit.SelectCard = 1;
											ReadCardData.CardStatus.Bit.SystemCard = 1;
										}
										if (ReadCardData.CardStatus.Bit.SelectCard || ReadCardData.CardStatus.Bit.SystemCard)
										{
												gUserDB.cardReader_indicating = 0x21; // Ѱ������
												uReadCardCommand = READCARD_NothingDo;
										}
										else
										{
												uReadCardCommand = READCARD_GetSerialNum;
										}
								}break;
						}
				case READCARD_GetSerialNum:    // ��ȡ�����к�
						{
                READCARD_GetSerialNumBuff[READCARD_GetSerialNumTxBuffLength-1] = 
                CalXodCheck(READCARD_GetSerialNumBuff,READCARD_GetSerialNumTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_GetSerialNumBuff,READCARD_GetSerialNumTxBuffLength,
                                          READCARD_GetSerialNumRxBuffLength)==true)
                {
                    INT8U IcCardSerial[4];  // �����к�
									
										if(RxBuff[5] == 0x59) // ��Y' �ɹ�
										{
											ReadCardData.CardStatus.Bit_M1.GetSerialFail = 0;
											uReadCardCommand = READCARD_VerifyKeyA;
										}
										else
										{
											ReadCardData.CardStatus.Bit_M1.GetSerialFail = 1;
											gUserDB.cardReader_indicating = 0x21;  // Ѱ������
											uReadCardCommand = READCARD_NothingDo;
										}	
										memcpy(IcCardSerial,&RxBuff[6],4);
                }
                break;
						}
				case READCARD_VerifyKeyA:      // ��֤keyA����
						{
                INT8U keyAPsw[6];      // keyA����
							  INT8U SectionNum = 0;  // ������
							  memset(keyAPsw,0xFF,6);
							
							  memcpy(&READCARD_VerifyKeyABuff[6],keyAPsw,6); // ����
							  READCARD_VerifyKeyABuff[5] = SectionNum;       // ����
							
								READCARD_VerifyKeyABuff[READCARD_VerifyKeyATxBuffLength-1] = 
                CalXodCheck(READCARD_VerifyKeyABuff,READCARD_VerifyKeyATxBuffLength-2);
                if (READCARDReaderCommand(READCARD_VerifyKeyABuff,READCARD_VerifyKeyATxBuffLength,
                                          READCARD_VerifyKeyARxBuffLength)==true)
                {									
										if(RxBuff[5] == SectionNum)  // ����������ͬ
										{
											if(RxBuff[6] == 0x59)      // ��Y' ��֤�ɹ�
											{
												ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 0;
												uReadCardCommand = READCARD_ReadSectionData;
											}
											else if(RxBuff[6] == 0x33)
											{
												ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;
												gUserDB.cardReader_indicating = 0x21;   // �������
												uReadCardCommand = READCARD_NothingDo;
											}
											else
											{
												ReadCardData.CardStatus.Bit.SelectCard = 1;
												gUserDB.cardReader_indicating = 0x21;  // Ѱ������
												uReadCardCommand = READCARD_NothingDo;
											}
										}
										else
										{
											ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;	
											gUserDB.cardReader_indicating = 0x21;  // Ѱ������
											uReadCardCommand = READCARD_NothingDo;
										}
                }
                break;
						}
				case READCARD_VerifyKeyB:      // ��֤keyB����
						{
                INT8U keyAPsw[6];      // keyA����
							  INT8U SectionNum = 0;  // ������
							  memset(keyAPsw,0xFF,6);
							
							  memcpy(&READCARD_VerifyKeyBBuff[6],keyAPsw,6); // ����
							  READCARD_VerifyKeyBBuff[5] = SectionNum;       // ����
							
								READCARD_VerifyKeyBBuff[READCARD_VerifyKeyBTxBuffLength-1] = 
                CalXodCheck(READCARD_VerifyKeyBBuff,READCARD_VerifyKeyBTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_VerifyKeyBBuff,READCARD_VerifyKeyBTxBuffLength,
                                          READCARD_VerifyKeyBRxBuffLength)==true)
                {									
										if(RxBuff[5] == SectionNum)  // ����������ͬ
										{
											if(RxBuff[6] == 0x59)      // ��Y' ��֤�ɹ�
											{
												ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 0;
												uReadCardCommand = READCARD_ReadSectionData;
											}
											else if(RxBuff[6] == 0x33)
											{
												ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;
												gUserDB.cardReader_indicating = 0x21;  // �������
												uReadCardCommand = READCARD_NothingDo;
											}
											else
											{
												ReadCardData.CardStatus.Bit.SelectCard = 1;
												gUserDB.cardReader_indicating = 0x21;  // Ѱ������
												uReadCardCommand = READCARD_NothingDo;
											}
										}
										else
										{
											ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;	
											gUserDB.cardReader_indicating = 0x21;  // Ѱ������
											uReadCardCommand = READCARD_NothingDo;
										}
                }
                break;
						}
				case READCARD_ReadSectionData: // ����������
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U BlockNum = 0;    // ���
							
							  READCARD_ReadSectionDataBuff[5] = SectionNum;   // ����
							  READCARD_ReadSectionDataBuff[6] = BlockNum;     // ���
							
								READCARD_ReadSectionDataBuff[READCARD_ReadSectionDataTxBuffLength-1] = 
                CalXodCheck(READCARD_ReadSectionDataBuff,READCARD_ReadSectionDataTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_ReadSectionDataBuff,READCARD_ReadSectionDataTxBuffLength,
                                          READCARD_ReadSectionDataRxBuffLength)==true)
                {																		
									  if((RxBuff[5] == SectionNum)&&(RxBuff[6] == BlockNum))  // ��������/����ͬ
										{
											switch(RxBuff[7])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.ReadSectionFail = 0;
													memcpy(ReadCardData.CardNo,&RxBuff[8],16);  // ��ȡ����
													
													ReadCardData.CardStatus.Bit.AdvanceHandle=0;
													ReadCardData.CaseType = 1;
													ReadCardData.CardStatus.Bit.BusinessStatus = 0;
												}break;
												case 0x30:
												{
													ReadCardData.CardStatus.Bit.SelectCard = 1;  // Ѱ������
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;  // ������������/����֤��
												}break;
												case 0x32:
												{
													ReadCardData.CardStatus.Bit_M1.CardSerialErr = 1;  // ���������кŴ�
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;  // ������֤��
												}break;
												case 0x34:
												{
												 ReadCardData.CardStatus.Bit_M1.ReadSectionFail = 1; // �����ݴ�
												}break;												
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;    // Ѱ������
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
				case READCARD_WriteSectionData:    // д��������
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U BlockNum = 0;    // ���
							  INT8U tempbuff[16];
							  memset(tempbuff,0,16);
							
							  READCARD_WriteSectionDataBuff[5] = SectionNum;   // ����
							  READCARD_WriteSectionDataBuff[6] = BlockNum;     // ���
							  memcpy(&READCARD_WriteSectionDataBuff[7],tempbuff,sizeof(tempbuff));  // �������
							
								READCARD_WriteSectionDataBuff[READCARD_WriteSectionDataTxBuffLength-1] = 
                CalXodCheck(READCARD_WriteSectionDataBuff,READCARD_WriteSectionDataTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_WriteSectionDataBuff,READCARD_WriteSectionDataTxBuffLength,
                                          READCARD_WriteSectionDataRxBuffLength)==true)
                {																		
									  if((RxBuff[5] == SectionNum)&&(RxBuff[6] == BlockNum))  // ��������/����ͬ
										{
											switch(RxBuff[7])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.WriteSectionFail = 0;    // д���ݳɹ�
												}break;
												case 0x30:
												{
													ReadCardData.CardStatus.Bit.SelectCard = 1;  // Ѱ������
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;  // ������������/����֤��
												}break;
												case 0x32:
												{
													ReadCardData.CardStatus.Bit_M1.CardSerialErr = 1;  // ���������кŴ�
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;   // ������֤��
												}break;
												case 0x34:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyDataFail = 1;   //  У��д������ݴ�
												}break;												
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
				case READCARD_ModifyPSW:    // ��������,���޸�ֻ�ܸ�keyA
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U tempbuff[6];
							  memset(tempbuff,0,6);
							
							  READCARD_ModifyPSWBuff[5] = SectionNum;   // ����
							  memcpy(&READCARD_ModifyPSWBuff[6],tempbuff,sizeof(tempbuff));  // �������
							
								READCARD_ModifyPSWBuff[READCARD_ModifyPSWTxBuffLength-1] = 
                CalXodCheck(READCARD_ModifyPSWBuff,READCARD_ModifyPSWTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_ModifyPSWBuff,READCARD_ModifyPSWTxBuffLength,
                                          READCARD_ModifyPSWRxBuffLength)==true)
                {																		
									  if(RxBuff[5] == SectionNum)  // ����������ͬ
										{
											switch(RxBuff[6])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.ModifyPswFail = 0;    // ������ɹ�
												}break;
												case 0x30:
												{
													ReadCardData.CardStatus.Bit.SelectCard = 1;  // Ѱ������
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;  // ������������/����֤��
												}break;
												case 0x32:
												{
													ReadCardData.CardStatus.Bit_M1.CardSerialErr = 1;  // ���������кŴ�
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;   // ������֤��
												}break;										
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
				case READCARD_InitValue:    // ��ʼ��ֵ
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U BlockNum = 0;    // ���
							  INT8U tempbuff[4];
							  memset(tempbuff,0,4);
							
							  READCARD_InitValueBuff[5] = SectionNum;   // ����
							  READCARD_InitValueBuff[6] = BlockNum;     // ���
							  memcpy(&READCARD_InitValueBuff[7],tempbuff,sizeof(tempbuff));  // �������
							
								READCARD_InitValueBuff[READCARD_InitValueTxBuffLength-1] = 
                CalXodCheck(READCARD_InitValueBuff,READCARD_InitValueTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_InitValueBuff,READCARD_InitValueTxBuffLength,
                                          READCARD_InitValueRxBuffLength)==true)
                {																		
									  if((RxBuff[5] == SectionNum)&&(RxBuff[6] == BlockNum))  // ��������/����ͬ
										{
											switch(RxBuff[7])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.InitValueFail = 0;  // �����ɹ�
												}break;
												case 0x4E:
												{
													ReadCardData.CardStatus.Bit_M1.InitValueFail = 1;  // ����ʧ��
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;  // ������������/����֤��
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;   // ������֤��
												}break;										
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
				case READCARD_AddValue:    // ��ֵ����
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U BlockNum = 0;    // ���
							  INT8U tempbuff[4];
							  memset(tempbuff,0,4);
							
							  READCARD_AddValueBuff[5] = SectionNum;   // ����
							  READCARD_AddValueBuff[6] = BlockNum;     // ���
							  memcpy(&READCARD_AddValueBuff[7],tempbuff,sizeof(tempbuff));  // �������
							
								READCARD_AddValueBuff[READCARD_AddValueTxBuffLength-1] = 
                CalXodCheck(READCARD_AddValueBuff,READCARD_AddValueTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_AddValueBuff,READCARD_AddValueTxBuffLength,
                                          READCARD_AddValueRxBuffLength)==true)
                {																		
									  if((RxBuff[5] == SectionNum)&&(RxBuff[6] == BlockNum))  // ��������/����ͬ
										{
											switch(RxBuff[7])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.AddValueFail = 0;    // �����ɹ�
												}break;
												case 0x30:
												{
													ReadCardData.CardStatus.Bit.SelectCard = 1;  // Ѱ������
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;  // ������������/����֤��
												}break;
												case 0x32:
												{
													ReadCardData.CardStatus.Bit_M1.CardSerialErr = 1;  // ���������кŴ�
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;   // ������֤��
												}break;		
												case 0x34:
												{
												 ReadCardData.CardStatus.Bit_M1.BlockFormatFail = 1;   // �����ݸ�ʽ��
												}break;	
												case 0x35:
												{
												 ReadCardData.CardStatus.Bit_M1.AddDataOverflow = 1;   // ��ֵ���
												}break;	
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
				case READCARD_ReduceValue:    // ��ֵ����
						{
							  INT8U SectionNum = 0;  // ������
							  INT8U BlockNum = 0;    // ���
							  INT8U tempbuff[4];
							  memset(tempbuff,0,4);
							
							  READCARD_ReduceValueBuff[5] = SectionNum;   // ����
							  READCARD_ReduceValueBuff[6] = BlockNum;     // ���
							  memcpy(&READCARD_ReduceValueBuff[7],tempbuff,sizeof(tempbuff));  // �������
							
								READCARD_ReduceValueBuff[READCARD_ReduceValueTxBuffLength-1] = 
                CalXodCheck(READCARD_ReduceValueBuff,READCARD_ReduceValueTxBuffLength-2);
                if (READCARDReaderCommand(READCARD_ReduceValueBuff,READCARD_ReduceValueTxBuffLength,
                                          READCARD_ReduceValueRxBuffLength)==true)
                {																		
									  if((RxBuff[5] == SectionNum)&&(RxBuff[6] == BlockNum))  // ��������/����ͬ
										{
											switch(RxBuff[7])
											{
												case 0x59:    // �ɹ�
												{
													ReadCardData.CardStatus.Bit_M1.ReduceValueFail = 0;  // �����ɹ�
												}break;
												case 0x30:
												{
													ReadCardData.CardStatus.Bit.SelectCard = 1;       // Ѱ������
												}break;
												case 0x31:
												{
													ReadCardData.CardStatus.Bit_M1.SectionNumErr = 1;    // ������������/����֤��
												}break;
												case 0x32:
												{
													ReadCardData.CardStatus.Bit_M1.CardSerialErr = 1;    // ���������кŴ�
												}break;
												case 0x33:
												{
												 ReadCardData.CardStatus.Bit_M1.VerifyPswFail = 1;     // ������֤��
												}break;		
												case 0x34:
												{
												 ReadCardData.CardStatus.Bit_M1.BlockFormatFail = 1;   // �����ݸ�ʽ��
												}break;	
												case 0x35:
												{
												 ReadCardData.CardStatus.Bit_M1.ReduceDataOverflow = 1;// ��ֵ���
												}break;	
												default:
													break;
											}											
										}
										gUserDB.cardReader_indicating = 0x21;
										uReadCardCommand = READCARD_NothingDo;
                }
                break;
						}
        }
				

//        if (ReadCardComm.TxFaultCnt >= ReadCardTxFaultMaxCnt)
//        {
//            ReadCardComm.TxFaultCnt = ReadCardTxFaultMaxCnt;
//            ReadCardComm.ErrCode = TxFault;
//        }
//        else if (ReadCardComm.RxFaultCnt >= ReadCardRxFaultMaxCnt)
//        {
//            ReadCardComm.RxFaultCnt = ReadCardRxFaultMaxCnt;
//            ReadCardComm.ErrCode = RxFault;
//        }
//        else if (ReadCardComm.TimeOutCnt >= ReadCardTimeOutFaultMaxCnt)
//        {
//            ReadCardComm.TimeOutCnt = ReadCardTimeOutFaultMaxCnt;
//            ReadCardComm.ErrCode = TimeOut;
//        }
//        else
//        {
//            ReadCardComm.ErrCode = 0;
//        }
//        if (ReadCardComm.ErrCode != NoErr)
//        {
//            ReadCardData.bak1 = 1; // ͨѶ�쳣
//            if ((gUserDB.cardReader_indicating & 0xF0) == 0x10)
//                gUserDB.cardReader_indicating += 0x10;
//        }
//        else
//        {
//            ReadCardData.bak1 = 0;
//        }
    }
}
