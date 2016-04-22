#ifndef __DEFINE_H
#define __DEFINE_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
 #include "ucos_ii.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short int   uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int   int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int  uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int  int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float           fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
typedef unsigned long uint64;
typedef signed long int64;

#define HANDLE_STAGE                		0X01     //握手阶段
#define PARA_SET_STAGE              		0X02     //参数配置阶段
#define CHARGEING_STAGE                	    0X03     //充电阶段
#define ENDCHARGE_STAGE                     0X04     //结束充电阶段
#define IDLE_STAGE                          0X00     //空闲阶段

#define  NORMAL_MODE                        0X00
#define  TEST_MODE                          0X01
#define  ERR_MAX_CNT                        10        //错误计数最大值
#define  ERR_OUT_VOLTCURR_CNT               50
#define  RUN                                0X01      //模块运行
#define  STOP                               0X00	  //模块停止

#define  MANUAL_CTL                         0X01
#define  AUTO_CTL                           0X00

#define ON                                  0X01
#define OFF                                 0X00


#define   ALARM                              0X01
#define   NOALARM                            0X00

#define    OK                                0X01	  //
#define    NOK 							     0X00     //

#define  WATCHDOG_EN                 0         //看门狗使能  

//#define  AC_MODULE                   1

//**************************modbus地址****************************************************//
#define  MODBUS_DEVICEID          2
#define  MODBUSBUFMAX             130
#define  PCU_STATUS_ADR           0          //PCU状态
#define  PDU_VOLT_ADR       	    1		   //电表电压地址
#define  PDU_CUR_ADR       		2	 	   //电表输出电流地址
#define  PDU_POWER_L_ADR         	3 		   //电表电能低位地址
#define  PDU_POWER_H_ADR       	4 		   //电表电能高位地址
#define  TIME1_ADR         		5 		   //时间1地址
#define  TIME2_ADR                 6  		   //时间2地址
#define  TIME3_ADR                 7 		   //时间3地址
#define  REMOTE_CTL_ADR            8 		   //远程控制地址

#define  M_PDU_ADR                  109 		       //电表地址
#define  M_SET_SINGLE_SET_TYPE          110 		   //单体设置地址
#define  M_SET_P_CELL_VOLT              111 		   //单体设置地址
#define  M_SET_P_CELL_TEMP              112		   //单体设置地址
#define  M_SET_STAGE_TYPE               113 		   //阶段设置地址
#define  M_SET_OUT_VALUE                114 		   //阶段设置地址
#define  M_SET_LIMT_VALUE               115 		   //阶段设置地址
#define  M_SET_STOP_VALUE               116 		   //阶段设置地址

#define  M_R_SINGLE_SET_TYPE           110 		   //单体设置地址
#define  M_R_P_CELL_VOLT               111 		   //单体设置地址
#define  M_R_P_CELL_TEMP               112		    //单体设置地址
#define  M_R_STAGE_TYPE                113 		   //阶段设置地址
#define  M_R_OUT_VALUE                 114 		   //阶段设置地址
#define  M_R_LIMT_VALUE                115 		   //阶段设置地址
#define  M_R_STOP_VALUE                116 		  // 阶段设置地址
#define  M_CCS_STATUS                  117		  //充电机状态地址















#define  POWER_TOTAL_ADR            9          //已充电量地址
#define  ADJ_CONFIRM_ADR            10         //校正确认
//#define  MODULES_TEMPER_ADR         11	       //模块温度地址
#define  HEART_ADR                  12	       //心跳地址



#define  SET_OUT_VOLT_K1_ADR        4		   //设置输出电流K值   
#define  SET_OUT_VOLT_K2_ADR        6		   //设置输出电流K值 
#define  SET_OUT_VOLT_K3_ADR        11		   //设置输出电流K值 

#define  DATA_RESET_ADR             13          //参数复位
#define  DISP_IN_VOLT_ADR           14			//输入电压地址
#define  DISP_TEMP_ADR              15         //温度地址
#define  SET_OUT_CUR_K1_ADR         16		   //设置输出电流K值
#define  SET_OUT_CUR_K2_ADR         17		   //设置输出电流K值
#define  SET_OUT_CUR_K3_ADR         18		   //设置输出电流K值

#define  SET_DISP_VOLT_K1_ADR        19		   //设置显示电压K值
#define  SET_DISP_CUR_K1_ADR         21	 	   //设置显示电流K值

#define  SET_DISP_IN_VOLT_K1_ADR      20		   //设置显示电压K值
#define  SET_DISP_TEMP_K1_ADR         22	 	   //设置显示电流K值   

