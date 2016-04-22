/**********************************************************
按键扫描程序
	1、识别是哪个按键按下
	2、按键连续被按下的时间或扫描次数
	3、按键的处理（调整各个参数）
	4、当无按键时，计时5S退出设置状态
	5、通过改变gSegShowStatus让现实程序显示相应的状态

负载越大，输出的电压可能会下降几伏，是否需要补偿，
或根据检测输出电压调整输出电压值，补偿必须有范围限制.
在初始化时，发送一个虚拟按键，以初始化显示状态
**********************************************************/
#include "Global.h"

#define DFKeyLongTime  		6								//按键检测的最长时间 300ms
#define DFKeyShortTime 		2								//定义按键有效的时间 100ms 单位50ms
#define DFKeyWaitTime		30								//每次按键的时间单位50ms
#define DFKeyCHGTime		4								//恒压恒流按键切换的时间单位500ms

#define KEYSETVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)	//按键低电平有效 S1
#define KEYCHGVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)	//按键低电平有效 S3
#define KEYUPVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//按键低电平有效 S2
#define KEYDOWNVALID	 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)	//按键低电平有效 S4

INT8U  gChargeMode;											//bit0-0/1-auto/manual,bit1-0/1-autoV/C,bit2-0/1-manualV/C
INT8U  gKeyValue;											//按键值
INT8U  gKeyTimes;											//连续按键的次数或时间
INT8U  gSegShowStatus;										//电压电流设置所处的状态
INT8U  gSegShowElse;
INT16U gSetBak;												//当设置电压电流时的备份值
extern INT8U gSysPWMchg ;
void DataCHGint(INT16U *DataVal, INT16U, INT16U, INT16S); 	//增加减少键调整当前的整型数值
void SystemSetting(void);


extern INT16U gSysSetCurpwm;										//系统设置的电流PMW
extern INT16U gSysSetVolpwm;										//系统设置的电压PMW

/**********************************************************
	设置当前电压模式
	0-低压模式
	1-高压模式
**********************************************************/
INT8U gVoltageMode=0;										//电压模式全局变量
void SetVolMode(INT8U Mode)								
{															//
	if(Mode)	gVoltageMode = 0x10;						//高压
	if(!Mode)	gVoltageMode = 0x01;						//低压
}

INT8U GetVolMode()
{
	if(gVoltageMode == 0x10){	return 1;	  }		 		//高压模式
	if(gVoltageMode == 0x01){	return 0;	  }				//低压模式
	return 0;												//未知或错误模式返回低压模式
}

