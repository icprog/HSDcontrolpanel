#ifndef __CAN_CFG_H
#define __CAN_CFG_H
/*********************************************************************************************************************
*                                            头文件包含
*********************************************************************************************************************/
#include "stm32f10x.h"
#include "define.h"

/*********************************************************************************************************************
*                                            宏定义
*********************************************************************************************************************/
#define PCU_ADR               (gUserDB.pcu_adr)           //PCU地址
#define CCS_ADR               0X2B			//充电机地址
#define BROADCAST_ADR         0XFF			//广播地址
#define BMS_MAX_SIZE          200           //BMS缓冲区字节长度
#define CCS_MAX_SIZE          200           //CCS缓冲区字节长度

#define CAN_REC_FILTER	   0x00
#define CAN_REC_ALL 		   0x01


//=========多数据传输协议的PGN的值===========//
#define	M_PGN_BRM		0x02  //只取中间2个字节，其它默认0x00
#define	M_PGN_VBI		0x30
#define M_PGN_BCP		0x06
#define M_PGN_BMV		0x15
#define M_PGN_BMT		0x16
#define M_PGN_BSOC		0x17 

//*************************PC 发送CAN 通信报文ID***************************************************
#define PC_PC_DATE                      0X1800FF00                         //系统对时报文
#define PC_PCU_P_SET                    (0X18010000+(PCU_ADR<<8))          //协议版本设置报文
#define PC_INQUIRE_ALL                  (0X18040000+(PCU_ADR<<8))          //查询命令报文 


#define PC_MCM_STOP_ALL                (0X045CFF00)                      //紧急停机命令 
#define PC_MCM_START_SINGLE           (0X085D0000+(PCU_ADR<<8))          //启动命令 
#define PC_MCM_STOP_SINGLE            (0X045E0000+(PCU_ADR<<8))          //停止命令 
#define PC_MCM_SET_SINGLE             (0X0C5F0000+(PCU_ADR<<8))          //单体控制参数设置 
#define PC_MCM_SET_STAGE              (0X0C600000+(PCU_ADR<<8))          //阶段控制参数设置
#define PC_MCM_READ_SET_ALL           (0X10660000+(PCU_ADR<<8))          //全部控制参数读取命令 
#define PC_MCM_READ_SET_SPECIAL      (0X10670000+(PCU_ADR<<8))          //查询命令报文 



//*************************PCU 发送CAN 通信报文ID************************************************
#define PCU_PCU_LIFE                     (0X08020000+(PCU_ADR<<8))          //生命报文 
#define PCU_BMS_IDENTICATION        (0X18060000+PCU_ADR)          //BMS辨识报文
#define PCU_BMS_INFORMATION1        (0X18070000+PCU_ADR)          //BMS信息1报文
#define PCU_BMS_INFORMATION2        (0X18080000+PCU_ADR)          //BMS信息2报文
#define PCU_BMS_INFORMATION3        (0X18090000+PCU_ADR)          //BMS信息3报文
#define PCU_BMS_INFORMATION4        (0X18500000+PCU_ADR)          //BMS信息4报文
#define PCU_BMS_LIFE                 (0X180A0000+PCU_ADR)          //BMS生命帧报文 
#define PCU_BAT_REQIRE               (0X180B0000+PCU_ADR)          //电池充电级别需求报文 
#define PCU_BAT_STATE                (0X180C0000+PCU_ADR)          //电池充电状态报文 
#define PCU_BAT_STATE_LIMIT         (0X180D0000+PCU_ADR)          //电池最高状态报文
#define PCU_BAT_STOP                 (0X180E0000+PCU_ADR)          //电池充电停止报文
#define PCU_BAT_COUNT                (0X180F0000+PCU_ADR)          //电池充电状态报文 
#define PCU_BAT_ERR                  (0X18010000+PCU_ADR)           //BMS错误报文 
#define PCU_BAT_FAIL                 (0X18011000+PCU_ADR)          //BMS故障报文
#define PCU_BAT_FAIL2                (0X18012000+PCU_ADR)          //BMS故障报文2 

