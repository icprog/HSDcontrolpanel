//modbusͷ�ļ�
#ifndef	__MODBUS_H__
#define	__MODBUS_H__



//485ƬѡIO��
#define MAX485SL    P2_2   //����485ͨ�ŵ�Ƭѡ�ź�----

//�ӻ���ַ
#define  CAN_485_DEVICE_ID  100  //Ĭ�ϵ�ַ 100

//ϵͳ��ʱ������ʱֵ
#define CCAPXL_MODBUS  CCAP2L  //Modbus T3.5�ֽ�
#define CCAPXH_MODBUS  CCAP2H
#define CCAPMX_MODBUS  CCAPM2
#define CCFX_MODBUS    CCF2
#define MODBUS_T_ON    {CCAPMX_MODBUS=0x49;}  //�򿪶�ʱ��
#define MODBUS_T_OFF   {CCAPMX_MODBUS=0x00;}  //�رն�ʱ��
#define PCA_T35mS_L     0x00
#define PCA_T35mS_H     0x6C

//���鳤��
//#define  COM_BUF_RN   200   //�������ݵĳ���
//#define  COM_BUF_SN   200   //�������ݵĳ���
#define  END_REG_LEN  107  //�Ĵ�������

//����һ���Ĵ�����ַ
#define END_REG_ADR  108    

//ȫ�ֱ���
//extern uint8 Device_ID;
//extern uint8 RevComflag,ModbusT_CloseFlag;
//extern uint8 RevComCount,CrcCounter;
//extern  uint8 RevComBuff[COM_BUF_RN];  //���ڽ��յ�buff
//
////ȫ�ֺ���
//extern void UartSendStr(uint8 *p,uint8 i);  //����һ������
//extern void UARTInit(void);  //���ڳ�ʼ��
//extern void ModbusCommunication(uint16 *ptr);  //modbusͨ��

#endif


#ifndef  CAN_H
#define  CAN_H

//======����ϵͳʱ�����=====//
#define OSDELAYTIME10MS     10   //��ʱ10ms
#define OSDELAYTIME20MS     20
#define OSDELAYTIME50MS     50
#define OSDELAYTIME100MS    100  //��ʱ100ms
#define OSDELAYTIME150MS    150
#define OSDELAYTIME200MS    200
#define OSDELAYTIME250MS    250  //��ʱ250ms
#define OSDELAYTIME300MS    300  //��ʱ300ms
#define OSDELAYTIME500MS    500
#define OSDELAYTIME1000MS   1000
#define OSDELAYTIME1500MS   1500
#define OSDELAYTIME2000MS   2000
#define OSDELAYTIME2500MS   2500  //��ʱ2.5s 

//======��ʱֵ===========//
#define TIMER5S      500    //1S��ֵ100�� ��ʱ��10ms���ܶ�ʱ5s,��ʱ����5s
#define TIMERRESS    1500    //15s����û��ͨ�ųɹ����Զ�����

//======PCA��ʱ�����ã�����PCA��ʱ��,PCA0,PCA2
#define CCAPXL_TC      CCAP0L  //��ʱ10ms
#define CCAPXH_TC      CCAP0H
#define CCAPMX_TC      CCAPM0
#define CCFX_TC        CCF0
#define PCA_10mS_L     0x00
#define PCA_10mS_H     0x48

//=====��׼֡����չ֡===========//
#define  STANDARD_FRAME_TYPE    1   //��׼֡
#define  EXTEND_FRAME_TYPE      2   //��չ֡

//======CAN�շ�����buf����
#define  CAN_BUF_RN   8    //�������ݵĳ���,8������
#define  CAN_BUF_SN   8    //�������ݵĳ��ȣ�8������

//=====Ĭ�Ϸ��͵���Ч����===========//
#define  INVALID_CANDATA   0xFF 

