
/*********************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************/
#include "acPduCom.h"

/*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************/
#define  ACCOM_BUF_RN   			200   //�������ݵĳ���
#define  ACCOM_BUF_SN   			200   //�������ݵĳ���	
#define  ACMODULE_REG_MAX 			15


#define ACPDU_UART     UART_4
//#define ACPDU_TX_BUF   (rx3_Buf)
//#define MODULE_RX_BUF   (rx3_Buf)

/*********************************************************************************************************
*                                            ������������������
*********************************************************************************************************/	
uint8 AC_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_6_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
uint8 AC_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_2_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
extern INT16U getCRC16(INT8U *crcdata, INT8U datalen);
//uint8 dataExchange(uint8 * pdat, uint8 len);

/*********************************************************************************************************
*                                            ȫ�ֱ�������
*********************************************************************************************************/
uint8 acRevComBuff[ACCOM_BUF_RN],acSendComBuff[ACCOM_BUF_SN];  //ע����������
//_MODULE_INFO gModuleInfo[MODULE_SUM];
uint16 acModuleReg[ACMODULE_REG_MAX];   //15�����ģ��Ĵ���


INT8U irat=0;   //�������ֵ


/*******************************************************************************************
��������: readAcPduDat
��    ��: ��ȡģ���������������Ϣ
�������: pdat ���ݻ��� len ���ݳ���
�������: ��
��    ��: ��
********************************************************************************************/
void task_AcPduCom(void *pdata)
{
	INT8U ret=0,i;
//	INT16U table[11]={ST_REG,PT_REG,QT_REG,PFT_REG,P_WH_REG,HUA_REG,HUB_REG,HUC_REG,HIA_REG,HIB_REG,HIC_REG}; 	
	
	ctl_CMD(1,1,0);		                                             // �ر�ģ��
	while(1)
	{	
		moduleComManage();                                           //ģ��ͨ�Ź���
	    OSTimeDlyHMSM(0,0,0,20);				//1S����һ��  	      	
	   
	  // OSTimeDlyHMSM(0,0,0,100);			
	}   
}