#define PCU_BAT_VOLT_N1               (0X18013000+PCU_ADR)          //电池单体电压数据报文1
#define PCU_BAT_VOLT_N2               (0X18014000+PCU_ADR)          //电池单体电压数据报文2
#define PCU_BAT_VOLT_N3               (0X18015000+PCU_ADR)          //电池单体电压数据报文3
#define PCU_BAT_VOLT_N4               (0X18016000+PCU_ADR)          //电池单体电压数据报文4
#define PCU_BAT_VOLT_N5               (0X18017000+PCU_ADR)          //电池单体电压数据报文5
#define PCU_BAT_VOLT_N6               (0X18018000+PCU_ADR)          //电池单体电压数据报文6
#define PCU_BAT_VOLT_N7               (0X18019000+PCU_ADR)          //电池单体电压数据报文7
#define PCU_BAT_VOLT_N8               (0X1801A000+PCU_ADR)          //电池单体电压数据报文8
#define PCU_BAT_VOLT_N9               (0X1801B000+PCU_ADR)          //电池单体电压数据报文9
#define PCU_BAT_VOLT_N10               (0X1801C000+PCU_ADR)          //电池单体电压数据报文10
#define PCU_BAT_VOLT_N11               (0X1801D000+PCU_ADR)          //电池单体电压数据报文11
#define PCU_BAT_VOLT_N12               (0X1801E000+PCU_ADR)          //电池单体电压数据报文12
#define PCU_BAT_VOLT_N13               (0X1801F000+PCU_ADR)          //电池单体电压数据报文13
#define PCU_BAT_VOLT_N14               (0X18020000+PCU_ADR)          //电池单体电压数据报文14
#define PCU_BAT_VOLT_N15               (0X18021000+PCU_ADR)          //电池单体电压数据报文15
#define PCU_BAT_VOLT_N16               (0X18022000+PCU_ADR)          //电池单体电压数据报文16
#define PCU_BAT_VOLT_N17               (0X18023000+PCU_ADR)          //电池单体电压数据报文17
#define PCU_BAT_VOLT_N18               (0X18024000+PCU_ADR)          //电池单体电压数据报文18
#define PCU_BAT_VOLT_N19               (0X18025000+PCU_ADR)          //电池单体电压数据报文19
#define PCU_BAT_VOLT_N20               (0X18026000+PCU_ADR)          //电池单体电压数据报文20

#define PCU_BAT_TEMP_N1               (0X18027000+PCU_ADR)          //电池单体温度数据报文1
#define PCU_BAT_TEMP_N2               (0X18028000+PCU_ADR)          //电池单体温度数据报文2
#define PCU_BAT_TEMP_N3               (0X18029000+PCU_ADR)          //电池单体温度数据报文3
#define PCU_BAT_TEMP_N4               (0X1802A000+PCU_ADR)          //电池单体温度数据报文4
#define PCU_BAT_TEMP_N5               (0X1802B000+PCU_ADR)          //电池单体温度数据报文5
#define PCU_BAT_TEMP_N6               (0X1802C000+PCU_ADR)          //电池单体温度数据报文6
#define PCU_BAT_TEMP_N7               (0X1802D000+PCU_ADR)          //电池单体温度数据报文7
#define PCU_BAT_TEMP_N8               (0X1802E000+PCU_ADR)          //电池单体温度数据报文8
#define PCU_BAT_TEMP_N9               (0X1802F000+PCU_ADR)          //电池单体温度数据报文9
#define PCU_BAT_TEMP_N10               (0X18030000+PCU_ADR)          //电池单体温度数据报文10
#define PCU_BAT_TEMP_N11               (0X18031000+PCU_ADR)          //电池单体温度数据报文11
#define PCU_BAT_TEMP_N12               (0X18032000+PCU_ADR)          //电池单体温度数据报文12
#define PCU_BAT_TEMP_N13               (0X18033000+PCU_ADR)          //电池单体温度数据报文13
#define PCU_BAT_TEMP_N14               (0X18034000+PCU_ADR)          //电池单体温度数据报文14
#define PCU_BAT_TEMP_N15               (0X18035000+PCU_ADR)          //电池单体温度数据报文15
#define PCU_BAT_TEMP_N16               (0X18036000+PCU_ADR)          //电池单体温度数据报文16
#define PCU_BAT_TEMP_N17               (0X18037000+PCU_ADR)          //电池单体温度数据报文17
#define PCU_BAT_TEMP_N18               (0X18038000+PCU_ADR)          //电池单体温度数据报文18
#define PCU_BAT_TEMP_N19               (0X18039000+PCU_ADR)          //电池单体温度数据报文19
#define PCU_BAT_TEMP_N20               (0X1803A000+PCU_ADR)          //电池单体温度数据报文20
#define PCU_BAT_ID1                     (0X1803B000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID2                     (0X1803C000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID3                     (0X1803D000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID4                     (0X1803E000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID5                     (0X1803F000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID6                     (0X18040000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID7                     (0X18041000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID8                     (0X18042000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID9                     (0X18043000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID10                     (0X18044000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID11                     (0X18045000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID12                     (0X18046000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID13                     (0X18047000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID14                     (0X18048000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID15                     (0X18049000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID16                     (0X1804A000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID17                     (0X1804B000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID18                     (0X1804C000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID19                     (0X1804D000+PCU_ADR)            //电池箱ID报文
#define PCU_BAT_ID20                     (0X1804E000+PCU_ADR)            //电池箱ID报文




