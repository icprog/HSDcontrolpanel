#ifndef __CAN_H
#define __CAN_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "stm32f10x.h"
#include "define.h"
#include "can_cfg.h"
/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
/*====================================���ⲿ���ýṹ��ͱ���====================================*/
typedef struct {
	INT32U 	BMR_1 ;// pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;		//ͨѶЭ��汾��
	INT8U 	BMR_4 ;//  pu8Mesg[3];					//�������
	FP32 	BMR_5 ;//  (FP32)(pu8Mesg[4] + pu8Mesg[5]<<8)/10;			//������ض����
	FP32 	BMR_7 ;//  (FP32)(pu8Mesg[6] + pu8Mesg[7]<<8)/10;			//������ܵ�ѹ
	INT8U 	BMR_9[4] ;//  pu8Mesg[8];				//��س������� 4	
	INT8U 	BMR_13[4] ;//  pu8Mesg[12];				//�������� 4	
	INT16U 	BMR_17 ;//  pu8Mesg[16]+1985;			//���������������
	INT8U 	BMR_18 ;//  pu8Mesg[17];				//�������������  ��
	INT8U 	BMR_19 ;//  pu8Mesg[18];				//��
	INT32U 	BMR_20 ;// pu8Mesg[19] + pu8Mesg[20]<<8 + pu8Mesg[21]<<16;//������ 
	INT8U 	BMR_23 ;//  pu8Mesg[22];				//��Ȩ��ʶ
	INT8U 	BMR_24 ;//  pu8Mesg[23];				//Ԥ��
	INT8U 	BMR_25[25] ;//  pu8Mesg[24+u8AdrCnt];		//����ʶ����Ϣ 25byte			
	INT32U 	VBI_1 ;//   pu8Mesg[0] + pu8Mesg[1]<<8 + pu8Mesg[2]<<16;	//ͨѶЭ��汾��;
	INT8U 	VBI_4[17] ;//  pu8Mesg[3+u8AdrCnt];				
	INT8U 	VBI_21[4] ;//  pu8Mesg[20+u8AdrCnt];				
	INT8U 	VBI_25[8] ;//  pu8Mesg[24+u8AdrCnt];						
	INT8U 	VBI_33[8] ;//  pu8Mesg[32+u8AdrCnt];				
	FP32 	BCP_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/100;			//����ģ������������ѹ			
	FP32 	BCP_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10;			//������������ 
	INT16U 	BCP_5 ;// pu8Mesg[4]*10;									//������������� 
	FP32 	BCP_6 ;// (FP32)(pu8Mesg[5] + pu8Mesg[6]<<8)/10;			//����������ܵ�ѹ 
	INT16S	BCP_8 ;// pu8Mesg[7]-50;									//��������¶� 
	INT16U	BCP_9 ;// (pu8Mesg[8] + pu8Mesg[9]<<8);						//�������ع���ʣ������ 
	FP32 	BCP_11 ;// (FP32)(pu8Mesg[10] + pu8Mesg[11]<<8)/10;			//���������ܵ�ѹ 
	INT8U 	BCP_13 ;// pu8Mesg[12];			
	INT8U 	BRO_1 ;// pu8Mesg[0];//	
	FP32 	BCL_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//��ѹ����
	FP32 	BCL_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10-800;		//��������
	INT8U 	BCL_5 ;// pu8Mesg[4]; 	
	FP32 	BCS_1 ;// (FP32)(pu8Mesg[0] + pu8Mesg[1]<<8)/10;			//����ѹ����ֵ��V�� 
	FP32 	BCS_3 ;// (FP32)(pu8Mesg[2] + pu8Mesg[3]<<8)/10 - 800;		//����������ֵ��A��
	INT16S	BCS_5 ;// pu8Mesg[4]-50;	//���������¶ȣ�oC��  
	INT16S	BCS_6 ;// pu8Mesg[5]-50;	//���������¶ȣ�oC��  
	INT8U 	BCS_7 ;// pu8Mesg[6];		//��ǰ�ɵ�״̬  SOC��%��  
	INT8U 	BCS_8 ;// pu8Mesg[7];		//����ʣ��ʱ�䣨min��
	INT16U	BSM_1 ;// (pu8Mesg[0] + pu8Mesg[1]<<8);
	INT8U 	BSM_2 ;
	INT8U 	BSM_3 ;// pu8Mesg[2];
	INT8U 	BSM_4 ;// pu8Mesg[3];
	INT8U 	BSM_5 ;// pu8Mesg[4];
	INT8U 	BSM_6 ;// pu8Mesg[5];					//�������ͬʱ���룬���������������
	INT8U 	BSM_7 ;// pu8Mesg[6];	
	INT8U 	BSM_6_1;// (pu8Mesg[5] >> 0) &0x01;
	INT8U 	BSM_6_2 ;// (pu8Mesg[5] >> 1) &0x01;
	INT8U 	BSM_6_3 ;// (pu8Mesg[5] >> 2) &0x01;
	INT8U 	BSM_6_4 ;// (pu8Mesg[5] >> 3) &0x01;
	INT8U 	BSM_6_5 ;// (pu8Mesg[5] >> 4) &0x01;
	INT8U 	BSM_6_6 ;// (pu8Mesg[5] >> 5) &0x01;
	INT8U 	BSM_6_7 ;// (pu8Mesg[5] >> 6) &0x01;
	INT8U 	BSM_6_8 ;// (pu8Mesg[5] >> 7) &0x01;	
	INT8U 	BSM_7_1 ;// (pu8Mesg[6] >> 0) &0x01;
	INT8U 	BSM_7_2 ;// (pu8Mesg[6] >> 1) &0x01;	
	INT8U 	BST_1 ;// pu8Mesg[0];
	INT8U 	BST_2 ;// pu8Mesg[1];
	INT8U 	BST_3 ;// pu8Mesg[2];	
	INT8U 	BSD_1 ;// pu8Mesg[0];										//��ֹ�ɵ�״̬  SOC��%��  
	FP32 	BSD_2 ;// (FP32)(pu8Mesg[1] + pu8Mesg[2]<<8)/1000;			//���ص�����͵�ѹ��V
	FP32 	BSD_4 ;// (FP32)(pu8Mesg[3] + pu8Mesg[4]<<8)/1000;			//���ص�����ߵ�ѹ��V
	INT16S 	BSD_6 ;// pu8Mesg[5]-50;									//����������¶ȣ�oC�� 
	INT16S	BSD_7 ;// pu8Mesg[6]-50;									//����������¶ȣ�oC��
	
	
	INT8U	CRM_1;
	INT8U	CRM_2;
	INT8U	CRM_3;
	INT8U	CRM_4;
	INT32U	CRM_5;
	INT8U	CTS_1;
	INT8U	CTS_2;
	INT8U	CTS_3;
	INT8U	CTS_4;
	INT8U	CTS_5;
	INT16U	CTS_6;
	FP32	CCS_1;
	FP32 	CCS_2;
	INT16U	CCS_3;
	INT8U	CST_1;//��ֹ���ԭ��
	INT8U	CST_2;//��ֹ������ԭ��			
	INT8U	CST_3;//��ֹ������ԭ��	
	INT16U	CSD_1;			//�ۼƳ��ʱ�� 1min
	FP32 	CSD_2;//�������	0.1kWh 0~500kWh	
	INT8U	CSD_3;			//
	
} _T_BMS_DAT;