#define  VOLT_PWM_ADR                23	 	   //设置显示电流K值  
#define  CUR_PWM_ADR                 24	 	   //设置显示电流K值  

#define	SOFTVERSION				11060100					//软件版本号1106V1.00
#define	PWM_PERIOD				12000						//PWM的最大值,也是设定输出pwm的分辨率
/***********需要保存的参数在24c16中的存储地址***************/	

#define ADDR_FIRSTFLAG				1							//参数是否写入标志位存储地址

#define ADDR_AUTO_FLAG	        	2							//手动/自动控制标志存储地址占用1个字节
#define ADDR_AVER_FLAG	        	3							//均浮充模式标志存储地址占用1个字节	

#define ADDR_MANUAL_AVER_VOLT		4							//手动均充电压存储地址占用4个字节
#define ADDR_MANUAL_FLOAT_VOLT		8							//手动浮充电压存储地址占用4个字节
#define ADDR_MANUAL_SET_VOLT		12							//手动设置电压存储地址占用4个字节
#define ADDR_MANUAL_SET_CURR		16							//手动设置电流存储地址占用4个字节
#define ADDR_MAX_VOLT	        	20							//最高电压存储地址占用4个字节
#define ADDR_MAX_CURR	        	24							//最高电流存储地址占用4个字节
#define SINGLE_VOLT	        	    28							//最高单体电压存储地址占用4个字节	 



#define BEEP_OFF        GPIO_ResetBits(GPIOC, GPIO_Pin_0)				//led1亮 
#define BEEP_ON      GPIO_SetBits(GPIOC, GPIO_Pin_0)			        //led1灭        

#define LED1_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_8)				//led1亮 
#define LED1_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_8)			        //led1灭  
#define LED2_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_9)				//led2亮  
#define LED2_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_9)			        //led2灭
#define LED3_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_10)				//led3亮
#define LED3_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_10)			        //led3灭
#define LED4_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_11)				//led1亮 
#define LED4_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_11)			        //led1灭  
#define LED5_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_12)				//led2亮  
#define LED5_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_12)			        //led2灭
#define LED6_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_13)				//led3亮
#define LED6_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_13)			        //led3灭 
#define LED7_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_14)				//led1亮 
#define LED7_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_14)			        //led1灭  
#define LED8_ON        GPIO_ResetBits(GPIOD, GPIO_Pin_15)				//led2亮  
#define LED8_OFF       GPIO_SetBits(GPIOD, GPIO_Pin_15)			        //led2灭

#define OUT1_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_0)				//OUT1亮 
#define OUT1_ON       GPIO_SetBits(GPIOE, GPIO_Pin_0)			        //OUT1灭  
#define OUT2_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_1)				//OUT2亮  
#define OUT2_ON       GPIO_SetBits(GPIOE, GPIO_Pin_1)			        //OUT2灭
#define OUT3_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_2)				//OUT3亮
#define OUT3_ON       GPIO_SetBits(GPIOE, GPIO_Pin_2)			        //OUT3灭
#define OUT4_OFF        GPIO_ResetBits(GPIOE, GPIO_Pin_3)				//OUT1亮 
#define OUT4_ON       GPIO_SetBits(GPIOE, GPIO_Pin_3)			        //OUT1灭  
#define OUT5_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_4)				//OUT2亮  
#define OUT5_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_4)			        //OUT2灭
#define OUT6_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_5)				//OUT3亮
#define OUT6_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_5)			        //OUT3灭 
#define OUT7_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_6)				//OUT1亮 
#define OUT7_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_6)			        //OUT1灭  
#define OUT8_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_7)				//OUT2亮  
#define OUT8_OFF       GPIO_SetBits(GPIOE, GPIO_Pin_7)			        //OUT2灭


#define PWM_ON        GPIO_ResetBits(GPIOB, GPIO_Pin_8)				//
#define PWM_OFF       GPIO_SetBits(GPIOB, GPIO_Pin_8)			        //

#define PWM_L       GPIO_ResetBits(GPIOB, GPIO_Pin_0)				//
#define PWM_H       GPIO_SetBits(GPIOB, GPIO_Pin_0)			        //

#define SET_4051_A_H    GPIO_ResetBits(GPIOC, GPIO_Pin_1)				//高电平 
#define SET_4051_A_L    GPIO_SetBits(GPIOC, GPIO_Pin_1)				    //低电平 
#define SET_4051_B_H    GPIO_ResetBits(GPIOC, GPIO_Pin_2)				//高电平 
#define SET_4051_B_L    GPIO_SetBits(GPIOC, GPIO_Pin_2)				    //低电平
#define SET_4051_C_H    GPIO_ResetBits(GPIOC, GPIO_Pin_3)				//高电平 
#define SET_4051_C_L    GPIO_SetBits(GPIOC, GPIO_Pin_3)				    //低电平    


