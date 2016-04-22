#ifndef __CAN_CFG_H
#define __CAN_CFG_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "stm32f10x.h"
#include "define.h"

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define PCU_ADR               (gUserDB.pcu_adr)           //PCU��ַ
#define CCS_ADR               0X2B			//������ַ
#define BROADCAST_ADR         0XFF			//�㲥��ַ
#define BMS_MAX_SIZE          200           //BMS�������ֽڳ���
#define CCS_MAX_SIZE          200           //CCS�������ֽڳ���

#define CAN_REC_FILTER	   0x00
#define CAN_REC_ALL 		   0x01


//=========�����ݴ���Э���PGN��ֵ===========//
#define	M_PGN_BRM		0x02  //ֻȡ�м�2���ֽڣ�����Ĭ��0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17 

//*************************PC ����CAN ͨ�ű���ID***************************************************
#define PC_PC_DATE                      0X1800FF00                         //ϵͳ��ʱ����
#define PC_PCU_P_SET                    (0X18010000+(PCU_ADR<<8))          //Э��汾���ñ���
#define PC_INQUIRE_ALL                  (0X18040000+(PCU_ADR<<8))          //��ѯ����� 


#define PC_MCM_STOP_ALL                (0X045CFF00)                      //����ͣ������ 
#define PC_MCM_START_SINGLE           (0X085D0000+(PCU_ADR<<8))          //�������� 
#define PC_MCM_STOP_SINGLE            (0X045E0000+(PCU_ADR<<8))          //ֹͣ���� 
#define PC_MCM_SET_SINGLE             (0X0C5F0000+(PCU_ADR<<8))          //������Ʋ������� 
#define PC_MCM_SET_STAGE              (0X0C600000+(PCU_ADR<<8))          //�׶ο��Ʋ�������
#define PC_MCM_READ_SET_ALL           (0X10660000+(PCU_ADR<<8))          //ȫ�����Ʋ�����ȡ���� 
#define PC_MCM_READ_SET_SPECIAL      (0X10670000+(PCU_ADR<<8))          //��ѯ����� 



//*************************PCU ����CAN ͨ�ű���ID************************************************
#define PCU_PCU_LIFE                     (0X08020000+(PCU_ADR<<8))          //�������� 
#define PCU_BMS_IDENTICATION        (0X18060000+PCU_ADR)          //BMS��ʶ����
#define PCU_BMS_INFORMATION1        (0X18070000+PCU_ADR)          //BMS��Ϣ1����
#define PCU_BMS_INFORMATION2        (0X18080000+PCU_ADR)          //BMS��Ϣ2����
#define PCU_BMS_INFORMATION3        (0X18090000+PCU_ADR)          //BMS��Ϣ3����
#define PCU_BMS_INFORMATION4        (0X18500000+PCU_ADR)          //BMS��Ϣ4����
#define PCU_BMS_LIFE                 (0X180A0000+PCU_ADR)          //BMS����֡���� 
#define PCU_BAT_REQIRE               (0X180B0000+PCU_ADR)          //��س�缶�������� 
#define PCU_BAT_STATE                (0X180C0000+PCU_ADR)          //��س��״̬���� 
#define PCU_BAT_STATE_LIMIT         (0X180D0000+PCU_ADR)          //������״̬����
#define PCU_BAT_STOP                 (0X180E0000+PCU_ADR)          //��س��ֹͣ����
#define PCU_BAT_COUNT                (0X180F0000+PCU_ADR)          //��س��״̬���� 
#define PCU_BAT_ERR                  (0X18010000+PCU_ADR)           //BMS������ 
#define PCU_BAT_FAIL                 (0X18011000+PCU_ADR)          //BMS���ϱ���
#define PCU_BAT_FAIL2                (0X18012000+PCU_ADR)          //BMS���ϱ���2 