typedef struct {
	FP32 ActivePower;					//����й�����kW
	FP32 AH;							//���γ���ۼ�AH
	FP32 kWh;							//���γ���ۼ�kWh
	FP32 ReactivePower;					//����޹�����kvar
	FP32 ACAllPower	;					//���������ܹ���
	INT8U Wave_A_Vol;					//A���ѹг��
	INT8U Wave_B_Vol;					//B���ѹг��
	INT8U Wave_C_Vol;					//C���ѹг��
	INT8U Wave_A_Cur;					//A�����г��
	INT8U Wave_B_Cur;					//B�����г��
	INT8U Wave_C_Cur;					//C�����г��		
}_T_METER_DAT;	 


typedef struct {
	INT8U 	FlagValid;					//0,�ý׶���Ч��1-�ý׶���Ч����δ��ɾ��
	INT8U 	WorkType;					//������ʽ��0-ֹͣ��1-������ѹ��2-��ѹ������3-������ʱ��4-��ѹ��ʱ
	FP32 	SetValue;					//�������ֵ
	FP32 	LimitValue;					//�������ֵ
	FP32 	StopValue;					//���ֵֹͣ
}_T_MCT_STAGE;

typedef struct {
	INT8U 	FlagValid;					//������Ч��־λ��=1����Ч
	FP32	MaxCellVoltage;				//��������������ѹ��ѹ
	INT16S	MaxTemperature;				//����������¶�
}_T_MCT_SINGLE;

