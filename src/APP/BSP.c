/******************************************************************************
*	���ļ���Ҫ����˳�ʼ���Ĺ���
*	GPIO����ʼ��������������������
*	ADC��ADת����ʼ������Ӧ��GPIO��ʼ��
*	IIC��IIC��ʼ������Ӧ��GPIO��ʼ��
*	RCC��ϵͳʱ�ӳ�ʼ������ȫʱ��
*	USART:���ڷ��ͺͽ��ճ�ʼ��
*	IWDG�����Ź���ʼ��
*	TIM����ʱ����ʼ��
*	NVIC���ж�������ʼ��
*	PWR���ɱ�̵�Դ�������ʼ��
***********************************************************/
#include "Global.h"	
		  

/**********************ϵͳ��������************************/
#define SYS_CORE_CLK			24*1000000					//ϵͳ�ں�ʱ��24M

#define I2C_SPEED				20*1000					    //I2C�ٶ�50K�����100K
#define IWDG_MAXTIME			1000						//���ÿ��Ź��ĸ�λʱ��3�룬���4.095S*8
/********************ϵͳ�������ý���*********************/




#define ADC1_DR_Address    ((u32)0x4001244C)  	 
#define AD_MAX_CH_NUM           1


INT16U AD_Value[AD_MAX_CH_NUM];	       						    //ADֵ	 
extern void GPIO_UART_Configuration_RS485EN(void);


/*********************************************************************************************************************
��������: 	GPIO_Configuration          
��������:   ��ͨI/O�ڳ�ʼ��
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
void GPIO_Configuration(void)
{
 	GPIO_InitTypeDef 				GPIO_InitStructure; 					//���� GPIO �Ľṹ
																			//������ʼ��
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6;				//����Ҫ���õ����� PA5,PA6--led1,led2
//	GPIO_InitStructure.GPIO_Mode 	=GPIO_Mode_Out_PP;                       // GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2Mʱ���ٶ�
//	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ������GPIOʹ������Ч
//
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14|GPIO_Pin_15;	            //����Ҫ���õ�����	
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//�������
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//10Mʱ���ٶ�
//	GPIO_Init(GPIOB, &GPIO_InitStructure);									//��ʼ������GPIOʹ������Ч

																			
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;							//����Ҫ���õ�����  //ERR3
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2Mʱ���ٶ�
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//									
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15;	  //����Ҫ���õ�����  ERR1,ERR2,ģʽѡ��24������
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2Mʱ���ٶ�
//	GPIO_Init(GPIOB, &GPIO_InitStructure);									//��ʼ������GPIOʹ������Ч

																			//�̵��� 
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;							//
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//�������
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2Mʱ���ٶ�
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11|GPIO_Pin_15;							//����Ҫ���õ�����
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//��ѹ��λ�л��̵���
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2Mʱ���ٶ�
//	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ������GPIOʹ������Ч
																			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_5 | GPIO_Pin_8;							// I2C WPʹ��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						//2Mʱ���ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;	// 4051 A,B,C
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2Mʱ���ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;	//LED1~LED8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;	//OUT1~OUT8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	SET_4051_A_H;	  //ѡ���4ͨ����Ϊ���Ӽ�⣬��PCB��˿ӡIN1
	SET_4051_B_H;
	SET_4051_C_L;

	LED1_OFF;	      //�ر�LED��ʾ
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED5_OFF;
	LED6_OFF;
	LED7_OFF;
	LED8_OFF;
	OUT4_OFF;         //�Ӵ�������
	OUT3_OFF ;
	OUT2_OFF ;
	OUT1_OFF ;

}

/*********************************************************************************************************************
��������: RCC_Configuration          
��������: ʱ�ӳ�ʼ��
�䡡��:   channel ͨ����   	*value AD����
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//ϵͳʱ�ӳ�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//��TIM2ʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);		//��I2Cʱ��
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);		//��I2Cʱ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	//��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	//��GPIODʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 	//��GPIOEʱ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//��ADCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 	//�򿪴���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 	//�򿪴���2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 	//�򿪴���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 	//�򿪴���4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 	//�򿪴���4ʱ��

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);       //��DMAʱ��

}

/*********************************************************************************************************************
��������: 	adc_read          
��������:   ��ADֵ
�䡡��:     channel ͨ����   	*value AD����
�䡡��:     ��	  	                 
*********************************************************************************************************************/
INT8U adc_read(INT8U channel, INT16U* value)
{ 	
//	INT16U ulValue[AD_MAX_CH_NUM];
//	char i;		

	if(channel<1 || channel>AD_MAX_CH_NUM)
		return 0;  

  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  					//����ADC1ת�� 

 	 while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));	 				// ��� DMA1 channel1 ת���Ƿ���ɱ�־  
  	 DMA_ClearFlag(DMA1_FLAG_TC1);	   							// ��� DMA1 channel1 ת����־ 
	