#define PCU_MCM_RETURN_SET_SINGLE_ALL          (0X10690000+PCU_ADR)            //读取所有单体控制参数
#define PCU_MCM_RETURN_SET_STAGE_ALL           (0X106A0000+PCU_ADR)            //读取所有阶段控制参数
#define PCU_MCM_RETURN_SET_SINGLE              (0X10700000+PCU_ADR)            //读取单体控制参数
#define PCU_MCM_RETURN_SET_STAGE           	  (0X10710000+PCU_ADR)            //读取单体控制参数
#define PCU_MCM_STATE0                           (0X18780000+PCU_ADR)            //充电机状态帧1
#define PCU_MCM_STATE1                           (0X187A0000+PCU_ADR)            //充电机状态帧2
#define PCU_MCM_STATE2                           (0X187B0000+PCU_ADR)            //充电机状态帧3
#define PCU_MCM_STATE3                           (0X187C0000+PCU_ADR)            //充电机状态帧4
#define PCU_MCM_PDU_INFO                         (0X187D0000+PCU_ADR)            //电表信息帧
#define PCU_MCM_DEVICE_ID                        (0X18800000+PCU_ADR)            //充电设备信息帧

#define PCU_CCS_IDENTIFY                        (0X187E0000+PCU_ADR)            //充电辨识报文
#define PCU_CCS_MAX_OUTPUT                      (0X187F0000+PCU_ADR)            //充电辨识报文
#define PCU_CCS_STATE                            (0X18820000+PCU_ADR)            //充电状态报文
#define PCU_CCS_STOP                            (0X14830000+PCU_ADR)            //充电中止报文
#define PCU_CCS_COUNT                           (0X18840000+PCU_ADR)            //充电统计报文
#define PCU_CCS_ERR                             (0X10850000+PCU_ADR)            //充电错误报文
#define PCU_CCS_FAULT                           (0X10860000+PCU_ADR)            //充电故障报文




//=========蓄电池管理系统BMS PGN================================================//
#define BMS_BCP1_PGN     		0X60         		//蓄电池充电参数
#define BMS_CBP1_PGN     		0X70		  		//整车蓄电池参数1
#define BMS_CBP2_PGN     		0X80 				//整车蓄电池参数2
#define BMS_BCS_PGN             0X90				//蓄电池充电状态
#define BMS_CTS_PGN             0XA0                //时间同步信息
#define BMS_MVD_PGN             0XB0				//蓄电池模块电压数据
#define BMS_BTD_PGN             0XC0				//蓄电池温度数据
#define BMS_SOC_PGN             0XD0                //蓄电池SOC数据
#define BMS_AVD_PGN             0XE0                //蓄电池平均电压数据
#define BMS_BBI_PGN             0XF1               	//蓄电池基本信息
#define BMS_BBI1_PGN            0XF100               	//蓄电池基本信息1
#define BMS_BBI2_PGN            0XF101               	//蓄电池基本信息2

//=========充电机CCS PGN=======================================================//
#define CCS_CLF_PGN             0X20                //充电机生命信息帧
#define CCS_CCS1_PGN            0X30                //充电机状态1
#define CCS_CCS2_PGN            0X31                //充电机状态2
#define CCS_CCP_PGN             0X40                //充电机参数
#define CCS_EST_PGN             0X51                //充电机紧急停止命令
#define CCS_CST_PGN             0X52                //充电机启动充电命令
#define CCS_CSP_PGN             0X53                //充电机停止充电命令 

//=========蓄电池管理系统BMS PGN LEN ================================================//
#define BMS_BCP1_PGN_LEN     		8         		//蓄电池充电参数
#define BMS_CBP1_PGN_LEN     		8		  		//整车蓄电池参数1
#define BMS_CBP2_PGN_LEN     		8 				//整车蓄电池参数2
#define BMS_BCS_PGN_LEN             8				//蓄电池充电状态
#define BMS_CTS_PGN_LEN             8                //时间同步信息
#define BMS_MVD_PGN_LEN             8				//蓄电池模块电压数据
#define BMS_BTD_PGN_LEN             8				//蓄电池温度数据
#define BMS_SOC_PGN_LEN             8                //蓄电池SOC数据
#define BMS_AVD_PGN_LEN             8                //蓄电池平均电压数据
#define BMS_BBI_PGN_LEN             8               	//蓄电池基本信息
#define BMS_BBI1_PGN_LEN            8               	//蓄电池基本信息1
#define BMS_BBI2_PGN_LEN            8               	//蓄电池基本信息2