/**********************************************************
*   按键扫描
*	返回扫描的按键值
**********************************************************/
INT8U KeyScan()
{
	INT8U tmp=0;
	INT8U KeyValue=0;										//清空按键	
	tmp = 0;												// 	
	if((KEYCHGVALID) && !(KEYSETVALID))						//仅恒压恒流切换按键有效
	{
		while((tmp<=DFKeyLongTime) && (KEYCHGVALID) && !(KEYSETVALID))	//等待100-300ms
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)								//if 确实有按键按下
		{
			KeyValue = 0x01;
		}
	}

	tmp = 0;
	if(!(KEYCHGVALID) && (KEYSETVALID))						//仅自动手动切换按键有效
	{									
		while((tmp<=DFKeyLongTime) && !(KEYCHGVALID) && (KEYSETVALID))
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)
		{
			KeyValue = 0x02;
		}
	}
	tmp = 0;
	if((KEYCHGVALID) && (KEYSETVALID))						//两个切换按键均有效
	{									
		while((tmp<=DFKeyLongTime) && (KEYCHGVALID) && (KEYSETVALID))
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)
		{
			KeyValue = 0x03;
		}
	}


	tmp = 0;
	if((KEYUPVALID) && !(KEYDOWNVALID))						//增加按键
	{
		while((tmp<=DFKeyLongTime) && (KEYUPVALID)&& !(KEYDOWNVALID))
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)
		{
			KeyValue = 0x04;
		}
	}														

	tmp = 0;
	if(!(KEYUPVALID) && (KEYDOWNVALID))						//减小/关机按键
	{
		while((tmp<=DFKeyLongTime) && !(KEYUPVALID)&& (KEYDOWNVALID))
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)
		{
			KeyValue = 0x08;
		}
	}
	tmp = 0;
	if((KEYUPVALID) && (KEYDOWNVALID))						//增加/减小两个同时有效
	{
		while((tmp<=DFKeyLongTime) && (KEYUPVALID)&& (KEYDOWNVALID))
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)
		{
			KeyValue = 0x0C;
		}
	}
	return KeyValue; 										//返回按键值
}
/**********************************************************
   按键扫描任务
**********************************************************/ 
void task_key(void *pdata)
{
	unsigned char lKeyValue = 0;								//本次按键扫描的值
	unsigned char lRuntime = 0;
	pdata = pdata;
while(1)
{	
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);	
	gKeyValue = KeyScan();									//按键扫描
	if(gKeyValue == lKeyValue) 
	{
		gKeyTimes++;										//和上次按键值一样则说明是连续按键
		if(gKeyTimes >= 255) gKeyTimes--;					//防止计数溢出								
	}
	else													//if not the same key,clear the times
	{
		gKeyTimes=0;
	}
	lKeyValue=gKeyValue;									//save the key
	//----------------------------------------------------- //exit
	if(((gKeyValue == 0) && (gKeyTimes >= 40))				//5 second and no key is down,now we left the set mode
	   ||(gKeyValue == 0x01) ||(gKeyValue == 0x02))
	{
		if(gSegShowStatus == 0x02)						  	//set voltage
		{
		   gSegShowStatus = 0x01;
		   if(gSetBak != gSetVoltage)
		   {	EE_SaveSetVol(gSetVoltage);}
		}
		if(gSegShowStatus == 0x04)						  	//set current
		{
		   gSegShowStatus = 0x03;
		   if(gSetBak != gSetCurrent)
		   {	EE_SaveSetCur(gSetCurrent);	}				//save current
		}
	}
	if(((gKeyValue == 0) && (gKeyTimes >= 30))				//1 second and no key is down,now we left the set mode
	   ||(gKeyValue == 0x01) ||(gKeyValue == 0x02))			//
	{
		if(gSegShowStatus == 0x20)							//set voltage
		{
		   gSegShowStatus = 0x10;
		   //save voltage
		}
		if(gSegShowStatus == 0x40)							//set current
		{
		   gSegShowStatus = 0x30;
		   //save current
		}
	}
	if((gKeyValue == 0) && (gKeyTimes >= 250))				//10 second and no key is down,now we left the set mode
	{
		if((gSegShowStatus & 0x88) == 0x80)					
		{
		   gSegShowStatus &= ~0x80;
		   //save voltage
		}
	}

	//-----------------------------------------------------
	if(gSegShowStatus ==0)								   //启动的时候的循环显示的画面
	{
		lRuntime ++;		
		if((lRuntime) < 58) 
		{	gSystemERR =0x00;
			gKeyValue =0 ;
			gSysSetVolpwm = 12000;
			gSysSetCurpwm = 12000;
			TIM_SetCompare3(TIM3, 12000);					//置PWM为0
			TIM_SetCompare4(TIM3, 12000);					//置PWM为0
		}
		else
		{
			gKeyValue=0x55;									//退出待机状态
		}
	}
															
	if((gSegShowStatus == 0x10)||(gSegShowStatus == 0x20))  //在外部给定电压情况下，在显示任务中调整电压，
	{														//当低压档切换到高压当时会出现过冲
		lRuntime ++;
		if(lRuntime >= 10) 								   	//每1S更新一次电压设置
		{
			lRuntime=0;
			PWM_SetVoltage(gExtSetVol);						//调整电压PWM
		} 		
	}

	if(gKeyValue == 0)										//如果无按键则不进行下面的按键操作 
	{
		OSTimeDlyHMSM(0,0,0,100);
		continue ;
	}
	/******************************************************///自动和手动方式切换
	if((gKeyValue == 0x02) &&(gKeyTimes ==DFKeyCHGTime)) 	//从按键的角度出发
	{														
		TIM_SetCompare3(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动
		TIM_SetCompare4(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动
		if((gSegShowStatus >= 0x01) && (gSegShowStatus <= 0x04))//It is manual mode
		{
			if(!(gChargeMode & 0x04)) 						//bit0-0/1-auto/manual,bit1-0/1-autoV/C,bit2-0/1-manualV/C
			{											
				gSegShowStatus=0x10;						//enter auto model constant voltage
				gChargeMode &= ~0x02;
			}
			else
			{
				gSegShowStatus=0x30;						//enter auto model constant current 
				gChargeMode |= 0x02;
			}
			gChargeMode &= ~0x01;							//only chg bit0
		}
		else if((gSegShowStatus >= 0x10) && (gSegShowStatus <= 0x40))
		{
	   		if(!(gChargeMode & 0x02)) 						
			{
				gSegShowStatus=0x01;						//enter manual model constant voltage
				gChargeMode &= ~0x04;

			   	PWM_SetVoltage(gSetVoltage);				//调整电压PWM
				PWM_SetCurrent(gSetCurrent);				//调整电流PWM
			}
			else
			{
				gSegShowStatus=0x03;						//enter manual model constant current
				gChargeMode |= 0x04;
				PWM_SetCurrent(gSetCurrent);				//调整电流PWM
				PWM_SetVoltage(gSetVoltage);				//设置电压600.0V

			}
			gChargeMode |= 0x01;							//only chg bit0
		}
		else
		{
		}
		//gChargeMode ^= 0x01;
		gUserDB.uartCommAlarm=1;
		EE_SaveSetMode(gChargeMode);						//保存模式
	}
	/******************************************************///手动即按键设置的处理
	if(gSegShowStatus == 0x00)								//待处理状态
	{
	 														//电压电流均显示0
		if(gKeyValue ==0x55 )								//开机4S之后自动开机
		{
			TIM_SetCompare3(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动
			TIM_SetCompare4(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动		
			//gSystemERR &=0x00;

			if(gChargeMode & 0x01)							//手动模式
			{
				if(gChargeMode & 0x04)						//恒流模式
				{
			 		PWM_SetVoltage(gSetVoltage);			//设置电压600.0V
					PWM_SetCurrent(gSetCurrent);			//
					gSegShowStatus = 0x03;					//手动恒流
				}
				else										//恒压模式
				{
					PWM_SetCurrent(gSetCurrent);			//设置电流5.00A
					PWM_SetVoltage(gSetVoltage);						
					gSegShowStatus = 0x01;
				}											//手动恒压
			}
			else
			{	
				if(gChargeMode & 0x02)						//恒流模式
				{
			 		PWM_SetVoltage(gSetVoltage);			//设置电压600.0V
					PWM_SetCurrent(10);						//
					gSegShowStatus = 0x30;					//手动恒流
				}
				else										//恒压模式
				{
			 		PWM_SetVoltage(60);						
					PWM_SetCurrent(gSetCurrent);			//设置电流5.00A
					gSegShowStatus = 0x10;
				}	
			}
		}
	}
	else if(gSegShowStatus == 0x01)							//手动恒压模式
	{
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))
		{
			TIM_SetCompare3(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动
			TIM_SetCompare4(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动					
			gSegShowStatus=0x03;							//进入手动恒流模式
			gChargeMode |= 0x04;
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
			PWM_SetVoltage(gSetVoltage);					//设置电压600.0V
			EE_SaveSetMode(gChargeMode);					//保存模式
		}

		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//进入手动电压设置
		{
			gSegShowStatus=0x02;
			gSetBak = gSetVoltage;	
		}
	}
	else if(gSegShowStatus == 0x02)							//手动电压设置
	{
	 	if(gKeyValue == 0x04)								//增加按键有效
		{
			if(gKeyTimes <= 3) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)1);		 	//增加0.1V
			else if(gKeyTimes <= 13) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)10);	//增加1.0V
			else if(gKeyTimes <= 255) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)100);	//增加10.0V
			else {}
			PWM_SetVoltage(gSetVoltage);					//调整电压PWM
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
		}
	 	if(gKeyValue == 0x08)								//减少按键有效
		{																					   	
			if(gKeyTimes <= 3) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-1);			//减小0.1V
			else if(gKeyTimes <= 13) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-10);	//减小1.0V
			else if(gKeyTimes <= 255) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-100);//减小10.0V
			else {}
			PWM_SetVoltage(gSetVoltage);					//调整电压PWM
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
		}
	}
	else if(gSegShowStatus == 0x03)							//手动恒流模式
	{		
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))	//如果恒压恒流切换按键
		{
			TIM_SetCompare3(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动
			TIM_SetCompare4(TIM3, 12000);					//置PWM为高电平，下次将重新进行软启动					
			gSegShowStatus = 0x01;							//进入手动恒压模式
			gChargeMode &= ~0x04;
			PWM_SetVoltage(gSetVoltage);					//调整电压PWM
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
			EE_SaveSetMode(gChargeMode);					//保存模式
		}

		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//如果增加或减少按键
		{
			gSegShowStatus=0x04;							//进入手动电流设置模式
			gSetBak = gSetCurrent;
		}
	}
	else if(gSegShowStatus == 0x04)
	{
	 	if(gKeyValue == 0x04)								//增加按键有效
		{
			if(gKeyTimes <= 4) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)10);
			else if(gKeyTimes <= 255) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)50);
			else {}
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
			PWM_SetVoltage(gSetVoltage);					//设置电压600.0V
		}													  
	 	if(gKeyValue == 0x08)								//减小按键
		{
			if(gKeyTimes <= 4) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)-10);
			else if(gKeyTimes <= 255) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)-50);
			else {}
			PWM_SetCurrent(gSetCurrent);					//调整电流PWM
			PWM_SetVoltage(gSetVoltage);					//设置电压600.0V
		}
	}
	/******************************************************///自动即外部给定的处理
	else if(gSegShowStatus == 0x10)							//自动恒压模式
	{
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))	//恒压恒流切换按键
		{
			TIM_SetCompare3(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动
			TIM_SetCompare4(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动					
			gSegShowStatus=0x30;							//进入自动恒流模式
			gChargeMode |= 0x02;
			EE_SaveSetMode(gChargeMode);					//保存模式 
		}
		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//
		{
			gSegShowStatus = 0x20;							//enter set voltage
		}
	}
	else if(gSegShowStatus == 0x20)							//set current and ledc flash
	{
		if((gKeyValue == 0x01)||(gKeyValue == 0x02))
		{
			gSegShowStatus = 0x10;
		}	
	}
	else if(gSegShowStatus == 0x30)							//
	{
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))
		{
			TIM_SetCompare3(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动
			TIM_SetCompare4(TIM3, 12000);						//置PWM为高电平，下次将重新进行软启动					
			gSegShowStatus=0x10;							//enter constant current
			gChargeMode &= ~0x02;
			EE_SaveSetMode(gChargeMode);					//保存模式 			 
		}
		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//
		{
			gSegShowStatus = 0x40;							//
		}
	}
	else if(gSegShowStatus == 0x40)
	{
		if((gKeyValue == 0x01)||(gKeyValue == 0x02))
		{
			gSegShowStatus = 0x30;
		}		
	}
	else if((gSegShowStatus & 0x80) == 0x80)
	{
		if((gKeyValue == 0x03)&&(gKeyTimes == 2))		   		//exit
		{
			gSegShowStatus &= ~0x80;
			gKeyTimes =0;
			if((gSegShowElse == 26)&&(gSysPWMchg == 101))
			{
				INT8U  tmpArr[30];					//定义一个临时的数组，主要是应对24C16的读写，任何地方均可更改
				INT8U  tmp ;
				for(tmp=0; tmp<7; tmp++)									
				{
					gAdjSetVol_F[tmp] = ModbusREGDat[12+tmp];						//设置电压校正数据
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol_F[tmp]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol_F[tmp]%256);						
				}
				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_1, 14);	  				//保存设置输出电压校正参数
				for(tmp=0; tmp<8; tmp++)									
				{
					gAdjSetVol[tmp] = ModbusREGDat[50+tmp];						//设置电压校正数据
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol[tmp]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol[tmp]%256);						
				}