//  	for(i=0;i< AD_MAX_CH_NUM;i++)
//  	{
//  		ulValue[i]=AD_Value[i] * 330 / 4096 ;   	           //ת��Ϊ��ѹֵ
//  	}

  	value[0]= AD_Value[0];


	return 1;
}


/*********************************************************************************************************************
* Function Name : ADC1_Configuration
* Description    : ADC1�������
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************************/
void ADC1_Configuration(void)
{    
	GPIO_InitTypeDef 					GPIO_InitStructure;	   								//�����˿�����
	ADC_InitTypeDef  					ADC_InitStructure;									//����ADת������

	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_5;		//A/D�����
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AIN;												//ģ������
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;										//��������ģʽ
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;											//ɨ�跽ʽ
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;										//����ת��  ENABLE  ����ת�� DISENABLE
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;						//�ⲿ������ֹ
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;									//�����Ҷ���
  	ADC_InitStructure.ADC_NbrOfChannel = AD_MAX_CH_NUM;									    //����ת����ͨ���� AD_MAX_CH_NUM
  	ADC_Init(ADC1, &ADC_InitStructure);	 

   //ADC�����¶ȴ�����ʹ�ܣ�Ҫʹ��Ƭ���¶ȴ��������м�Ҫ��������
    //ADC_TempSensorVrefintCmd(ENABLE);    
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);   //����ת������1��ͨ��10    
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);  //����ת������2��ͨ��16���ڲ��¶ȴ�������������ʱ��>2.2us,(239cycles)

  	/* ADC1 regular channels configuration [����ģʽͨ������]*/ 
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15 , 1, ADC_SampleTime_239Cycles5);	 //ͨ��1
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_2 , 2, ADC_SampleTime_239Cycles5);	 //ͨ��2
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_3 , 3, ADC_SampleTime_239Cycles5); 	 //ͨ��3	
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 , 4, ADC_SampleTime_239Cycles5);  	 //ͨ��4
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_5 , 5, ADC_SampleTime_239Cycles5);    //ͨ��5
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5);
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5);
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5);  

  	
  	ADC_DMACmd(ADC1, ENABLE);	 													//ʹ��ADC1 DMA   	
  	ADC_Cmd(ADC1, ENABLE);   														//ʹ��ADC1	     	 
  	ADC_ResetCalibration(ADC1);		 												// Enable ADC1 reset calibaration register
  	
  	while(ADC_GetResetCalibrationStatus(ADC1));	   									// Check the end of ADC1 reset calibration register
  
  	ADC_StartCalibration(ADC1);	   													// Start ADC1 calibaration 	  	
  	while(ADC_GetCalibrationStatus(ADC1));	  										//* Check the end of ADC1 calibration */  
  	
  //	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  										//* Start ADC1 Software Conversion */  
}



/*********************************************************************************************************************
* Function Name : DMA_Configuration
* Description    : DMA���ã���ADCģ���Զ���ת��������ڴ�
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************************/
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure; 
  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;						//�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Value;								//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//dma���䷽����
  DMA_InitStructure.DMA_BufferSize = AD_MAX_CH_NUM;									//����DMA�ڴ���ʱ�������ĳ��� word
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//����DMA���ڴ����ģʽ��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//���������ֳ�
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//�ڴ������ֳ�
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//����DMA�Ĵ���ģʽ:ѭ��ģʽ����,Bufferд����,�Զ��ص���ʼ��ַ��ʼ����
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//����DMA�����ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//����DMA��2��memory�еı����������
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
} 


/*********************************************************************************************************************
��������: 	ADC_Configuration2          
��������:   AD����ģʽͨ������
�䡡��:     ��
�䡡��:     ��	  	                 
*********************************************************************************************************************/
INT8U ADC_Configuration2(void)
{ 	  
	 DMA_Configuration();
	 ADC1_Configuration();

	return 1;
}