//=========BMS���Ľ��벻ͬ����׶�ֵPGN����ΪCAN������������ȴ���λ�ٴ���λ�����Խ���ʱ��������Э���е����ݸߵ�λ�෴==========//
//#define	BMS_DEC_BRM						0x02	//BMS�ͳ�����ʶ����BRM��250ms
//#define	BMS_DEC_VBI						0x30	//�����͵�ر�ʶ����VBI��250ms
//#define	BMS_DEC_BCP						0x06	//���س�����BCP��500ms��ʵ����13���ֽ�
//#define	BMS_DEC_BRO						0x09	//��س��׼������״̬BRO��250ms
//#define	BMS_DEC_BCL						0x10	//��س��������BCL��20ms
//#define	BMS_DEC_BCS						0x11	//��س����״̬����BCS��250ms
//#define	BMS_DEC_BSM    					0x12	//����״̬��ϢBSM��250ms
//#define	BMS_DEC_BMV     				0x15	//������С������Ԫ��ѹBMV��1000ms
//#define	BMS_DEC_BMT     				0x16	//�������¶�BMT��1000ms
//#define	BMS_DEC_BSOC      				0x17	//������ɵ�����SOC ,BSOC��1000ms
//#define	BMS_DEC_BST       				0x19	//BMS��ֹ��籨��BST��10ms
//#define	BMS_DEC_BSD      				0x1C	//BMSͳ������BSD��250ms
//#define	BMS_DEC_BEM						0x1E	//BMS������BEM��250ms
//#define	BMS_DEC_BFM						0x33	//BMS���ϱ���BFM��250ms

#define WZL_BMS_DEC_BCP					0X06    //������BMS���س�����BCP��500ms��ʵ����13���ֽ�

//================BMS����PGNֵ����ΪCAN������������ȴ���λ�ٴ���λ�����Խ���ʱ��������Э���е����ݸߵ�λ�෴================//
#define	M_BRM_PGN						0x02	//BMS�ͳ�����ʶ����BRM��250ms
#define	M_VBI_PGN						0x30	//�����͵�ر�ʶ����VBI��250ms
#define	M_BCP_PGN						0x06	//���س�����BCP��500ms��ʵ����13���ֽ�
#define	M_BRO_PGN						0x09	//��س��׼������״̬BRO��250ms
#define	M_BCL_PGN						0x10	//��س��������BCL��20ms
#define	M_BCS_PGN						0x11	//��س����״̬����BCS��250ms
#define	M_BSM_PGN    					0x12	//����״̬��ϢBSM��250ms
#define	M_BMV_PGN     				    0x15	//������С������Ԫ��ѹBMV��1000ms
#define	M_BMT_PGN     				    0x16	//�������¶�BMT��1000ms
#define	M_BSOC_PGN      				0x17	//������ɵ�����SOC ,BSOC��1000ms
#define	M_BST_PGN       				0x19	//BMS��ֹ��籨��BST��10ms
#define	M_BSD_PGN      				    0x1C	//BMSͳ������BSD��250ms
#define	M_BEM_PGN						0x1E	//BMS������BEM��250ms
#define	M_BFM_PGN						0x33	//BMS���ϱ���BFM��250ms

//=========CCS���Ľ��벻ͬ����׶�ֵ==========//
#define	CCS_DEC_CRM_0					0x01	//������ʶ����CRM��250ms
#define	CCS_DEC_CRM_1					0x01	//������ʶ����CRM��250ms
#define	CCS_DEC_CTS				        0x07	//�������͵�ʱ��ͬ����ϢCTS��500ms
#define	CCS_DEC_CML				        0x08	//��������������CML��250ms
#define	CCS_DEC_CRO						0x0A	//�������׼������״̬CRO��250ms
#define	CCS_DEC_CCS						0x14	//�������״̬CCS,20ms
#define	CCS_DEC_CST						0x1A	//������ֹ���CST��10ms
#define	CCS_DEC_CSD						0x1D	//����ͳ�����ݱ���CSD��250ms
#define	CCS_DEC_CEM						0x1F	//����������CEM��250ms
#define	CCS_DEC_CFM						0x34	//�������ϱ���CFM��250ms

#define WZL_DEC_CCS                     0xFF    //������������״̬����

