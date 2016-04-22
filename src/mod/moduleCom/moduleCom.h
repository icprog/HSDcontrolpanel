#ifndef __MODULECOM_H
#define __MODULECOM_H
/*********************************************************************************************************************
*                                            ͷ�ļ�����
*********************************************************************************************************************/
#include "../../app/global.h"
#include "../../app/includes.h"

/*********************************************************************************************************************
*                                            �궨��
*********************************************************************************************************************/
#define REG_ON_CTL_ADR	  				(0X1000)  //�������ƼĴ���
#define REG_OFF_CTL_ADR	  				(0X1001)  //�ػ����ƼĴ���
#define REG_AVER_CTL_ADR	    		(0X1002)  //������ƼĴ���
#define REG_FLOAT_CTL_ADR	    		(0X1003)  //������ƼĴ���

#define REG_AVER_VOLT_SET_ADR        	(0X0000)    //�����ѹ����
#define REG_FLOAT_VOLT_SET_ADR       	(0X0001)    //�����ѹ����
#define REG_OVER_VOLT_SET_ADR        	(0X0002)    //��ѹֵ����
#define REG_LOW_VOLT_SET_ADR         	(0X0003)    //Ƿѹֵ����
#define REG_OUT_VOLT_SET_ADR         	(0X0004)    //�����ѹ����
#define REG_OUT_CURR_SET_ADR         	(0X0005)    //�����������
#define REG_OUT_VOLT_DISP_ADJ_ADR       (0X0006)    //��ѹ����У׼
#define REG_OUT_VOLT_SET_ADJ_ADR        (0X0007)    //��ѹ����У׼
#define REG_OUT_CURR_DISP_ADJ_ADR       (0X0008)    //��������У׼
#define REG_OUT_CURR_SET_ADJ_ADR        (0X0009)    //��������У׼



#define GET_ANALOG_CMD            0X41
#define GET_STATUS_CMD            0X43
#define GET_ALARM_CMD             0X44
#define CTL_CMD                   0X45
#define REMOTE_CMD                0X80

#define DC_MODULE                 0
#define AC_MODULE                 1



/*********************************************************************************************************
*                                            ������������������
*********************************************************************************************************/

uint8 readModuleStatus(INT8U devAdr);
uint8 readModuleAnalogDat(INT8U devAdr);
uint8 readModuleConfigDat(INT8U devAdr);
uint8 writeModuleConfigDat(INT8U devAdr,INT16U regAdr,INT16U dat);
uint8 writeModuleCtl(INT8U devAdr,INT16U regAdr,INT16U dat);











#endif








