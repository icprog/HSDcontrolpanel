#ifndef __DEFINE_H
#define __DEFINE_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
 #include "ucos_ii.h"

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short int   uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int   int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int  uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int  int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float           fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
typedef unsigned long uint64;
typedef signed long int64;

#define HANDLE_STAGE                		0X01     //���ֽ׶�
#define PARA_SET_STAGE              		0X02     //�������ý׶�
#define CHARGEING_STAGE                	    0X03     //���׶�
#define ENDCHARGE_STAGE                     0X04     //�������׶�
#define IDLE_STAGE                          0X00     //���н׶�

#define  NORMAL_MODE                        0X00
#define  TEST_MODE                          0X01
#define  ERR_MAX_CNT                        10        //����������ֵ
#define  ERR_OUT_VOLTCURR_CNT               50
#define  RUN                                0X01      //ģ������
#define  STOP                               0X00	  //ģ��ֹͣ

#define  MANUAL_CTL                         0X01
#define  AUTO_CTL                           0X00

#define ON                                  0X01
#define OFF                                 0X00


#define   ALARM                              0X01
#define   NOALARM                            0X00

#define    OK                                0X01	  //
#define    NOK 							     0X00     //

#define  WATCHDOG_EN                 0         //���Ź�ʹ��  

//#define  AC_MODULE                   1

//**************************modbus��ַ****************************************************//
#define  MODBUS_DEVICEID          2
#define  MODBUSBUFMAX             130
#define  PCU_STATUS_ADR           0          //PCU״̬
#define  PDU_VOLT_ADR       	    1		   //����ѹ��ַ
#define  PDU_CUR_ADR       		2	 	   //������������ַ
#define  PDU_POWER_L_ADR         	3 		   //�����ܵ�λ��ַ
#define  PDU_POWER_H_ADR       	4 		   //�����ܸ�λ��ַ
#define  TIME1_ADR         		5 		   //ʱ��1��ַ
#define  TIME2_ADR                 6  		   //ʱ��2��ַ
#define  TIME3_ADR                 7 		   //ʱ��3��ַ
#define  REMOTE_CTL_ADR            8 		   //Զ�̿��Ƶ�ַ

#define  M_PDU_ADR                  109 		       //����ַ
#define  M_SET_SINGLE_SET_TYPE          110 		   //�������õ�ַ
#define  M_SET_P_CELL_VOLT              111 		   //�������õ�ַ
#define  M_SET_P_CELL_TEMP              112		   //�������õ�ַ
#define  M_SET_STAGE_TYPE               113 		   //�׶����õ�ַ
#define  M_SET_OUT_VALUE                114 		   //�׶����õ�ַ
#define  M_SET_LIMT_VALUE               115 		   //�׶����õ�ַ
#define  M_SET_STOP_VALUE               116 		   //�׶����õ�ַ

#define  M_R_SINGLE_SET_TYPE           110 		   //�������õ�ַ
#define  M_R_P_CELL_VOLT               111 		   //�������õ�ַ
#define  M_R_P_CELL_TEMP               112		    //�������õ�ַ
#define  M_R_STAGE_TYPE                113 		   //�׶����õ�ַ
#define  M_R_OUT_VALUE                 114 		   //�׶����õ�ַ
#define  M_R_LIMT_VALUE                115 		   //�׶����õ�ַ
#define  M_R_STOP_VALUE                116 		  // �׶����õ�ַ
#define  M_CCS_STATUS                  117		  //����״̬��ַ















#define  POWER_TOTAL_ADR            9          //�ѳ������ַ
#define  ADJ_CONFIRM_ADR            10         //У��ȷ��
//#define  MODULES_TEMPER_ADR         11	       //ģ���¶ȵ�ַ
#define  HEART_ADR                  12	       //������ַ



#define  SET_OUT_VOLT_K1_ADR        4		   //�����������Kֵ   
#define  SET_OUT_VOLT_K2_ADR        6		   //�����������Kֵ 
#define  SET_OUT_VOLT_K3_ADR        11		   //�����������Kֵ 

#define  DATA_RESET_ADR             13          //������λ
#define  DISP_IN_VOLT_ADR           14			//�����ѹ��ַ
#define  DISP_TEMP_ADR              15         //�¶ȵ�ַ
#define  SET_OUT_CUR_K1_ADR         16		   //�����������Kֵ
#define  SET_OUT_CUR_K2_ADR         17		   //�����������Kֵ
#define  SET_OUT_CUR_K3_ADR         18		   //�����������Kֵ

#define  SET_DISP_VOLT_K1_ADR        19		   //������ʾ��ѹKֵ
#define  SET_DISP_CUR_K1_ADR         21	 	   //������ʾ����Kֵ

