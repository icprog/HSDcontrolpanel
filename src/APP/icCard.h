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

////  IC卡类型
typedef enum
{
	 NOMAL_CARD,     // 普通卡
	 SZ_CARD,        // 市政卡

}IC_CARD_TYPE;
//读卡器通讯命令类型 
    typedef enum
    {
        READCARD_NoComm, // 初始化，不通讯
        READCARD_SelectCard,//寻卡命令
        READCARD_AdvanceHandle,// 预处理
        READCARD_StartCharge, // 开始加电
        READCARD_EndCharge, // 结束加电
        READCARD_AddBusiness,// 补充交易
        READCARD_RetrunCard,//退卡
        READCARD_NothingDo,//
			
				// add by TQL 20150818  射频M1卡
			  READCARD_SelectM1Card,  // 寻M1卡命令
				READCARD_GetSerialNum,  // 获取序列号
				READCARD_VerifyKeyA,    // 验证keyA密码
				READCARD_VerifyKeyB,    // 验证keyB密码
				READCARD_ReadSectionData,    // 读扇区块数据
			  READCARD_WriteSectionData,   // 写扇区块数据
				READCARD_ModifyPSW,          // 修改密码
				READCARD_InitValue,          // 初始化值
				READCARD_AddValue,           // 增值操作
				READCARD_ReduceValue,        // 减值操作
				// end
    } EnumReaderCardCommCommand;     

    typedef struct
    {
        union 
        {
            uint16_t value;
            struct
            {
                uint16_t SelectCard:1;     //  Bit0：0：卡机内有卡1：卡机内无卡，
                uint16_t SystemCard:1;     //  Bit1：0：系统卡1：非系统卡，
                uint16_t AdvanceHandle:1;  //  Bit2：0：预处理成功，1：预处理失败   
                uint16_t ResetPSAM:1;      //  Bit3：0：PSAM复位成功，1：PSAM复位失败
                uint16_t ResetUserCard:1;  //  Bit4：0：用户卡复位成功，1：用户卡复位失败
                uint16_t BusinessStatus:2; //  Bit5～Bit6：00：正常卡（可以加电），01：灰卡（可以结束加电），10：灰锁卡（进行补充交易）
                uint16_t ChargeStatus:1;   //  Bit7：0：加电成功，1：加电失败
                uint16_t EndChargeStatus:1;//  Bit8：0：结束加电成功，1：结束加电失败
                uint16_t Unlock:1;         //  Bit9：0：解锁成功，1：解锁失败
                uint16_t AddBusiness:1;    //  Bit10：0：补充交易成功，1：补充交易失败
                uint16_t CalcMAC2:1;       //  Bit11：0：计算MAC2成功，1：计算MAC2失败
                uint16_t GetRand:1;        //  Bit12：0：取随机数成功，1：取随机数失败
                uint16_t UpdateMAC:1;      //  Bit13：0：更新密钥成功，1：更新密钥失败
                uint16_t PopupCard:1;      //  Bit14：0：弹卡成功，1：弹卡失败
							  uint16_t PwdERR:1;         //  Bit15：0：弹卡成功，1：弹卡失败
							  
            }Bit;
						struct
            {
							  //// 射频M1卡状态 add by TQL 20150819
							  uint16_t SelectCard:1;     //  Bit0：0：卡机内有卡1：卡机内无卡，
                uint16_t SystemCard:1;     //  Bit1：0：系统卡1：非系统卡，
							  uint16_t GetSerialFail:1;  //  Bit2：0: 获取序列号成功 1:失败
							  uint16_t VerifyPswFail:1;  //  Bit3：0: 验证密码成功 1:失败
							  uint16_t ReadSectionFail:1;//  Bit4：0: 读取扇区数据成功 1:失败
							  uint16_t WriteSectionFail:1;// Bit5：0: 写扇区数据成功 1:失败
							  uint16_t SectionNumErr:1;  //  Bit6：0: 扇区号正确  1:失败
							  uint16_t CardSerialErr:1;  //  Bit7：0: 卡序列号正确 1:失败
							  uint16_t VerifyDataFail:1; //  Bit8：0: 校验数据成功1:失败
								uint16_t ModifyPswFail:1;  //  Bit9：0: 更改密码成功 1:失败
								uint16_t InitValueFail:1;  //  Bit10：0: 初始化值成功 1:失败
								uint16_t BlockFormatFail:1;//  Bit11：0: 块格式正确 1:失败
								uint16_t AddValueFail:1;   //  Bit12：0: 增值操作成功 1:失败
								uint16_t AddDataOverflow:1;//  Bit13：0: 增值溢出 1:失败
								uint16_t ReduceValueFail:1;//  Bit14：0: 减值操作成功1:失败
								uint16_t ReduceDataOverflow:1;//  Bit15：0: 减值溢出 1:失败
            }Bit_M1;
        }CardStatus;
        uint16_t bak1;              //预留
        uint32_t RemainMoney; //卡内金额0.01元/位
        uint32_t BlockMoney; // 冻结金额0.01元/位
        uint32_t ReturnMoney; //解冻金额0.01元/位
        uint32_t Availablemoney; //可用余额0.01元/位
        uint16_t TradeOffline; // 联机交易次数
        uint16_t TradeOnline; //脱机交易次数
        uint16_t CheckPSAMNo;// PSAM 卡座设置
        uint8_t CardNo[16]; //卡号
        uint16_t CaseType; // 卡类型 0x01 用户卡，识别出是用户卡，使用“开始加电”、“结束加电”命
        //令实现汽车充电操作。
        //0x02 密钥卡，识别出是密钥卡后，使用“密钥修改”命令修改充电
        //桩ESAM 模块中密钥。
        //0x41 设置卡，在修改密钥前使用，根据最新深圳市标准，已取消使
        //用本卡。
        //0x51 员工卡，充电桩员工使用，充电桩识别出是员工卡即可开始加
        //电，没有灰锁、解扣流程。
        //0x61 充值卡，使用方法同用户卡。
        //0x11 功能测试卡，识别出是功能测试卡，充电桩做相应的动作。
        //0x12 故障检修卡，识别出是故障检修卡，充电桩做相应的自检动作。
        //0x00 返回状态字为0x4E 时为0x00
        uint32_t ExpenseMoney;//消费金额
        uint8_t ICC[4];                 //伪随机数，4 个字节
        uint16_t Serial;            //电子钱包联机交易序号
        uint16_t BusinessType; //交易类型标示
        uint8_t EtermNo[12];  //桩体号
        uint8_t UnlockTime[8];//解索时间
        uint16_t tserial;           //脱机交易序号 
        uint16_t NormalCard;// 
    }ReadCardDataStr;

    typedef struct
    {
        union 
        {
            uint16_t value;
            struct
            {
                uint16_t ChargeFlag:1;//充电标志
                uint16_t :15;
            }Bit;
        }Status;//运行状态
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
   // extern osThreadId ReadCardThreadHandle;// 刷卡器通讯句柄
   // extern CommStr ReadCardComm;
    extern ReadCardDataStr ReadCardData;
    extern ChargeDataStr ChargeData;
    extern  EnumReaderCardCommCommand uReaderCardCommCommand;
   // extern RTC_HandleTypeDef hrtc;	






#ifdef  __cplusplus
}
#endif
#endif