//=====CCS��BMS�������ݵ��ֽ���========//
#define CCS_NUM_CRM		8  //������ʶ����
#define CCS_NUM_CTS 	7  //CCS���͵�ʱ��ͬ����Ϣ
#define CCS_NUM_CML		8 //CCS����������
#define CCS_NUM_CRO 	1 //CCS���׼������
#define CCS_NUM_CCS		6 //CCS���״̬
#define CCS_NUM_CST		3  //CCS��ֹ���CST
#define CCS_NUM_CSD		7  //CCS���ͳ��
#define CCS_NUM_CEM		3  //ת�����׶η��͵Ĵ���ͳ�Ʊ��� 
#define CCS_NUM_CFM		2  //���Ͻ׶εĹ��ϱ���

//=============BMS CAN IDǰ�����ֽ�============================//
#define BMSID_BRM	0xC00A			//0x1802E5F4,BMS��ʶ����BRM
#define BMSID_VBI	0xC188			//0x1830E5F4,����ʶ����VBI
#define BMSID_BCP	0xC02A			//0x1806E5F4,BMS���س�����BCP
#define BMSID_BRO	0xA049			//0x1409E5F4,BMS���׼����������BRO
#define BMSID_BCL	0xC088			//0x1810E5F4,��س������BCL
#define BMSID_BCS	0xC089			//0x1811E5F4,��س��״̬BCS
#define BMSID_BSM	0xC08A			//0x1812E5F4,����״̬��ϢBSM
#define BMSID_BMV	0xC0A9			//0x1815E5F4,������С������Ԫ��ѹBMV
#define BMSID_BMT	0xC0AA			//0x1816E5F4,�������¶�BMT
#define BMSID_BSOC	0xC0AB			//0x1817E5F4,������ɵ�����BSOC
#define BMSID_BST	0xA0C9			//0x1419E5F4,BMS��ֹ���BST
#define BMSID_BSD	0xC0E8			//0x181CE5F4,BMS���ͳ��BSD
#define BMSID_BEM	0x80EA			//0x101E05F4,ת�����׶η��͵Ĵ���ͳ�Ʊ���BEM
#define BMSID_BFM	0x818B			//0x103305F4,���ϱ���BFM

#define WZL_BMSID_BCP 0XC02A       //������BMS

//==============CCS ������̽׶�ֵ===========================//
#define CCS_SEND_CRM_0		    0
#define CCS_SEND_CRM_INVALID    1
#define CCS_SEND_CRM_1		    2
#define CCS_SEND_CTS_CML	    3 
#define CCS_SEND_CRO		    4	
#define CCS_SEND_CCS		    5
#define CCS_SEND_CST		    6	
#define CCS_SEND_CSD		    7	

//=========CCS���Ľ��벻ͬ����׶�ֵ����Ӧ��ModbusREGDat��ʼ��ַ�����ݳ���==========//
#define	CCS_CRM_START_ADR		9	//start Adr
#define	CCS_CRM_START_LEN		8
#define CCS_CTS_START_ADR		13	//�������ʱ��ͬ��CTS�����ý׶�
#define CCS_CTS_START_LEN		7
#define CCS_CML_START_ADR		17	//��������������CML�����ý׶� 
#define CCS_CML_START_LEN		8
#define CCS_CRO_START_ADR		21	//�������׼������״̬CRO�����ý׶�
#define CCS_CRO_START_LEN		1
#define CCS_CCS_START_ADR		22	//�������״̬CCS�����׶�
#define CCS_CCS_START_LEN		6
#define CCS_CST_START_ADR		25	//������ֹ���CST�����׶�
#define CCS_CST_START_LEN		3
#define CCS_CSD_START_ADR		27	//����ͳ�����ݱ���CSD�������׶�
#define CCS_CSD_START_LEN		5
#define CCS_CEM_START_ADR		30	//����������CEM  
#define CCS_CEM_START_LEN		3
#define CCS_CFM_START_ADR		32	//�������ϱ���CFM  
#define CCS_CFM_START_LEN		1


