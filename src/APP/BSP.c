/******************************************************************************
*	本文件主要完成了初始化的工作
*	GPIO：初始化按键，故障输入引脚
*	ADC：AD转换初始化及相应的GPIO初始化
*	IIC：IIC初始化及相应的GPIO初始化
*	RCC：系统时钟初始化、安全时钟
*	USART:串口发送和接收初始化
*	IWDG：看门狗初始化
*	TIM：定时器初始化
*	NVIC：中断向量初始化
*	PWR：可编程电源检测器初始化
***********************************************************/
#include "Global.h"	
		  

/**********************系统参数设置************************/
#define SYS_CORE_CLK			24*1000000					//系统内核时钟24M

#define I2C_SPEED				20*1000					    //I2C速度50K，最大100K
#define IWDG_MAXTIME			1000						//设置看门狗的复位时间3秒，最大4.095S*8
/********************系统参数设置结束*********************/




#define ADC1_DR_Address    ((u32)0x4001244C)  	 
#define AD_MAX_CH_NUM           1


INT16U AD_Value[AD_MAX_CH_NUM];	       						    //AD值	 
extern void GPIO_UART_Configuration_RS485EN(void);


/*********************************************************************************************************************
函数名称: 	GPIO_Configuration          
功能描述:   普通I/O口初始化
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
void GPIO_Configuration(void)
{
 	GPIO_InitTypeDef 				GPIO_InitStructure; 					//声明 GPIO 的结构
																			//按键初始化
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6;				//声明要设置的引脚 PA5,PA6--led1,led2
//	GPIO_InitStructure.GPIO_Mode 	=GPIO_Mode_Out_PP;                       // GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2M时钟速度
//	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化外设GPIO使设置生效
//
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14|GPIO_Pin_15;	            //声明要设置的引脚	
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//推挽输出
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//10M时钟速度
//	GPIO_Init(GPIOB, &GPIO_InitStructure);									//初始化外设GPIO使设置生效

																			
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;							//声明要设置的引脚  //ERR3
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2M时钟速度
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//									
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15;	  //声明要设置的引脚  ERR1,ERR2,模式选择，24输入检测
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;				//
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2M时钟速度
//	GPIO_Init(GPIOB, &GPIO_InitStructure);									//初始化外设GPIO使设置生效

																			//继电器 
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;							//
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//推挽输出
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2M时钟速度
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11|GPIO_Pin_15;							//声明要设置的引脚
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//电压档位切换继电器
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;						//2M时钟速度
//	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化外设GPIO使设置生效
																			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_5 | GPIO_Pin_8;							// I2C WP使能
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						//2M时钟速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;	// 4051 A,B,C
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2M时钟速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;	//LED1~LED8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;	//OUT1~OUT8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;						    //推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz ;						    //2M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	SET_4051_A_H;	  //选择第4通道作为连接检测，即PCB板丝印IN1
	SET_4051_B_H;
	SET_4051_C_L;

	LED1_OFF;	      //关闭LED显示
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED5_OFF;
	LED6_OFF;
	LED7_OFF;
	LED8_OFF;
	OUT4_OFF;         //接触器吸合
	OUT3_OFF ;
	OUT2_OFF ;
	OUT1_OFF ;

}

/*********************************************************************************************************************
函数名称: RCC_Configuration          
功能描述: 时钟初始化
输　入:   channel 通道号   	*value AD缓冲
输　出:   无	  	                 
*********************************************************************************************************************/
void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//系统时钟初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//打开TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//打开TIM2时钟

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);		//打开I2C时钟
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);		//打开I2C时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//打开GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	//打开GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	//打开GPIOD时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 	//打开GPIOE时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//打开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 	//打开串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 	//打开串口2时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 	//打开串口3时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 	//打开串口4时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 	//打开串口4时钟

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);       //打开DMA时钟

}

/*********************************************************************************************************************
函数名称: 	adc_read          
功能描述:   读AD值
输　入:     channel 通道号   	*value AD缓冲
输　出:     无	  	                 
*********************************************************************************************************************/
INT8U adc_read(INT8U channel, INT16U* value)
{ 	
//	INT16U ulValue[AD_MAX_CH_NUM];
//	char i;		

	if(channel<1 || channel>AD_MAX_CH_NUM)
		return 0;  

  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  					//启动ADC1转换 

 	 while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));	 				// 检测 DMA1 channel1 转换是否完成标志  
  	 DMA_ClearFlag(DMA1_FLAG_TC1);	   							// 清除 DMA1 channel1 转换标志 
	
