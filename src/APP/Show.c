/**********************************************************
	本文件包含两个任务
	1-
	2-对于显示方式的相关操作
	手动
		恒压、正常显示，标志0x01，LEDManaul亮、LEDV亮，数码管正常显示
		恒压、设置电压，标志0x02，LEDManaul亮、LEDV亮，电压数码管闪烁
		恒流、正常显示，标志0x03，LEDManaul亮、LEDC亮，数码管正常显示
		恒流、设置电流，标志0x04，LEDManaul亮、LEDC亮，电流数码管闪烁
	自动
		恒压、正常显示，标志0x10，LEDAuto亮、LEDV亮，数码管正常显示
		恒压、设置电压，标志0x20，LEDAuto亮、LEDV亮，电压数码管闪烁
		恒流、正常显示，标志0x30，LEDAuto亮、LEDC亮，数码管正常显示
		恒流、设置电流，标志0x40，LEDAuto亮、LEDC亮，电流数码管闪烁
	电压值放大10倍，电流值放大100倍
	PIN 02467135 共阴极	"U"-D6 	"n"98 "-"08 "F"8B "P"8F	"S"5B
	小数点 |0x20 “o”D8 
***********************************************************/
#include"Global.h"				   		//应用程序的头文件声明

const INT8U gSegTable[11]={0xD7,0x14,0xCD,0x5D,0x1E,0x5B,0xDB,0x15,0xDF,0x5F,0x00}; //|0x20数码管，0-9，NULL
extern INT16U gSetVoltage;										//设置电压值
extern INT16U gSetCurrent;										//设置电流值
INT16U gOutVoltage=0;										//输出电压值
INT16U gOutVoltage_100=0;									//输出电压值放大100倍

INT16U gOutCurrent=0;										//输出电流值
INT16U gSystemERR;											//各种故障
INT16U gExtVoltage=0;										//外部给定实际电压，放大1000倍
INT16U gExtSetVol=0;										//对应600V的设定电压值
INT16U gExtSetCur=0;										//对应5.00A的设电流值
INT32U gModelWorkTime;										//模块已经工作的时间，单位1S
INT8U  gSystemLED=0;
INT16U gAdjExtVol=(10000-125); 								//放大1000倍，AD采样4000对应的电压值10.264
void SegClearVoltage(void);									//数码管清除显示电压
void SegClearCurrent(void);									//数码管清除显示电流
void SegShowData(INT16U SSData1,INT16U SSData2);			//数码管显示电压电流
void SegShowErrCode(INT16U SSData1,INT16U SSData2);			//数码管显示故障代码
void SystemErrShow(INT8U  *lRunT);							//当有多个错误时轮流显示
void ShowSysArgExt(INT16U lExtVoltage);						//显示外部给定电压
void SegShowPending(INT8U );								//开机待处理状态

void alarmTest(INT16U *ptrErr);								//错误检查
void GlideADC(INT16U *ptrVol,INT16U *ptrCur,INT16U *ptrExt);//AD转换各个电压
INT16U ComputeVol(INT16U lVolValue);						//计算显示电压
INT16U ComputeCur(INT16U lCurValue);						//计算显示电流
INT16U ComputeExtVol(INT16U lVolValue);						//计算外部给定电压

/******************************************************
*	求取两个int16U数的绝对值
******************************************************/
INT16U abs_int16u(INT16U data1,INT16U data2)
{
	if(data1 >= data2) 
	{	return 	(data1 - data2);}
	else
	{	return 	(data2 - data1);}
}
 


/******************************************************
*	显示模块已经工作时间
******************************************************/
void ShowSysArgTime(INT16U gExtVoltage)
{

}

/******************************************************
*	显示模块序列号1
*	-Sn 201005 0001- 表示2010年5月0001号
******************************************************/
void ShowSysArgSeqence(INT16U gExtVoltage)
{
	static INT8U lTurn=0;
	INT8U SN[18];
	INT8U tmp = 0;
	SN[tmp++] = 0x08;	   //"-"
	SN[tmp++] = 0x08;	   //"-"

	SN[tmp++] = 0x5B;	   //"S"
	SN[tmp++] = 0x98;	   //"n"-98  "N" -97
//	SN[tmp++] = 0x40;	   //"."
	SN[tmp++] = gSegTable[2];	   //"2"
	SN[tmp++] = gSegTable[0];	   //"0"
	SN[tmp++] = gSegTable[(ModbusREGDat[8]/1000)/10];	   //"1"
	SN[tmp++] = gSegTable[(ModbusREGDat[8]/1000)%10];	   //"0"
	SN[tmp++] = gSegTable[(ModbusREGDat[8]%1000)/32/10];	   //"0"
	SN[tmp++] = gSegTable[((ModbusREGDat[8]%1000)/32)%10] ;	   //"5"
	SN[tmp++] = gSegTable[((ModbusREGDat[8]%1000)%32)/10];	   //"0"
	SN[tmp++] = gSegTable[((ModbusREGDat[8]%1000)%32)%10] ;	   //"1"


//	SN[tmp++] = 0x40;	//"_"

	SN[tmp++] = gSegTable[ModbusREGDat[9]/1000];	   //"0"
	SN[tmp++] = gSegTable[(ModbusREGDat[9]%1000)/100];	   //"0"
	SN[tmp++] = gSegTable[(ModbusREGDat[9]%100)/10];	   //"0"
	SN[tmp++] = gSegTable[(ModbusREGDat[9]%10)];	   //"1"
	SN[tmp++] = 0x08;	   //"-"
	SN[tmp++] = 0x08;	   //"-"

	lTurn ++;
	if(lTurn >=(18-7)*10) lTurn = 0;
	for(tmp=0; tmp<8; tmp++)
	{
		gSegShowArr[7-tmp] = SN[tmp + lTurn/10];
	}
						   


}

