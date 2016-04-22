// CanBms.cpp : 实现文件
//

#include "../../app/global.h"
#include "../../app/includes.h"

#include "CanBms.h"
#include "canbusBms.h"
//#include "dcsInfolib.h"
//#include "dcsDlg.h"
#include "mcp2515_reg.h"


#define HIGH      1
#define LOW       0

#define ALARM					0X01  //告警
#define NOALARM					0X00  //正常

// CCanBms




// CCanBms 消息处理程序

BMS_MSG gBMS_msg;
CCS_MSG gCCS_msg;

uint8  wzlMsgBuf[40];  //五洲龙BMS通信缓冲区
WZL_BMS_INFO g_wzlBmsInfo;
WZL_CCS_INFO g_wzlCcsInfo;

uint8 BMS_ID_PGN_CTL[4] = {0xC0, 0x0A, 0xF4, 0xE5};  //BRS充电控制
uint8 CCS_ID_PGN_CTL[4] = {0xC0, 0x0A, 0xF4, 0xE5};  //充电机输出电压电流状态
//extern _DCSInfo gDCS_Info; //定义充电桩信息结构体




/*******************************************************************************************
函数名称: CDcsDlg::BMS_init
描    述: BMS初始化函数
输入参数: 
输出参数: 无
返    回: 无
********************************************************************************************/
uint8 Mcp2515_init(void)
{	
//ORD dwThreadId1,dwThreadId2,dwThreadId3;
//	HANDLE handle1,handle2,handle3;	

	//gDCS_Info.charge_ctl=1;  //test
//	SPIInit();	//SPI初始化
//	CanInit();	//BMS_CAN初始化
//	m_CCanBms.bms_pdu_format_init();  //五洲龙BMS格式初始化	

	return 1;
}

/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  CanInit(void)
{
	uint8 byte_IR;

    Reset_2515(); //复位    

    Write_BYTE_2515(CANCTRL, 0x80); //CANCTRL寄存器－－进入配置模式 中文DATASHEET 58页

	byte_IR = Read_BYTE_2515(CANCTRL);

	//可以设置的波特率 5K 10K 15K 20K 25K 40K 50K 80K 100K 125K 200K 400K 500K 667K 800K 1M
    Write_BYTE_2515(CNF1, CNF1_250K); //CNF1位定时配置寄器   中文DATASHEET 41-42页
	byte_IR = Read_BYTE_2515(CNF1);

    Write_BYTE_2515(CNF2, CNF2_250K); //CNF2位定时配置寄器   中文DATASHEET 41-42页
	byte_IR = Read_BYTE_2515(CNF2);

     Write_BYTE_2515(CNF3, CNF3_250K); //CNF3位定时配置寄器   中文DATASHEET 41-43页
	byte_IR = Read_BYTE_2515(CNF3);

    Write_BYTE_2515(CANINTE, 0x03); //CANINTE中断使能寄存器  中文DATASHEET 50 页  //只允许使能接收中断
	byte_IR = Read_BYTE_2515(CANINTE);
//rxb0
    Write_BYTE_2515(RXB0CTRL, 0x40); //RXB0CTRL接收缓冲器0 控制寄存器 中文DATASHEET 27 页  使能屏蔽，只接受扩展帧。 // 禁止屏幕,接收所有数据
	byte_IR = Read_BYTE_2515(RXB0CTRL);

	Write_BYTE_2515(CANCTRL, 0x80); //CANCTRL寄存器－－进入配置模式 中文DATASHEET 58页
	byte_IR = Read_BYTE_2515(CANCTRL);


////Write_BYTE_2515(0x60, 0x60); //RXB0CTRL接收缓冲器0 控制寄存器 中文DATASHEET 27 页， 禁止屏蔽
    //Write_BYTE_2515(0x60, 0x64); //与上面相比，该指令表示使用缓冲寄存器，即第一个寄存器用完之后，数据转存到第二个寄存器中，也就是所谓的滚存
//rxb1
    Write_BYTE_2515(RXB1CTRL, 0x42); //RXB1CTRL接收缓冲器1 控制寄存器 使能屏蔽，只接受扩展帧。// 禁止屏弊,接收所有数据
	byte_IR = Read_BYTE_2515(RXB1CTRL);
    MODIFY_2515(BFPCTRL, 0x0f, 0x0f); //BFPCTRL_RXnBF 引脚控制寄存器和状态寄存器 中文DATASHEET 29 页 

////屏蔽寄存器，1--使能该屏蔽位，0--禁止该屏蔽位
////M0
    Write_BYTE_2515(RXM0SIDH, 0xff); //验收滤波屏蔽寄存器0标准标识符的高位,主要是屏蔽高四位
    Write_BYTE_2515(RXM0SIDL, 0xff); //验收滤波屏蔽寄存器0标准标识符的低位，不屏蔽
    Write_BYTE_2515(RXM0EID8, 0xff); //验收滤波屏蔽寄存器0扩展标识符的高位，全屏蔽
    Write_BYTE_2515(RXM0EID0, 0xff); //验收滤波屏蔽寄存器0扩展标识符的低位，全屏蔽
////M1
    Write_BYTE_2515(RXM1SIDH, 0xf0); //验收滤波屏蔽寄存器0标准标识符的高位,主要是屏蔽高四位
    Write_BYTE_2515(RXM1SIDL, 0x00); //验收滤波屏蔽寄存器0标准标识符的低位，不屏蔽
    Write_BYTE_2515(RXM1EID8, 0xff); //验收滤波屏蔽寄存器0扩展标识符的高位，全屏蔽
    Write_BYTE_2515(RXM1EID0, 0xff); //验收滤波屏蔽寄存器0扩展标识符的低位，全屏蔽
//
////滤波寄存器，输入需要滤波的ID，可以只是某一位。
////RXB0 FILter
    Write_BYTE_2515(RXF0SIDH, 0xE7); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF0SIDL, 0x4B); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF0EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF0EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位

    Write_BYTE_2515(RXF1SIDH, 0xE7); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF1SIDL, 0x68); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF1EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF1EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位