#define  SET_DISP_IN_VOLT_K1_ADR      20		   //������ʾ��ѹKֵ
#define  SET_DISP_TEMP_K1_ADR         22	 	   //������ʾ����Kֵ   

#define  VOLT_PWM_ADR                23	 	   //������ʾ����Kֵ  
#define  CUR_PWM_ADR                 24	 	   //������ʾ����Kֵ  

#define	SOFTVERSION				11060100					//����汾��1106V1.00
#define	PWM_PERIOD				12000						//PWM�����ֵ,Ҳ���趨���pwm�ķֱ���
/***********��Ҫ����Ĳ�����24c16�еĴ洢��ַ***************/	

#define ADDR_FIRSTFLAG				1							//�����Ƿ�д���־λ�洢��ַ

#define ADDR_AUTO_FLAG	        	2							//�ֶ�/�Զ����Ʊ�־�洢��ַռ��1���ֽ�
#define ADDR_AVER_FLAG	        	3							//������ģʽ��־�洢��ַռ��1���ֽ�	

#define ADDR_MANUAL_AVER_VOLT		4							//�ֶ������ѹ�洢��ַռ��4���ֽ�
#define ADDR_MANUAL_FLOAT_VOLT		8							//�ֶ������ѹ�洢��ַռ��4���ֽ�
#define ADDR_MANUAL_SET_VOLT		12							//�ֶ����õ�ѹ�洢��ַռ��4���ֽ�
#define ADDR_MANUAL_SET_CURR		16							//�ֶ����õ����洢��ַռ��4���ֽ�
#define ADDR_MAX_VOLT	        	20							//��ߵ�ѹ�洢��ַռ��4���ֽ�
#define ADDR_MAX_CURR	        	24							//��ߵ����洢��ַռ��4���ֽ�
#define SINGLE_VOLT	        	    28							//��ߵ����ѹ�洢��ַռ��4���ֽ�	 



#define BEEP_OFF        GPIO_ResetBits(GPIOC, GPIO_Pin_0)				//led1�� 
#define BEEP_ON      GPIO_SetBits(GPIOC, GPIO_Pin_0)			        //led1��        

#define LED1_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_8)				//led1�� 
#define LED1_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_8)			        //led1��  
#define LED2_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_9)				//led2��  
#define LED2_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_9)			        //led2��
#define LED3_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_10)				//led3��
#define LED3_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_10)			        //led3��
#define LED4_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_11)				//led1�� 
#define LED4_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_11)			        //led1��  
#define LED5_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_12)				//led2��  
#define LED5_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_12)			        //led2��
#define LED6_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_13)				//led3��
#define LED6_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_13)			        //led3�� 
#define LED7_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_14)				//led1�� 
#define LED7_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_14)			        //led1��  
#define LED8_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_15)				//led2��  
#define LED8_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_15)			        //led2��

#define OUT1_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_0)				//OUT1�� 
#define OUT1_ON       GPIO_SetBits(GPIOE, GPIO_Pin_0)			        //OUT1��  
#define OUT2_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_1)				//OUT2��  
#define OUT2_ON       GPIO_SetBits(GPIOE, GPIO_Pin_1)			        //OUT2��
#define OUT3_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_2)				//OUT3��
#define OUT3_ON       GPIO_SetBits(GPIOE, GPIO_Pin_2)			        //OUT3��
#define OUT4_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_3)				//OUT1�� 
#define OUT4_ON       GPIO_SetBits(GPIOE, GPIO_Pin_3)			        //OUT1��  
#define OUT5_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_4)				//OUT2��  
#define OUT5_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_4)			        //OUT2��
#define OUT6_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_5)				//OUT3��
#define OUT6_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_5)			        //OUT3�� 
#define OUT7_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_6)				//OUT1�� 
#define OUT7_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_6)			        //OUT1��  
#define OUT8_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_7)				//OUT2��  
#define OUT8_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_7)			        //OUT2��


#define PWM_ON        GPIO_ResetBits(GPIOB, GPIO_Pin_8)				//
#define PWM_OFF       GPIO_SetBits(GPIOB, GPIO_Pin_8)			        //

#define PWM_L       GPIO_ResetBits(GPIOB, GPIO_Pin_0)				//
#define PWM_H       GPIO_SetBits(GPIOB, GPIO_Pin_0)			        //

#define SET_4051_A_H    GPIO_ResetBits(GPIOC, GPIO_Pin_1)				//�ߵ�ƽ 
#define SET_4051_A_L    GPIO_SetBits(GPIOC, GPIO_Pin_1)				    //�͵�ƽ 
#define SET_4051_B_H    GPIO_ResetBits(GPIOC, GPIO_Pin_2)				//�ߵ�ƽ 
#define SET_4051_B_L    GPIO_SetBits(GPIOC, GPIO_Pin_2)				    //�͵�ƽ
#define SET_4051_C_H    GPIO_ResetBits(GPIOC, GPIO_Pin_3)				//�ߵ�ƽ 
#define SET_4051_C_L    GPIO_SetBits(GPIOC, GPIO_Pin_3)				    //�͵�ƽ    


