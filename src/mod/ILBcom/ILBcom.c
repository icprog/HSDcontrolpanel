
/*********************************************************************************************************
*                                            头文件包含
*********************************************************************************************************/
#include "ILBCom.h"

/*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************/
#define  ILBCOM_BUF_RN   			200   //接收数据的长度
#define  ILBCOM_BUF_SN   			200   //发送数据的长度	
#define  ILBMODULE_REG_MAX 			5


#define ILBPDU_UART          UART_5
#define ILB_DEV_ADR          1


/*********************************************************************************************************
*                                            函数或其它调用声明
*********************************************************************************************************/	
uint8 ILB_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_6_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
uint8 ILB_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
//uint8 Modbus_Function_2_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff);
extern INT16U getCRC16(INT8U *crcdata, INT8U datalen);
//uint8 dataExchange(uint8 * pdat, uint8 len);

/*********************************************************************************************************
*                                            全局变量定义
*********************************************************************************************************/
uint8 ILBRevComBuff[ILBCOM_BUF_RN],ILBSendComBuff[ILBCOM_BUF_SN];  //注意数据类型
//_MODULE_INFO gModuleInfo[MODULE_SUM];
uint16 ILBModuleReg[ILBMODULE_REG_MAX];   //15个充电模块寄存器
//ILB_PDU_INFO gILBPduInfo;


/*******************************************************************************************  
函数名称: Modbus_Function_3_main  
描    述:多寄存器读 主机端程序程序 
输入参数:   SlaveAddr 从机地址    regAdr 寄存器地址    regBuf 接收缓冲  Reg_sum 寄存器个数
输出参数:   
返    回: 1 成功  0失败
********************************************************************************************/ 
uint8 ILB_Modbus_Function_3_main(uint8 SlaveAddr, uint16 regAdr,uint16 * regBuf,  uint16 Reg_sum)
{    
    uint16 crcresult=0; 
    uint8  Send_counter;
	uint8  dataLen,ret=0;
	uint8  *Rev_buff=ILBRevComBuff;  
	uint8  *Send_buff=ILBSendComBuff;


	memset(ILBRevComBuff,0,ILBCOM_BUF_RN);
	memset(ILBSendComBuff,0,ILBCOM_BUF_SN);   


	Send_buff[0] = SlaveAddr;			//从机地址
	Send_buff[1] =3;			        //功能码
    Send_buff[2] = regAdr>>8;			//寄存器首地址
	Send_buff[3] = (uint8)regAdr;
	Send_buff[4] = Reg_sum>>8;			//读寄存器个数
	Send_buff[5] =(uint8)Reg_sum;
       
    Send_counter = 6;								 //包括设备地址，功能码，个数共3个字节 
    crcresult = getCRC16(Send_buff,Send_counter);    			//获得CRC校验码
    Send_buff[Send_counter] = crcresult & 0xff;	     			//先低位
    Send_buff[Send_counter+1] = (crcresult >> 8) & 0xff;  		//后高位
  //     UartSendStr(Rev_buff,8);  //将接收到的前8个字节数据原封不动的返回 

	ret=uart_write(ILBPDU_UART,Send_buff,8);

	ret=ILB_Modbus_Function_3_main_rev(Rev_buff,SlaveAddr,&dataLen,regBuf);
	if(ret==0)
		return 0;
	return 1;
}