typedef struct {
	INT8U 	Switch_on;						//���ػ��źţ�0-ֹͣ��1-����
	INT8U 	Switch_off;						//���ػ��źţ�0-ֹͣ��1-����	
	INT8U 	ChargeNum;					//���׶�����
	INT8U 	ChargerID;					//�������
	INT8U 	NowStage;					//��ǰ�����ĳ��׶�
	FP32 	Power;						//�������	
	INT8U	BMSStage;					//��bmsͨѶ�����ĸ��׶�
	_T_MCT_SINGLE 	MCT_Single;			//������Ʋ���	
	_T_MCT_STAGE  	MCT_Stage[6];		//�����׶ε�ֵ
	
}_T_MCT_DAT;  	                        //����ն˽ṹ��


typedef struct 
{
	INT8U OutTime;						//ͨѶ��ʱ,0-����ʱ��1��ʾ��ʱ
	INT8U NoCharge;						//BMS����Ҫ��磬1-����Ҫ���
	INT8U FullCharge;					//BMS�ѳ��� ��1-�Ѿ�����
	INT8U Charging;						//����б�־λ��1-�����
	INT8U Stop;							//��ʾ��λ�����͵�ֹͣ��1-����Ϊ��λ��Ҫ���ֹͣ����
}_CAN_CHARGE_FLAG;                      //�������̽ṹ��


/*====================================���ڲ����ýṹ��ͱ���====================================*/
/*
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
*/
typedef struct	  						//���յ�����չ֡����������
{
  INT8U  PGN;							//����Ⱥ��ţ�ֻȡ�м���ֽ�
  INT8U  RxFlag;						//���ձ�־ =1 ���ڽ��� =2 �������
  INT16U DataLen; 						//��ȡ�����ݻ�����λ�ã�����16λ�����ݻ�������Ҫ��
  INT8U  FrameNum;						//���ݰ�����Ŀ
  INT8U	 FramePointer;					//�Ѿ����յ�����֡��λ��
  INT8U  Buff[200];						//��Ӧ����Ϣ����ʵ100���Ѿ��㹻��
} CANPACKAGE;							//CAN����Ϣ�������ʱ�Ĵ���

typedef struct	  						//���͵���չ֡����������
{
  INT8U  PGN;							//����Ⱥ��ţ�ֻȡ�м���ֽ�
  INT8U  SendFlag;						//���ͱ�־ =1 ���ڷ��� =2 �������
  INT16U DataLen; 						//���͵����ݻ�����λ�ã�����16λ�����ݻ�������Ҫ��
  INT8U  FrameNum;						//���ݰ�����Ŀ
  INT8U	 FramePointer;					//�Ѿ����͵�����֡��λ�ã���1��ʼ
  INT8U  Buff[100];						//��Ӧ����Ϣ����ʵ100���Ѿ��㹻��
} CANSENDPACKAGE;


#define CANMESSAGEBUFFLEN	16			//������ջ������ĳ���

#define CAN_REC_BMS	0x00
#define CAN_REC_ALL 0x01
//extern void 		 CAN_Initial(INT8U u8Type);
extern INT16U 		 ModbusREGDat[300];

extern _T_BMS_DAT gtBMS_Dat;
extern _T_METER_DAT gtMeter_Dat;
extern _T_MCT_DAT gtMCT_Dat; 
extern _CAN_CHARGE_FLAG CAN_ChargeFlag;						//�������еı�־
										//Ĭ�Ϸ��͵���Ч����
//#define INVALID_CANDATA   0xFF 

//BMS���Ľ��벻ͬ����׶�ֵPGN
#define	BMS_DEC_BRM				0x02	//BMS�ͳ�����ʶ����BRM��250ms
#define	BMS_DEC_VBI				0x30	//�����͵�ر�ʶ����VBI��250ms
#define	BMS_DEC_BCP				0x06	//���س�����BCP��500ms��ʵ����13���ֽ�
#define	BMS_DEC_BRO				0x09	//��س��׼������״̬BRO��250ms
#define	BMS_DEC_BCL				0x10	//��س��������BCL��20ms
#define	BMS_DEC_BCS				0x11	//��س����״̬����BCS��250ms
#define	BMS_DEC_BSM    			0x13	//����״̬��ϢBSM��250ms
#define	BMS_DEC_BMV     		0x15	//������С������Ԫ��ѹBMV��1000ms
#define	BMS_DEC_BMT     		0x16	//�������¶�BMT��1000ms
#define	BMS_DEC_BSOC      		0x17	//������ɵ�����SOC ,BSOC��1000ms
#define	BMS_DEC_BST       		0x19	//BMS��ֹ��籨��BST��10ms
#define	BMS_DEC_BSD      		0x1C	//BMSͳ������BSD��250ms
#define	BMS_DEC_BEM				0x1E	//BMS������BEM��250ms
#define	BMS_DEC_BFM				0x33	//BMS���ϱ���BFM��250ms

