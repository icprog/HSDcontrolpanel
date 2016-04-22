/************************************************************


*************************************************************/
#include "Global.h"
#include "includes.h"

INT8U  gModbusRecArr[100];				//串口接收数据的临时寄存器
INT8U  gModbusRecNum;					//接收到的数据个数，最大50


UART_INFO gUartInfo[COM_PORT_NUM];                        //串口接收缓冲信息
//INT16U ModbusREGDat[MBREGNUM]={0};		//定义Modbus 寄存器，也是全局变量的桥梁						
										
INT16U lSetVoltageBak=0;				//设置电压备份
INT16U lSetCurrentBak=0;				//设置电流备份
INT16U lSaveData=0;						//保存数据备份
INT16U gSetVoltageDA=0;					//设置电压对应的DA值
INT16U gSetCurrentDA=0;					//设置电流对应的DA值
INT16U gOutVoltageAD=0;					//输出电压AD采样值
INT16U gOutCurrentAD=0;					//输出电流AD采样值

//INT16U gSystmERR=0;					//模块的故障状态
INT16U gModelRunFlag=0;					//模块工作与否的标志
										
INT16U gAdjOutCur[6]={0};				//显示电流校正数据
INT16U gAdjSetCur[6]={0};				//设置电流校正数据
INT16U gAdjOutVol[6]={0};				//显示电压校正数据
INT16U gAdjSetVol[16]={0};				//设置电压校正数据
INT16U gAdjSetVol_F[8]={0};				//设置电压校正数据 6-45V共5个数据
//INT8U  gSystemCommFlag=0;				//if lFlag=1 is commcation OK


void UartSendByte(INT8U lSend);			//声明发送单个字节函数
void ModbusCommunication(void);			//声明MODBUS通讯处理函数
void StatusLinkPageId(void);
/*********************************************************************************************************************
函数名称: 	task_modbus          
功能描述: 	task_modbus任务,主要用于PCU与触摸屏通信
输　入:   	无

输　出:   	无                 
*********************************************************************************************************************/

void task_modbus(void *pdata)			//MODBUS任务
{
	INT8U tmp=0;
	INT16U Runtimes=0;					//程序运行的次数，同时还用于计算时间
	INT8U  tmpArr[30];					//定义一个临时的数组，主要是应对24C16的读写，任何地方均可更改
	pdata = pdata;
	OSTimeDlyHMSM(0,0,5,0);				   //2S延时 
 while(1)
 {	   
	OSTimeDlyHMSM(0,0,0,10);  
	//ModbusCommunication();										//modbus通讯处理  
  StatusLinkPageId();
	gUserDB.m485CommAlarmCnt=(gUserDB.m485CommAlarmCnt>=100)?100:(++gUserDB.m485CommAlarmCnt);
	if(gUserDB.m485CommAlarmCnt>=100)
	  gUserDB.m485comLinkStatus=ALARM;	

 }	//end while
}