//=========蓄电池管理系统BMS PGN ADR 缓冲区地址排序 ==================================//
#define BMS_BCP1_PGN_ADR     		0         										//蓄电池充电参数
#define BMS_CBP1_PGN_ADR     		(BMS_BCP1_PGN_ADR+BMS_BCP1_PGN_LEN)		  		//整车蓄电池参数1
#define BMS_CBP2_PGN_ADR     		(BMS_CBP1_PGN_ADR+BMS_CBP1_PGN_LEN) 			//整车蓄电池参数2
#define BMS_BCS_PGN_ADR             (BMS_CBP2_PGN_ADR+BMS_CBP2_PGN_LEN)				//蓄电池充电状态
#define BMS_CTS_PGN_ADR             (BMS_BCS_PGN_ADR+BMS_BCS_PGN_LEN)               //时间同步信息
#define BMS_MVD_PGN_ADR             (BMS_CTS_PGN_ADR+BMS_CTS_PGN_LEN)				//蓄电池模块电压数据，多包
#define BMS_BTD_PGN_ADR             (BMS_MVD_PGN_ADR+BMS_MVD_PGN_LEN)				//蓄电池温度数据，多包
#define BMS_SOC_PGN_ADR             (BMS_BTD_PGN_ADR+BMS_BTD_PGN_LEN)               //蓄电池SOC数据，多包
#define BMS_AVD_PGN_ADR             (BMS_SOC_PGN_ADR+BMS_SOC_PGN_LEN)               //蓄电池平均电压数据，多包
#define BMS_BBI_PGN_ADR             (BMS_AVD_PGN_ADR+BMS_AVD_PGN_LEN)               //蓄电池基本信息
#define BMS_BBI1_PGN_ADR            (BMS_BBI_PGN_ADR+BMS_BBI_PGN_LEN)               //蓄电池基本信息1
#define BMS_BBI2_PGN_ADR            (BMS_BBI1_PGN_ADR+BMS_BBI1_PGN_LEN)             //蓄电池基本信息2

//=========充电机CCS PGN LEN=======================================================//
#define CCS_CLF_PGN_LEN             8                //充电机生命信息帧
#define CCS_CCS1_PGN_LEN            8                //充电机状态1
#define CCS_CCS2_PGN_LEN            8                //充电机状态2
#define CCS_CCP_PGN_LEN             8                //充电机参数，多包
#define CCS_EST_PGN_LEN             8                //充电机紧急停止命令
#define CCS_CST_PGN_LEN             8                //充电机启动充电命令
#define CCS_CSP_PGN_LEN             8                //充电机停止充电命令 

//=========充电机CCS PGN ADR =======================================================//
#define CCS_CLF_PGN_ADR             0                								 //充电机生命信息帧
#define CCS_CCS1_PGN_ADR            (CCS_CLF_PGN_ADR+CCS_CLF_PGN_LEN)                //充电机状态1
#define CCS_CCS2_PGN_ADR            (CCS_CCS1_PGN_ADR+CCS_CCS1_PGN_LEN)                //充电机状态2
#define CCS_CCP_PGN_ADR             (CCS_CCS2_PGN_ADR+CCS_CCS2_PGN_LEN)                //充电机参数，多包
#define CCS_EST_PGN_ADR             (CCS_CCP_PGN_ADR+CCS_CCP_PGN_LEN)                //充电机紧急停止命令
#define CCS_CST_PGN_ADR             (CCS_EST_PGN_ADR+CCS_EST_PGN_LEN)                //充电机启动充电命令
#define CCS_CSP_PGN_ADR             (CCS_CST_PGN_ADR+CCS_CST_PGN_LEN)                //充电机停止充电命令 



//==============CCS 充电流程阶段值============================================//
#define CCS_SEND_CRM_0		    0	//充电机辨识报文CRM
#define CCS_SEND_CRM_INVALID    1	//无效
#define CCS_SEND_CRM_1		    2	//充电机辨识报文CRM
#define CCS_SEND_CTS_CML	    3 	//CCS最大输出能力CML
#define CCS_SEND_CRO		    4	//CCS充电准备报文CRO
#define CCS_SEND_CCS		    5	//CCS充电状态CCS
#define CCS_SEND_CST		    6	//CCS充电状态CCS
#define CCS_SEND_CSD		    7	//CCS充电统计CSD

    


/********************************************************************************************************************
*                                            函数或其它定义
*********************************************************************************************************************/

	

#endif



