
typedef enum
{
	TOUCH_CMD_NONE            =0,
	TOUCH_CMD_WRITE_REGISTER  =0X80,
	TOUCH_CMD_READ_REGISTER   =0X81,
	TOUCH_CMD_WRITE_VARIABLE  =0X82,
	TOUCH_CMD_READ_VARIABLE   =0X83,
	TOUCH_CMD_DRAW_CURVE      =0X84,
}TOUCH_CMD;
typedef enum
{
    PAGE_ID_MAIN                        =0,                             //空闲窗口
    PAGE_ID_RUSH_CARD                   =2,                             //充电刷卡
    PAGE_ID_SYSTEM_MANAGE_ERR           =4,                            //系统管理异常
    PAGE_ID_SYSTEM_MANAGE               =5,                            //系统管理
	  PAGE_ID_CRGING_MSG                  =6,                            //充电信息界面
    PAGE_ID_CHARGE_INFO                 =8,                            //本次消费信息
	  PAGE_ID_RUSH_CARD_FAIL              =12,                            //刷卡失败    
    PAGE_ID_OVER_RUSH_CARD              =23,                            //结束充电等待刷卡
	  PAGE_ID_INFO_CARD                   =24,                            //卡号信息	
    PAGE_ID_PWD_ERR                     =25,                            //密码错误  
    PAGE_ID_PWD2_ERR                     =26,                            //密码错误    
    PAGE_ID_PARA_MANAGE                 =32,                            //参数管理     
    PAGE_ID_CRG_MSG_SURE                =51,
    PAGE_ID_CRG_MSG_NOTICE              =52,                            //充电信息提示
//    PAGE_ID_HISTORY_FAULT               =53,                            //历史故障
//    PAGE_ID_CRG_HISTORY                 =54,                            //历史记录
//    PAGE_ID_NOTICE_OI_MSG               =55,                            //导入导出提示
//    PAGE_ID_FAULT_CANOT_CRG              =60,                            //充电桩故障，无法启动充电 
//    PAGE_ID_DEVICE_MSG_PG1              =37,                           //设备信息第一页	
//    PAGE_ID_DEVICE_MSG_PG2              =40,                           //设备信息第二页		
//    PAGE_ID_USB_OR_SD_SEL              	=73,                           //SD卡或USB选择提示页
//    PAGE_ID_IMPORT_EXPORT_ING           =70,                           //导入导出处理中提示页			
//    PAGE_ID_IMPORT_EXPORT_END           =71,                           //导入导出结果提示页	
//    PAGE_ID_CURVE_MSG           				=79,                           //显示曲线页		
//    PAGE_ID_FAULT_STOP_CRG           		=58,                           //充电桩故障，停止充电	
//		PAGE_ID_RE_GUN_AGAIN           			=82,                           //请重新插枪
//		PAGE_ID_TIME_LIGHT_SET           		=84,                           //时间背光设置
//		PAGE_ID_SYSTEM_FAULT			          =83,                           //系统故障页
//		PAGE_ID_LAST_CONSUME_MSG			      =93,                           //上次消费信息
//    PAGE_ID_SAVE_MSG		              	=94,                           //保存提示
//    PAGE_ID_OTHERCFG                        =95,                            //其他设置
//    PAGE_ID_INPUTDEVICMSG                   =99,
}PAGE_ID;
typedef enum
{
    BUTTON_ADDR_NONE                     =0,
    BUTTON_ADDR_GO_RUSH_CARD             =0X0001,
    BUTTON_ADDR_a                        =0x8888,
}BUTTON_ADDR;
typedef enum
{
    CURRENT_FAULT_INDEX_NONE            =0,
    CURRENT_FAULT_INDEX_METER_COMM      =1,
    CURRENT_FAULT_INDEX_STOP            =2,
    CURRENT_FAULT_INDEX_OVER_V          =3,
    CURRENT_FAULT_INDEX_OVER_I          =4,
    CURRENT_FAULT_INDEX_LOW_V           =5,
    CURRENT_FAULT_INDEX_THOUND          =6,
    CURRENT_FAULT_INDEX_TOUCH           =7,
    CURRENT_FAULT_INDEX_READ_CARD       =8,
}CURRENT_FAULT_INDEX;