/******************************************************
*	显示CPU温度，放大10倍且在实际值上加了100度
*	eg：1238 --》23.8度
*		952	 --》-4.8度
******************************************************/
void ShowSysArgTemper(INT16U l_Val)
{
	gSegShowArr[7] = gSegTable[2];							//"1"							
	gSegShowArr[6] = gSegTable[10];							//NULL		
	gSegShowArr[5] = gSegTable[10];							//NULL	
	gSegShowArr[4] = gSegTable[10];							//NULL
	if(l_Val >= 50)
	{
		gSegShowArr[3] = gSegTable[10];			
		l_Val = l_Val -50;
	}
	else
	{
		gSegShowArr[3] = 0x08;				//"-";		
		l_Val = 50 - l_Val ;
	
	}	
	gSegShowArr[2] = gSegTable[(l_Val%100)/10];	
	gSegShowArr[1] = gSegTable[(l_Val%10)];	
	gSegShowArr[0] = 0xC3;			//"C"度

}

/******************************************************
*	显示软件版本号
******************************************************/
void ShowSysArgSoftVersion(INT16U l_Dat)
{	
	gSegShowArr[7]=0x5B;							 //SOFT
	gSegShowArr[6]=0xD8;
	gSegShowArr[5]=0x8B;		
	gSegShowArr[4]=0x83;
	gSegShowArr[3]=gSegTable[10];					//V01.00
	gSegShowArr[2]=gSegTable[(l_Dat%1000)/100] | 0x20;				//显示小数点
	gSegShowArr[1]=gSegTable[(l_Dat%100)/10];
	gSegShowArr[0]=gSegTable[l_Dat%10/1];
}