#define PCU_BAT_VOLT_N1               (0X18013000+PCU_ADR)          //��ص����ѹ���ݱ���1
#define PCU_BAT_VOLT_N2               (0X18014000+PCU_ADR)          //��ص����ѹ���ݱ���2
#define PCU_BAT_VOLT_N3               (0X18015000+PCU_ADR)          //��ص����ѹ���ݱ���3
#define PCU_BAT_VOLT_N4               (0X18016000+PCU_ADR)          //��ص����ѹ���ݱ���4
#define PCU_BAT_VOLT_N5               (0X18017000+PCU_ADR)          //��ص����ѹ���ݱ���5
#define PCU_BAT_VOLT_N6               (0X18018000+PCU_ADR)          //��ص����ѹ���ݱ���6
#define PCU_BAT_VOLT_N7               (0X18019000+PCU_ADR)          //��ص����ѹ���ݱ���7
#define PCU_BAT_VOLT_N8               (0X1801A000+PCU_ADR)          //��ص����ѹ���ݱ���8
#define PCU_BAT_VOLT_N9               (0X1801B000+PCU_ADR)          //��ص����ѹ���ݱ���9
#define PCU_BAT_VOLT_N10               (0X1801C000+PCU_ADR)          //��ص����ѹ���ݱ���10
#define PCU_BAT_VOLT_N11               (0X1801D000+PCU_ADR)          //��ص����ѹ���ݱ���11
#define PCU_BAT_VOLT_N12               (0X1801E000+PCU_ADR)          //��ص����ѹ���ݱ���12
#define PCU_BAT_VOLT_N13               (0X1801F000+PCU_ADR)          //��ص����ѹ���ݱ���13
#define PCU_BAT_VOLT_N14               (0X18020000+PCU_ADR)          //��ص����ѹ���ݱ���14
#define PCU_BAT_VOLT_N15               (0X18021000+PCU_ADR)          //��ص����ѹ���ݱ���15
#define PCU_BAT_VOLT_N16               (0X18022000+PCU_ADR)          //��ص����ѹ���ݱ���16
#define PCU_BAT_VOLT_N17               (0X18023000+PCU_ADR)          //��ص����ѹ���ݱ���17
#define PCU_BAT_VOLT_N18               (0X18024000+PCU_ADR)          //��ص����ѹ���ݱ���18
#define PCU_BAT_VOLT_N19               (0X18025000+PCU_ADR)          //��ص����ѹ���ݱ���19
#define PCU_BAT_VOLT_N20               (0X18026000+PCU_ADR)          //��ص����ѹ���ݱ���20

#define PCU_BAT_TEMP_N1               (0X18027000+PCU_ADR)          //��ص����¶����ݱ���1
#define PCU_BAT_TEMP_N2               (0X18028000+PCU_ADR)          //��ص����¶����ݱ���2
#define PCU_BAT_TEMP_N3               (0X18029000+PCU_ADR)          //��ص����¶����ݱ���3
#define PCU_BAT_TEMP_N4               (0X1802A000+PCU_ADR)          //��ص����¶����ݱ���4
#define PCU_BAT_TEMP_N5               (0X1802B000+PCU_ADR)          //��ص����¶����ݱ���5
#define PCU_BAT_TEMP_N6               (0X1802C000+PCU_ADR)          //��ص����¶����ݱ���6
#define PCU_BAT_TEMP_N7               (0X1802D000+PCU_ADR)          //��ص����¶����ݱ���7
#define PCU_BAT_TEMP_N8               (0X1802E000+PCU_ADR)          //��ص����¶����ݱ���8
#define PCU_BAT_TEMP_N9               (0X1802F000+PCU_ADR)          //��ص����¶����ݱ���9
#define PCU_BAT_TEMP_N10               (0X18030000+PCU_ADR)          //��ص����¶����ݱ���10
#define PCU_BAT_TEMP_N11               (0X18031000+PCU_ADR)          //��ص����¶����ݱ���11
#define PCU_BAT_TEMP_N12               (0X18032000+PCU_ADR)          //��ص����¶����ݱ���12
#define PCU_BAT_TEMP_N13               (0X18033000+PCU_ADR)          //��ص����¶����ݱ���13
#define PCU_BAT_TEMP_N14               (0X18034000+PCU_ADR)          //��ص����¶����ݱ���14
#define PCU_BAT_TEMP_N15               (0X18035000+PCU_ADR)          //��ص����¶����ݱ���15
#define PCU_BAT_TEMP_N16               (0X18036000+PCU_ADR)          //��ص����¶����ݱ���16
#define PCU_BAT_TEMP_N17               (0X18037000+PCU_ADR)          //��ص����¶����ݱ���17
#define PCU_BAT_TEMP_N18               (0X18038000+PCU_ADR)          //��ص����¶����ݱ���18
#define PCU_BAT_TEMP_N19               (0X18039000+PCU_ADR)          //��ص����¶����ݱ���19
#define PCU_BAT_TEMP_N20               (0X1803A000+PCU_ADR)          //��ص����¶����ݱ���20
#define PCU_BAT_ID1                     (0X1803B000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID2                     (0X1803C000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID3                     (0X1803D000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID4                     (0X1803E000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID5                     (0X1803F000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID6                     (0X18040000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID7                     (0X18041000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID8                     (0X18042000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID9                     (0X18043000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID10                     (0X18044000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID11                     (0X18045000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID12                     (0X18046000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID13                     (0X18047000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID14                     (0X18048000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID15                     (0X18049000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID16                     (0X1804A000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID17                     (0X1804B000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID18                     (0X1804C000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID19                     (0X1804D000+PCU_ADR)            //�����ID����
#define PCU_BAT_ID20                     (0X1804E000+PCU_ADR)            //�����ID����




