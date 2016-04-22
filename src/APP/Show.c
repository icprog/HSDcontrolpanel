/**********************************************************
	���ļ�������������
	1-
	2-������ʾ��ʽ����ز���
	�ֶ�
		��ѹ��������ʾ����־0x01��LEDManaul����LEDV���������������ʾ
		��ѹ�����õ�ѹ����־0x02��LEDManaul����LEDV������ѹ�������˸
		������������ʾ����־0x03��LEDManaul����LEDC���������������ʾ
		���������õ�������־0x04��LEDManaul����LEDC���������������˸
	�Զ�
		��ѹ��������ʾ����־0x10��LEDAuto����LEDV���������������ʾ
		��ѹ�����õ�ѹ����־0x20��LEDAuto����LEDV������ѹ�������˸
		������������ʾ����־0x30��LEDAuto����LEDC���������������ʾ
		���������õ�������־0x40��LEDAuto����LEDC���������������˸
	��ѹֵ�Ŵ�10��������ֵ�Ŵ�100��
	PIN 02467135 ������	"U"-D6 	"n"98 "-"08 "F"8B "P"8F	"S"5B
	С���� |0x20 ��o��D8 
***********************************************************/
#include"Global.h"				   		//Ӧ�ó����ͷ�ļ�����

const INT8U gSegTable[11]={0xD7,0x14,0xCD,0x5D,0x1E,0x5B,0xDB,0x15,0xDF,0x5F,0x00}; //|0x20����ܣ�0-9��NULL
extern INT16U gSetVoltage;										//���õ�ѹֵ
extern INT16U gSetCurrent;										//���õ���ֵ
INT16U gOutVoltage=0;										//�����ѹֵ
INT16U gOutVoltage_100=0;									//�����ѹֵ�Ŵ�100��

INT16U gOutCurrent=0;										//�������ֵ
INT16U gSystemERR;											//���ֹ���
INT16U gExtVoltage=0;										//�ⲿ����ʵ�ʵ�ѹ���Ŵ�1000��
INT16U gExtSetVol=0;										//��Ӧ600V���趨��ѹֵ
INT16U gExtSetCur=0;										//��Ӧ5.00A�������ֵ
INT32U gModelWorkTime;										//ģ���Ѿ�������ʱ�䣬��λ1S
INT8U  gSystemLED=0;
INT16U gAdjExtVol=(10000-125); 								//�Ŵ�1000����AD����4000��Ӧ�ĵ�ѹֵ10.264
void SegClearVoltage(void);									//����������ʾ��ѹ
void SegClearCurrent(void);									//����������ʾ����
void SegShowData(INT16U SSData1,INT16U SSData2);			//�������ʾ��ѹ����
void SegShowErrCode(INT16U SSData1,INT16U SSData2);			//�������ʾ���ϴ���
void SystemErrShow(INT8U  *lRunT);							//���ж������ʱ������ʾ
void ShowSysArgExt(INT16U lExtVoltage);						//��ʾ�ⲿ������ѹ
void SegShowPending(INT8U );								//����������״̬

void alarmTest(INT16U *ptrErr);								//������
void GlideADC(INT16U *ptrVol,INT16U *ptrCur,INT16U *ptrExt);//ADת��������ѹ
INT16U ComputeVol(INT16U lVolValue);						//������ʾ��ѹ
INT16U ComputeCur(INT16U lCurValue);						//������ʾ����
INT16U ComputeExtVol(INT16U lVolValue);						//�����ⲿ������ѹ

/******************************************************
*	��ȡ����int16U���ľ���ֵ
******************************************************/
INT16U abs_int16u(INT16U data1,INT16U data2)
{
	if(data1 >= data2) 
	{	return 	(data1 - data2);}
	else
	{	return 	(data2 - data1);}
}
 


/******************************************************
*	��ʾģ���Ѿ�����ʱ��
******************************************************/
void ShowSysArgTime(INT16U gExtVoltage)
{

}

