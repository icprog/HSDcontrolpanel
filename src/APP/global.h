#ifndef __GLOBAL_H
#define __GLOBAL_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
 #include "define.h"
 #include "includes.h"
#include "ICCARD.h"
/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define COM_PORT_NUM       5
#define COM_LEN_MAX        2048
#define COM1               0
#define COM2               1
#define COM3               2
#define COM4               3
#define COM5               4

#define MID_MODBUS_COM       COM1

#define  MODULE_SUM        10

#define DCDC_START_ADR      5

#define  COM_BUF_RN   1024   //�������ݵĳ���
#define  COM_BUF_SN   1024   //�������ݵĳ���

/*********************************************************************************************************************
*                                            �������
*********************************************************************************************************************/

typedef struct
{
  INT16U year;
  INT8U month;
  INT8U day;
  INT8U hour;
  INT8U min;
  INT8U sec;
   
}_TIME;

typedef struct
{ 
  INT16U user_indicating;               //�û�ָʾ����  000���� 0001���ǰˢ������  0002�������  003������ˢ������ 004��ӡ���� 
  INT8U inputOverVoltAlrm;		        //�����ѹ�澯
  INT8U inputLowVoltAlrm;		        //����Ƿѹ�澯
  INT8U CCS_outOverVoltAlarm ;		    //�����ѹ�澯
  INT8U CCS_outOverCurrAlarm;			//��������澯

  INT32U C_CCSFaultCode;               //�������ϴ���
  INT16U B_BmsFaultCode;               //BMS���ϴ���
  INT8U B_BatFaultCode[5];             //��ع��ϴ���	
  
  INT8U m485comLinkStatus;              //��485ͨ��״̬
  INT16U m485CommAlarmCnt;               //�������

  INT8U DcPduComAlarm;                         //���״̬ 0--����  1--�澯	
  INT8U insulationAlarm;                       //��Ե���澯
  INT8U ILB_comAlarm;                          //��Ե����ͨ�Ÿ澯
  INT8U beepFlag;                              //������־ 0--ֹͣ����   1--��������	 
  
  INT8U CCS_chargeEnable;					   //�������
  //INT8U CCS_DischargeEnable; 				   //�ŵ�����
  
  INT16U activeILB_value;		             //�����Ե������ֵ
  INT16U inactiveILB_value; 		         //�����Ե������ֵ 
  INT16U ILB_alarm_value_set; 		         //��Ե����澯ֵ   

  INT16U systemAlarmCode;                    //ϵͳ״̬����

  FP32   cellOverProtect;                    //�����ѹ����
  FP32   cellBatVoltMax;					 //�����ѹ���ֵ

  FP32   cellLowProtect;                    //�����ѹ����ֵ
  FP32   cellBatVoltMin;					 //�����ѹ���ֵ

  FP32   BMS_needVolt;                  //bms�����ѹ
  FP32   BMS_needCurr;					 //BMS�������

  FP32   BMS_Volt;                  //bms����ѹ
  FP32   BMS_Curr;					 //BMS������

  FP32 manualSetCurr;  
  FP32 manualSetVolt;

  FP32 CCS_setOutVolt;
  FP32 CCS_setOutCurr;
  FP32 CCS_outCurrMax;
  FP32 CCS_outVoltMin;
  FP32 CCS_outVoltMax;
  FP32 moduleCurrMax;

  INT8U  moduleNormalTable[MODULE_SUM];      //ģ�����������б�
  INT8U startModuleSum;
  INT8U moduleCurrResetFlag;
  INT8U manualRunCtlFlag;
  INT8U autoRunCtlFlag;

  INT8U M_carLinkState;
  INT8U moduleNornalSum;


  INT8U BatAlarm;                        //��ظ澯	

  INT8U   BatTempMAx;							 //������¶�
  INT8U   BatTempProtect;						 //������¶ȱ���
  INT8U   cellSoc;							 //�����ɵ�

  FP32 C_outVolt ;						    //
  FP32 C_outCurr ;							//

  INT8U CCS_chargeRunStatus;
  INT8U CCS_runCtlFlag;

  INT8U autoCtlFlag;
  INT8U bms_readyStatus;

  INT8U BMS_canCommAlarm;      //CANͨ��״̬
  INT8U BMS_canCommAlarmCnt;  //CANͨ�Ź��ϼ���

  INT8U ccu1_CommAlarm;      //ccuͨ��״̬
  INT8U ccu1_CommAlarmCnt;  //ccuͨ�Ź��ϼ���

  
  INT8U systemCtl;                //ϵͳ����  0X00 ����  0X01 ���  0X02 �ŵ� 0X03 ��ŵ�
  INT8U CCS_adr;					   //������ַ
  INT32U CCS_charge_time;			  //�������ʱ��
  INT32U CCS_charge_power;			  //�������ʱ��
  INT8U sumContactor_ctl;             //�ܿ��Ƽ̵���

  INT8U setCharge_flag;

  INT8U dev_adr;					  //�豸��ַ

  INT8U doubleGunPlugStatus;           //˫ǹ����״̬
  INT8U doubleGun_ID;                  //˫ǹID��

  INT8U mainGunBms_ready;		      //˫ǹBMSͨ��׼��״̬
  FP32  doubleGun_disCurrent;		  //˫ǹ��������

  INT16U doubleGun_timeout;           //˫ǹ��ʱ

  INT8U send_cur_cnt;                     //�·�����
  INT8U send_vol_cnt;
  INT8U	send_onff_cnt;

  INT8U M_pdu_ID[8];                     //���ID
  FP32  M_degrees;                    //�������
  INT32U M_pdu_failCode[4];             //�����ϴ���

  INT8U ledCtl;
  INT8U sysCtlMode;		                
  INT8U Machine2RunFlag;	
	
	uint32	tickMs;
	uint8		crgStyle;
	_TIME delayStartTime;
	uint32  delayTime; 	
	uint32  passWord;
  uint32	getPayId;
	uint32  systemStartT;
	uint16  nowPrice;
  uint32  delayLeftTime;
  uint16  setPageId;
	
	INT32U leftMoney;
	INT8U userCard[16];
	INT16U crgStatus;
	INT8U cardReader_indicating;
	INT32U user_CMD_timeout;              //�û�ָʾ���ʱ
	INT8U  userCardState;                       //�û���״̬
	INT8U  userCardState2;                      //�û���״̬2
  INT8U card_cmd_err;                   //0 ������   1�쳣��
	INT8U  user_CMD_return_status;        //�û�����ؽ��  00��Ч״̬   01���� 02ʧ��
	INT8U  user_CMD_ing;                  //�û�����ִ��״̬,��ִֹ�й����У���λ�������·�,�����ٴ��ظ�����
	
	FP32   M_thisPower;                     //�����������
    INT16U M_thisChargeAH;                  //���γ��AH
    FP32   M_thisChargeDegree;              //���γ�������
    INT32U M_thisChargeTime;                //���γ��ʱ��   ��λ:s
		INT8U  M_thisChargeTime_en;             //����ʱʹ��
    FP32 M_thisChargeMoney;               //���γ����   ��λ:Ԫ
		INT32U M_thisRemainTime;              //����ʣ����ʱ��

    FP32   M_meterDegree;                   //���ʵʱ����
    FP32   M_startDegree;                   //��ʼ���ʱ�ĵ�����	
    FP32   M_endDegree;                     //�������ʱ�ĵ�����  
		 INT8U userRunRequestFlag;                    //�û���ʾ��������ͣ��־  0--ֹͣ  1--����
		 
	INT32U charge_timeCnt;
	INT8U  charge_temeEn;

	INT8U cardCom_timeCnt;
	INT8U  cardComAlarm;
	INT8U  chargeFull;
	INT8U  cardBeepEn;
} USER_DB; 