//=========BMS���Ľ��벻ͬ����׶�ֵ����Ӧ��ModbusREGDat��ʼ��ַ�����ݳ���==========//
#define	M_BRM_START_ADR		35	//start Adr
#define	M_BRM_ADR_LEN		41
#define	M_VBI_START_ADR		56	//35+21
#define	M_VBI_ADR_LEN		40
#define	M_BCP_START_ADR		76	//56+20
#define	M_BCP_ADR_LEN		13
#define	M_BRO_START_ADR		83	//76+7
#define	M_BRO_ADR_LEN		1
#define	M_BCL_START_ADR		84	//83+1
#define	M_BCL_ADR_LEN		5
#define	M_BCS_START_ADR		87	//84+3
#define	M_BCS_ADR_LEN		8
#define	M_BST_START_ADR		91	//87+4
#define	M_BST_ADR_LEN		3
#define	M_BSD_START_ADR		93	//91+2
#define	M_BSD_ADR_LEN		7
#define	M_BEM_START_ADR		97	//93+4
#define	M_BEM_ADR_LEN		4
#define	M_BFM_START_ADR		99	//97+2
#define	M_BFM_ADR_LEN		7	//99+4=103,��modbusreg[103]
//=====���ò������ε�======//
#define	M_BSM_START_ADR		103
#define	M_BSM_ADR_LEN		7	//7
#define	M_BMV_START_ADR		X
#define	M_BMV_ADR_LEN		X	//0xFF
#define	M_BMT_START_ADR		X
#define	M_BMT_ADR_LEN		X	//0xFF
#define	M_BSOC_START_ADR	X
#define	M_BSOC_ADR_LEN		X	//0xFF

//=========�����ݴ���Э���PGN��ֵ===========//
#define	M_PGN_BRM		0x02  //ֻȡ�м�2���ֽڣ�����Ĭ��0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17

#define MUTICAN_CMD0    0xE7
#define MUTICAN_CMD1    0x68
#define MUTICAN_DAT0    0xE7
#define MUTICAN_DAT1    0x4B

//=========�����ݴ���Э��Ŀ���ָ��ֵ===========//
#define M_TP_CM_RTS_CMD		16
#define M_TP_CM_CTS_CMD 	17
#define M_TP_ENDMSGACK_CMD 	19
#define M_TP_CONN_ABORT_CMD 255     //��������

//========CANһ֡����Ĭ�Ϸ��͵��ֽ���===========//
#define	M_CAN_DEFAULT_LEN	8

//=====����֡�ĸߵ��ֽڶ���=======//
#define BYTE_LOW_8  0   //�͵�ַΪ���ֽ�
#define BYTE_HIGH_8 8   //�ߵ�ַΪ���ֽ�

//==========������==========//
#define  TRUE	1
#define  FALSE	0

//=====CCS��BMS�������ݵ�CAN ID========//
#define BMS_CCS_STATUS_ADR			0		//BMS-CCS״̬�Ĵ���
#define CHARGE_PHASE_ADR			1		//���׶μĴ���
#define CHARGE_CCS_START_ADR		2		//��������Ĵ���
#define BMS_TRANSMIT_ADR			3		//CAN����Ĵ���


//=====CCS��BMS�������ݵ�CAN ID========//
 uint8 CCSID_CRM_0[4] = {0xC0, 0x09, 0xF4, 0xE5,};  //0x1801F4E5,������ʶ����CRM
 uint8 CCSID_CRM_1[4] = {0xC0, 0x09, 0xF4, 0xE5,};  //0x1801F4E5,������ʶ����CRM
 uint8 CCSID_CTS[4]   = {0xC0, 0x2B, 0xF4, 0xE5,};  //0x1807F4E5,CCSʱ��ͬ����ϢCTS
 uint8 CCSID_CML[4]   = {0xC0, 0x48, 0xF4, 0xE5,};  //0x1808F4E5,CCS����������CML
 uint8 CCSID_CRO[4]   = {0xA0, 0x4A, 0xF4, 0xE5,};  //0X140AF4E5,CCS���׼������CRO
 uint8 CCSID_CCS[4]   = {0xC0, 0xA8, 0xF4, 0xE5,};  //0x1814F4E5,CCS���״̬CCS
 uint8 CCSID_CST[4]   = {0xA0, 0xCA, 0xF4, 0xE5,};  //0x141AF4E5,CCS��ֹ���CST
 uint8 CCSID_CSD[4]   = {0xC0, 0xE9, 0xF4, 0xE5,};  //0x181DF4E5,CCS���ͳ��CSD
 uint8 CCSID_CEM[4]   = {0x80, 0xEB, 0xF4, 0xE5,};  //0x101FF4E5,ת�����׶η��͵Ĵ���ͳ�Ʊ���CEM 
 uint8 CCSID_CFM[4]   = {0x81, 0xA8, 0xF4, 0xE5,};  //0x1034F4E5,�������͵Ĺ��ϱ���CFM
 uint8 MUTICAN_CTS_ID[4] = {0xE7, 0x68, 0xF4, 0xE5};//�������ָ֡��

//======PGN���Э��CCS��BMS���͵�����CAN ID============//
 uint8 CCS_ID_PGN_BRM[4] = {0xC0, 0x0A, 0xF4, 0xE5,};  //BRM
 uint8 CCS_ID_PGN_VBI[4] = {0xC1, 0x88, 0xF4, 0xE5,};  //VBI
 uint8 CCS_ID_PGN_BCP[4] = {0xC0, 0x2A, 0xF4, 0xE5,};  //BCP
 uint8 CCS_ID_PGN_BMV[4] = {0xC0, 0xA9, 0xF4, 0xE5,};  //BMV
 uint8 CCS_ID_PGN_BMT[4] = {0xC0, 0xAA, 0xF4, 0xE5,};  //BMT
 uint8 CCS_ID_PGN_BSOC[4] ={0xC0, 0xAB, 0xF4, 0xE5,};  //BSOC

//==========ȫ�ֱ���===========//
//uint16  ModbusREGDat[END_REG_LEN];  //����Modbus�Ĵ���  
uint8 g_bCANBUSTimeoutFlag;   //1--��ʾcanbusͨ�ųɹ���0--��ʾcanbusͨ��ʧ�ܡ�Ĭ��ֵΪ1����������̬ͨ�Ĵ������������йء�
uint8 g_bChargeFullFlag;	//1--��ʾ�����磬0--��ʾδ������
uint8 g_bChargeStartFlag;	//1--��ʾ�������ӣ�0--��ʾ���ܽ�������
uint8 g_bChargingTimeoutFlag; //���������Ƿ���ֳ�ʱ����
uint8 g_bChargeEndFlag; //��������־
uint8 g_bSoftRestStartFlag;
uint16 g_u16Run5SCount,g_u16RunResSCount;

struct _BMSFlow
{
	uint8 u8BMSStep;
	uint8 btransmitingFlag;
	uint8 noChargeFlag;
	uint8 u8ChargingFlag;
	uint8 u8ChargingTimeoutCnt;
} gs_BMSFlow;

uint8  byte_IR;
#define bit_RI_0    (byte_IR & 0x01)        //λ0
#define  bit_RI_1  ((byte_IR & 0x02) >>1) //λ1
#define  bit_TI    ((byte_IR & 0x04) >>2)   //λ2

#define  bit_MERRF  ((byte_IR & 128) >>7)   //λ7
#define  bit_WAKIF  ((byte_IR & 64) >>6)   //λ6
#define  bit_ERRIF  ((byte_IR & 32) >>5)   //λ5

uint8  byte_RXBnSIDL,byte_RXBnDLC;
#define bit_IDE  ((byte_RXBnSIDL & 0x08) >>3) //λ3
#define bit_SRR  ((byte_RXBnSIDL & 16) >>4)   //λ4
#define bit_RTR  ((byte_RXBnDLC & 64) >>6)    //λ6