#define	CLK_SET			GPIOB->BSRR = GPIO_Pin_14			//设置CLK引脚为高电平
#define	CLR_SET			GPIOB->BSRR = GPIO_Pin_13			//设置CLR引脚为低电平
#define	CLR_RESET		GPIOA->BRR = GPIO_Pin_13			//设置CLR引脚为低电平
#define	CLK_RESET		GPIOB->BRR = GPIO_Pin_14			//设置CLK引脚为低电平
#define	SDA_VALUE		(*(INT32U*)(0x422181BC))			//GPIOB->PIN_15的地址



#define UART_1			0
#define UART_2			1
#define UART_3			2
#define UART_4			3
#define UART_5			4


#define USER_CMD_WAIT                         0X00     //待机
#define USER_CMD_SEARCH_CARD_REQUEST          0X01     //寻卡请求
#define USER_CMD_CHARGE_REQUEST              0X02      //开始加电请求
#define USER_CMD_END_CARD_REQUEST            0X03      //结束加电请求
#define USER_CMD_PRINT_REQUEST               0X04      //打印请求
#define USER_CMD_EXIT_CARD_REQUEST           0X05      //退卡请求
#define USER_CMD_ADDCHARGE_CARD_REQUEST      0X06      //补充交易请求

#define USER_CMD_SEARCH_CARD_ING              0X11     //寻卡进行中
#define USER_CMD_CHARGE_ING                  0X12      //正在充电中
#define USER_CMD_END_CARD_ING                0X13      //结束加电中
#define USER_CMD_PRINT_ING                   0X14      //打印中
#define USER_CMD_EXIT_CARD_ING               0X15      //退卡进行中
#define USER_CMD_ADDCHARGE_CARD_ING          0X16      //补充交易中

#define USER_CMD_SEARCH_CARD_END              0X21     //寻卡结束
#define USER_CMD_CHARGE_END                  0X22      //充电结束
#define USER_CMD_END_CARD_END                0X23      //加电命令结束
#define USER_CMD_PRINT_END                   0X24      //打印结束
#define USER_CMD_EXIT_CARD_END               0X25      //退卡结束
#define USER_CMD_ADDCHARGE_CARD_END         0X26       //补充交易结束

#define CARD_CMD_S_SEARCH_REQUEST           0X01     //寻卡请求
#define CARD_CMD_S_IDENT_REQUEST            0X02     //开始加电请求
//#define CARD_CMD_E_SEARCH_REQUEST           0X03
#define CARD_CMD_E_IDENT_REQUEST            0X03    //结束加电请求
#define CARD_CMD_ADDCHARGE_REQUEST         0X04     //补充交易请求
#define CARD_CMD_EXIT_REQUEST              0X05     //退卡请求


#define CARD_CMD_S_SEARCH_ING               0X11    //寻卡进行中
#define CARD_CMD_S_IDENT_ING                0X12    //开始加电读卡中
#define CARD_CMD_E_SEARCH_ING               0X13    //结束加电读卡中
#define CARD_CMD_ADDCHARGE_ING              0X14    //补充交易读卡中
#define CARD_CMD_EXIT_ING                   0X15    //退卡读卡中

#define CARD_CMD_S_SEARCH_END               0X21   //寻卡结束
#define CARD_CMD_S_IDENT_END                0X22   //开始加电读卡结束
//#define CARD_CMD_E_SEARCH_END               0X23
#define CARD_CMD_E_IDENT_END                 0X23   //结束加电读卡结束
#define CARD_CMD_ADDCHARGE_END               0X24   //补充交易读卡结束
#define CARD_CMD_EXIT_END                    0X25   //退卡读卡结束

#define KEY_SCAN_CMD_REQUEST                0X01     //键盘扫描请求
#define KEY_SCAN_CMD_ING                    0X11     //键盘扫描进行中
#define KEY_SCAN_CMD_END                    0X21     //键盘扫描结束

#define PRINTER_CMD_REQUEST                 0X01     //打印请求
#define PRINTER_CMD_ING                     0X11     //打印进行中
#define PRINTER_CMD_END                     0X21     //打印结束

#define COMM_ALARM_CNT_MAX                  250      //打印结束
#define REMOTE_CONTROL_ON                  0X01      //开机 
#define REMOTE_CONTROL_OFF                 0X02     //关机
#define REMOTE_CONTROL_INVALID            0X00     //无效


/*********************************************************************************************************************
*                                            定义包含
*********************************************************************************************************************/





#endif