#define	CLK_SET			GPIOB->BSRR = GPIO_Pin_14			//����CLK����Ϊ�ߵ�ƽ
#define	CLR_SET			GPIOB->BSRR = GPIO_Pin_13			//����CLR����Ϊ�͵�ƽ
#define	CLR_RESET		GPIOA->BRR = GPIO_Pin_13			//����CLR����Ϊ�͵�ƽ
#define	CLK_RESET		GPIOB->BRR = GPIO_Pin_14			//����CLK����Ϊ�͵�ƽ
#define	SDA_VALUE		(*(INT32U*)(0x422181BC))			//GPIOB->PIN_15�ĵ�ַ



#define UART_1			0
#define UART_2			1
#define UART_3			2
#define UART_4			3
#define UART_5			4


#define USER_CMD_WAIT                         0X00     //����
#define USER_CMD_SEARCH_CARD_REQUEST          0X01     //Ѱ������
#define USER_CMD_CHARGE_REQUEST              0X02      //��ʼ�ӵ�����
#define USER_CMD_END_CARD_REQUEST            0X03      //�����ӵ�����
#define USER_CMD_PRINT_REQUEST               0X04      //��ӡ����
#define USER_CMD_EXIT_CARD_REQUEST           0X05      //�˿�����
#define USER_CMD_ADDCHARGE_CARD_REQUEST      0X06      //���佻������

#define USER_CMD_SEARCH_CARD_ING              0X11     //Ѱ��������
#define USER_CMD_CHARGE_ING                  0X12      //���ڳ����
#define USER_CMD_END_CARD_ING                0X13      //�����ӵ���
#define USER_CMD_PRINT_ING                   0X14      //��ӡ��
#define USER_CMD_EXIT_CARD_ING               0X15      //�˿�������
#define USER_CMD_ADDCHARGE_CARD_ING          0X16      //���佻����

#define USER_CMD_SEARCH_CARD_END              0X21     //Ѱ������
#define USER_CMD_CHARGE_END                  0X22      //������
#define USER_CMD_END_CARD_END                0X23      //�ӵ��������
#define USER_CMD_PRINT_END                   0X24      //��ӡ����
#define USER_CMD_EXIT_CARD_END               0X25      //�˿�����
#define USER_CMD_ADDCHARGE_CARD_END         0X26       //���佻�׽���

#define CARD_CMD_S_SEARCH_REQUEST           0X01     //Ѱ������
#define CARD_CMD_S_IDENT_REQUEST            0X02     //��ʼ�ӵ�����
//#define CARD_CMD_E_SEARCH_REQUEST           0X03
#define CARD_CMD_E_IDENT_REQUEST            0X03    //�����ӵ�����
#define CARD_CMD_ADDCHARGE_REQUEST         0X04     //���佻������
#define CARD_CMD_EXIT_REQUEST              0X05     //�˿�����


#define CARD_CMD_S_SEARCH_ING               0X11    //Ѱ��������
#define CARD_CMD_S_IDENT_ING                0X12    //��ʼ�ӵ������
#define CARD_CMD_E_SEARCH_ING               0X13    //�����ӵ������
#define CARD_CMD_ADDCHARGE_ING              0X14    //���佻�׶�����
#define CARD_CMD_EXIT_ING                   0X15    //�˿�������

#define CARD_CMD_S_SEARCH_END               0X21   //Ѱ������
#define CARD_CMD_S_IDENT_END                0X22   //��ʼ�ӵ��������
//#define CARD_CMD_E_SEARCH_END               0X23
#define CARD_CMD_E_IDENT_END                 0X23   //�����ӵ��������
#define CARD_CMD_ADDCHARGE_END               0X24   //���佻�׶�������
#define CARD_CMD_EXIT_END                    0X25   //�˿���������

#define KEY_SCAN_CMD_REQUEST                0X01     //����ɨ������
#define KEY_SCAN_CMD_ING                    0X11     //����ɨ�������
#define KEY_SCAN_CMD_END                    0X21     //����ɨ�����

#define PRINTER_CMD_REQUEST                 0X01     //��ӡ����
#define PRINTER_CMD_ING                     0X11     //��ӡ������
#define PRINTER_CMD_END                     0X21     //��ӡ����

#define COMM_ALARM_CNT_MAX                  250      //��ӡ����
#define REMOTE_CONTROL_ON                  0X01      //���� 
#define REMOTE_CONTROL_OFF                 0X02     //�ػ�
#define REMOTE_CONTROL_INVALID            0X00     //��Ч


/*********************************************************************************************************************
*                                            �������
*********************************************************************************************************************/





#endif

