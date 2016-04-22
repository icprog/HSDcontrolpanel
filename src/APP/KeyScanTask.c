/**********************************************************
����ɨ�����
	1��ʶ�����ĸ���������
	2���������������µ�ʱ���ɨ�����
	3�������Ĵ�����������������
	4�����ް���ʱ����ʱ5S�˳�����״̬
	5��ͨ���ı�gSegShowStatus����ʵ������ʾ��Ӧ��״̬

����Խ������ĵ�ѹ���ܻ��½��������Ƿ���Ҫ������
����ݼ�������ѹ���������ѹֵ�����������з�Χ����.
�ڳ�ʼ��ʱ������һ�����ⰴ�����Գ�ʼ����ʾ״̬
**********************************************************/
#include "Global.h"

#define DFKeyLongTime  		6								//���������ʱ�� 300ms
#define DFKeyShortTime 		2								//���尴����Ч��ʱ�� 100ms ��λ50ms
#define DFKeyWaitTime		30								//ÿ�ΰ�����ʱ�䵥λ50ms
#define DFKeyCHGTime		4								//��ѹ���������л���ʱ�䵥λ500ms

#define KEYSETVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)	//�����͵�ƽ��Ч S1
#define KEYCHGVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)	//�����͵�ƽ��Ч S3
#define KEYUPVALID		 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//�����͵�ƽ��Ч S2
#define KEYDOWNVALID	 	!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)	//�����͵�ƽ��Ч S4

INT8U  gChargeMode;											//bit0-0/1-auto/manual,bit1-0/1-autoV/C,bit2-0/1-manualV/C
INT8U  gKeyValue;											//����ֵ
INT8U  gKeyTimes;											//���������Ĵ�����ʱ��
INT8U  gSegShowStatus;										//��ѹ��������������״̬
INT8U  gSegShowElse;
INT16U gSetBak;												//�����õ�ѹ����ʱ�ı���ֵ
extern INT8U gSysPWMchg ;
void DataCHGint(INT16U *DataVal, INT16U, INT16U, INT16S); 	//���Ӽ��ټ�������ǰ��������ֵ
void SystemSetting(void);


extern INT16U gSysSetCurpwm;										//ϵͳ���õĵ���PMW
extern INT16U gSysSetVolpwm;										//ϵͳ���õĵ�ѹPMW

/**********************************************************
	���õ�ǰ��ѹģʽ
	0-��ѹģʽ
	1-��ѹģʽ
**********************************************************/
INT8U gVoltageMode=0;										//��ѹģʽȫ�ֱ���
void SetVolMode(INT8U Mode)								
{															//
	if(Mode)	gVoltageMode = 0x10;						//��ѹ
	if(!Mode)	gVoltageMode = 0x01;						//��ѹ
}

INT8U GetVolMode()
{
	if(gVoltageMode == 0x10){	return 1;	  }		 		//��ѹģʽ
	if(gVoltageMode == 0x01){	return 0;	  }				//��ѹģʽ
	return 0;												//δ֪�����ģʽ���ص�ѹģʽ
}