#pragma pack(1)
typedef struct
{
    INT32U logID;                 //��־���
    INT8U pay_cardID[16];         //֧��������
//	INT8U physics_cardID[16];   //��������
//	INT8U phase_pay_flag;       //�ֶμƷѱ�־
    _TIME startTime;              //��翪ʼʱ��
    _TIME endTime;                //������ʱ��
    FP32   M_startDegree;                   //��ʼ���ʱ�ĵ�����	
    FP32   M_endDegree;                     //�������ʱ�ĵ�����  
/*	FP32 tine_start_degree;      //����ʼֵ
    FP32 tine_end_power;
    FP32 tartness_start_degree;   //����ʼֵ
    FP32 tartness_end_power;
    FP32 flat_start_degree;       //ƽ��ʼֵ
    FP32 flat_end_power;
    FP32 vale_start_degree;       //����ʼֵ
    FP32 vale_end_power;	
    
    FP32 tine_price;             //�嵥��
    FP32 tine_total_degree;
    FP32 tine_total_money;
    
    FP32 tartness_price;          //����
    FP32 tartness_total_degree;
    FP32 tartness_total_money;
    
    FP32 flat_price;              //ƽ���
    FP32 flat_total_degree;
    FP32 flat_total_money;
    
    FP32 vale_price;              //�ȵ��
    FP32 vale_total_degree;	
    FP32 vale_total_money;
*/  
    FP32 charge_degree;             //����ܵ���	
    FP32 chargeVoltMax;              //����ѹ���ֵ
    FP32 chargeCurrMax;              //���������ֵ
    FP32 consum_money;              //���ѽ��
    INT8U car_id[17];           //������ʶ��

    INT8U trade_flag;           //���ױ�־  0��¼��Ч 1����δ���  2�������������   3������ͨ���쳣�������
    INT8U autoCtlFlag;          //�ֶ�/�Զ�����
		INT8U remote_enable;        //Զ��ʹ��
		INT8U charge_mod;           //���ģʽ
		INT32U charge_time;           //���ʱ��
		INT8U  machineID;             //����ID;
		/*add member for portocol*/ //add

		uint8 crg_type;		//1�������2ԤԼ���
		uint8 card_type;	//������
		uint8 soc;			//soc 
		uint8 upLoadF;		//�Ƿ��ϴ���վ
		uint8 Pay_flg;		//�Ƿ񸶷�
		uint32 stopReason;//������ԭ��
		uint32 remainTime;//ʣ��ʱ��
		INT8U checkSum;              //У����
		
    INT8U bargain_Num[16];        //������ˮ��		
		FP32 purseBalance;           //Ǯ�����
		FP32 price_per_degree;       //���ѵ���
}CHARGE_LOG_INFO;  //�����־��Ϣ
#pragma pack()
typedef  struct
{
  INT8U UartRBuf[COM_LEN_MAX];
  INT16U UartRLen;

}UART_INFO;

