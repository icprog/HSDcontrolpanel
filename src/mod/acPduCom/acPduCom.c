
/*********************************************************************************************************
*                                            头文件包含
*********************************************************************************************************/
#include "acPduCom.h"

/*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************/
#define  ACCOM_BUF_RN   			200   //接收数据的长度
#define  ACCOM_BUF_SN   			200   //发送数据的长度	
#define  ACMODULE_REG_MAX 			15


#define ACPDU_UART     UART_4
//#define ACPDU_TX_BUF   (rx3_Buf)
//#define MODULE_RX_BUF   (rx3_Buf)

/*********************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************/	
uint8 AC_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_6_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
uint8 AC_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_2_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
extern INT16U getCRC16(INT8U *crcdata, INT8U datalen);
//uint8 dataExchange(uint8 * pdat, uint8 len);

/*********************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************/
uint8 acRevComBuff[ACCOM_BUF_RN],acSendComBuff[ACCOM_BUF_SN];  //注意数据类型
//_MODULE_INFO gModuleInfo[MODULE_SUM];
uint16 acModuleReg[ACMODULE_REG_MAX];   //15个充电模块寄存器


INT8U irat=0;   //电流变比值


/*******************************************************************************************
函数名称: readAcPduDat
描    述: 读取模块输入输出数据信息
输入参数: pdat 数据缓冲 len 数据长度
输出参数: 无
返    回: 无
********************************************************************************************/
void task_AcPduCom(void *pdata)
{
	INT8U ret=0,i;
//	INT16U table[11]={ST_REG,PT_REG,QT_REG,PFT_REG,P_WH_REG,HUA_REG,HUB_REG,HUC_REG,HIA_REG,HIB_REG,HIC_REG}; 	
	
	ctl_CMD(1,1,0);		                                             // 关闭模块
	while(1)
	{	
		moduleComManage();                                           //模块通信管理
	    OSTimeDlyHMSM(0,0,0,20);				//1S运行一次  	      	
	   
	  // OSTimeDlyHMSM(0,0,0,100);			
	}   
}