/******************************************************
*	��ʾģ�����к�1
*	-Sn 201005 0001- ��ʾ2010��5��0001��
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
*	��ʾCPU�¶ȣ��Ŵ�10������ʵ��ֵ�ϼ���100��
*	eg��1238 --��23.8��
*		952	 --��-4.8��
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
	gSegShowArr[0] = 0xC3;			//"C"��

}

/******************************************************
*	��ʾ����汾��
******************************************************/
void ShowSysArgSoftVersion(INT16U l_Dat)
{	
	gSegShowArr[7]=0x5B;							 //SOFT
	gSegShowArr[6]=0xD8;
	gSegShowArr[5]=0x8B;		
	gSegShowArr[4]=0x83;
	gSegShowArr[3]=gSegTable[10];					//V01.00
	gSegShowArr[2]=gSegTable[(l_Dat%1000)/100] | 0x20;				//��ʾС����
	gSegShowArr[1]=gSegTable[(l_Dat%100)/10];
	gSegShowArr[0]=gSegTable[l_Dat%10/1];
}

/******************************************************
*	��ʾ�����ѹ������PWM��
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
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//��ʾС����
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
	else
	{
		l_chg = 100 - l_chg;
		gSegShowArr[3]=0x08;							//"-"
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//��ʾС����
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
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//��ʾС����
		gSegShowArr[1]=gSegTable[(l_chg%100)/10];
		gSegShowArr[0]=gSegTable[(l_chg%10)/1];
	}
	else
	{
		l_chg = 100 - l_chg;
		l_chg *= 5;
		gSegShowArr[3]=0x08;							//"-"
		gSegShowArr[2]=gSegTable[l_chg/100] | 0x20;			//��ʾС����
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
*	ϵͳ������ʾ��������
*	1-����汾�ţ�2-�������ڣ�3-�ⲿ��ѹ��4-��ǰ�¶�
******************************************************/
void SystemArgumentShow(INT8U lShowSeq)
{
	if(lShowSeq == 1) ShowSysArgSoftVersion(101);			//��ʾ����汾��1.01
	if(lShowSeq == 2) ShowSysArgSeqence(1);					//��ʾ�������ڻ����
	if(lShowSeq == 3) ShowSysArgExt(gExtVoltage);			//��ʾ�ⲿ������ѹ
	if(lShowSeq == 4) ShowSysArgTemper(25+50);				//��ʾ��ǰ�¶�25
	if((lShowSeq >= 5) && (lShowSeq <= 25))
	{	ShowSysArgOUT2VTO99V(lShowSeq-5);	}
	if(lShowSeq == 26) ShowSysArgSaveASK();
	//save dat														


}