//  	for(i=0;i< AD_MAX_CH_NUM;i++)
//  	{
//  		ulValue[i]=AD_Value[i] * 330 / 4096 ;   	           //转换为电压值
//  	}

  	value[0]= AD_Value[0];


	return 1;
}


/*********************************************************************************************************************
* Function Name : ADC1_Configuration
* Description    : ADC1相关配置
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************************/
void ADC1_Configuration(void)
{    
	GPIO_InitTypeDef 					GPIO_InitStructure;	   								//声明端口设置
	ADC_InitTypeDef  					ADC_InitStructure;									//声明AD转换设置

	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_5;		//A/D输入端
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AIN;												//模拟输入
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;										//独立工作模式
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;											//扫描方式
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;										//连续转换  ENABLE  单次转换 DISENABLE
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;						//外部触发禁止
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;									//数据右对齐
  	ADC_InitStructure.ADC_NbrOfChannel = AD_MAX_CH_NUM;									    //用于转换的通道数 AD_MAX_CH_NUM
  	ADC_Init(ADC1, &ADC_InitStructure);	 

   //ADC内置温度传感器使能（要使用片内温度传感器，切忌要开启它）
    //ADC_TempSensorVrefintCmd(ENABLE);    
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);   //常规转换序列1：通道10    
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);  //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)

  	/* ADC1 regular channels configuration [规则模式通道配置]*/ 
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15 , 1, ADC_SampleTime_239Cycles5);	 //通道1
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_2 , 2, ADC_SampleTime_239Cycles5);	 //通道2
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_3 , 3, ADC_SampleTime_239Cycles5); 	 //通道3	
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 , 4, ADC_SampleTime_239Cycles5);  	 //通道4
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_5 , 5, ADC_SampleTime_239Cycles5);    //通道5
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5);
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5);
//  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5);  

  	
  	ADC_DMACmd(ADC1, ENABLE);	 													//使能ADC1 DMA   	
  	ADC_Cmd(ADC1, ENABLE);   														//使能ADC1	     	 
  	ADC_ResetCalibration(ADC1);		 												// Enable ADC1 reset calibaration register
  	
  	while(ADC_GetResetCalibrationStatus(ADC1));	   									// Check the end of ADC1 reset calibration register
  
  	ADC_StartCalibration(ADC1);	   													// Start ADC1 calibaration 	  	
  	while(ADC_GetCalibrationStatus(ADC1));	  										//* Check the end of ADC1 calibration */  
  	
  //	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  										//* Start ADC1 Software Conversion */  
}



/*********************************************************************************************************************
* Function Name : DMA_Configuration
* Description    : DMA设置：从ADC模块自动读转换结果至内存
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************************/
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure; 
  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;						//外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Value;								//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//dma传输方向单向
  DMA_InitStructure.DMA_BufferSize = AD_MAX_CH_NUM;									//设置DMA在传输时缓冲区的长度 word
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//设置DMA的外设递增模式，一个外设
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//设置DMA的内存递增模式，
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//外设数据字长
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//内存数据字长
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//设置DMA的传输模式:循环模式开启,Buffer写满后,自动回到初始地址开始传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//设置DMA的优先级别
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//设置DMA的2个memory中的变量互相访问
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
} 


/*********************************************************************************************************************
函数名称: 	ADC_Configuration2          
功能描述:   AD规则模式通道配置
输　入:     无
输　出:     无	  	                 
*********************************************************************************************************************/
INT8U ADC_Configuration2(void)
{ 	  
	 DMA_Configuration();
	 ADC1_Configuration();

	return 1;
}