#define BMS_DOUBLE_STATUS			0XF8    //
#define BMS_DOUBLE_OUT              0XF9	//

#define CCS_DOUBLE_STATUS			0XF8    //
#define CCS_DOUBLE_OUT              0XF9	//

//CCS���Ľ��벻ͬ����׶�ֵ
#define	CCS_DEC_CRM_0			0x01	//������ʶ����CRM��250ms
#define	CCS_DEC_CRM_1			0x02	//������ʶ����CRM��250ms
#define	CCS_DEC_CTS				0x03	//�������͵�ʱ��ͬ����ϢCTS��500ms
#define	CCS_DEC_CML				0x04	//��������������CML��250ms
#define	CCS_DEC_CRO				0x05	//�������׼������״̬CRO��250ms
#define	CCS_DEC_CCS				0x06	//�������״̬CCS,20ms
#define	CCS_DEC_CST				0x07	//������ֹ���CST��10ms
#define	CCS_DEC_CSD				0x08	//����ͳ�����ݱ���CSD��250ms
#define	CCS_DEC_CEM				0x09	//����������CEM��250ms
#define	CCS_DEC_CFM				0x0A	//�������ϱ���CFM��250ms

//CCS���Ľ��벻ͬ����׶�ֵ����Ӧ����ʼ��ַ�����ݳ���
#define	M_CRM_START_ADR		10	//start Adr ������λ������11
#define	M_CRM_ADR_LEN		8
#define	M_CTS_START_ADR		(M_CRM_START_ADR + (M_CRM_ADR_LEN+1)/2 + 0)	//10+4
#define	M_CTS_ADR_LEN		7
#define	M_CML_START_ADR		(M_CTS_START_ADR + (M_CTS_ADR_LEN+1)/2 + 0)	//14+4
#define	M_CML_ADR_LEN		8
#define	M_CRO_START_ADR		(M_CML_START_ADR + (M_CML_ADR_LEN+1)/2 + 0)	//18+4
#define	M_CRO_ADR_LEN		1
#define	M_CCS_START_ADR		(M_CRO_START_ADR + (M_CRO_ADR_LEN+1)/2 + 0)	//22+1
#define	M_CCS_ADR_LEN		6
#define	M_CST_START_ADR		(M_CCS_START_ADR + (M_CCS_ADR_LEN+1)/2 + 0)	//23+3
#define	M_CST_ADR_LEN		3
#define	M_CSD_START_ADR		(M_CST_START_ADR + (M_CST_ADR_LEN+1)/2 + 0)	//start Adr
#define	M_CSD_ADR_LEN		7
#define	M_CEM_START_ADR		(M_CSD_START_ADR + (M_CSD_ADR_LEN+1)/2 + 0)	//���ֽڴ���
#define	M_CEM_ADR_LEN		9
#define	M_CFM_START_ADR		(M_CEM_START_ADR + (M_CEM_ADR_LEN+1)/2 + 0)	//start Adr
#define	M_CFM_ADR_LEN		2



//=========BMS���Ľ��벻ͬ����׶�ֵ����Ӧ����ʼ��ַ�����ݳ���==========//
#define	M_BRM_START_ADR		35	//������λ������11
#define	M_BRM_ADR_LEN		41
#define	M_VBI_START_ADR		(M_BRM_START_ADR + (M_BRM_ADR_LEN+1)/2 + 0)	//35+21
#define	M_VBI_ADR_LEN		40
#define	M_BCP_START_ADR		(M_VBI_START_ADR + (M_VBI_ADR_LEN+1)/2 + 0)	//56+20
#define	M_BCP_ADR_LEN		13
#define	M_BRO_START_ADR		(M_BCP_START_ADR + (M_BCP_ADR_LEN+1)/2 + 0)	//76+7
#define	M_BRO_ADR_LEN		1
#define	M_BCL_START_ADR		(M_BRO_START_ADR + (M_BRO_ADR_LEN+1)/2 + 0)	//83+1
#define	M_BCL_ADR_LEN		5
#define	M_BCS_START_ADR		(M_BCL_START_ADR + (M_BCL_ADR_LEN+1)/2 + 0)	//84+3
#define	M_BCS_ADR_LEN		8
#define	M_BST_START_ADR		(M_BCS_START_ADR + (M_BCS_ADR_LEN+1)/2 + 0)	//87+4
#define	M_BST_ADR_LEN		3
#define	M_BSD_START_ADR		(M_BST_START_ADR + (M_BST_ADR_LEN+1)/2 + 0)	//91+2
#define	M_BSD_ADR_LEN		7
#define	M_BEM_START_ADR		(M_BSD_START_ADR + (M_BSD_ADR_LEN+1)/2 + 0)	//93+4
#define	M_BEM_ADR_LEN		4
#define	M_BFM_START_ADR		(M_BEM_START_ADR + (M_BEM_ADR_LEN+1)/2 + 0)	//97+2
#define	M_BFM_ADR_LEN		7	//99+4=103,��modbusreg[103]
//=====���ò������ε�======//
#define	M_BSM_START_ADR		103
#define	M_BSM_ADR_LEN		7	//7
//#define	M_BMV_START_ADR		X
//#define	M_BMV_ADR_LEN		X	//0xFF
//#define	M_BMT_START_ADR		X
//#define	M_BMT_ADR_LEN		X	//0xFF
//#define	M_BSOC_START_ADR	X
//#define	M_BSOC_ADR_LEN		X	//0xFF