#define PCU_MCM_RETURN_SET_SINGLE_ALL          (0X10690000+PCU_ADR)            //��ȡ���е�����Ʋ���
#define PCU_MCM_RETURN_SET_STAGE_ALL           (0X106A0000+PCU_ADR)            //��ȡ���н׶ο��Ʋ���
#define PCU_MCM_RETURN_SET_SINGLE              (0X10700000+PCU_ADR)            //��ȡ������Ʋ���
#define PCU_MCM_RETURN_SET_STAGE           	  (0X10710000+PCU_ADR)            //��ȡ������Ʋ���
#define PCU_MCM_STATE0                           (0X18780000+PCU_ADR)            //����״̬֡1
#define PCU_MCM_STATE1                           (0X187A0000+PCU_ADR)            //����״̬֡2
#define PCU_MCM_STATE2                           (0X187B0000+PCU_ADR)            //����״̬֡3
#define PCU_MCM_STATE3                           (0X187C0000+PCU_ADR)            //����״̬֡4
#define PCU_MCM_PDU_INFO                         (0X187D0000+PCU_ADR)            //�����Ϣ֡
#define PCU_MCM_DEVICE_ID                        (0X18800000+PCU_ADR)            //����豸��Ϣ֡

#define PCU_CCS_IDENTIFY                        (0X187E0000+PCU_ADR)            //����ʶ����
#define PCU_CCS_MAX_OUTPUT                      (0X187F0000+PCU_ADR)            //����ʶ����
#define PCU_CCS_STATE                            (0X18820000+PCU_ADR)            //���״̬����
#define PCU_CCS_STOP                            (0X14830000+PCU_ADR)            //�����ֹ����
#define PCU_CCS_COUNT                           (0X18840000+PCU_ADR)            //���ͳ�Ʊ���
#define PCU_CCS_ERR                             (0X10850000+PCU_ADR)            //��������
#define PCU_CCS_FAULT                           (0X10860000+PCU_ADR)            //�����ϱ���




//=========���ع���ϵͳBMS PGN================================================//
#define BMS_BCP1_PGN     		0X60         		//���س�����
#define BMS_CBP1_PGN     		0X70		  		//�������ز���1
#define BMS_CBP2_PGN     		0X80 				//�������ز���2
#define BMS_BCS_PGN             0X90				//���س��״̬
#define BMS_CTS_PGN             0XA0                //ʱ��ͬ����Ϣ
#define BMS_MVD_PGN             0XB0				//����ģ���ѹ����
#define BMS_BTD_PGN             0XC0				//�����¶�����
#define BMS_SOC_PGN             0XD0                //����SOC����
#define BMS_AVD_PGN             0XE0                //����ƽ����ѹ����
#define BMS_BBI_PGN             0XF1               	//���ػ�����Ϣ
#define BMS_BBI1_PGN            0XF100               	//���ػ�����Ϣ1
#define BMS_BBI2_PGN            0XF101               	//���ػ�����Ϣ2

//=========����CCS PGN=======================================================//
#define CCS_CLF_PGN             0X20                //����������Ϣ֡
#define CCS_CCS1_PGN            0X30                //����״̬1
#define CCS_CCS2_PGN            0X31                //����״̬2
#define CCS_CCP_PGN             0X40                //��������
#define CCS_EST_PGN             0X51                //��������ֹͣ����
#define CCS_CST_PGN             0X52                //���������������
#define CCS_CSP_PGN             0X53                //����ֹͣ������� 

//=========���ع���ϵͳBMS PGN LEN ================================================//
#define BMS_BCP1_PGN_LEN     		8         		//���س�����
#define BMS_CBP1_PGN_LEN     		8		  		//�������ز���1
#define BMS_CBP2_PGN_LEN     		8 				//�������ز���2
#define BMS_BCS_PGN_LEN             8				//���س��״̬
#define BMS_CTS_PGN_LEN             8                //ʱ��ͬ����Ϣ
#define BMS_MVD_PGN_LEN             8				//����ģ���ѹ����
#define BMS_BTD_PGN_LEN             8				//�����¶�����
#define BMS_SOC_PGN_LEN             8                //����SOC����
#define BMS_AVD_PGN_LEN             8                //����ƽ����ѹ����
#define BMS_BBI_PGN_LEN             8               	//���ػ�����Ϣ
#define BMS_BBI1_PGN_LEN            8               	//���ػ�����Ϣ1
#define BMS_BBI2_PGN_LEN            8               	//���ػ�����Ϣ2