uint8 byte_EFLG;
#define bit_RX0OVR	((byte_EFLG & 64) >>6)   //λ6
#define bit_RX1OVR	((byte_EFLG & 128) >>7)   //λ7


///////////////////////���ֽ׶�/////////////////////////
typedef struct 
{
	uint8 chargeMachine_code;		//�������
	uint8 plug_code;				//��ͷ���
	uint8 chargeMachine_station;	//����λ�� 0X00 ���� 0X01 ����
	uint8 descern_result;			//��ʶ���  0X00 BMS����ʶ�� 0X01��ʶ��
	uint32 chargeMachine_area;		//���������������

}_CRM;								//������ʶ����

typedef struct
{
	uint8 bms_ver[3];			//BMS�汾��
	uint8 bat_type;				//�������
	uint16 vated_soc;			//�����
	uint16 vated_volt;			//���ѹ
	uint32 bat_manufacturer;	//�����������
	uint32 bat_serial_num;		//��������
	uint8 produce_date[3];		//�������������
	uint8 charge_time[3];		//������
	uint8 bat_flag;				//��ز�Ȩ��ʶ
	uint8 reserve;				//����
	uint8 descern_info[8];		//��ʶ��Ϣ

}_BRM;							//BMS��ʶ����


///////////////////////���������ý׶�/////////////////////////

typedef struct
{
	uint16 permitHighModuleCharge_volt;			//���ģ������������ѹ  �ֱ��ʣ�0.01V/λ�� 0Vƫ���� ����Χ�� 0~600V
	uint16 permitCharge_curr;					//������������	�ֱ��ʣ�0.1A/λ�� -800Vƫ���� ����Χ�� -800~0A
	uint8  max_charge_soc;						//������������	�ֱ��ʣ�1%/λ��0%ƫ���� ����Χ�� 0~100%
	uint16 highChargeSum_volt;					//����������ܵ�ѹ	�ֱ��ʣ�0.1A/λ�� -800Vƫ���� ����Χ��0~800V
	uint8  PermitHigh_temper;					//��������¶�	�ֱ��ʣ�1��/λ�� -40��ƫ���� ����Χ�� -40~210��
	uint16 bat_soc;								//���ʣ������	�ֱ��ʣ�1AH/λ�� 0AHƫ���� ����Χ�� 0~1000AH
	uint16 batSum_volt;							//����ܵ�ѹ	�ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V

}_BCP;											//���س���������

typedef struct
{
	uint8 time[7];								//���ʱ�䣬������ʱ����	

}_CTS;											//��������ʱ��ͬ����Ϣ����

typedef struct 
{
	uint16 outVolt_H;							//��������ѹ	�ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V
	uint16 outVolt_L;							//��������ѹ  �ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V
	uint16 outCurr_H;							//����������  �ֱ��ʣ�0.1V/λ�� -800Aƫ���� ����Χ��-800~0A
	uint8  pulse_curr;							//����ŵ����	�ֱ��ʣ�2A/λ�� 0Aƫ���� ����Χ��0~500A
	uint8 pulse_time;							//����ʱ��		�ֱ��ʣ�1S/λ�� 0Sƫ���� ����Χ��0~255S

}_CML;											//������������������


///////////////////////���׼���׶�/////////////////////////
typedef struct
{
	uint8 bsm_prepare_flag;						//BMS���׼����־  0000B ���ܳ��   0010B BMSδ׼����  0100B BMS��ɳ��׼��

}_BRO;											//BMS���׼����������

typedef struct
{
	uint8 chargeMachine_prepare_flag;			//�������׼����־   0010B δ׼����  0100B ��ɳ��׼��

}_CRO;											//�������׼����������


///////////////////////���׶�/////////////////////////
typedef struct 
{
	uint16 needVolt;							//��������ѹ	�ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V
	uint16 needCurr;							//��������ѹ  �ֱ��ʣ�0.AV/λ��-800Vƫ���� ����Χ��-800~500A
	uint16 chargeMode;							//���ģʽ 001B ��ѹ���   010B�������

}_BCL;											//��س��������


typedef struct 
{
	uint16 chargeMeasure_volt;					//����ѹ����ֵ	�ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V
	uint16 chargeMeasure_curr;					//����������ֵ	�ֱ��ʣ�0.AV/λ��-800Vƫ���� ����Χ��-800~500A
	uint8  bat_temper_L;						//�������¶�		�ֱ��ʣ�1��/λ ��-50��ƫ���� ����Χ��150~200��
	uint8  bat_temper_H;						//�������¶�		�ֱ��ʣ�1��/λ ��-50��ƫ���� ����Χ��150~200��
	uint8  soc;									//�������			�ֱ��ʣ�1%/λ��0%ƫ���� ����Χ�� 0~100%
	uint8  remain_time;							//����ʣ��ʱ��		�ֱ��ʣ�1min/λ��0minƫ���� ����Χ�� 0~250min  

}_BCS;//��س����״̬����


typedef struct 
{
	uint16 out_volt;					//�����ѹ	�ֱ��ʣ�0.1V/λ�� 0Vƫ���� ����Χ��0~800V
	uint16 out_curr;					//�������	�ֱ��ʣ�0.AV/λ��-800Vƫ���� ����Χ��-800~500A
	uint8  remain_time;					//���ʱ��		�ֱ��ʣ�1min/λ��0minƫ���� ����Χ�� 0~250min  

}_CCS;//�������״̬����



typedef struct 
{	
	uint16  highBatVolt_group;		
	uint8   highBatVolt_addrCode;
	uint8   highBatTemper;
	uint8   highBatTestCode;
	uint8   batCell_highVolt;
	uint8   batCell_lowVolt;
	uint8   baSOC_high;
	uint8   baSOC_low;
	uint8   bat_highCurr;
	uint8   bat_highTemper;
	uint8   isolationState;
	uint8   batConnectState;
	uint8   wachdogState;
	uint8   chargePermit;

}_BSM;//BMS��������״̬��Ϣ����

typedef struct 
{	
			
	uint16   batVolt[16];	

}_BMV;//������С��Ԫ��ѹ����

typedef struct 
{	
			
	uint8   battemper[20];	

}_BMT;//������С��Ԫ�¶ȱ���


typedef struct 
{	
			
	uint8   bSOC[20];	

}_BSOC;//������С��Ԫ�¶ȱ���


typedef struct 
{	
			
	uint8   stopCause;				//BMS��ֹ���ԭ��
	uint8   failureCause;			//BMS��ֹ������ԭ��
	uint8   errCause;				//BMS��ֹ������ԭ��

}_BST;//BMS��ֹ���ԭ��  

typedef struct 
{	
			
	uint8   stopCause;				//��ֹ���ԭ��
	uint8   failureCause;			//��ֹ������ԭ��
	uint8   errCause;				//��ֹ������ԭ��

}_CST;//������ֹ���ԭ����


///////////////////////�������׶�/////////////////////////

typedef struct 
{	
			
	uint8   stop_SOC;				//��ֹ״̬��SOC
	uint16   cell_lowVolt;			//��͵����ѹ
	uint16   cell_highVolt;			//��ߵ����ѹ
	uint16   bat_lowTemper;			//����¶�
	uint16	 bat_highTemper;		//����¶�

}_BSD;//BMSͳ�����ݱ���


typedef struct 
{	
			
	uint16   chargeTime;				//���ʱ��
	uint16   outPower;					//�������
	uint8    mechineCode;			    //�������
	

}_CSD;//����ͳ�����ݱ���


///////////////////////������/////////////////////////

typedef struct 
{			
	uint8   assistant_power_err;		//������Դ�Ƿ�����?
	uint8   CRM0_timeOut;				//����SPN2563=0������ʶ���ĳ�ʱ
	uint8   CRM1_timeOut;				//����SPN2563=1������ʶ���ĳ�ʱ
	uint8   CTS_CML_timeOut;			//���ճ���ͬ��ʱ����������������ĳ�ʱ
	uint8   CRO_timeOut;				//���ճ�����ɳ��׼�����ĳ�ʱ
	uint8   CCS_timeOut;				//���ճ���״̬���ĳ�ʱ
	uint8   CST_timeOut;				//���ճ�����ֹ��籨�ĳ�ʱ
	uint8   CSD_timeOut;				//���ճ���ͳ�Ʊ��ĳ�ʱ

}_BEM;//BMS������


typedef struct 
{	
			
	uint8   BRM_timeOut;				//����BMS��ʶ���ĳ�ʱ
	uint8   BCP_timeOut;				//���յ�س��������ĳ�ʱ
	uint8   BRO_timeOut;				//����BMS���׼�����ĳ�ʱ
	uint8   BSM_timeOut;				//����BMS���״̬���ĳ�ʱ
	uint8   BCL_timeOut;				//����BMS���Ҫ���ĳ�ʱ
	uint8   BST_timeOut;				//����BMS��ֹ��籨�ĳ�ʱ
	uint8   BSD_timeOut;				//����BMSͳ�����ݱ��ĳ�ʱ

}_CEM;//����������



typedef struct
{
	_BRM brm_cmd;		//BMS��ʶ����
	_BCP bcp_cmd;		//���س���������
	_BRO bro_cmd;		//BMS���׼����������
	_BCL bcl_cmd;		//��س��������
	_BCS bcs_cmd;		//��س����״̬����
	_BSM bsm_cmd;		//BMS��������״̬��Ϣ����
	_BMV bmv_cmd;		//������С��Ԫ��ѹ����
	_BMT bmt_cmd;		//������С��Ԫ�¶ȱ���
	_BSOC bsoc_cmd;		//������С��Ԫ�¶ȱ���
	_BST bst_cmd;		//BMS��ֹ���ԭ��  
	_BSD bsd_cmd;		//BMSͳ�����ݱ���
	_BEM bem_cmd;		//BMS������

}BMS_MSG;


typedef struct
{
	_CRM crm_cmd;   //������ʶ����
	_CTS cts_cmd;	//��������ʱ��ͬ����Ϣ����
	_CML cml_cmd;   //������������������
	_CRO cro_cmd;   //�������׼����������
	_CCS ccs_cmd;   //�������״̬����
	_CST cst_cmd;	//������ֹ���ԭ����
	_CSD csd_cmd;	//����ͳ�����ݱ���
	_CEM cem_cmd;	//����������


}CCS_MSG;



/****************************************�����꣬������BMSЭ��*********************************************/

#define  VOL_SET_H						0
#define  VOL_SET_L						1
#define  CUR_SET_H						2
#define  CUR_SET_L						3
#define  CONTROL_FLG					4

#define  AH_marker_H					8
#define  AH_marker_L					9
#define  AH_actual_H					10
#define  AH_actual_L					11
#define  VOL_CELL_OVER_protect_H		12
#define  VOL_CELL_OVER_protect_L		13
#define  BAT_NUM						14

#define  VOL_CELL_MAX_H					16
#define  VOL_CELL_MAX_L					17
#define  VOL_CELL_MINH					18
#define  VOL_CELL_MIN_L					19
#define  VOL_CELL_ULV_protect_H			20
#define  VOL_CELL_ULV_protect_L			21
#define  BAT_STATE						22

#define VOL_BAT_H						24
#define VOL_BAT_L						25
#define CUR_CHARGE_H					26
#define CUR_CHARGE_L					27
#define SOC								28
#define TEMP_MAX						29
#define TEMP_MIN						30

#define OUT_VOL_H						32
#define OUT_VOL_L						33
#define OUT_CUR_H						34
#define OUT_CUR_L						35
#define STATUS							36


#define BMS_CTL_CMD						0



typedef struct
{
	fp32 volt_limit;
	fp32 curr_limit;
	uint8 control_flg; 
	fp32  bat_volt;
	fp32  bat_charge_cur;
	uint8 soc;

}WZL_BMS_INFO;  //������BMS��Ϣ


typedef struct
{
	fp32 out_volt;
	fp32 out_cur;
	uint8 status; 

}WZL_CCS_INFO;//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

#endif