//				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_2, 16);	  				//保存设置输出电压校正参数
				for(tmp=0; tmp<6; tmp++)									
				{
					gAdjSetVol[tmp+8] = ModbusREGDat[58+tmp];						//设置电压校正数据
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol[tmp+8]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol[tmp+8]%256);						
				}   
//				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_3, 12);	  				//保存设置输出电压校正参数			

				tmpArr[0] = (INT8U)(gAdjExtVol/256);				//
				tmpArr[1] = (INT8U)(gAdjExtVol%256);	
				I2C_SYS_PageWrite(tmpArr,ADDR_ADJEXTVOL , 2);	   	//保存设置显示电流校正参数


			}
			gSysPWMchg = 100;

		}
		if((gKeyValue == 0x01) &&(gSegShowElse<26))
		{
			gSegShowElse ++;
			gSysPWMchg = 100;
			if(gSegShowElse == 5) PWM_SetVoltage(50);
			if(gSegShowElse == 12) PWM_SetVoltage(1100);

		}
		if((gKeyValue == 0x02) &&(gSegShowElse>1))
		{
			gSegShowElse --;
			gSysPWMchg = 100;
			if(gSegShowElse == 11) PWM_SetVoltage(900);
			if(gSegShowElse == 25) PWM_SetVoltage(5900);

		}
		SystemSetting();									//调用系统设定
	}
	else													//意外陷阱
	{
		//MODELSTOP;											//关模块
		//while(1)											//死循环
		//{	;	}
	}

	if((gKeyValue == 0x03)&&(gKeyTimes == 4))		   		//
	{
	 	if((gSegShowStatus & 0x88) == 0)
		{
			gSegShowStatus += 0x80;
			gSegShowElse=0x01;
		}
	}