//=========���ع���ϵͳBMS PGN ADR ��������ַ���� ==================================//
#define BMS_BCP1_PGN_ADR     		0         										//���س�����
#define BMS_CBP1_PGN_ADR     		(BMS_BCP1_PGN_ADR+BMS_BCP1_PGN_LEN)		  		//�������ز���1
#define BMS_CBP2_PGN_ADR     		(BMS_CBP1_PGN_ADR+BMS_CBP1_PGN_LEN) 			//�������ز���2
#define BMS_BCS_PGN_ADR             (BMS_CBP2_PGN_ADR+BMS_CBP2_PGN_LEN)				//���س��״̬
#define BMS_CTS_PGN_ADR             (BMS_BCS_PGN_ADR+BMS_BCS_PGN_LEN)               //ʱ��ͬ����Ϣ
#define BMS_MVD_PGN_ADR             (BMS_CTS_PGN_ADR+BMS_CTS_PGN_LEN)				//����ģ���ѹ���ݣ����
#define BMS_BTD_PGN_ADR             (BMS_MVD_PGN_ADR+BMS_MVD_PGN_LEN)				//�����¶����ݣ����
#define BMS_SOC_PGN_ADR             (BMS_BTD_PGN_ADR+BMS_BTD_PGN_LEN)               //����SOC���ݣ����
#define BMS_AVD_PGN_ADR             (BMS_SOC_PGN_ADR+BMS_SOC_PGN_LEN)               //����ƽ����ѹ���ݣ����
#define BMS_BBI_PGN_ADR             (BMS_AVD_PGN_ADR+BMS_AVD_PGN_LEN)               //���ػ�����Ϣ
#define BMS_BBI1_PGN_ADR            (BMS_BBI_PGN_ADR+BMS_BBI_PGN_LEN)               //���ػ�����Ϣ1
#define BMS_BBI2_PGN_ADR            (BMS_BBI1_PGN_ADR+BMS_BBI1_PGN_LEN)             //���ػ�����Ϣ2

//=========����CCS PGN LEN=======================================================//
#define CCS_CLF_PGN_LEN             8                //����������Ϣ֡
#define CCS_CCS1_PGN_LEN            8                //����״̬1
#define CCS_CCS2_PGN_LEN            8                //����״̬2
#define CCS_CCP_PGN_LEN             8                //�������������
#define CCS_EST_PGN_LEN             8                //��������ֹͣ����
#define CCS_CST_PGN_LEN             8                //���������������
#define CCS_CSP_PGN_LEN             8                //����ֹͣ������� 

//=========����CCS PGN ADR =======================================================//
#define CCS_CLF_PGN_ADR             0                								 //����������Ϣ֡
#define CCS_CCS1_PGN_ADR            (CCS_CLF_PGN_ADR+CCS_CLF_PGN_LEN)                //����״̬1
#define CCS_CCS2_PGN_ADR            (CCS_CCS1_PGN_ADR+CCS_CCS1_PGN_LEN)                //����״̬2
#define CCS_CCP_PGN_ADR             (CCS_CCS2_PGN_ADR+CCS_CCS2_PGN_LEN)                //�������������
#define CCS_EST_PGN_ADR             (CCS_CCP_PGN_ADR+CCS_CCP_PGN_LEN)                //��������ֹͣ����
#define CCS_CST_PGN_ADR             (CCS_EST_PGN_ADR+CCS_EST_PGN_LEN)                //���������������
#define CCS_CSP_PGN_ADR             (CCS_CST_PGN_ADR+CCS_CST_PGN_LEN)                //����ֹͣ������� 



//==============CCS ������̽׶�ֵ============================================//
#define CCS_SEND_CRM_0		    0	//������ʶ����CRM
#define CCS_SEND_CRM_INVALID    1	//��Ч
#define CCS_SEND_CRM_1		    2	//������ʶ����CRM
#define CCS_SEND_CTS_CML	    3 	//CCS����������CML
#define CCS_SEND_CRO		    4	//CCS���׼������CRO
#define CCS_SEND_CCS		    5	//CCS���״̬CCS
#define CCS_SEND_CST		    6	//CCS���״̬CCS
#define CCS_SEND_CSD		    7	//CCS���ͳ��CSD

    


/********************************************************************************************************************
*                                            ��������������
*********************************************************************************************************************/

	

#endif