/*********************************************************************************************************************
函数名称: ADC_Configuration          
功能描述: AD注入模式通道配置
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef 					GPIO_InitStructure;	   					//声明端口设置
	ADC_InitTypeDef  					ADC_InitStructure;						//声明AD转换设置

	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		//A/D输入端
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AIN;									//模拟输入
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode          		= ADC_Mode_Independent;			//独立模式
	ADC_InitStructure.ADC_ScanConvMode  		= ENABLE;						//多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;						//不连续转换
	ADC_InitStructure.ADC_ExternalTrigConv  	= ADC_ExternalTrigConv_None;	//转换不受外设决定，有软件触发
	ADC_InitStructure.ADC_DataAlign     		= ADC_DataAlign_Right;			//右对齐
	ADC_InitStructure.ADC_NbrOfChannel  		= 4;							//扫描通道数
	ADC_Init(ADC1, &ADC_InitStructure);	
  	ADC_InjectedSequencerLengthConfig(ADC1, 4);									//注入通道个数
  	ADC_InjectedChannelConfig(ADC1,ADC_Channel_1,3,ADC_SampleTime_55Cycles5);	//通道1,采用时间为55.5周期,1代表注入通道第1个
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_55Cycles5);	//注入通道设置
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5);	//注入通道设置
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_17,4,ADC_SampleTime_55Cycles5);	//注入通道设置
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1, 0);						//设定转换通道的偏移量
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2, 0);						//设定转换通道的偏移量
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3, 0);						//设定转换通道的偏移量
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_4, 0);						//设定转换通道的偏移量
  	
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);	//外部触发条件设置
	ADC_Cmd(ADC1, ENABLE);           											//使能AD转换器
  	ADC_ResetCalibration(ADC1);													//复位校准器
  	while(ADC_GetResetCalibrationStatus(ADC1));									//等待复位结束
  	ADC_StartCalibration(ADC1);													//开始校准AD转换器
  	while(ADC_GetCalibrationStatus(ADC1));										//等待AD校准完毕
}

/*********************************************************************************************************************
函数名称: PWM_Configuration          
功能描述: PWM初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
INT16U	gSetVolDuty=50;
INT16U	gSetCurDuty=50;
void PWM_Configuration(void)
{
  	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; 								//定义tim3的通道2-PA7
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;//GPIO_Mode_AF_PP;							//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;							//10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 |GPIO_Pin_1; 					//定义tim3的通道3-PB0
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;							//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;							//10MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  	
	TIM_TimeBaseStructure.TIM_Period 		= PWM_PERIOD;	   					//Frequency = 2kHz
  	TIM_TimeBaseStructure.TIM_Prescaler 	= 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;			  		// PWM1 Mode 
  	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse 			= 6000;								//占空比= (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
  	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
  	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse 			= 6000;
  	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM_ARRPreloadConfig(TIM3, ENABLE);						//使能自动重装功能
  	TIM_Cmd(TIM3, ENABLE);   								//TIM3使能
}
/*********************************************************************************************************************
函数名称: TIM_Configuration          
功能描述: 定时器初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  		TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period 			= 65535;					   	//最大值
  	TIM_TimeBaseStructure.TIM_Prescaler 		= SYS_CORE_CLK/1000;			//1ms
  	TIM_TimeBaseStructure.TIM_ClockDivision 	= 0;
  	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//输出比较非主动模式
	TIM_OCInitStructure.TIM_Pulse 				= 5;  							//5ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//极性为正
  	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//输出比较非主动模式
	TIM_OCInitStructure.TIM_Pulse 				= 50;  						//1000ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//极性为正
  	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Active;    		//输出比较非主动模式
	TIM_OCInitStructure.TIM_Pulse 				= 50;  						//1000ms
	TIM_OCInitStructure.TIM_OCPolarity 			= TIM_OCPolarity_High;			//极性为正
  	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);											//使能预装载
  	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);										//TIM IT enable
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);									//TIM IT enable
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);									//TIM IT enable
	

  	TIM_Cmd(TIM2, ENABLE);   													//TIM2 enable counter
}

///*********************************************************************************************************************
//函数名称: UART_Configuration          
//功能描述: 串口初始化
//输　入:   无
//输　出:   无	  	                 
//*********************************************************************************************************************/
//void UART_Configuration(void)
//{
//    GPIO_InitTypeDef 	GPIO_InitStructure;
//    USART_InitTypeDef 	USART_InitStructure;
//		 
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_9;							//发送接口对应引脚
//    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10;							//接收接口对应引脚
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    USART_InitStructure.USART_BaudRate 	= USART_BPS;	 						//设置波特率
//	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//设置数据位
//	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
//	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
//	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
//    USART_Init(USART1, &USART_InitStructure);
//
//    USART_Cmd(USART1, ENABLE);													//使能串口
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		   						//开中断
///*
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2;							//发送接口对应引脚
//    GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;						//推挽输出-TX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_3;							//接收接口对应引脚
//    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;				//浮空输入-RX
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    USART_InitStructure.USART_BaudRate 	= USART_BPS;	 						//设置波特率
//	USART_InitStructure.USART_WordLength= USART_WordLength_8b; 					//设置数据位
//	USART_InitStructure.USART_StopBits 	= USART_StopBits_1; 					//设置停止位
//	USART_InitStructure.USART_Parity 	= USART_Parity_No;						//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件握手协议
//	USART_InitStructure.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;		//允许发送和接收功能
//    USART_Init(USART2, &USART_InitStructure);
//
//    USART_Cmd(USART2, ENABLE);													//使能串口
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		   						//开中断
//*/
//}