/*	if((gKeyValue == 0x03)&&(gKeyTimes == 10))		   		//
	{
	 	if((gSegShowStatus & 0x88) == 0x80)
		{
			gSegShowStatus += 0x08;	  //0x88
			gSegShowElse=0x08;
		}
	}
*/
	OSTimeDlyHMSM(0,0,0,100);							   	//delay 100ms
}	//end while
}	//end task


/**********************************************************
   增加减少键调整当前的数值，目标对象是int
   入口参数：当前值，最大值，最小值，步进值
**********************************************************/ 
void DataCHGint(INT16U *DataVal,INT16U DataMAX,INT16U DataMIN,INT16S Step)
{
	if((*DataVal) > DataMAX)								//如果超过最大值则不操作	
	{
		*DataVal=DataMAX;
		return;
	}
	if((*DataVal) < DataMIN)								//如果超小于最小值则不操作
	{
		*DataVal=DataMIN;
		return;
	}
	if(DataMAX < DataMIN)
	{
	 	return ;
	}
	if(Step > 0)						  					//增加
	{
			Step = (INT16U)Step;
			if((*DataVal)+Step <= DataMAX)
			{  
				(*DataVal) += Step;
			}
			else 
			{
				(*DataVal) = DataMAX ;
			}
	}
	else							   						//减小
	{
		Step = -(Step);
		Step = (INT16U)Step;
		if(*DataVal>=DataMIN + Step) 						//step<0
		{ 
			*DataVal -= Step;
		}
		else 
		{
			*DataVal=DataMIN;
		}
	}
}