typedef struct
{
	uint8  uploadFlag;																//�Ƿ��ϴ���վ         1,�ϴ�
	uint8  overFlag;																	//����Ƿ���������     1,����
	uint8  payFlag;																		//�������Ƿ���������   1,���� 2.����
	uint32  recordIndex;															//��¼��ţ���һֱ����
	uint32  recordId;																	//��¼������һֱ���ӣ������ᳬ������洢������¼��
	uint8			carNo[8];
	uint8			cardNo[8];
	uint32		startEnergeValue;
	uint32		endEnergeValue;
	uint32		startCardMoney;
	uint32		endCardMoney;
	uint16    chargeV;
	uint16    chargeI;
	uint32		crgLastT;
	uint32  	crgEnergy;
	uint32		crgMoney;
	 _TIME	startTime;
	 _TIME	endTime;
	uint32    leftTime;
	uint8 		crgType;
	uint8			crgMode;
	uint8			cardType;
	uint8			stopReason;//0112�����ֹԭ��
	uint8			currentSOC;
}ChargeRecord;


#define TRUE                              1
#define	FALSE															0
//#define ON                             		1
//#define	OFF																0
#define PWMOFFV                           11
#define PWMLINKV                          8
#define PWMONV                            5


typedef enum
{
	IC_TYPE_NONE										=0,									
	IC_TYPE_USER										=0X01,											//�û��� 
	IC_TYPE_EMPLOYEE								=0X51,											//Ա���� 
	IC_TYPE_RECHARGE								=0X61,											//��翨 
}IC_TYPE;
typedef enum
{
	AC_STATUS_IDLE                      			=0,											//����
	AC_STATUS_START_CHECK_CARD_EXIST          =1,   									//���ǰ��⿨�Ĵ���
	AC_STATUS_PRE_DEAL_CARD									  =2,											//��Ԥ����
	AC_STATUS_PRE_WAIT_VERIFY_PWD							=3,											//���ǰ�ȴ���֤��������
	AC_STATUS_PRE_VERIFY_PWD                  =4,											//���ǰ��֤������
	AC_STATUS_CRG_STYLE											  =5,											//ѡ���緽ʽ
	AC_STATUS_CRG_MODE											 	=6,											//ѡ����ģʽ						
	AC_STATUS_PRE_LOCK_CARD									  =7,											//���ǰ����
	AC_STATUS_WAIT_DELAY_TIME									=8,											//ԤԼ���
	AC_STATUS_CRGING												  =9,											//�����
	AC_STATUS_OTHER_OVER_CHECK_CARD					  =10,									  //��������ȴ�ˢ��
	AC_STATUS_OTHER_OVER_WAIT_PWD     			  =11,										//�������ȴ���������
	AC_STATUS_OTHER_OVER_VERIFY_PWD   				=12,										//�������ȴ���֤����
	AC_STATUS_CRGING_EXIST_CARD							  =13,										//����м�⵽��
	AC_STATUS_CRGING_EXIST_CARD_WAIT_PWD      =14,										//����м�⵽���ȴ���֤����
	AC_STATUS_CRGING_EXIST_CARD_VERIFY_PWD    =15,										//����м�⵽����֤����
	AC_STATUS_CRGING_EXIST_CARD_OVER_CRG			=16,										//����м�⵽��ֹͣ���
	AC_STATUS_UNLOCK_CARD                     =17,										//�����ӵ�
	AC_STATUS_CRG_FINISH 										  =18,										//����������̽���
	AC_STATUS_PRINT													  =19,										//��ӡ
	AC_STATUS_INSERT_GUN_AGAIN   							=20,										//���²�ǹ
	AC_STATUS_OTHER_OVER_EXIST_CARD           =21,   									//���������⵽��
	AC_STATUS_OTHER_OVER_UNLOCK_CARD					=22,                    //������ʽ�����������ӵ�״̬
//	AC_STATUS_END_WAIT_VERIFY_PWD						,
//	AC_STATUS_END_VERIFY_PWD							  ,
  AC_STATUS_START_CHECK_CARD								=23,										//���ǰ��⿨
	AC_STATUS_UNLOCK_PRE_LOCK_CARD						=24,                    //���ǰ�����ӵ�
	AC_STATUS_UNLOCK_PRE_LOCK_CARD_ADD        =25,										//���ǰ����
	AC_STATUS_SYSTEM_MANAGE                     =26,                //ϵͳ����
}AC_STATUS;