//=========�����ݴ���Э���PGN��ֵ===========//
/*
#define	M_PGN_BRM		0x02  //ֻȡ�м�2���ֽڣ�����Ĭ��0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17
*/

#define	CCS_DEC_VBI		0x01
#define	CCS_DEC_BRM		0x02
#define	CCS_DEC_BCP		0x03
#define	CCS_DEC_BRO		0x04
#define	CCS_DEC_BCS		0x05
#define	CCS_DEC_BCL		0x06
#define	CCS_DEC_BST		0x07
#define	CCS_DEC_BSD		0x08
#define	CCS_DEC_OVER	0x09

#define	CCS_DEC_INVALID	0x00
#define	COMM_OUTTIME	500	//��ʱʱ��ֵ����λ10ms


//����ն˵�����
#define MCM_STOP_ALL		0x0440E500						//��վ�����г��׮/��ͣ��
#define MCM_START_SINGLE	0x0841E500						//��վ��ĳ̨���׮/�����
#define MCM_STOP_SINGLE		0x0442E500						//��վ��ĳ̨���׮/��ֹͣ���
#define MCM_SET_SINGLE		0x0C43E500						//��վ���õ����������Ʋ���
#define MCM_SET_STAGE_1		0x0C44E500 						//��վ���ý׶γ����Ʋ���
#define MCM_SET_STAGE_2		0x0C45E500 
#define MCM_SET_STAGE_3		0x0C46E500 
#define MCM_SET_STAGE_4		0x0C47E500 
#define MCM_SET_STAGE_5		0x0C48E500 
#define MCM_SET_STAGE_6		0x0C49E500 
#define MCM_READSET_ALL		0x104AE500						//��վ��ȡ���׮/�����г����Ʋ���
#define MCM_READSET_SPECIAL_PARA 0x104BE500					//��վ��ȡ���׮/���ض������Ʋ���
#define MCM_SET_POWER		0x0C60E500						//��վ���ó��׮/�������
//����������ն�  
#define MCM_RETURNSET_STAGE_ALL_SINGLE	0x104C00E5			//��վ����ȫ���׶ε����������Ʋ���
#define MCM_RETURNSET_STAGE_ALL_1		0x104D00E5			//��վ�������н׶γ����Ʋ���
#define MCM_RETURNSET_STAGE_ALL_2		0x104E00E5
#define MCM_RETURNSET_STAGE_ALL_3		0x104F00E5
#define MCM_RETURNSET_STAGE_ALL_4		0x105000E5
#define MCM_RETURNSET_STAGE_ALL_5		0x105100E5
#define MCM_RETURNSET_STAGE_ALL_6		0x105200E5
#define MCM_RETURNSET_SINGLE			0x105300E5			//��վ���ص����������Ʋ���
#define MCM_RETURNSET_SPECIAL_STAGE_1	0x105400E5			//��վ����ָ���׶γ����Ʋ�������6���׶Σ�
#define MCM_RETURNSET_SPECIAL_STAGE_2	0x105500E5
#define MCM_RETURNSET_SPECIAL_STAGE_3	0x105600E5
#define MCM_RETURNSET_SPECIAL_STAGE_4	0x105700E5
#define MCM_RETURNSET_SPECIAL_STAGE_5	0x105800E5
#define MCM_RETURNSET_SPECIAL_STAGE_6	0x105900E5
#define MCM_STATE0			0x185A00E5						//���׮/��״̬֡0
#define MCM_STATE1			0x185B00E5						//���׮/��״̬֡1
#define MCM_STATE2			0x185C00E5						//���׮/��״̬֡2
#define MCM_STATE3			0x185D00E5						//���׮/��״̬֡3
#define MCM_STATE4			0x185E00E5						//���׮/��״̬֡4

#endif






