/***********************************************************
 	��ȡģ���״̬�������ػ�
	0-�ػ�
	1-����
***********************************************************/
INT8U GetModelStation()
{
 	return !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
}															//�ⲿ�����Ƿ���������ϵͳ����Ƶ��
/**********************************************************
	�������ʾ��LED��ʾ�����ȼ�5,100ms
	ADת���������⣬�忴�Ź�
	gSegShowStatus
		0x01����ѹ��led�����������ʾ��ѹ����
		0x02����ѹ���õ�ѹ��led������ѹ�������
		0x03����ѹ���õ�����led���������������

		0x10��������led�����������ʾ��ѹ����
		0x02���������õ�����led���������������
		0x03���������õ�ѹ��led������ѹ�������

		0xFE:ϵͳ����
***********************************************************/
extern unsigned short lVolValue,lCurValue,lExtValue;			//��ѹ��������ֵ
void task_show(void *pdata)
{
	unsigned char  lRunTimes=0;								//�������еĴ���
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
	alarmTest(&gSystemERR);									//������
	if(gSystemERR & 0xFFFF)									//���β��Ǳ���ػ��Ĺ���
	{
		MODULESTOP;
		TIM_SetCompare3(TIM3, 12000);						//��PWMΪ0
		TIM_SetCompare4(TIM3, 12000);						//��PWMΪ0
		if(gSystemERR & 0x03FF)								//���β���Ҫ��ʾ�Ĺ��ϻ��־λ
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
			if((GetVolMode() == 1)) 						//�����ѹģʽѡ���ѹ��  &&(gSetVoltage>=1000)
			{											 	//�����趨��ѹ>=100V,��ʵ�������û�б�Ҫ
				VOLMODE_RLY1H;							 	//ͬʱ��֤����ʱ��ѡ���ѹ��
				VOLMODE_RLY2H;
			}
			else
			{					   							//��ѹģʽѡ���ѹ����
				VOLMODE_RLY1L;
				VOLMODE_RLY2L;
			}

			if(GetModelStation() == 0)						//����ǹػ�״̬ �������������̣��ȴ�600ms
			{
				unsigned char Temp_FOR;
				MODULERUN;
				for(Temp_FOR=0; Temp_FOR<14; Temp_FOR++)
				{
					TIM_SetCompare3(TIM3, 12000);			//��PWMΪ0
					TIM_SetCompare4(TIM3, 12000);			//��PWMΪ0
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
	if(((gSystemERR & 0x0C00)>0) && (1 == gUserDB.uartCommAlarm))	//��ͨѶ����������ǿ�Ʊ�ʶʱ
	{
		if((gSystemERR & 0x0800)>0)  						//���ǿ��ѡ���ѹ�� 
		{											 
			VOLMODE_RLY1H;							 
			VOLMODE_RLY2H;
		}
		else
		{					   								//��ѹģʽѡ���ѹ����
			VOLMODE_RLY1L;
			VOLMODE_RLY2L;
		}		
	}
	else
	{
		//if(gSegShowStatus > 0x44) gSegShowStatus -= 0x44;	//�˳�������ʾ״̬
	}

	/******************************************************///���ݲ���ֵ������ʾ��ѹֵ
	if((++lADCTimes) >= 5)									//ÿ400ms����һ��
	{
		ComputeVol(lVolValue);								//������ʾ��ѹֵ
		ComputeCur(lCurValue);								//������ʾ����ֵ
		ComputeExtVol(lExtValue);							//����ɼ�����ʵ�ʵ�ѹֵ����Ӧ���趨��ѹ����ֵ
		lADCTimes = 0;
	}
	/******************************************************///��ͬ����ʾ״̬
	if(gSegShowStatus == 0x00)								//��ʼ��״̬ϵͳ������Ŀǰδʹ��
	{
	   	lRunTimes ++;
		if(lRunTimes>=41) lRunTimes=0; 
		if(lRunTimes<21) SegShowPending(lRunTimes /3 );
		if(lRunTimes>=21) SegShowPending((41-lRunTimes) /3 );
	}	
	else if(gSegShowStatus == 0x01)						   	//
	{
		LED_MANUALON;										//�ֶ�ģʽָʾ����
		LED_AUTOOFF;										//�Զ�ģʽָʾ����
		LED_VOLTAGEON;										//��ѹָʾ����
		LED_CURRENTOFF;										//����ָʾ����
		SegShowData_2(gOutVoltage_100,gOutCurrent);				//��ʾ�����ѹ����		
	}
	else if(gSegShowStatus == 0x02)
	{
		SegShowData(gSetVoltage,gSetCurrent);			//��ʾ���õ�ѹ���������

		lRunTimes ++;
		if(lRunTimes>=10)
		{
			lRunTimes=0;
			LED_VOLTAGEOFF;									//��ѹLED��
		} 
		else if(lRunTimes == 6)								//��ѹ����ܺ�LED��˸
		{
			//SegClearVoltage();							//��ѹ�������
			LED_VOLTAGEON;									//��ѹLED��
		}

	}
	else if(gSegShowStatus == 0x03)
	{
		LED_MANUALON;										//�ֶ�ģʽָʾ����
		LED_CURRENTON;										//�������ָʾ����
		LED_VOLTAGEOFF;
		SegShowData(gOutVoltage,gOutCurrent);				//��ʾ�����ѹ����
	}
	else if(gSegShowStatus == 0x04)
	{
		SegShowData(gSetVoltage,gSetCurrent);				//��ʾ���õ�ѹ���������
		lRunTimes ++;
		if(lRunTimes>=10) 
		{
			lRunTimes=0;
			LED_CURRENTON;
		} 
		else if(lRunTimes == 6)									//�õ�����ʾ�γ���˸��Ч��
		{
			//SegClearCurrent();
			LED_CURRENTOFF;
		}
	}
	else if(gSegShowStatus == 0x10)
	{
		LED_AUTOON;											//�Զ�ģʽָʾ����
		LED_VOLTAGEON;										//��ѹ���ָʾ����
		LED_CURRENTOFF;
//		lRunTimes ++;
//		if(lRunTimes>=10) 								   	//ÿ1S����һ�ε�ѹ����
//		{
//			lRunTimes=0;
//			PWM_SetVoltage(gExtSetVol);						//������ѹPWM
//		} 		
		PWM_SetCurrent(gSetCurrent);						//��������PWM
		SegShowData(gOutVoltage,gOutCurrent);				//��ʾ�����ѹ����
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
		//���õ�ѹ
	}
	else if(gSegShowStatus == 0x30)
	{
		LED_AUTOON;											//�Զ�ģʽָʾ����
		LED_CURRENTON;										//�������ָʾ����
		LED_VOLTAGEOFF;
		PWM_SetCurrent(gExtSetCur);							//��������PWM
		PWM_SetVoltage(gSetVoltage);						//������ѹPWM
		SegShowData(gOutVoltage,gOutCurrent);				//��ʾ�����ѹ����
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
	else if((gSegShowStatus &0x08) ==0x08)					//������ʾ
	{
		SystemErrShow(&lRunTimes);
	}

	else if((gSegShowStatus &0x88) ==0x80)					//�޸�ϵͳ��������������
	{
		SystemArgumentShow(gSegShowElse);
	}
	else 											 		//�������д���
	{
	}
}
 
}

/*************************************************************************************************/
/*	�Ӻ���																						 */
/*************************************************************************************************/

/**********************************************************
	����������ʾ��ѹ
***********************************************************/ 
void SegClearVoltage()
{
	gSegShowArr[4]=gSegTable[10];
	gSegShowArr[5]=gSegTable[10];
	gSegShowArr[6]=gSegTable[10];
	gSegShowArr[7]=gSegTable[10];
}

/**********************************************************
	����������ʾ����
***********************************************************/
void SegClearCurrent()
{
	gSegShowArr[0]=gSegTable[10];
	gSegShowArr[1]=gSegTable[10];
	gSegShowArr[2]=gSegTable[10];
	gSegShowArr[3]=gSegTable[10];
}

/**********************************************************
*	�������ʾ��ѹ����
***********************************************************/
void SegShowData(INT16U SSData1,INT16U SSData2)
{
	SSData1 %= 10000;										//��ֹ���ݳ���
	SSData2 %= 1000;										//��ֹ���ݳ���
	if(SSData1 < 3)	SSData1 =0;								//0.3V
	if(SSData2 < 5)	SSData2 =0;								//0.05A
	gSegShowArr[7]=gSegTable[SSData1/1000];
	gSegShowArr[6]=gSegTable[(SSData1%1000)/100];
	gSegShowArr[5]=gSegTable[(SSData1%100)/10] | 0x20;		//��ʾС����
	gSegShowArr[4]=gSegTable[(SSData1%10)/1];

	gSegShowArr[3]=gSegTable[0];							//"0"
	gSegShowArr[2]=gSegTable[SSData2/100] | 0x20;			//��ʾС����
	gSegShowArr[1]=gSegTable[(SSData2%100)/10];
	gSegShowArr[0]=gSegTable[(SSData2%10)/1];

}
void SegShowData_2(INT16U SSData1,INT16U SSData2)
{
	SSData2 %= 1000;										//��ֹ���ݳ���
	if(SSData1 < 30)	SSData1 =0;								//0.3V
	if(SSData2 < 5)	SSData2 =0;								//0.05A
	if(SSData1 >=10000)
	{	
		SSData1 /=10;
		gSegShowArr[7]=gSegTable[SSData1/1000];
		gSegShowArr[6]=gSegTable[(SSData1%1000)/100];
		gSegShowArr[5]=gSegTable[(SSData1%100)/10] | 0x20;		//��ʾС����
		gSegShowArr[4]=gSegTable[(SSData1%10)/1];
	}
	else
	{	gSegShowArr[7]=gSegTable[SSData1/1000];
		gSegShowArr[6]=gSegTable[(SSData1%1000)/100]| 0x20;
		gSegShowArr[5]=gSegTable[(SSData1%100)/10] ;		//��ʾС����
		gSegShowArr[4]=gSegTable[(SSData1%10)/1];
		
	}
	gSegShowArr[3]=gSegTable[0];							//"0"
	gSegShowArr[2]=gSegTable[SSData2/100] | 0x20;			//��ʾС����
	gSegShowArr[1]=gSegTable[(SSData2%100)/10];
	gSegShowArr[0]=gSegTable[(SSData2%10)/1];

}

/**********************************************************
*	�������ʾ�������
***********************************************************/
void SegShowErrCode(INT16U SSData1,INT16U SSData2)
{
	SSData1 %= 10000;										//��ֹ���ݳ���
	SSData2 %= 100;											//��ֹ���ݳ���
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
*	��ʾ������״̬
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
*	���ݲ��������ѹֵ
*	���㷽����������˵����
*	Ϊ��֤��ѹ���ȣ�����ֵ�Ŵ���10��
******************************************************/
INT16U ComputeVol(INT16U lVolValue)
{
	unsigned int   tmp32=0;
	if(lVolValue < 8000)
	{
		tmp32= lVolValue - 0;
		if(gAdjOutVol[0] >= gAdjOutVol[1]) gAdjOutVol[1] = gAdjOutVol[0]+1;	  //��ֹ����
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
		if(gAdjOutVol[2] >= gAdjOutVol[3]) gAdjOutVol[3] = gAdjOutVol[2]+1;	  //��ֹ����
		tmp32= (tmp32 * (gAdjOutVol[3]-gAdjOutVol[2]))/(2400-1600)+gAdjOutVol[2]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else if(lVolValue < 32000)
	{
		tmp32= lVolValue - 24000;
		if(gAdjOutVol[3] >= gAdjOutVol[4]) gAdjOutVol[4] = gAdjOutVol[3]+1;	  //��ֹ����
		tmp32= (tmp32 * (gAdjOutVol[4]-gAdjOutVol[3]))/(3200-2400)+gAdjOutVol[3]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	else //if(lVolValue >= gAdjOutVol[4])
	{
		tmp32= lVolValue - 32000;
		if(gAdjOutVol[4] >= gAdjOutVol[5]) gAdjOutVol[5] = gAdjOutVol[4]+1;	  //��ֹ����
		tmp32= (tmp32 * (gAdjOutVol[5]-gAdjOutVol[4]))/(4000-3200)+gAdjOutVol[4]*10;
		gOutVoltage = (unsigned short ) tmp32;
	}
	gOutVoltage_100 = gOutVoltage;
	gOutVoltage	/= 10;
	return gOutVoltage;
}

/******************************************************
*	�����ⲿ������ѹֵ  �궨��Ϊ����ֵ4000��Ӧ�ĵ�ѹֵ	10000,10.000V
*	���ĺ궨����Ը��Ĳ�������ĵ�ѹֵ���Ժ�ĳɿɽ���
*	���ⲿ������ѹ����Ƚ�����ʱ������1V�ļ�ϵ㣬
*	������Ч�ı���ģ�鲻�����ڸ�ѹ��ѹ��֮���л�
*	��10V��Ӧ�Ĳ���ֵ40000
******************************************************/
#define EXTVOL_101V		101*100/6	//101*10000/600	  	//�趨101V�Ƕ�Ӧ���ⲿ������ѹֵ
#define EXTVOL_100V		100*100/6
#define EXTVOL_99V		99*100/6
INT16U ComputeExtVol(INT16U lVolValue)
{
	INT32U   tmp32=0;
	static INT16U lExtSetVol;
	static INT16U lExtSetCur;
	//static INT16U lVol[10];
	tmp32 = lVolValue;										//
	tmp32 =	tmp32*gAdjExtVol/4000/10;							//����10V��Ӧ����ֵ4000,
	gExtVoltage = (INT16U)tmp32;							//ϵͳ��⵽���ⲿ��ѹʵ��
	if(tmp32 > 10000) tmp32 = 10000;						//��ֹ�趨�������ֵ

	if((gSegShowStatus == 0x10) ||(gSegShowStatus == 0x20))	//��������ⲿ������ѹģʽ
	{
	 	if(GetVolMode() == 1)								//���ڸ�ѹģʽ
		{
			if(gExtVoltage >= EXTVOL_100V)					//����100V��һֱ�Ǹ�ѹģʽ
			{
			 	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
			else if(gExtVoltage >= EXTVOL_99V)				//����99V��һֱ�Ǹ�ѹģʽ
			{
			 	gExtSetVol = 1000;							//�趨Ϊ100.0V
			}
			else
			{												//С��99V���л�����ѹ��
			   	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
		}
		else
		{
			if(gExtVoltage < EXTVOL_100V)					//С��100V��һֱ�ǵ�ѹģʽ��99.9V
			{
			 	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
			else if(gExtVoltage <= EXTVOL_101V)				//С��101V��һֱ�ǵ�ѹģʽ
			{
			 	gExtSetVol = 999;							//�趨Ϊ99.9V
			}
			else
			{												//>=100VV���л�����ѹ��
			   	gExtSetVol = (INT16U)(tmp32*6/10);	
			}
		}
	}
	else
	{												
		gExtSetVol = (INT16U)(tmp32*6/10);						//0-0.0V 10.000-600.0V
	}							
	gExtSetCur = (INT16U)(tmp32/20);						//�����Ӧ600V���趨ֵ *500/10000

	if(gExtSetVol > MAXSETVOL)	gExtSetVol = MAXSETVOL;		//��ѹ����ֵ����
	if(gExtSetVol < MINSETVOL)	gExtSetVol = MINSETVOL;
	if(gExtSetCur > MAXSETCUR)	gExtSetCur = MAXSETCUR;
	if(gExtSetCur < MINSETCUR)	gExtSetCur = MINSETCUR;

	if(lExtSetVol  > gExtSetVol) 		//�����ε�ѹС���ϴε�ѹ
	{
		if(lExtSetVol - gExtSetVol <= 2)//�����ε�ѹ����0.2V֮�ڣ��򱣳��ϴε�ֵ����
		{
			gExtSetVol = lExtSetVol;
		}
	}
	if(lExtSetVol  < gExtSetVol)		//�����ε�ѹ�����ϴε�ѹ
	{
		if(gExtSetVol - lExtSetVol <= 2)//�����ε�ѹ����0.2V֮�ڣ��򱣳��ϴε�ֵ����
		{
			gExtSetVol = lExtSetVol;
		}
	}
	if(lExtSetCur  > gExtSetCur)		//�����ε���С���ϴε���
	{
		if(lExtSetCur - gExtSetCur <= 10)//�����ε�������0.1A֮�ڣ��򱣳��ϴε�ֵ����
		{
			gExtSetCur = lExtSetCur;
		}
	}
	if(lExtSetCur  < gExtSetCur)		 //�����ε��������ϴε���
	{
		if(gExtSetCur - lExtSetCur <= 10)//�����ε�������0.1A֮�ڣ��򱣳��ϴε�ֵ����
		{
			gExtSetCur = lExtSetCur;
		}
	}


	return 0;
}

/******************************************************
*	���ݲ����������ֵ
******************************************************/
INT16U ComputeCur(INT16U lCurValue)
{
	unsigned int   tmp32=0;
	if(lCurValue < 800)
	{
		tmp32= lCurValue - 0;
		if(gAdjOutCur[0] >= gAdjOutCur[1]) gAdjOutCur[1] = gAdjOutCur[0]+1;	  //��ֹ����
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
*	���д���ʱ��ʾ������ܱ仯
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
*	��ʾ�ⲿ������ѹ
*	��ڲ����Ŵ�1000��
*	eg��5632 --��5.632
*		10568--��10.56	δ��������
******************************************************/
void ShowSysArgExt(INT16U lExtVoltage)
{
	gSegShowArr[7] = gSegTable[1];							//"1"							
	gSegShowArr[6] = gSegTable[10];							//NULL		
	gSegShowArr[5] = gSegTable[10];							//NULL	
	gSegShowArr[4] = gSegTable[10];							//NULL
	if(lExtVoltage>=10000)									//if value>10000 С���������λ
	{
		lExtVoltage /= 10;
		gSegShowArr[3] = gSegTable[lExtVoltage/1000];		//��ʾʮλ		
		gSegShowArr[2] = gSegTable[(lExtVoltage%1000)/100]|0x20;//��ʾ��λ��С����	
		gSegShowArr[1] = gSegTable[(lExtVoltage%100)/10];	//��ʾʮ��λ
		gSegShowArr[0] = gSegTable[lExtVoltage%10];			//��ʾ�ٷ�λ
	}
	else
	{
		gSegShowArr[3] = gSegTable[lExtVoltage/1000]|0x20;	//��ʾС����			
		gSegShowArr[2] = gSegTable[(lExtVoltage%1000)/100];	//��ʾʮ��λ
		gSegShowArr[1] = gSegTable[(lExtVoltage%100)/10];	//��ʾ�ٷ�λ
		gSegShowArr[0] = gSegTable[lExtVoltage%10];			//��ʾǧ��λ
	}
}