typedef enum
    {
        NoErr,
        TxFault,
        RxFault,
        TimeOut,
    } EnumErrCode;


typedef enum
{
	SWITCH_VALUE_ON=0,
	SWITCH_VALUE_OFF=1,
}SWITCH_VALUE;


typedef struct {

INT8U comAlarm;	                    //ͨ��״̬
INT8U comAlarmCnt;	                //ͨ�Ŵ������

INT8U moduleFail;	                //��������
INT8U runStatus;                    //���ػ�״̬
//INT8U averFloatStatus;              //������״̬
INT8U overVoltShut;	                //��ѹ�ػ�
INT8U overVoltAlarm;                //��ѹ
INT8U lowVoltAlarm;	                //Ƿѹ

FP32  outVolt;		                //�����ѹ
FP32  outCurr;	                    //�������
FP32  moduleTemp;                   //ģ���¶�
FP32  currPiont;                   //ģ�������
//INT16U  pf;                          //��������ֵ
//FP32  setAverVolt;		            //�����ѹ
//FP32  setFloatVolt;                 //�����ѹ
//FP32  setVoltUp;	                //��ѹֵ
//FP32  setVoltDown;	                //Ƿѹֵ
FP32  setVolt;                      //���õ�ѹ
FP32  setCurr;		                //���õ���

//FP32  getSetAverVolt;		            //�����ѹ
//FP32  getSetFloatVolt;                 //�����ѹ
//FP32  getSetVoltUp;	                //��ѹֵ
//FP32  getSetVoltDown;	                //Ƿѹֵ
FP32  getSetVolt;                      //���õ�ѹ
FP32  getSetCurr;		                //���õ���

INT8U onoffCtl;				        //���ػ�����
//INT8U averFloatCtl;                 //���������

}_MODULE_INFO;