/**********************************************************
*   ����ɨ��
*	����ɨ��İ���ֵ
**********************************************************/
INT8U KeyScan()
{
	INT8U tmp=0;
	INT8U KeyValue=0;										//��հ���	
	tmp = 0;												// 	
	if((KEYCHGVALID) && !(KEYSETVALID))						//����ѹ�����л�������Ч
	{
		while((tmp<=DFKeyLongTime) && (KEYCHGVALID) && !(KEYSETVALID))	//�ȴ�100-300ms
		{
			OSTimeDlyHMSM(0,0,0,DFKeyWaitTime);
			tmp++;
		}
		if(tmp>=DFKeyShortTime)								//if ȷʵ�а�������
		{
			KeyValue = 0x01;
		}
	}

	tmp = 0;
	if(!(KEYCHGVALID) && (KEYSETVALID))						//���Զ��ֶ��л�������Ч
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
	if((KEYCHGVALID) && (KEYSETVALID))						//�����л���������Ч
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
	if((KEYUPVALID) && !(KEYDOWNVALID))						//���Ӱ���
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
	if(!(KEYUPVALID) && (KEYDOWNVALID))						//��С/�ػ�����
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
	if((KEYUPVALID) && (KEYDOWNVALID))						//����/��С����ͬʱ��Ч
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
	return KeyValue; 										//���ذ���ֵ
}
/**********************************************************
   ����ɨ������
**********************************************************/ 
void task_key(void *pdata)
{
	unsigned char lKeyValue = 0;								//���ΰ���ɨ���ֵ
	unsigned char lRuntime = 0;
	pdata = pdata;
while(1)
{	
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);	
	gKeyValue = KeyScan();									//����ɨ��
	if(gKeyValue == lKeyValue) 
	{
		gKeyTimes++;										//���ϴΰ���ֵһ����˵������������
		if(gKeyTimes >= 255) gKeyTimes--;					//��ֹ�������								
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
	if(gSegShowStatus ==0)								   //������ʱ���ѭ����ʾ�Ļ���
	{
		lRuntime ++;		
		if((lRuntime) < 58) 
		{	gSystemERR =0x00;
			gKeyValue =0 ;
			gSysSetVolpwm = 12000;
			gSysSetCurpwm = 12000;
			TIM_SetCompare3(TIM3, 12000);					//��PWMΪ0
			TIM_SetCompare4(TIM3, 12000);					//��PWMΪ0
		}
		else
		{
			gKeyValue=0x55;									//�˳�����״̬
		}
	}
															
	if((gSegShowStatus == 0x10)||(gSegShowStatus == 0x20))  //���ⲿ������ѹ����£�����ʾ�����е�����ѹ��
	{														//����ѹ���л�����ѹ��ʱ����ֹ���
		lRuntime ++;
		if(lRuntime >= 10) 								   	//ÿ1S����һ�ε�ѹ����
		{
			lRuntime=0;
			PWM_SetVoltage(gExtSetVol);						//������ѹPWM
		} 		
	}

	if(gKeyValue == 0)										//����ް����򲻽�������İ������� 
	{
		OSTimeDlyHMSM(0,0,0,100);
		continue ;
	}
	/******************************************************///�Զ����ֶ���ʽ�л�
	if((gKeyValue == 0x02) &&(gKeyTimes ==DFKeyCHGTime)) 	//�Ӱ����ĽǶȳ���
	{														
		TIM_SetCompare3(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������
		TIM_SetCompare4(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������
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

			   	PWM_SetVoltage(gSetVoltage);				//������ѹPWM
				PWM_SetCurrent(gSetCurrent);				//��������PWM
			}
			else
			{
				gSegShowStatus=0x03;						//enter manual model constant current
				gChargeMode |= 0x04;
				PWM_SetCurrent(gSetCurrent);				//��������PWM
				PWM_SetVoltage(gSetVoltage);				//���õ�ѹ600.0V

			}
			gChargeMode |= 0x01;							//only chg bit0
		}
		else
		{
		}
		//gChargeMode ^= 0x01;
		gUserDB.uartCommAlarm=1;
		EE_SaveSetMode(gChargeMode);						//����ģʽ
	}
	/******************************************************///�ֶ����������õĴ���
	if(gSegShowStatus == 0x00)								//������״̬
	{
	 														//��ѹ��������ʾ0
		if(gKeyValue ==0x55 )								//����4S֮���Զ�����
		{
			TIM_SetCompare3(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������
			TIM_SetCompare4(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������		
			//gSystemERR &=0x00;

			if(gChargeMode & 0x01)							//�ֶ�ģʽ
			{
				if(gChargeMode & 0x04)						//����ģʽ
				{
			 		PWM_SetVoltage(gSetVoltage);			//���õ�ѹ600.0V
					PWM_SetCurrent(gSetCurrent);			//
					gSegShowStatus = 0x03;					//�ֶ�����
				}
				else										//��ѹģʽ
				{
					PWM_SetCurrent(gSetCurrent);			//���õ���5.00A
					PWM_SetVoltage(gSetVoltage);						
					gSegShowStatus = 0x01;
				}											//�ֶ���ѹ
			}
			else
			{	
				if(gChargeMode & 0x02)						//����ģʽ
				{
			 		PWM_SetVoltage(gSetVoltage);			//���õ�ѹ600.0V
					PWM_SetCurrent(10);						//
					gSegShowStatus = 0x30;					//�ֶ�����
				}
				else										//��ѹģʽ
				{
			 		PWM_SetVoltage(60);						
					PWM_SetCurrent(gSetCurrent);			//���õ���5.00A
					gSegShowStatus = 0x10;
				}	
			}
		}
	}
	else if(gSegShowStatus == 0x01)							//�ֶ���ѹģʽ
	{
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))
		{
			TIM_SetCompare3(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������
			TIM_SetCompare4(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������					
			gSegShowStatus=0x03;							//�����ֶ�����ģʽ
			gChargeMode |= 0x04;
			PWM_SetCurrent(gSetCurrent);					//��������PWM
			PWM_SetVoltage(gSetVoltage);					//���õ�ѹ600.0V
			EE_SaveSetMode(gChargeMode);					//����ģʽ
		}

		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//�����ֶ���ѹ����
		{
			gSegShowStatus=0x02;
			gSetBak = gSetVoltage;	
		}
	}
	else if(gSegShowStatus == 0x02)							//�ֶ���ѹ����
	{
	 	if(gKeyValue == 0x04)								//���Ӱ�����Ч
		{
			if(gKeyTimes <= 3) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)1);		 	//����0.1V
			else if(gKeyTimes <= 13) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)10);	//����1.0V
			else if(gKeyTimes <= 255) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)100);	//����10.0V
			else {}
			PWM_SetVoltage(gSetVoltage);					//������ѹPWM
			PWM_SetCurrent(gSetCurrent);					//��������PWM
		}
	 	if(gKeyValue == 0x08)								//���ٰ�����Ч
		{																					   	
			if(gKeyTimes <= 3) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-1);			//��С0.1V
			else if(gKeyTimes <= 13) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-10);	//��С1.0V
			else if(gKeyTimes <= 255) DataCHGint(&gSetVoltage,MAXSETVOL,MINSETVOL,(INT16S)-100);//��С10.0V
			else {}
			PWM_SetVoltage(gSetVoltage);					//������ѹPWM
			PWM_SetCurrent(gSetCurrent);					//��������PWM
		}
	}
	else if(gSegShowStatus == 0x03)							//�ֶ�����ģʽ
	{		
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))	//�����ѹ�����л�����
		{
			TIM_SetCompare3(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������
			TIM_SetCompare4(TIM3, 12000);					//��PWMΪ�ߵ�ƽ���´ν����½���������					
			gSegShowStatus = 0x01;							//�����ֶ���ѹģʽ
			gChargeMode &= ~0x04;
			PWM_SetVoltage(gSetVoltage);					//������ѹPWM
			PWM_SetCurrent(gSetCurrent);					//��������PWM
			EE_SaveSetMode(gChargeMode);					//����ģʽ
		}

		if((gKeyValue == 0x04) || (gKeyValue == 0x08)) 		//������ӻ���ٰ���
		{
			gSegShowStatus=0x04;							//�����ֶ���������ģʽ
			gSetBak = gSetCurrent;
		}
	}
	else if(gSegShowStatus == 0x04)
	{
	 	if(gKeyValue == 0x04)								//���Ӱ�����Ч
		{
			if(gKeyTimes <= 4) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)10);
			else if(gKeyTimes <= 255) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)50);
			else {}
			PWM_SetCurrent(gSetCurrent);					//��������PWM
			PWM_SetVoltage(gSetVoltage);					//���õ�ѹ600.0V
		}													  
	 	if(gKeyValue == 0x08)								//��С����
		{
			if(gKeyTimes <= 4) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)-10);
			else if(gKeyTimes <= 255) DataCHGint(&gSetCurrent,MAXSETCUR,MINSETCUR,(INT16S)-50);
			else {}
			PWM_SetCurrent(gSetCurrent);					//��������PWM
			PWM_SetVoltage(gSetVoltage);					//���õ�ѹ600.0V
		}
	}
	/******************************************************///�Զ����ⲿ�����Ĵ���
	else if(gSegShowStatus == 0x10)							//�Զ���ѹģʽ
	{
		if((gKeyValue == 0x01)&&(gKeyTimes ==DFKeyCHGTime))	//��ѹ�����л�����
		{
			TIM_SetCompare3(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������
			TIM_SetCompare4(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������					
			gSegShowStatus=0x30;							//�����Զ�����ģʽ
			gChargeMode |= 0x02;
			EE_SaveSetMode(gChargeMode);					//����ģʽ 
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
			TIM_SetCompare3(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������
			TIM_SetCompare4(TIM3, 12000);						//��PWMΪ�ߵ�ƽ���´ν����½���������					
			gSegShowStatus=0x10;							//enter constant current
			gChargeMode &= ~0x02;
			EE_SaveSetMode(gChargeMode);					//����ģʽ 			 
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
				INT8U  tmpArr[30];					//����һ����ʱ�����飬��Ҫ��Ӧ��24C16�Ķ�д���κεط����ɸ���
				INT8U  tmp ;
				for(tmp=0; tmp<7; tmp++)									
				{
					gAdjSetVol_F[tmp] = ModbusREGDat[12+tmp];						//���õ�ѹУ������
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol_F[tmp]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol_F[tmp]%256);						
				}
				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_1, 14);	  				//�������������ѹУ������
				for(tmp=0; tmp<8; tmp++)									
				{
					gAdjSetVol[tmp] = ModbusREGDat[50+tmp];						//���õ�ѹУ������
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol[tmp]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol[tmp]%256);						
				}
//				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_2, 16);	  				//�������������ѹУ������
				for(tmp=0; tmp<6; tmp++)									
				{
					gAdjSetVol[tmp+8] = ModbusREGDat[58+tmp];						//���õ�ѹУ������
					tmpArr[tmp*2]   = (INT8U)(gAdjSetVol[tmp+8]/256);				//
					tmpArr[tmp*2+1] = (INT8U)(gAdjSetVol[tmp+8]%256);						
				}   
//				I2C_SYS_PageWrite(tmpArr, ADDR_ADJOUTVOL_3, 12);	  				//�������������ѹУ������			

				tmpArr[0] = (INT8U)(gAdjExtVol/256);				//
				tmpArr[1] = (INT8U)(gAdjExtVol%256);	
				I2C_SYS_PageWrite(tmpArr,ADDR_ADJEXTVOL , 2);	   	//����������ʾ����У������


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
		SystemSetting();									//����ϵͳ�趨
	}
	else													//��������
	{
		//MODELSTOP;											//��ģ��
		//while(1)											//��ѭ��
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
   ���Ӽ��ټ�������ǰ����ֵ��Ŀ�������int
   ��ڲ�������ǰֵ�����ֵ����Сֵ������ֵ
**********************************************************/ 
void DataCHGint(INT16U *DataVal,INT16U DataMAX,INT16U DataMIN,INT16S Step)
{
	if((*DataVal) > DataMAX)								//����������ֵ�򲻲���	
	{
		*DataVal=DataMAX;
		return;
	}
	if((*DataVal) < DataMIN)								//�����С����Сֵ�򲻲���
	{
		*DataVal=DataMIN;
		return;
	}
	if(DataMAX < DataMIN)
	{
	 	return ;
	}
	if(Step > 0)						  					//����
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
	else							   						//��С
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
	ϵͳ���趨�������Ժ���չʹ��
***********************************************************/
void SystemSetting()
{
//	INT16U l_PWM;
//	if(gKeyValue == 0x04) gSysPWMchg++;
//	if(gKeyValue == 0x08) gSysPWMchg--;				   		//����С��0
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

						