//    
////RXB1 FILter
    Write_BYTE_2515(RXF2SIDH, 0xC0); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF2SIDL, 0x0A); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF2EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF2EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位

    Write_BYTE_2515(RXF3SIDH, 0xA0); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF3SIDL, 0x49); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF3EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF3EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位

    Write_BYTE_2515(RXF4SIDH, 0xC0); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF4SIDL, 0x0A); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF4EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF4EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位

    Write_BYTE_2515(RXF5SIDH, 0xA0); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0标准标识符的高位
    Write_BYTE_2515(RXF5SIDL, 0xC9); //RXFnSIDL验收滤波寄存器0标准标识符的低位
    Write_BYTE_2515(RXF5EID8, 0xE5); //RXFnSIDH//RXF0SIDH-验收滤波寄存器0扩展标识符的高位
    Write_BYTE_2515(RXF5EID0, 0xF4); //RXFnSIDL验收滤波寄存器0扩展标识符的低位
    
    Write_BYTE_2515(CANCTRL, 0x00); //CAN控制寄存器－－工作模式  设置为正常工作模式
}
/*******************************************************************************************
函数名称: SPIInit
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  SPIInit(void)
{
//	m_hFileSpi= INVALID_HANDLE_VALUE;
//	m_hFileSpi_Eint= INVALID_HANDLE_VALUE;

/*	//打开SPI驱动
	m_hFileSpi = CreateFile(TEXT("SPI1:"), GENERIC_READ | GENERIC_WRITE, 0,  NULL, OPEN_EXISTING, 0, 0);
	
	if ( m_hFileSpi== INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("Cannot Open spi1!"));				
	}

	//打开SPI接收中断驱动
	m_hFileSpi_Eint = CreateFile(TEXT("INT1:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	
	if ( m_hFileSpi== INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("Cannot Open INT1!"));				
	}
*/
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：向SPI总线发送一个字节
uint8  Write_SPI(uint8 Write_Byte)
{
	char ret;
	uint64 wbuff;
	uint64 len=1,wactlen=0;

	wbuff=Write_Byte;
//	if ( m_hFileSpi != INVALID_HANDLE_VALUE)
//	{
//		ret= ::WriteFile(m_hFileSpi, &wbuff,len ,&wactlen, NULL);
//	}
   
	return ret ;
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：从SPI总线读取一个字节
uint8  Read_SPI(void)
{
	unsigned long wactlen=0,ractlen=0,len=1;
	char  rbuff=0;
	char ret;
	
//	if ( m_hFileSpi != INVALID_HANDLE_VALUE)
//	{
//		ret = ::ReadFile(m_hFileSpi, &rbuff, len, &ractlen, NULL); /* 读取数据 */
//		if(ractlen>0)
//		{
//		
//		}
//	}
	
    return rbuff;
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：复位指令可以重新初始化MCP2515 的内部寄存器，并设置配置模式
void  Reset_2515(void)
{
    //CS_SPI = 0; //复位
	CS_enable( LOW );
	//delay(20);
	Write_SPI(MCP2515_RESET); 
	//CS_SPI=1; 
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：只修改寄存器中的某些位
//入口参数：Addr:寄丰器地址  MASK：屏蔽字  dat:数据字节
void  MODIFY_2515(uint8 Addr, uint8 MASK,uint8 dat)
{
    //CS_SPI = 0 ;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_BIT_MODIFY);
    Write_SPI(Addr) ;
    Write_SPI(MASK) ;
    Write_SPI(dat) ;
    //CS_SPI = 1 ;
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：从指定地址起始的寄存器读取数据。
uint8  Read_BYTE_2515(uint8 Addr)
{
    uint8 ret;
    //CS_SPI = 0;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_READ);
    Write_SPI(Addr);
	//delay(2000);
    ret = Read_SPI();//第一次预读
	ret = Read_SPI();//正式读数据
	
    //CS_SPI = 1;
	//delay(2000);
	CS_enable( HIGH );

    return(ret);
}
/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
//功能：将数据写入指定地址起始的寄存器。
void  Write_BYTE_2515(uint8 Addr,uint8 dat)
{
    //CS_SPI = 0;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_WRITE);
    Write_SPI(Addr);
    Write_SPI(dat);
    //CS_SPI = 1;
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
函数名称: TaskCanBusSend
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  TaskCanBusSend(void)
{

}
/*******************************************************************************************
函数名称: TaskCanBusRev
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  TaskCanBusRev(void)
{    
    
}


/*******************************************************************************************
函数名称: 
描    述: 
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  Can_Send(uint8 ID[], uint8 tx_buff[], uint8 num, uint8 Fream)
{
    uint8 i,ret;
	static uint8 err_cnt=0;

//	CSingleLock singelLock(semaphore_spi);			//定义信号锁
//	singelLock.Lock();								//信号量锁定

    uint16 j=0;

	byte_EFLG = Read_BYTE_2515(EFLG);				//接收溢出?
	if(bit_RX0OVR==1 || bit_RX1OVR==1 )
	{
		Write_BYTE_2515(EFLG, 0x00);				//清除溢出寄存器
	}

	byte_IR = Read_BYTE_2515(CANINTF);					//读中断寄存器
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//错误
    {
        Write_BYTE_2515(CANINTF, 0x00);				//全部
		ret=3;
        //return(3); //错误
    }	
	else if (bit_TI == 1)							//发送中断
	{
	    MODIFY_2515(CANINTF, 0x04, 0x00);				//地址，屏蔽，数据。屏蔽字节确定允许修改寄存器中的哪一位。											
		ret=3;		
	}

    do 
    {
        ret = Read_BYTE_2515(TXB0CTRL);					//TXB0CTRL发送缓冲器0控制寄存器
        if(++j > 1000)
		{
			err_cnt++;
			//MODIFY_2515(0x30, 0x08, 0x01);				//地址，屏蔽，数据。屏蔽字节确定允许修改寄存器中的哪一位。	
			if(err_cnt>2)									//错误发送次数计数
			{						
				err_cnt=0;				
				CanInit();									//复位并重新初始化MCP2515
			}
			goto LastLine;
		}
    } while (ret&0x08);										//发送缓冲中的请求报文是否发送完毕？

	err_cnt=0;
	ret = Read_BYTE_2515(EFLG);
	if(ret !=0)
	{
		Write_BYTE_2515(EFLG, 0);						//发送缓冲器0标准标识符高位
	}

    if (Fream==STANDARD_FRAME_TYPE)						//标准数据帧
    {
        Write_BYTE_2515(TXB0SIDH, ID[0]);				//发送缓冲器0标准标识符高位,
		ID[1]=ID[1]&0xe0;
        Write_BYTE_2515(TXB0SIDL, ID[1]);				//发送缓冲器0标准标识符低位 
        Write_BYTE_2515(TXB0DLC, num);					//发送缓冲器0数据长度码--------------------写入发送的数据数
    }
	else if (Fream==EXTEND_FRAME_TYPE)									//扩展数据帧
	{        
        Write_BYTE_2515(TXB0SIDH, ID[0]);				//发送缓冲器0标准标识符高位     (优先级3字节+R1+DP+PF8~PF6) 
        ID[1]=(ID[1]&0xe3)|0x08;  
        Write_BYTE_2515(TXB0SIDL, ID[1]);				//发送缓冲器0标准标识符低位     (PF5~PF1)参数组编号
        Write_BYTE_2515(TXB0EID8, ID[2]);				//发送缓冲器0扩展标识符高位		(PS8~PS1)目标地址
        Write_BYTE_2515(TXB0EID0, ID[3]);				//发送缓冲器0扩展标识符低位     (SA8~SA1)源地址
        Write_BYTE_2515(TXB0DLC, num);					//发送缓冲器0数据长度码--------------------写入发送的数据数
	}
	
  //  CS_SPI = 0;
	CS_enable( LOW );
    Write_SPI(MCP2515_WRITE);
    Write_SPI(TXB0D0);
    for(i = 0; i < num; i++)  Write_SPI(tx_buff[i]);
  //  CS_SPI = 1;  //在CS_SPI为低电平的情况下可以连续写入字节，地址指针自动增一，直到CS_SPI=1为止。
	CS_enable( HIGH );
    //Write_BYTE_2515(0x35,0x08);
  //  CS_SPI = 0;
	CS_enable( LOW );
	Write_SPI(MCP_RTS_TX0);  //0x1000 0nnn请求发送报文  nnn=001则表示发送缓冲器1中的报文。
//	CS_SPI = 1;//发送请求
	CS_enable( HIGH );

LastLine: 										//最后一行，什么也不执行。

//	singelLock.Unlock();						//信号量解锁
}

/*******************************************************************************************
函数名称: Can_Receive1
描    述: CAN接收单帧数据命令,利用屏蔽RXB1和相关滤波寄存器来接收单帧
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8  Can_Receive1(uint8 ID[], uint8 rx_buff[], uint8 *ptr)
{
    uint8 i;
	uint8 tmp;
	uint8 ret=0;

//	CSingleLock singelLock(semaphore_spi);	//定义信号锁
//	singelLock.Lock();	//信号量锁定

    byte_IR = Read_BYTE_2515(CANINTF);			//读取中断状态
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//错误
    {
        Write_BYTE_2515(CANINTF, 0x00);
		ret=3;
       // return(3); //错误
    }    
  //   if (byte_IR == 0xe0)				//无数据
  //  {
  //      Write_BYTE_2515(0x2c, 0x00);
		//ret=0;
  //     // return(0);//无数据
  //  }
     if (bit_RI_1 == 1)					//RXB1接收中断
    {
      //  CS_SPI = 0;
		CS_enable( LOW );
        Write_SPI(MCP2515_READ);
        Write_SPI(RXB1SIDH);

		tmp=Read_SPI();						//预读一个字节,因为该字节为0XFF无效字节
        for(i = 0; i < 4; i++)				//读取ID
			ID[i] = Read_SPI();
		byte_RXBnSIDL = ID[1];
		byte_RXBnDLC = Read_SPI();				//应该是表示长度数据的，通过这个知道接收到几个数据
        *ptr = (byte_RXBnDLC & 0x0f);			//读取数据的个数
		if(*ptr>8)							//防止内存泄漏
			goto END_RXB1;
        for(i = 0; i < *ptr; i++) 
			rx_buff[i] = Read_SPI();
      //  CS_SPI = 1;
		CS_enable( HIGH );
        MODIFY_2515(CANINTF,0x02,0x00);		//清除中断
		
		if ((bit_IDE==0)&&(bit_SRR==0))				//接收标准数据帧？
		{
			ret=1;
			//return(1);//接收标准数据帧
		}
 		else if ((bit_IDE==1)&&(bit_RTR==0))		//接收扩展数据帧？
		{
			ret=2;
			//return(2);//接收扩展数据帧
		}
    }

	 Write_BYTE_2515(CANINTF, 0x00);

END_RXB1:

//	singelLock.Unlock();  //信号量解锁
    return ret;
}

/*******************************************************************************************
函数名称: Can_Receive0
描    述: CAN接收多帧数据命令,利用屏蔽RXB0和相关滤波寄存器来接收多帧
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8  Can_Receive0(uint8 ID[], uint8 rx_buff[], uint8 *ptr)
{	
    uint8 i;
	uint8 tmp;
	uint8 ret=0;

//	CSingleLock singelLock(semaphore_spi);	//定义信号锁
//	singelLock.Lock();	//信号量锁定

    byte_IR = Read_BYTE_2515(CANINTF);
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//错误
    {
        Write_BYTE_2515(CANINTF, 0x00);			//全部
		ret=3;
        //return(3); //错误
    }    
  
     if (bit_RI_0 == 1)						//RXB0接收到数据
    {
      //  CS_SPI = 0;
		CS_enable( LOW );
        Write_SPI(MCP2515_READ);
        Write_SPI(RXB0SIDH);
		tmp=Read_SPI();

        for(i = 0; i < 4; i++) ID[i] = Read_SPI();
		byte_RXBnSIDL = ID[1];
		byte_RXBnDLC = Read_SPI();					//应该是表示长度数据的，通过这个知道接收到几个数据
        *ptr = (byte_RXBnDLC & 0x0f);				//读取数据的个数
		if(*ptr>8)
			goto END_RXB0;
        for(i = 0; i < *ptr; i++)  rx_buff[i] = Read_SPI();
       // CS_SPI = 1;
		CS_enable( HIGH );
        MODIFY_2515(CANINTF,0x01,0x00);
		Write_BYTE_2515(CANINTF, 0x00);
		if ((bit_IDE==0)&&(bit_SRR==0))					//接收标准数据帧
		{	
			ret=1;
			//return(1);//接收标准数据帧
		}
 		else if ((bit_IDE==1)&&(bit_RTR==0))			//接收扩展数据帧
		{
			ret=2;
			//return(2);//接收扩展数据帧
		}
    }
   
    //Write_BYTE_2515(0x2c, 0x00);

END_RXB0:
//	singelLock.Unlock();  //信号量解锁
    return ret;

}

/*******************************************************************************************
函数名称: Task_Init
描    述: BMS_CAN任务初始化
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8  Task_Init(void)
{
	return 1;
}

/*******************************************************************************************
函数名称: CS_enable
描    述: CS
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
uint8  CS_enable(uint8 enable)
{	
	uint8 ret=1;
	uint8 inbuf,outbuf;
//	DWORD len;
	
//	ret = ::DeviceIoControl(m_hFileSpi, enable, &inbuf, 0, &outbuf, 0, &len, NULL);
//	if (ret != TRUE)
//		ret=0;

	return ret;
}
/*******************************************************************************************
函数名称: CS_enable
描    述: CS
输入参数: 无
输出参数: 无
返    回: 无
********************************************************************************************/
void  delay(int us)
{
	while(--us)
	{
		;
	}
}
/*******************************************************************************************
函数名称: waitSpiRecciveIntr
描    述: 等待SPI接收中断
输入参数: 无
输出参数: 无
返    回: ： 1 成功        0 失败
********************************************************************************************/
uint8  waitSpiRecciveIntr(void)
{

	unsigned long wactlen=0,ractlen=0,len=1;
	char  rbuff=0;
	char ret;
	
//	if ( m_hFileSpi_Eint != INVALID_HANDLE_VALUE)
//	{
//		ret = ::ReadFile(m_hFileSpi_Eint, &rbuff, len, &ractlen, NULL); /* 读取数据 */
//		if(ractlen>0)
//		{
//		
//		}
//	}

	return rbuff;
}

/*******************************************************************************************
函数名称: waitSpiRecciveIntr
描    述: 等待SPI接收中断
输入参数: 无
输出参数: 无
返    回: ： 1 成功        0 失败
********************************************************************************************/
void  closeSpi(void)
{
//	if(m_hFileSpi_Eint != INVALID_HANDLE_VALUE)
//		CloseHandle(m_hFileSpi_Eint);				/* 关闭接收线程退出事件句柄 */	
//	if(m_hFileSpi != INVALID_HANDLE_VALUE)
//		CloseHandle(m_hFileSpi);				    /* 关闭接收线程退出事件句柄 */
//
//	m_hFileSpi = INVALID_HANDLE_VALUE ;
//	m_hFileSpi_Eint = INVALID_HANDLE_VALUE ;
	
}

/*******************************************************************************************
函数名称: BmsComAffairManage
描    述: BMS通信事务管理
输入参数: 无
输出参数: 无
返    回: ： 
********************************************************************************************/
void  BmsComAffairManage(void)
{
	static uint8 bmsComErr_cnt=0;

	
}

/*******************************************************************************************
函数名称: data_callback
描    述: 数据处理
输入参数: 无
输出参数: 无
返    回: ： 
********************************************************************************************/
void  data_callback(uint8 cmd)
{
	switch(cmd)
	{	
	case 1:
		break;
	}

}

/*******************************************************************************************
函数名称: bms_pdu_format_init
描    述: 协议格式转换，转换为MCP2515寄存器相对应的标识符
输入参数: 无
输出参数: 无
返    回: ： 
********************************************************************************************/
void  bms_pdu_format_init(void)
{
	uint8 priorty,pf,ps,sa,tmp;	

	//ID:0X1806E5F4   五洲龙BMS命令
	priorty=0x06;
	pf=0x01;
	ps=0xE5;
	sa=0xF4;
	BMS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	BMS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	BMS_ID_PGN_CTL[2]=ps;
	BMS_ID_PGN_CTL[3]=sa;


	//ID:0X1806E5F4   五洲龙BMS命令
	priorty=0x06;
	pf=0x06;
	ps=0xE5;
	sa=0xF4;
	BMS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	BMS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	BMS_ID_PGN_CTL[2]=ps;
	BMS_ID_PGN_CTL[3]=sa;

	//ID:0X18FF50E5  五洲龙充电机命令
	priorty=0x06;
	pf=0xFF;
	ps=0x50;
	sa=0xE5;
	tmp=(pf&0x1c);
	tmp=((pf&0x1c)<<3);
	tmp=((pf&0x1c)<<3)+(1<<3);
	tmp=(pf&0x03);
	CCS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	CCS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	CCS_ID_PGN_CTL[2]=ps;
	CCS_ID_PGN_CTL[3]=sa;

}
/*******************************************************************************************
函数名称: CCS_info
描    述: 充电机信息
输入参数: 无
输出参数: 无
返    回: ： 
********************************************************************************************/
void  CCS_info(void)
{
	uint32 tmp; 

	g_wzlCcsInfo.status=0;
//	g_wzlCcsInfo.out_volt=gDCS_Info.pduInfo.pdu_volt;
//	g_wzlCcsInfo.out_cur=gDCS_Info.pduInfo.pdu_curr;
//
//	tmp=(uint32)(g_wzlCcsInfo.out_volt*10);
//	wzlMsgBuf[OUT_VOL_H]= (tmp&0xff00)>>8;
//	wzlMsgBuf[OUT_VOL_L]= tmp&0xff;
//
//	tmp=(uint32)(g_wzlCcsInfo.out_cur*10);
//	wzlMsgBuf[OUT_CUR_H]= (tmp&0xff00)>>8;
//	wzlMsgBuf[OUT_CUR_L]= tmp&0xff;
//
//	wzlMsgBuf[STATUS]= g_wzlCcsInfo.status;
}