/*******************************************************************************************  
函数名称: ILB_Modbus_Function_3_main_rev  
描    述:寄存器读 主机端程序接收程序 
输入参数:            
输出参数:   
返    回: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_3_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff)
{    
    uint16 crcresult=0;     
	uint16 i, revLen=0;
	INT8U * pbuf =(INT8U*)Reg_buff;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;  //获取父窗口句柄	

	revLen=uart_read(ILBPDU_UART,Rev_buff,200);
	//uartRead(port,Rev_buff,*revLen);
	if(revLen<5)
		return 0;  	
	
	if(Rev_buff[0] != SlavAdr) //设备地址
		return 0;
	if(Rev_buff[1] != 3) //功能码
		return 0;
	crcresult = getCRC16(ILBRevComBuff,(uint8)revLen); 
    if(crcresult != 0) //CRC校验不正确
		return 0;

    *dataLen=Rev_buff[2];
	for( i=0;i<*dataLen;i++) //数据接收
	{
		*pbuf++ =Rev_buff[3+i];
	}

   return 1;
}


/*******************************************************************************************  
函数名称: Modbus_Function_16_main  
描    述:多寄存器写 主机端程序程序 
输入参数:            
输出参数:   
返    回: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_16_main(uint8 SlaveAddr, uint16 regAdr,uint16 * regBuf,  uint16 Reg_sum)
{	uint16 crcresult=0; 
    uint8  Send_counter;
	uint8  dataLen,ret=0;
	uint8  *Rev_buff=ILBRevComBuff;  
	uint8  *Send_buff=ILBSendComBuff;

	memset(ILBRevComBuff,0,ILBCOM_BUF_RN);
	memset(ILBSendComBuff,0,ILBCOM_BUF_SN);
   
	Send_buff[0] = SlaveAddr;		//从机地址
	Send_buff[1] = 16;				//功能码
    Send_buff[2] = regAdr>>8;		//寄存器首地址
	Send_buff[3] = (uint8)regAdr;
	Send_buff[4] = Reg_sum>>8;		//读寄存器个数
	Send_buff[5] = (uint8)Reg_sum;
	Send_buff[6] = Reg_sum*2;       //数据字节
	memcpy(&Send_buff[7],(uint8 *)regBuf,Reg_sum*2);  //数据
       
    Send_counter = 7+ Reg_sum*2;								 //包括设备地址，功能码，个数共3个字节 
    crcresult = getCRC16(Send_buff,Send_counter);    //获得CRC校验码
    Send_buff[Send_counter] = crcresult & 0xff;	     //先低位
    Send_buff[Send_counter+1] = (crcresult >> 8) & 0xff;  //后高位	  

	ret=uart_write(ILBPDU_UART,Send_buff,Send_counter+2);

	ret=ILB_Modbus_Function_16_main_rev(Rev_buff,SlaveAddr,&dataLen,regBuf);
	if(ret==0)
		return 0;

	return 1;
}
/*******************************************************************************************  
函数名称: Modbus_Function_16_main_rev  
描    述:多寄存器写 主机端程序接收程序 
输入参数:            
输出参数:   
返    回: 
********************************************************************************************/ 
uint8 ILB_Modbus_Function_16_main_rev(uint8 *Rev_buff,uint8 SlavAdr,uint8 * dataLen,uint16 *Reg_buff)
{    
    uint16 crcresult=0;     
	uint16  revLen=0;
//	CdcsDlg *pDlg=(CdcsDlg *)theApp.m_pMainWnd;  //获取父窗口句柄

	//uartRead(port,Rev_buff,*revLen);
	revLen=uart_read(ILBPDU_UART,Rev_buff,200);
	
	if(revLen<8)
		return 0;
	
	if(Rev_buff[0] != SlavAdr) //设备地址
		return 0;
	if(Rev_buff[1] != 16) //功能码
		return 0;
	crcresult = getCRC16(ILBRevComBuff,(uint8)revLen); 
    if(crcresult != 0) //CRC校验不正确
		return 0;
    
   return 1;
}




/*******************************************************************************************
函数名称: readILBDat
描    述: 读取交流电表数据
输入参数: pdat 数据缓冲 len 数据长度
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 readILBDat(INT16U startAdr ,INT8U num)
{
	INT8U ret=0,i;
	static INT8U comAlmCnt=0;


	ret=ILB_Modbus_Function_3_main(ILB_DEV_ADR, startAdr,&ILBModuleReg[0], num);//读寄存器
	if(ret==0 )      //错误则返回
	{
		comAlmCnt ++;
		if(comAlmCnt>=5)
		{
			//gModuleInfo[i].comAlarmCnt=5;
			gUserDB.ILB_comAlarm=ALARM;
		}
		return 0;
	}
	else             //正确接收则解码
	{		
		
		dataExchange( (INT8U*)&ILBModuleReg[1],2);	           //把接收到的数据先进行高低字节交换	
		gUserDB.activeILB_value = ILBModuleReg[1];
		dataExchange( (INT8U*)&ILBModuleReg[2],2);	           //把接收到的数据先进行高低字节交换	
		gUserDB.inactiveILB_value = ILBModuleReg[2];
		dataExchange( (INT8U*)&ILBModuleReg[0],2);	           //把接收到的数据先进行高低字节交换		
		gUserDB.insulationAlarm = ILBModuleReg[0] & 0x01;		
		
		gUserDB.ILB_comAlarm = NOALARM;
		comAlmCnt=0;
	}

   return ret;
}  

/*******************************************************************************************
函数名称: readILBDat
描    述: 读取模块输入输出数据信息
输入参数: pdat 数据缓冲 len 数据长度
输出参数: 无
返    回: 无
********************************************************************************************/
void task_ILBCom(void )
{
	INT8U ret=0;  	
	
//	while(1)
//	{
		ret=readILBDat(0X00,5);        //读取绝缘检测板寄存器
		OSTimeDlyHMSM(0,0,0,200);  
	  			
//	}   
}