/**********************************************************
	系统的设定，保留以后扩展使用
***********************************************************/
void SystemSetting()
{
//	INT16U l_PWM;
//	if(gKeyValue == 0x04) gSysPWMchg++;
//	if(gKeyValue == 0x08) gSysPWMchg--;				   		//忽略小于0
//	if(gSysPWMchg > 199) gSysPWMchg=199;					//
//	if(gSysPWMchg < 1) gSysPWMchg=1;
//	if(gSegShowElse ==3)
//	{
//		gAdjExtVol = ModbusREGDat[45]+ gSysPWMchg-100;
//	}
//
//	if((gSegShowElse >=5) &&(gSegShowElse <=11))
//	{
//		l_PWM = gAdjSetVol_F[gSegShowElse-5] ;
//		if(l_PWM + gSysPWMchg < 100) l_PWM = 0;
//		else l_PWM = l_PWM + gSysPWMchg - 100;
//		if(l_PWM >= 12000) l_PWM = 12000;
//		gSysSetVolpwm = 12000-l_PWM;
//		ModbusREGDat[12+gSegShowElse-5] = l_PWM;
//	}
//	if((gSegShowElse >=12) &&(gSegShowElse <=25))
//	{
//		l_PWM = gAdjSetVol[gSegShowElse-12] ;
//		if(l_PWM + gSysPWMchg < 100) l_PWM = 0;
//		else l_PWM = l_PWM + gSysPWMchg - 100;
//		if(l_PWM >= 12000) l_PWM = 12000;
//		gSysSetVolpwm = 12000-l_PWM;
//		ModbusREGDat[50+gSegShowElse-12] = l_PWM;
//	}
}

						