/*********************************************************************************************************************
��������: ADC_Configuration          
��������: ADע��ģʽͨ������
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef 					GPIO_InitStructure;	   					//�����˿�����
	ADC_InitTypeDef  					ADC_InitStructure;						//����ADת������

	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		//A/D�����
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AIN;									//ģ������
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode          		= ADC_Mode_Independent;			//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode  		= ENABLE;						//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;						//������ת��
	ADC_InitStructure.ADC_ExternalTrigConv  	= ADC_ExternalTrigConv_None;	//ת������������������������
	ADC_InitStructure.ADC_DataAlign     		= ADC_DataAlign_Right;			//�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel  		= 4;							//ɨ��ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);	
  	ADC_InjectedSequencerLengthConfig(ADC1, 4);									//ע��ͨ������
  	ADC_InjectedChannelConfig(ADC1,ADC_Channel_1,3,ADC_SampleTime_55Cycles5);	//ͨ��1,����ʱ��Ϊ55.5����,1����ע��ͨ����1��
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_55Cycles5);	//ע��ͨ������
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5);	//ע��ͨ������
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_17,4,ADC_SampleTime_55Cycles5);	//ע��ͨ������
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1, 0);						//�趨ת��ͨ����ƫ����
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2, 0);						//�趨ת��ͨ����ƫ����
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3, 0);						//�趨ת��ͨ����ƫ����
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_4, 0);						//�趨ת��ͨ����ƫ����
  	
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);	//�ⲿ������������
	ADC_Cmd(ADC1, ENABLE);           											//ʹ��ADת����
  	ADC_ResetCalibration(ADC1);													//��λУ׼��
  	while(ADC_GetResetCalibrationStatus(ADC1));									//�ȴ���λ����
  	ADC_StartCalibration(ADC1);													//��ʼУ׼ADת����
  	while(ADC_GetCalibrationStatus(ADC1));										//�ȴ�ADУ׼���
}

/*********************************************************************************************************************
��������: PWM_Configuration          
��������: PWM��ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
INT16U	gSetVolDuty=50;
INT16U	gSetCurDuty=50;
void PWM_Configuration(void)
{
  	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; 								//����tim3��ͨ��2-PA7
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;//GPIO_Mode_AF_PP;							//�����������
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;							//10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_1; 					//����tim3��ͨ��3-PB0
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;							//�����������
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;							//10MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  	
	TIM_TimeBaseStructure.TIM_Period 		= PWM_PERIOD;	   					//Frequency = 2kHz
  	TIM_TimeBaseStructure.TIM_Prescaler 	= 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;			  		// PWM1 Mode 
  	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse 			= 6000;								//ռ�ձ�= (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
  	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
  	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse 			= 6000;
  	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM_ARRPreloadConfig(TIM3, ENABLE);						//ʹ���Զ���װ����
  	TIM_Cmd(TIM3, ENABLE);   								//TIM3ʹ��
}
/*********************************************************************************************************************
��������: TIM_Configuration          
��������: ��ʱ����ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  		TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period 			= 65535;					   	//���ֵ
  	TIM_TimeBaseStructure.TIM_Prescaler 		= SYS_CORE_CLK/1000;			//1ms
  	TIM_TimeBaseStructure.TIM_ClockDivision 	= 0;
  	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//����ȽϷ�����ģʽ
	TIM_OCInitStructure.TIM_Pulse 				= 5;  							//5ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//����Ϊ��
  	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//����ȽϷ�����ģʽ
	TIM_OCInitStructure.TIM_Pulse 				= 50;  						//1000ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//����Ϊ��
  	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//����ȽϷ�����ģʽ
	TIM_OCInitStructure.TIM_Pulse 				= 50;  						//1000ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//����Ϊ��
  	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);											//ʹ��Ԥװ��
  	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);										//TIM IT enable
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);									//TIM IT enable
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);									//TIM IT enable
	

  	TIM_Cmd(TIM2, ENABLE);   													//TIM2 enable counter
}

///*********************************************************************************************************************
//��������: UART_Configuration          
//��������: ���ڳ�ʼ��
//�䡡��:   ��
//�䡡��:   ��	  	                 
//*********************************************************************************************************************/
//void UART_Configuration(void)
//{
//    GPIO_InitTypeDef 	GPIO_InitStructure;
//    USART_InitTypeDef 	USART_InitStructure;
//		 
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9;							//���ͽӿڶ�Ӧ����
//    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//���սӿڶ�Ӧ����
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    USART_InitStructure.USART_BaudRate 	= USART_BPS;	 						//���ò�����
//	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//��������λ
//	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
//	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
//	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
//    USART_Init(USART1, &USART_InitStructure);
//
//    USART_Cmd(USART1, ENABLE);													//ʹ�ܴ���
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		   						//���ж�
///*
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//���ͽӿڶ�Ӧ����
//    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//�������-TX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3;							//���սӿڶ�Ӧ����
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//��������-RX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    USART_InitStructure.USART_BaudRate 	= USART_BPS;	 						//���ò�����
//	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//��������λ
//	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//����ֹͣλ
//	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������Э��
//	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//�����ͺͽ��չ���
//    USART_Init(USART2, &USART_InitStructure);
//
//    USART_Cmd(USART2, ENABLE);													//ʹ�ܴ���
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		   						//���ж�
//*/
//}