/*********************************************************************************************************************
函数名称: I2C_Configuration          
功能描述: IIC初始化
输　入:   无
输　出:   无	  	                 
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
    I2C_InitStructure.I2C_Mode 			= I2C_Mode_I2C;							//设置I2C模式 
    I2C_InitStructure.I2C_DutyCycle 	= I2C_DutyCycle_2; 						//I2C 快速模式 Tlow / Thigh = 2 ，只有在高于100K才有意义
    I2C_InitStructure.I2C_OwnAddress1 	= 0x30;	  								//设置第一个设备自身地址
    I2C_InitStructure.I2C_Ack 			= I2C_Ack_Enable;	 					//使能应答（ACK）
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 	//应答 7 位地址 
    I2C_InitStructure.I2C_ClockSpeed 	= I2C_SPEED;							//100K速度
	I2C_Init(I2C1, &I2C_InitStructure);
    
	I2C_Cmd(I2C1, ENABLE);														//使能I2C外设

/*
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_10 | GPIO_Pin_11;		   // PB10,11 SCL and SDA
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode 			= I2C_Mode_I2C;							//设置I2C模式 
    I2C_InitStructure.I2C_DutyCycle 	= I2C_DutyCycle_2; 						//I2C 快速模式 Tlow / Thigh = 2 ，只有在高于100K才有意义
    I2C_InitStructure.I2C_OwnAddress1 	= 0x30;	  								//设置第一个设备自身地址
    I2C_InitStructure.I2C_Ack 			= I2C_Ack_Enable;	 					//使能应答（ACK）
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 	//应答 7 位地址 
    I2C_InitStructure.I2C_ClockSpeed 	= I2C_SPEED;							//100K速度
	I2C_Init(I2C2, &I2C_InitStructure);
    
	I2C_Cmd(I2C2, ENABLE);														//使能I2C外设
*/
}

/*********************************************************************************************************************
函数名称: NVIC_Configuration          
功能描述: 向量中断初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								//优先级设置
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= UART5_IRQn;		//打开串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);
	
/*	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn;		//中断名字
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;				//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;				//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			//允许中断
	NVIC_Init(&NVIC_InitStructure);
*/												//初始化

}

/*********************************************************************************************************************
函数名称: IWDG_Configuration          
功能描述: 看门狗初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void IWDG_Configuration()
{
    if(WATCHDOG_EN)
	{
  		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	 		//使能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
  		IWDG_SetPrescaler(IWDG_Prescaler_32);	   				//IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz 
  		IWDG_SetReload(40*1000/32 * IWDG_MAXTIME/1000);			//最大值4095
  		IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);	 		//使能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
  		IWDG_ReloadCounter();									//重装初始，相当于清狗
  		IWDG_Enable();	 										//使能看门狗,
	}
}

/*********************************************************************************************************************
函数名称: PWR_Configuration          
功能描述: 电源监控初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void PWR_Configuration()
{
  	INT16U Temp;
	PWR_PVDCmd(ENABLE);										//使能电源检测
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9);					//
	for(Temp=0; Temp<65535; Temp++);						//等待电源电压采样
	for(Temp=0; Temp<65535; Temp++);						//等待电源电压采样
	for(Temp=0; Temp<100; Temp++)
	{	if(PWR_GetFlagStatus(PWR_FLAG_PVDO) == 1) Temp=0;	}
}
/*********************************************************************************************************************
函数名称: Periph_Configuration          
功能描述: 外围初始化
输　入:   无
输　出:   无	  	                 
*********************************************************************************************************************/
void Periph_Configuration(void)
{
	RCC_Configuration();				//初始化系统和各总线的时钟
	PWR_Configuration();				//电源电压检测
	IWDG_Configuration();				//独立看门狗初始化		
	GPIO_Configuration();				//IO口初始化
	ADC_Configuration2();				//AD转换初始化
	//TIM_Configuration();				//通用定时器初始化
	NVIC_Configuration();				//中断初始化
	UART_Configuration();				//串口1初始化
	UART2_Configuration();				//串口2初始化
	UART3_Configuration();				//串口3初始化
	UART4_Configuration();				//串口4初始化
	UART5_Configuration();				//串口5初始化
	GPIO_UART_Configuration_RS485EN();  //485使能端口初始化
	I2C_Configuration();				//I2C通讯初始化
	//PWM_Configuration();				//PWM/DA初始化
	PWR_Configuration();				//电源监控初始化
	//RTC_Configuration();				//记录模块工作了多长时间或用来计算电量，未使用
//	CAN_Initial(CAN_REC_ALL);  		    //仅能接收BMS数据，因为开始就进行连接

 PWM_ON;
 PWM_H;
 PWM_L;
 PWM_H;
 PWM_L;
 

}