/******************************************************
*	显示输出电压调整的PWM；
******************************************************/
void SegShowPWMchg_0(INT16U l_Vol,INT8U l_chg)
{
	gSegShowArr[7]=0xD6;							 //U
	gSegShowArr[6]=gSegTable[(l_Vol%1000)/100];
	gSegShowArr[5]=gSegTable[(l_Vol%100)/10] ;		
	gSegShowArr[4]=gSegTable[(l_Vol%10)/1];
	if(l_Vol == 999)
	{
		gSegShowArr[5]=gSegTable[(l_Vol%100)/10] | 0x20;//99.9V		
	}
	if(l_chg >= 100)
	{
		l_chg -= 100;
		gSegShowArr[3]=gSegTable[10];							//NULL
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//显示小数点
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
	else
	{
		l_chg = 100 - l_chg;
		gSegShowArr[3]=0x08;							//"-"
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//显示小数点
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
}
void SegShowPWMchg_1(INT16U l_Vol,INT16U l_chg)
{
	gSegShowArr[7]=0xD6;							 //U
	gSegShowArr[6]=gSegTable[(l_Vol%1000)/100];
	gSegShowArr[5]=gSegTable[(l_Vol%100)/10] ;		
	gSegShowArr[4]=gSegTable[(l_Vol%10)/1];
	if(l_chg >= 100)
	{
		l_chg -= 100;
		l_chg *= 5;
		gSegShowArr[3]=gSegTable[10];							//NULL
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//显示小数点
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
	else
	{
		l_chg = 100 - l_chg;
		l_chg *= 5;
		gSegShowArr[3]=0x08;							//"-"
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//显示小数点
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
}
INT8U gSysPWMchg = 100;
void ShowSysArgOUT2VTO99V(INT16U lSeq)
{
	switch(lSeq)
	{
		case 0:SegShowPWMchg_0(2,gSysPWMchg);break;
		case 1:SegShowPWMchg_0(5,gSysPWMchg);break;
		case 2:SegShowPWMchg_0(15,gSysPWMchg);break;
		case 3:SegShowPWMchg_0(30,gSysPWMchg);break;
		case 4:SegShowPWMchg_0(50,gSysPWMchg);break;
		case 5:SegShowPWMchg_0(75,gSysPWMchg);break;
		case 6:SegShowPWMchg_0(999,gSysPWMchg);break;
		case 7:SegShowPWMchg_1(100,gSysPWMchg);break;
		case 8:SegShowPWMchg_1(110,gSysPWMchg);break;
		case 9:SegShowPWMchg_1(130,gSysPWMchg);break;
		case 10:SegShowPWMchg_1(150,gSysPWMchg);break;
		case 11:SegShowPWMchg_1(175,gSysPWMchg);break;
		case 12:SegShowPWMchg_1(200,gSysPWMchg);break;
		case 13:SegShowPWMchg_1(250,gSysPWMchg);break;
		case 14:SegShowPWMchg_1(300,gSysPWMchg);break;
		case 15:SegShowPWMchg_1(350,gSysPWMchg);break;
		case 16:SegShowPWMchg_1(400,gSysPWMchg);break;
		case 17:SegShowPWMchg_1(450,gSysPWMchg);break;
		case 18:SegShowPWMchg_1(500,gSysPWMchg);break;
		case 19:SegShowPWMchg_1(550,gSysPWMchg);break;
		case 20:SegShowPWMchg_1(600,gSysPWMchg);break;

		default:break;
	}
}
void ShowSysArgSaveASK()
{
	if((gSysPWMchg %2 )==0) gSysPWMchg =100;
	else gSysPWMchg =101;

	gSegShowArr[7]=0x5B;							 //SAVE
	gSegShowArr[6]=0x9F;
	gSegShowArr[5]=0xD6 ;		
	gSegShowArr[4]=0xCB;
	if(gSysPWMchg == 100)							//OFF
	{
		gSegShowArr[3]=gSegTable[10];					
		gSegShowArr[2]=0xD8;			
		gSegShowArr[1]=0x8B;
		gSegShowArr[0]=0x8B;
	}
	else											//ON
	{
		gSegShowArr[3]=gSegTable[10];							
		gSegShowArr[2]=gSegTable[10];			
		gSegShowArr[1]=0xD8;
		gSegShowArr[0]=0x97;
	}


}
/******************************************************
*	系统参数显示的主函数
*	1-软件版本号，2-生产日期，3-外部电压，4-当前温度
******************************************************/
void SystemArgumentShow(INT8U lShowSeq)
{
	if(lShowSeq == 1) ShowSysArgSoftVersion(101);			//显示软件版本号1.01
	if(lShowSeq == 2) ShowSysArgSeqence(1);					//显示生产日期或序号
	if(lShowSeq == 3) ShowSysArgExt(gExtVoltage);			//显示外部给定电压
	if(lShowSeq == 4) ShowSysArgTemper(25+50);				//显示当前温度25
	if((lShowSeq >= 5) && (lShowSeq <= 25))
	{	ShowSysArgOUT2VTO99V(lShowSeq-5);	}
	if(lShowSeq == 26) ShowSysArgSaveASK();
	//save dat														


}

/***********************************************************
 	获取模块的状态，即开关机
	0-关机
	1-开机
***********************************************************/
INT8U GetModelStation()
{
 	return !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
}															//外部晶振是否工作正常及系统工作频率
/**********************************************************
	数码管显示和LED显示，优先级5,100ms
	AD转换，错误检测，清看门狗
	gSegShowStatus
		0x01：恒压，led亮，数码管显示电压电流
		0x02：恒压设置电压，led闪，电压数码管闪
		0x03：恒压设置电流，led亮，电流数码管闪

		0x10：恒流，led亮，数码管显示电压电流
		0x02：恒流设置电流，led闪，电流数码管闪
		0x03：恒流设置电压，led亮，电压数码管闪

		0xFE:系统保留
***********************************************************/
extern unsigned short lVolValue,lCurValue,lExtValue;			//电压电流采样值
void task_show(void *pdata)
{
	unsigned char  lRunTimes=0;								//程序运行的次数
	unsigned char  lADCTimes = 0;
	SegClearVoltage();
	SegClearCurrent();
	//SegShowData(0,0);
	MODULESTOP;
while(1)
{
	OSTimeDlyHMSM(0,0,0,100); 								//delay 100ms
	if(WATCHDOG_EN)
		IWDG_ReloadCounter();									//clear watch dog
	alarmTest(&gSystemERR);									//错误检测
	if(gSystemERR & 0xFFFF)									//屏蔽不是必须关机的故障
	{
		MODULESTOP;
		TIM_SetCompare3(TIM3, 12000);						//置PWM为0
		TIM_SetCompare4(TIM3, 12000);						//置PWM为0
		if(gSystemERR & 0x03FF)								//屏蔽不需要显示的故障或标志位
		{
			LED_ERRORON;//shutdown		
			LED_ERRORON;
			RELAYON;
			if(!(gSegShowStatus & 0x08)&& (gSegShowStatus != 0x00)) 
			{
				gSegShowStatus += 0x08;
				lRunTimes=0;
			}
		}
		else   gSegShowStatus &= ~0x08;	
	}
	else
	{
		if(gSegShowStatus != 0x00)
		{
			if((GetVolMode() == 1)) 						//如果高压模式选择高压档  &&(gSetVoltage>=1000)
			{											 	//而且设定电压>=100V,其实这个条件没有必要
				VOLMODE_RLY1H;							 	//同时保证意外时仍选择低压档
				VOLMODE_RLY2H;
			}
			else
			{					   							//低压模式选择低压档，
				VOLMODE_RLY1L;
				VOLMODE_RLY2L;
			}

			if(GetModelStation() == 0)						//如果是关机状态 跳过软启动过程，等待600ms
			{
				unsigned char Temp_FOR;
				MODULERUN;
				for(Temp_FOR=0; Temp_FOR<14; Temp_FOR++)
				{
					TIM_SetCompare3(TIM3, 12000);			//置PWM为0
					TIM_SetCompare4(TIM3, 12000);			//置PWM为0
					OSTimeDlyHMSM(0,0,0,40); 				//delay 30ms
				}
   			}
			MODULERUN;
		}
		else
		{
		 	MODULESTOP;
		}	 	
		LED_ERROROFF;									//open model
		RELAYOFF;
		if((gSegShowStatus & 0x08) ) gSegShowStatus -= 0x08;
	}
	if(((gSystemERR & 0x0C00)>0) && (1 == gUserDB.uartCommAlarm))	//在通讯正常而且有强制标识时
	{
		if((gSystemERR & 0x0800)>0)  						//如果强制选择高压档 
		{											 
			VOLMODE_RLY1H;							 
			VOLMODE_RLY2H;
		}
		else
		{					   								//低压模式选择低压档，
			VOLMODE_RLY1L;
			VOLMODE_RLY2L;
		}		
	}
	else
	{
		//if(gSegShowStatus > 0x44) gSegShowStatus -= 0x44;	//退出特殊显示状态
	}

	/******************************************************///根据采样值计算显示电压值
	if((++lADCTimes) >= 5)									//每400ms采样一次
	{
		ComputeVol(lVolValue);								//计算显示电压值
		ComputeCur(lCurValue);								//计算显示电流值
		ComputeExtVol(lExtValue);							//计算采集到的实际电压值，对应的设定电压电流值
		lADCTimes = 0;
	}
	/******************************************************///不同的显示状态
	if(gSegShowStatus == 0x00)								//初始化状态系统保留，目前未使用
	{
	   	lRunTimes ++;
		if(lRunTimes>=41) lRunTimes=0; 
		if(lRunTimes<21) SegShowPending(lRunTimes /3 );
		if(lRunTimes>=21) SegShowPending((41-lRunTimes) /3 );
	}	
	else if(gSegShowStatus == 0x01)						   	//
	{
		LED_MANUALON;										//手动模式指示灯亮
		LED_AUTOOFF;										//自动模式指示灯亮
		LED_VOLTAGEON;										//恒压指示灯亮
		LED_CURRENTOFF;										//恒流指示灯灭
		SegShowData_2(gOutVoltage_100,gOutCurrent);				//显示输出电压电流		
	}
	else if(gSegShowStatus == 0x02)
	{
		SegShowData(gSetVoltage,gSetCurrent);			//显示设置电压和输出电流

		lRunTimes ++;
		if(lRunTimes>=10)
		{
			lRunTimes=0;
			LED_VOLTAGEOFF;									//恒压LED灭
		} 
		else if(lRunTimes == 6)								//电压数码管和LED闪烁
		{
			//SegClearVoltage();							//电压数码管灭
			LED_VOLTAGEON;									//恒压LED亮
		}

	}
	else if(gSegShowStatus == 0x03)
	{
		LED_MANUALON;										//手动模式指示灯亮
		LED_CURRENTON;										//恒流充电指示灯亮
		LED_VOLTAGEOFF;
		SegShowData(gOutVoltage,gOutCurrent);				//显示输出电压电流
	}
	else if(gSegShowStatus == 0x04)
	{
		SegShowData(gSetVoltage,gSetCurrent);				//显示设置电压和输出电流
		lRunTimes ++;
		if(lRunTimes>=10) 
		{
			lRunTimes=0;
			LED_CURRENTON;
		} 
		else if(lRunTimes == 6)									//让电流显示形成闪烁的效果
		{
			//SegClearCurrent();
			LED_CURRENTOFF;
		}
	}
	else if(gSegShowStatus == 0x10)
	{
		LED_AUTOON;											//自动模式指示灯亮
		LED_VOLTAGEON;										//恒压充电指示灯亮
		LED_CURRENTOFF;
//		lRunTimes ++;
//		if(lRunTimes>=10) 								   	//每1S更新一次电压设置
//		{
//			lRunTimes=0;
//			PWM_SetVoltage(gExtSetVol);						//调整电压PWM
//		} 		
		PWM_SetCurrent(gSetCurrent);						//调整电流PWM
		SegShowData(gOutVoltage,gOutCurrent);				//显示输出电压电流
	}
	else if(gSegShowStatus == 0x20)
	{
		lRunTimes ++;
		if(lRunTimes>=10) lRunTimes=0; 
		if(lRunTimes == 0)	  
		{
			SegShowData(gExtSetVol,gSetCurrent);
			//LED_VOLTAGEON;
			LED_AUTOFLASHON;
		}
		if(lRunTimes == 6)									
		{
			//SegClearVoltage();
			//LED_VOLTAGEOFF;
			LED_AUTOFLASHOFF;
		}
		//设置电压
	}
	else if(gSegShowStatus == 0x30)
	{
		LED_AUTOON;											//自动模式指示灯亮
		LED_CURRENTON;										//恒流充电指示灯亮
		LED_VOLTAGEOFF;
		PWM_SetCurrent(gExtSetCur);							//调整电流PWM
		PWM_SetVoltage(gSetVoltage);						//调整电压PWM
		SegShowData(gOutVoltage,gOutCurrent);				//显示输出电压电流
	}
	else if(gSegShowStatus == 0x40)							//
	{
 		lRunTimes ++;
		if(lRunTimes>=10) lRunTimes=0; 
		if(lRunTimes == 0)	  
		{
			SegShowData(gSetVoltage,gExtSetCur);
			//LED_CURRENTON;
			LED_AUTOFLASHON;
		}
		if(lRunTimes == 6)									
		{
			//SegClearCurrent();
			//LED_CURRENTOFF;
			LED_AUTOFLASHOFF;
		}
	}
	else if((gSegShowStatus &0x08) ==0x08)					//错误显示
	{
		SystemErrShow(&lRunTimes);
	}

	else if((gSegShowStatus &0x88) ==0x80)					//修改系统参数，留作备用
	{
		SystemArgumentShow(gSegShowElse);
	}
	else 											 		//程序运行错误
	{
	}
}
 
}

/*************************************************************************************************/
/*	子函数																						 */
/*************************************************************************************************/

/**********************************************************
	数码管清除显示电压
***********************************************************/ 
void SegClearVoltage()
{
	gSegShowArr[4]=gSegTable[10];
	gSegShowArr[5]=gSegTable[10];
	gSegShowArr[6]=gSegTable[10];
	gSegShowArr[7]=gSegTable[10];
}

/**********************************************************
	数码管清除显示电流
***********************************************************/
void SegClearCurrent()
{
	gSegShowArr[0]=gSegTable[10];
	gSegShowArr[1]=gSegTable[10];
	gSegShowArr[2]=gSegTable[10];
	gSegShowArr[3]=gSegTable[10];
}

/**********************************************************
*	数码管显示电压电流
***********************************************************/
void SegShowData(INT16U SSData1,INT16U SSData2)
{
	SSData1 %= 10000;										//防止数据出界
	SSData2 %= 1000;										//防止数据出界
	if(SSData1 < 3)	SSData1 =0;								//0.3V
	if(SSData2 < 5)	SSData2 =0;								//0.05A
	gSegShowArr[7]=gSegTable[SSData1/1000];
	gSegShowArr[6]=gSegTable[(SSData1%1000)/100];
	gSegShowArr[5]=gSegTable[(SSData1%100)/10] | 0x20;		//显示小数点
	gSegShowArr[4]=gSegTable[(SSData1%10)/1];

	gSegShowArr[3]=gSegTable[0];							//"0"
	gSegShowArr[2]=gSegTable[SSData2/100] | 0x20;			//显示小数点
	gSegShowArr[1]=gSegTable[(SSData2%100)/10];
	gSegShowArr[0]=gSegTable[(SSData2%10)/1];

}
void SegShowData_2(INT16U SSData1,INT16U SSData2)
{
	SSData2 %= 1000;										//防止数据出界
	if(SSData1 < 30)	SSData1 =0;								//0.3V
	if(SSData2 < 5)	SSData2 =0;								//0.05A
	if(SSData1 >=10000)
	{	
		SSData1 /=10;
		gSegShowArr[7]=gSegTable[SSData1/1000];
		gSegShowArr[6]=gSegTable[(SSData1%1000)/100];
		gSegShowArr[5]=gSegTable[(SSData1%100)/10] | 0x20;		//显示小数点
		gSegShowArr[4]=gSegTable[(SSData1%10)/1];
	}
	else
	{	gSegShowArr[7]=gSegTable[SSData1/1000];
		gSegShowArr[6]=gSegTable[(SSData1%1000)/100]| 0x20;
		gSegShowArr[5]=gSegTable[(SSData1%100)/10] ;		//显示小数点
		gSegShowArr[4]=gSegTable[(SSData1%10)/1];
		
	}
	gSegShowArr[3]=gSegTable[0];							//"0"
	gSegShowArr[2]=gSegTable[SSData2/100] | 0x20;			//显示小数点
	gSegShowArr[1]=gSegTable[(SSData2%100)/10];
	gSegShowArr[0]=gSegTable[(SSData2%10)/1];

}

/**********************************************************
*	数码管显示错误代码
***********************************************************/
void SegShowErrCode(INT16U SSData1,INT16U SSData2)
{
	SSData1 %= 10000;										//防止数据出界
	SSData2 %= 100;											//防止数据出界
	gSegShowArr[7] = 0xCB;									//E
	gSegShowArr[6] = 0x88;									//r
	gSegShowArr[5] = 0x88;									//r
	gSegShowArr[4] = gSegTable[10];							//NULL

	gSegShowArr[3] = gSegTable[10];							//NULL
	gSegShowArr[2] = 0xCB;									//E
	gSegShowArr[1] = gSegTable[SSData2/10];
	gSegShowArr[0] = gSegTable[SSData2%10];
}

/**********************************************************
*	显示待处理状态
***********************************************************/
void SegShowPending(INT8U Turn)
{
/*	gSegShowArr[7] = 0x08;									//-
	gSegShowArr[6] = 0x08;									//-
	gSegShowArr[5] = 0x08;									//-
	gSegShowArr[4] = 0x08;									//-

	gSegShowArr[3] = 0x08;									//-
	gSegShowArr[2] = 0x08;									//-
	gSegShowArr[1] = 0x08;
	gSegShowArr[0] = 0x08;
*/
	if(Turn >= 7) Turn = 6;
	gSegShowArr[7] = 0x00;									//-	  P-0x8F,"-"-0x08
	gSegShowArr[6] = 0x00;									//-
	gSegShowArr[5] = 0x00;									//-
	gSegShowArr[4] = 0x00;									//-

	gSegShowArr[3] = 0x00;									//-
	gSegShowArr[2] = 0x00;									//-
	gSegShowArr[1] = 0x00;
	gSegShowArr[0] = 0x00;
	gSegShowArr[Turn] = 0x08;
	gSegShowArr[Turn+1] = 0x08;

}



/******************************************************
*	根据采样计算电压值
*	计算方法见软件设计说明书
*	为保证电压精度，采样值放大了10倍
******************************************************/
INT16U ComputeVol(INT16U lVolValue)
{
	unsigned int   tmp32=0;
	if(lVolValue < 8000)
	{
		tmp32= lVolValue - 0;
		if(gAdjOutVol[0] >= gAdjOutVol[1]) gAdjOutVol[1] = gAdjOutVol[0]+1;	  //防止出错
		tmp32= (tmp32 * (gAdjOutVol[1]-gAdjOutVol[0]))/(800-0)+	gAdjOutVol[0]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else if(lVolValue < 16000)
	{
		tmp32= lVolValue - 8000;
		if(gAdjOutVol[1] >= gAdjOutVol[2]) gAdjOutVol[2] = gAdjOutVol[1]+1;
		tmp32= (tmp32 * (gAdjOutVol[2]-gAdjOutVol[1]))/(1600-800)+gAdjOutVol[1]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else if(lVolValue < 24000)
	{
		tmp32= lVolValue - 16000;
		if(gAdjOutVol[2] >= gAdjOutVol[3]) gAdjOutVol[3] = gAdjOutVol[2]+1;	  //防止出错
		tmp32= (tmp32 * (gAdjOutVol[3]-gAdjOutVol[2]))/(2400-1600)+gAdjOutVol[2]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else if(lVolValue < 32000)
	{
		tmp32= lVolValue - 24000;
		if(gAdjOutVol[3] >= gAdjOutVol[4]) gAdjOutVol[4] = gAdjOutVol[3]+1;	  //防止出错
		tmp32= (tmp32 * (gAdjOutVol[4]-gAdjOutVol[3]))/(3200-2400)+gAdjOutVol[3]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else //if(lVolValue >= gAdjOutVol[4])
	{
		tmp32= lVolValue - 32000;
		if(gAdjOutVol[4] >= gAdjOutVol[5]) gAdjOutVol[5] = gAdjOutVol[4]+1;	  //防止出错
		tmp32= (tmp32 * (gAdjOutVol[5]-gAdjOutVol[4]))/(4000-3200)+gAdjOutVol[4]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	gOutVoltage_100 = gOutVoltage;
	gOutVoltage	/= 10;
	return gOutVoltage;
}

/******************************************************
*	计算外部给定电压值  宏定义为采样值4000对应的电压值	10000,10.000V
*	更改宏定义可以更改采样计算的电压值，以后改成可矫正
*	在外部给定电压跳变比较严重时，设置1V的间断点，
*	可以有效的保护模块不经常在高压低压档之间切换
*	当10V对应的采样值40000
******************************************************/
#define EXTVOL_101V		101*100/6	//101*10000/600	  	//设定101V是对应的外部给定电压值
#define EXTVOL_100V		100*100/6
#define EXTVOL_99V		99*100/6
INT16U ComputeExtVol(INT16U lVolValue)
{
	INT32U   tmp32=0;
	static INT16U lExtSetVol;
	static INT16U lExtSetCur;
	//static INT16U lVol[10];
	tmp32 = lVolValue;										//
	tmp32 =	tmp32*gAdjExtVol/4000/10;							//按照10V对应采样值4000,
	gExtVoltage = (INT16U)tmp32;							//系统检测到的外部电压实际
	if(tmp32 > 10000) tmp32 = 10000;						//防止设定超过最大值

	if((gSegShowStatus == 0x10) ||(gSegShowStatus == 0x20))	//如果处理外部给定恒压模式
	{
	 	if(GetVolMode() == 1)								//处于高压模式
		{
			if(gExtVoltage >= EXTVOL_100V)					//大于100V，一直是高压模式
			{
			 	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
			else if(gExtVoltage >= EXTVOL_99V)				//大于99V，一直是高压模式
			{
			 	gExtSetVol = 1000;							//设定为100.0V
			}
			else
			{												//小于99V，切换到低压端
			   	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
		}
		else
		{
			if(gExtVoltage < EXTVOL_100V)					//小于100V，一直是低压模式，99.9V
			{
			 	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
			else if(gExtVoltage <= EXTVOL_101V)				//小于101V，一直是低压模式
			{
			 	gExtSetVol = 999;							//设定为99.9V
			}
			else
			{												//>=100VV，切换到高压档
			   	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
		}
	}
	else
	{												
		gExtSetVol = (INT16U)(tmp32*6/10);						//0-0.0V 10.000-600.0V
	}							
	gExtSetCur = (INT16U)(tmp32/20);						//计算对应600V的设定值 *500/10000

	if(gExtSetVol > MAXSETVOL)	gExtSetVol = MAXSETVOL;		//电压电流值限制
	if(gExtSetVol < MINSETVOL)	gExtSetVol = MINSETVOL;
	if(gExtSetCur > MAXSETCUR)	gExtSetCur = MAXSETCUR;
	if(gExtSetCur < MINSETCUR)	gExtSetCur = MINSETCUR;

	if(lExtSetVol  > gExtSetVol) 		//当本次电压小于上次电压
	{
		if(lExtSetVol - gExtSetVol <= 2)//当两次电压差在0.2V之内，则保持上次的值不变
		{
			gExtSetVol = lExtSetVol;
		}
	}
	if(lExtSetVol  < gExtSetVol)		//当本次电压大于上次电压
	{
		if(gExtSetVol - lExtSetVol <= 2)//当两次电压差在0.2V之内，则保持上次的值不变
		{
			gExtSetVol = lExtSetVol;
		}
	}
	if(lExtSetCur  > gExtSetCur)		//当本次电流小于上次电流
	{
		if(lExtSetCur - gExtSetCur <= 10)//当两次电流差在0.1A之内，则保持上次的值不变
		{
			gExtSetCur = lExtSetCur;
		}
	}
	if(lExtSetCur  < gExtSetCur)		 //当本次电流大于上次电流
	{
		if(gExtSetCur - lExtSetCur <= 10)//当两次电流差在0.1A之内，则保持上次的值不变
		{
			gExtSetCur = lExtSetCur;
		}
	}


	return 0;
}

/******************************************************
*	根据采样计算电流值
******************************************************/
INT16U ComputeCur(INT16U lCurValue)
{
	unsigned int   tmp32=0;
	if(lCurValue < 800)
	{
		tmp32= lCurValue - 0;
		if(gAdjOutCur[0] >= gAdjOutCur[1]) gAdjOutCur[1] = gAdjOutCur[0]+1;	  //防止出错
		tmp32= (tmp32 * (gAdjOutCur[1]-gAdjOutCur[0]))/(800-0)+	gAdjOutCur[0];
		gOutCurrent = (unsigned short ) tmp32;
	}
	else if(lCurValue < 1600)
	{
		tmp32= lCurValue - 800;
		if(gAdjOutCur[1] >= gAdjOutCur[2]) gAdjOutCur[2] = gAdjOutCur[1]+1;
		tmp32= (tmp32 * (gAdjOutCur[2]-gAdjOutCur[1]))/(1600-800)+gAdjOutCur[1];
		gOutCurrent = (unsigned short ) tmp32;
	}
	else if(lCurValue < 2400)
	{
		tmp32= lCurValue - 1600;
		if(gAdjOutCur[2] >= gAdjOutCur[3]) gAdjOutCur[3] = gAdjOutCur[2]+1;
		tmp32= (tmp32 * (gAdjOutCur[3]-gAdjOutCur[2]))/(2400-1600)+gAdjOutCur[2];
		gOutCurrent = (unsigned short ) tmp32;
	}
	else if(lCurValue < 3200)
	{
		tmp32= lCurValue - 2400;
		if(gAdjOutCur[3] >= gAdjOutCur[4]) gAdjOutCur[4] = gAdjOutCur[3]+1;
		tmp32= (tmp32 * (gAdjOutCur[4]-gAdjOutCur[3]))/(3200-2400)+gAdjOutCur[3];
		gOutCurrent = (unsigned short ) tmp32;
	}
	else //if(lCurValue >= gAdjOutCur[4])
	{	
		tmp32= lCurValue - 3200;
		if(gAdjOutCur[4] >= gAdjOutCur[5]) gAdjOutCur[5] = gAdjOutCur[4]+1;
		tmp32= (tmp32 * (gAdjOutCur[5]-gAdjOutCur[4]))/(4000-3200)+gAdjOutCur[4];
		gOutCurrent = (unsigned short ) tmp32;
	}
	return gOutCurrent;
}
/******************************************************
*	当有错误时显示的数码管变化
******************************************************/
#define DFERRSHOWTIME	6
void SystemErrShow(INT8U  *lRunT)
{
	static INT8U lRunTimes;
	(*lRunT) ++;
	lRunTimes = *lRunT;
	if(lRunTimes ==1)
	{														//if this bit is true then it is err.
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01) 	//if err then show err code
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*1;}						//if no err then goto next
	}														
	if(lRunTimes ==DFERRSHOWTIME*1)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*2;	}
	}
	if(lRunTimes ==DFERRSHOWTIME*2)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*3;	}
	}
	if(lRunTimes ==DFERRSHOWTIME*3)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*4;	}
	}
	if(lRunTimes ==DFERRSHOWTIME*4)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*5;	}
	}
	if(lRunTimes ==DFERRSHOWTIME*5)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*6;	}
	}
	if(lRunTimes ==DFERRSHOWTIME*6)
	{
	 	if((gSystemERR>>(lRunTimes/DFERRSHOWTIME)) &0x01)
	 	{	SegShowErrCode(0,lRunTimes/DFERRSHOWTIME+1);	}
		else
		{	lRunTimes=DFERRSHOWTIME*7;	}
	}
	if(lRunTimes >=DFERRSHOWTIME*7)
	{
	   	(*lRunT)=0;
	}
}

/******************************************************
*	显示外部给定电压
*	入口参数放大1000倍
*	eg：5632 --》5.632
*		10568--》10.56	未四舍五入
******************************************************/
void ShowSysArgExt(INT16U lExtVoltage)
{
	gSegShowArr[7] = gSegTable[1];							//"1"							
	gSegShowArr[6] = gSegTable[10];							//NULL		
	gSegShowArr[5] = gSegTable[10];							//NULL	
	gSegShowArr[4] = gSegTable[10];							//NULL
	if(lExtVoltage>=10000)									//if value>10000 小数点后有两位
	{
		lExtVoltage /= 10;
		gSegShowArr[3] = gSegTable[lExtVoltage/1000];		//显示十位		
		gSegShowArr[2] = gSegTable[(lExtVoltage%1000)/100]|0x20;//显示个位带小数点	
		gSegShowArr[1] = gSegTable[(lExtVoltage%100)/10];	//显示十分位
		gSegShowArr[0] = gSegTable[lExtVoltage%10];			//显示百分位
	}
	else
	{
		gSegShowArr[3] = gSegTable[lExtVoltage/1000]|0x20;	//显示小数点			
		gSegShowArr[2] = gSegTable[(lExtVoltage%1000)/100];	//显示十分位
		gSegShowArr[1] = gSegTable[(lExtVoltage%100)/10];	//显示百分位
		gSegShowArr[0] = gSegTable[lExtVoltage%10];			//显示千分位
	}
}