/*********************************************************************************************************************
��������: I2C_Configuration          
��������: IIC��ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 
														 
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_6 | GPIO_Pin_7;		   // PB10,11 SCL and SDA
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode 			= I2C_Mode_I2C;							//����I2Cģʽ 
    I2C_InitStructure.I2C_DutyCycle 	= I2C_DutyCycle_2; 						//I2C ����ģʽ Tlow / Thigh = 2 ��ֻ���ڸ���100K��������
    I2C_InitStructure.I2C_OwnAddress1 	= 0x30;	  								//���õ�һ���豸�����ַ
    I2C_InitStructure.I2C_Ack 			= I2C_Ack_Enable;	 					//ʹ��Ӧ��ACK��
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 	//Ӧ�� 7 λ��ַ 
    I2C_InitStructure.I2C_ClockSpeed 	= I2C_SPEED;							//100K�ٶ�
	I2C_Init(I2C1, &I2C_InitStructure);
    
	I2C_Cmd(I2C1, ENABLE);														//ʹ��I2C����

/*
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10 | GPIO_Pin_11;		   // PB10,11 SCL and SDA
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode 			= I2C_Mode_I2C;							//����I2Cģʽ 
    I2C_InitStructure.I2C_DutyCycle 	= I2C_DutyCycle_2; 						//I2C ����ģʽ Tlow / Thigh = 2 ��ֻ���ڸ���100K��������
    I2C_InitStructure.I2C_OwnAddress1 	= 0x30;	  								//���õ�һ���豸�����ַ
    I2C_InitStructure.I2C_Ack 			= I2C_Ack_Enable;	 					//ʹ��Ӧ��ACK��
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 	//Ӧ�� 7 λ��ַ 
    I2C_InitStructure.I2C_ClockSpeed 	= I2C_SPEED;							//100K�ٶ�
	I2C_Init(I2C2, &I2C_InitStructure);
    
	I2C_Cmd(I2C2, ENABLE);														//ʹ��I2C����
*/
}

/*********************************************************************************************************************
��������: NVIC_Configuration          
��������: �����жϳ�ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//���ȼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= UART5_IRQn;		//�򿪴����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);
	
/*	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn;		//�ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//�����ж�
	NVIC_Init(&NVIC_InitStructure);
*/												//��ʼ��

}

/*********************************************************************************************************************
��������: IWDG_Configuration          
��������: ���Ź���ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void IWDG_Configuration()
{
    if(WATCHDOG_EN)
	{
  		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	 		//ʹ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
  		IWDG_SetPrescaler(IWDG_Prescaler_32);	   				//IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz 
  		IWDG_SetReload(40*1000/32 * IWDG_MAXTIME/1000);			//���ֵ4095
  		IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);	 		//ʹ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
  		IWDG_ReloadCounter();									//��װ��ʼ���൱���幷
  		IWDG_Enable();	 										//ʹ�ܿ��Ź�,
	}
}

/*********************************************************************************************************************
��������: PWR_Configuration          
��������: ��Դ��س�ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void PWR_Configuration()
{
  	INT16U Temp;
	PWR_PVDCmd(ENABLE);										//ʹ�ܵ�Դ���
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9);					//
	for(Temp=0; Temp<65535; Temp++);						//�ȴ���Դ��ѹ����
	for(Temp=0; Temp<65535; Temp++);						//�ȴ���Դ��ѹ����
	for(Temp=0; Temp<100; Temp++)
	{	if(PWR_GetFlagStatus(PWR_FLAG_PVDO) == 1) Temp=0;	}
}
/*********************************************************************************************************************
��������: Periph_Configuration          
��������: ��Χ��ʼ��
�䡡��:   ��
�䡡��:   ��	  	                 
*********************************************************************************************************************/
void Periph_Configuration(void)
{
	RCC_Configuration();				//��ʼ��ϵͳ�͸����ߵ�ʱ��
	PWR_Configuration();				//��Դ��ѹ���
	IWDG_Configuration();				//�������Ź���ʼ��		
	GPIO_Configuration();				//IO�ڳ�ʼ��
	ADC_Configuration2();				//ADת����ʼ��
	//TIM_Configuration();				//ͨ�ö�ʱ����ʼ��
	NVIC_Configuration();				//�жϳ�ʼ��
	UART_Configuration();				//����1��ʼ��
	UART2_Configuration();				//����2��ʼ��
	UART3_Configuration();				//����3��ʼ��
	UART4_Configuration();				//����4��ʼ��
	UART5_Configuration();				//����5��ʼ��
	GPIO_UART_Configuration_RS485EN();  //485ʹ�ܶ˿ڳ�ʼ��
	I2C_Configuration();				//I2CͨѶ��ʼ��
	//PWM_Configuration();				//PWM/DA��ʼ��
	PWR_Configuration();				//��Դ��س�ʼ��
	//RTC_Configuration();				//��¼ģ�鹤���˶೤ʱ����������������δʹ��
//	CAN_Initial(CAN_REC_ALL);  		    //���ܽ���BMS���ݣ���Ϊ��ʼ�ͽ�������

 PWM_ON;
 PWM_H;
 PWM_L;
 PWM_H;
 PWM_L;
 

}
