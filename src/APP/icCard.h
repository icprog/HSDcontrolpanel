#ifndef _TASK_IcCard_H
#define _TASK_IcCard_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include "ucos_ii.h"
#include "stm32f10x.h"
#include "global.h"
#include "includes.h"
	

#define true 1
#define false 0
/*Typedef--------------------------------------------------------------------*/

////  IC������
typedef enum
{
	 NOMAL_CARD,     // ��ͨ��
	 SZ_CARD,        // ������

}IC_CARD_TYPE;
//������ͨѶ�������� 
    typedef enum
    {
        READCARD_NoComm, // ��ʼ������ͨѶ
        READCARD_SelectCard,//Ѱ������
        READCARD_AdvanceHandle,// Ԥ����
        READCARD_StartCharge, // ��ʼ�ӵ�
        READCARD_EndCharge, // �����ӵ�
        READCARD_AddBusiness,// ���佻��
        READCARD_RetrunCard,//�˿�
        READCARD_NothingDo,//
			
				// add by TQL 20150818  ��ƵM1��
			  READCARD_SelectM1Card,  // ѰM1������
				READCARD_GetSerialNum,  // ��ȡ���к�
				READCARD_VerifyKeyA,    // ��֤keyA����
				READCARD_VerifyKeyB,    // ��֤keyB����
				READCARD_ReadSectionData,    // ������������
			  READCARD_WriteSectionData,   // д����������
				READCARD_ModifyPSW,          // �޸�����
				READCARD_InitValue,          // ��ʼ��ֵ
				READCARD_AddValue,           // ��ֵ����
				READCARD_ReduceValue,        // ��ֵ����
				// end
    } EnumReaderCardCommCommand;     

    typedef struct
    {
        union 
        {
            uint16_t value;
            struct
            {
                uint16_t SelectCard:1;     //  Bit0��0���������п�1���������޿���
                uint16_t SystemCard:1;     //  Bit1��0��ϵͳ��1����ϵͳ����
                uint16_t AdvanceHandle:1;  //  Bit2��0��Ԥ����ɹ���1��Ԥ����ʧ��   
                uint16_t ResetPSAM:1;      //  Bit3��0��PSAM��λ�ɹ���1��PSAM��λʧ��
                uint16_t ResetUserCard:1;  //  Bit4��0���û�����λ�ɹ���1���û�����λʧ��
                uint16_t BusinessStatus:2; //  Bit5��Bit6��00�������������Լӵ磩��01���ҿ������Խ����ӵ磩��10�������������в��佻�ף�
                uint16_t ChargeStatus:1;   //  Bit7��0���ӵ�ɹ���1���ӵ�ʧ��
                uint16_t EndChargeStatus:1;//  Bit8��0�������ӵ�ɹ���1�������ӵ�ʧ��
                uint16_t Unlock:1;         //  Bit9��0�������ɹ���1������ʧ��
                uint16_t AddBusiness:1;    //  Bit10��0�����佻�׳ɹ���1�����佻��ʧ��
                uint16_t CalcMAC2:1;       //  Bit11��0������MAC2�ɹ���1������MAC2ʧ��
                uint16_t GetRand:1;        //  Bit12��0��ȡ������ɹ���1��ȡ�����ʧ��
                uint16_t UpdateMAC:1;      //  Bit13��0��������Կ�ɹ���1��������Կʧ��
                uint16_t PopupCard:1;      //  Bit14��0�������ɹ���1������ʧ��
							  uint16_t PwdERR:1;         //  Bit15��0�������ɹ���1������ʧ��
							  
            }Bit;
						struct
            {
							  //// ��ƵM1��״̬ add by TQL 20150819
							  uint16_t SelectCard:1;     //  Bit0��0���������п�1���������޿���
                uint16_t SystemCard:1;     //  Bit1��0��ϵͳ��1����ϵͳ����
							  uint16_t GetSerialFail:1;  //  Bit2��0: ��ȡ���кųɹ� 1:ʧ��
							  uint16_t VerifyPswFail:1;  //  Bit3��0: ��֤����ɹ� 1:ʧ��
							  uint16_t ReadSectionFail:1;//  Bit4��0: ��ȡ�������ݳɹ� 1:ʧ��
							  uint16_t WriteSectionFail:1;// Bit5��0: д�������ݳɹ� 1:ʧ��
							  uint16_t SectionNumErr:1;  //  Bit6��0: ��������ȷ  1:ʧ��
							  uint16_t CardSerialErr:1;  //  Bit7��0: �����к���ȷ 1:ʧ��
							  uint16_t VerifyDataFail:1; //  Bit8��0: У�����ݳɹ�1:ʧ��
								uint16_t ModifyPswFail:1;  //  Bit9��0: ��������ɹ� 1:ʧ��
								uint16_t InitValueFail:1;  //  Bit10��0: ��ʼ��ֵ�ɹ� 1:ʧ��
								uint16_t BlockFormatFail:1;//  Bit11��0: ���ʽ��ȷ 1:ʧ��
								uint16_t AddValueFail:1;   //  Bit12��0: ��ֵ�����ɹ� 1:ʧ��
								uint16_t AddDataOverflow:1;//  Bit13��0: ��ֵ��� 1:ʧ��
								uint16_t ReduceValueFail:1;//  Bit14��0: ��ֵ�����ɹ�1:ʧ��
								uint16_t ReduceDataOverflow:1;//  Bit15��0: ��ֵ��� 1:ʧ��
            }Bit_M1;
        }CardStatus;
        uint16_t bak1;              //Ԥ��
        uint32_t RemainMoney; //���ڽ��0.01Ԫ/λ
        uint32_t BlockMoney; // ������0.01Ԫ/λ
        uint32_t ReturnMoney; //�ⶳ���0.01Ԫ/λ
        uint32_t Availablemoney; //�������0.01Ԫ/λ
        uint16_t TradeOffline; // �������״���
        uint16_t TradeOnline; //�ѻ����״���
        uint16_t CheckPSAMNo;// PSAM ��������
        uint8_t CardNo[16]; //����
        uint16_t CaseType; // ������ 0x01 �û�����ʶ������û�����ʹ�á���ʼ�ӵ硱���������ӵ硱��
        //��ʵ��������������
        //0x02 ��Կ����ʶ�������Կ����ʹ�á���Կ�޸ġ������޸ĳ��
        //׮ESAM ģ������Կ��
        //0x41 ���ÿ������޸���Կǰʹ�ã��������������б�׼����ȡ��ʹ
        //�ñ�����
        //0x51 Ա���������׮Ա��ʹ�ã����׮ʶ�����Ա�������ɿ�ʼ��
        //�磬û�л�����������̡�
        //0x61 ��ֵ����ʹ�÷���ͬ�û�����
        //0x11 ���ܲ��Կ���ʶ����ǹ��ܲ��Կ������׮����Ӧ�Ķ�����
        //0x12 ���ϼ��޿���ʶ����ǹ��ϼ��޿������׮����Ӧ���Լ춯����
        //0x00 ����״̬��Ϊ0x4E ʱΪ0x00
        uint32_t ExpenseMoney;//���ѽ��
        uint8_t ICC[4];                 //α�������4 ���ֽ�
        uint16_t Serial;            //����Ǯ�������������
        uint16_t BusinessType; //�������ͱ�ʾ
        uint8_t EtermNo[12];  //׮���
        uint8_t UnlockTime[8];//����ʱ��
        uint16_t tserial;           //�ѻ�������� 
        uint16_t NormalCard;// 
    }ReadCardDataStr;

    typedef struct
    {
        union 
        {
            uint16_t value;
            struct
            {
                uint16_t ChargeFlag:1;//����־
                uint16_t :15;
            }Bit;
        }Status;//����״̬
        uint16_t ChargeTimeLength; 
        uint32_t ChargeStartTime;
        uint32_t ChargeEndTime;
        uint32_t ChargeStartkWh;
        uint32_t ChargeEndkWh;
        uint32_t ChargekWh;
    }ChargeDataStr;
/*function---------------------------------------------------------------------*/
    void READCARDDataInit(void);
//  uint8_t Uart3Command(EnumReaderCardCommCommand uUart3Command);
    void ReadCardThread(void const *argument);
/*variable---------------------------------------------------------------------*/
   // extern osThreadId ReadCardThreadHandle;// ˢ����ͨѶ���
   // extern CommStr ReadCardComm;
    extern ReadCardDataStr ReadCardData;
    extern ChargeDataStr ChargeData;
    extern  EnumReaderCardCommCommand uReaderCardCommCommand;
   // extern RTC_HandleTypeDef hrtc;	






#ifdef  __cplusplus
}
#endif
#endif