/*********************************************************************************************************
函数名称: 	CRC16 
功能描述:  CRC校验

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
INT16U CRC16(INT8U *crcdata, INT8U datalen)
{
  INT8U crc16h = 0xFF ;    
  INT8U crc16l = 0xFF ;    
  INT8U savehi,savelo;
  INT8U i,j;
  for(i=0;i<datalen;i++)
   {  
    crc16l=crc16l^ *(crcdata++);
    for(j=0;j<8;j++)
	  {
		savehi = crc16h;
		savelo = crc16l;
        crc16h = crc16h>>1;
        crc16l = crc16l>>1;
        if ((savehi & 0x01) == 0x01)	 crc16l = crc16l|0x80;
        if ((savelo & 0x01) == 0x01) 
		  {
        	crc16h = crc16h ^ 0xA0 ;
            crc16l = crc16l ^ 0x01;
		  }	      
      }
   }
  return ((crc16h << 8) | crc16l) ;
}



/*********************************************************************************************************
函数名称: 	UartSendArr 
功能描述:  串口发送

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
void UartSendArr(INT8U *ptrSend,INT8U length) 
{
	INT8U tmp=0;
	//GPIO_ResetBits(GPIOB, GPIO_Pin_8);					//RS485
	SET_RS485_1_SEND;
	for(tmp = 0; tmp < length; tmp++)
	{
		UartSendByte(*(ptrSend++));							//发送数据
	}
	//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	SET_RS485_1_REC;
}
 
/*********************************************************************************************************
函数名称: 	Modbus_Function_3 
功能描述:  功能码3 多个寄存器读

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
void Modbus_Function_3(INT8U *Rev_buff) 
{    
    INT8U  RegAddr;
	INT8U  tmp=0;
	INT8U  SendBuff[MBREGNUM*2+5]={0};
	INT8U  SendNum=0;
	INT16U CRCResult=0;    
    RegAddr = Rev_buff[3];  								//得到寄存器的地址

	modbusReadCallBack();                                   //寄存器转换
    if(RegAddr + Rev_buff[5] <= MBREGNUM)					//所访问的地址在合理的范围之内
    {
        SendNum = 2 * Rev_buff[5] + 3;  					//包括设备地址，功能码，个数共3个字节
        for(tmp=3; tmp < SendNum; )							//顺序为先高字节后低字节
    	{
    	   SendBuff[tmp] = (ModbusREGDat[RegAddr] >> 8) & 0xff;           
           tmp++;
           SendBuff[tmp] = ModbusREGDat[RegAddr] & 0xff;
           tmp++;	
           RegAddr++;  										//下一个寄存器
        }
	    SendBuff[0] = MODBUS_DEVICEID;
        SendBuff[1] = 0x03; 
        SendBuff[2] = 2 * Rev_buff[5]; 						//发送的数据的个数
        CRCResult = CRC16(SendBuff,SendNum);  				//获得CRC校验码
        SendBuff[SendNum] = CRCResult & 0xff;  				//先低位
        SendBuff[SendNum+1] = (CRCResult >> 8) & 0xff;		//后高位
        //UartSendArr(SendBuff,SendNum+2);  					//发送数据
		uart_write(MID_MODBUS_COM,SendBuff,SendNum+2);  					//发送数据
    }     	        
} 
/*********************************************************************************************************
函数名称: 	Modbus_Function_6 
功能描述:  功能码6 单个寄存器写

输　入:      buf    接收缓冲区
			len     接收的数据长度

 输　出:    0		接收失败
			>0		接收成功						     
*********************************************************************************************************/
void Modbus_Function_6(INT8U *RecBuff) 
{ 
    INT8U lRegAddr; 
    lRegAddr = (RecBuff[2]<<8) + RecBuff[3];  				//寄存器的地址
    if(lRegAddr < MBREGNUM)							   		//所访问的地址在合理的范围之内
    {
       ModbusREGDat[lRegAddr] = (RecBuff[4]<<8) + RecBuff[5]; //修改指定寄存器中的内容
	   //UartSendArr(RecBuff,8);  							  //将接收到的数据原封不动的返回 .
	   uart_write(MID_MODBUS_COM,RecBuff,8);  					//发送数据
	   modbusWriteCallBack( lRegAddr,1);	   		          //处理MODBUS接收到的数据
    }
} 
/**********************************************************
	对接收到的modbus数据的处理，并发送应答数据
	数据处理机制：
		本次和上次（间隔20ms）处理接收到的数据个数相同
		且接收到的数据大于等于7
		处理之后清零且允许接收数据
		完成了对地址和CRC校验功能
***********************************************************/ 
void ModbusCommunication(void) 
{    
    static INT8U lastRecLen=0;															//接收数据长度的备份

	if(gUartInfo[MID_MODBUS_COM].UartRLen == 0) 		                               				//没有接收到数据返回
		return ;							

	if(gUartInfo[MID_MODBUS_COM].UartRLen != lastRecLen)					   						//两次不相同，表明正在接收数据，接收未完成
	{
	   lastRecLen	= gUartInfo[MID_MODBUS_COM].UartRLen;				   							//保存备份数据，供下次对比
	   return ;
	}
	else
	{
		if((gUartInfo[MID_MODBUS_COM].UartRLen < 7) && (gUartInfo[MID_MODBUS_COM].UartRLen >=1))			//小于7说明是错误帧，丢弃
		{																				//去掉=0时突然接收到数据却清零导致错误
			gUartInfo[MID_MODBUS_COM].UartRLen=0;							   						//重新接收数据
			lastRecLen=0;
			return ;
		}					
	}
	if(gUartInfo[MID_MODBUS_COM].UartRLen > 8 )													//说明是错误帧，丢弃
	{													
		gUartInfo[MID_MODBUS_COM].UartRLen=0;							   							//重新接收数据
		lastRecLen=0;
		return ;
	}	
	if((gUartInfo[MID_MODBUS_COM].UartRBuf[0] == MODBUS_DEVICEID))								// 通信地址
    {						  
    	if((gUartInfo[MID_MODBUS_COM].UartRLen == 8)&&( CRC16(gUartInfo[MID_MODBUS_COM].UartRBuf,8)== 0))	//CRC校验正确
		{ 
			switch(gUartInfo[MID_MODBUS_COM].UartRBuf[1])   										//功能码
			{ 
            	case 0x03: 	Modbus_Function_3(gUartInfo[MID_MODBUS_COM].UartRBuf); 						
        					break;  
                case 0x06: 	Modbus_Function_6(gUartInfo[MID_MODBUS_COM].UartRBuf); 
        			 		break; 
        		default:  	break;			 
            } 
    	}
	}
	gUartInfo[MID_MODBUS_COM].UartRLen=0;							   							//重新接收数据
	lastRecLen=0;    																//清零备份寄存器
}

/**********************************************************
	数据限制函数
	当在最大和最小值之间时返回0，否则返回1 2
**********************************************************/
INT8U DataLimit(INT16U *Dat,INT16U MaxDat,INT16U MinDat)	
{
	if(*Dat<MinDat)	{*Dat = MinDat;	return 1;}	
	if(*Dat>MaxDat)	{*Dat = MaxDat; return 2;}
	return 0;
}