/*====================================��CAN�ڲ����ýṹ��ͱ���====================================*/
typedef struct	  						//���յ�����չ֡����������
{
  INT32U ExtID;
  INT8U  DLC;
  INT8U  Data[8];
} CANRECMSG;

typedef struct	  						//���յ�����չ֡����������
{
  INT16U OutLen; 						//��ȡ�����ݻ�����λ�ã�����16λ�����ݻ�������Ҫ��
  INT16U InLen;							//���뻺������λ��
  CANRECMSG  Buff[16];					//��Ӧ����Ϣ
} CANRECMSGBUFF;

extern INT8U  gModbusRecArr[100];				//���ڽ������ݵ���ʱ�Ĵ���
extern INT8U  gModbusRecNum;					//���յ������ݸ��������50
//extern INT16U gModbusREG[MBREGNUM]={0};		//����Modbus �Ĵ�����Ҳ��ȫ�ֱ���������								
										




#define MBREGNUM 	150					//modbusͨѶ������ʵļĴ�������




extern INT8U gModbusRecNum;
extern INT8U gSystemCommFlag;
extern INT16U gModbusREG[MBREGNUM];
extern unsigned char  gChargeMode;									//bit0-0/1-auto/manual,bit1-0/1-autoV/C,bit2-0/1-manualV/C
extern unsigned char  gKeyValue;									//����ֵ
extern unsigned char  gKeyTimes;									//���������Ĵ�����ʱ��
extern unsigned char  gSegShowStatus;								//��ѹ��������������״̬
extern unsigned short gSystemERR;									//���ֹ���
extern INT16U gSysSetCurpwm;										//ϵͳ���õĵ���PMW
extern INT16U gSysSetVolpwm;										//ϵͳ���õĵ�ѹPMW

extern char PWM_SetVoltage(INT16U lSetDat);							//���õ�ѹ
extern char PWM_SetCurrent(INT16U lSetDat);							//���õ���
extern INT16U abs_int16u(INT16U data1,INT16U data2);				//�������޷������εľ���ֵ

extern void SetVolMode(INT8U Mode);
extern INT8U GetVolMode(void);	

extern USER_DB gUserDB;

extern UART_INFO gUartInfo[COM_PORT_NUM];                     		//���ڽ��ջ�����Ϣ
extern INT8U uart_write(INT8U porID,INT8U * buf,INT8U length);
//extern INT8U uart_read(INT8U porID,INT8U * buf,INT8U timeout);
INT16U uart_read(INT8U porID,INT8U * buf,INT16U timeout);
extern _MODULE_INFO gModuleInfo[30];


extern CANRECMSGBUFF CanMessageBuff;								//����CAN���ջ�����
extern CANRECMSGBUFF MCM_CanPackage;								//��������һ��can���ջ�����

extern uint8 dataExchange(uint8 * pdat, uint8 len); 				//���ݸߵ��ֽڽ���

#endif

