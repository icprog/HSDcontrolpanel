
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
    PAGE_ID_MAIN                        =0,                             //���д���
    PAGE_ID_RUSH_CARD                   =2,                             //���ˢ��
    PAGE_ID_SYSTEM_MANAGE_ERR           =4,                            //ϵͳ�����쳣
    PAGE_ID_SYSTEM_MANAGE               =5,                            //ϵͳ����
	  PAGE_ID_CRGING_MSG                  =6,                            //�����Ϣ����
    PAGE_ID_CHARGE_INFO                 =8,                            //����������Ϣ
	  PAGE_ID_RUSH_CARD_FAIL              =12,                            //ˢ��ʧ��    
    PAGE_ID_OVER_RUSH_CARD              =23,                            //�������ȴ�ˢ��
	  PAGE_ID_INFO_CARD                   =24,                            //������Ϣ	
    PAGE_ID_PWD_ERR                     =25,                            //�������  
    PAGE_ID_PWD2_ERR                     =26,                            //�������    
    PAGE_ID_PARA_MANAGE                 =32,                            //��������     
    PAGE_ID_CRG_MSG_SURE                =51,
    PAGE_ID_CRG_MSG_NOTICE              =52,                            //�����Ϣ��ʾ
//    PAGE_ID_HISTORY_FAULT               =53,                            //��ʷ����
//    PAGE_ID_CRG_HISTORY                 =54,                            //��ʷ��¼
//    PAGE_ID_NOTICE_OI_MSG               =55,                            //���뵼����ʾ
//    PAGE_ID_FAULT_CANOT_CRG              =60,                            //���׮���ϣ��޷�������� 
//    PAGE_ID_DEVICE_MSG_PG1              =37,                           //�豸��Ϣ��һҳ	
//    PAGE_ID_DEVICE_MSG_PG2              =40,                           //�豸��Ϣ�ڶ�ҳ		
//    PAGE_ID_USB_OR_SD_SEL              	=73,                           //SD����USBѡ����ʾҳ
//    PAGE_ID_IMPORT_EXPORT_ING           =70,                           //���뵼����������ʾҳ			
//    PAGE_ID_IMPORT_EXPORT_END           =71,                           //���뵼�������ʾҳ	
//    PAGE_ID_CURVE_MSG           				=79,                           //��ʾ����ҳ		
//    PAGE_ID_FAULT_STOP_CRG           		=58,                           //���׮���ϣ�ֹͣ���	
//		PAGE_ID_RE_GUN_AGAIN           			=82,                           //�����²�ǹ
//		PAGE_ID_TIME_LIGHT_SET           		=84,                           //ʱ�䱳������
//		PAGE_ID_SYSTEM_FAULT			          =83,                           //ϵͳ����ҳ
//		PAGE_ID_LAST_CONSUME_MSG			      =93,                           //�ϴ�������Ϣ
//    PAGE_ID_SAVE_MSG		              	=94,                           //������ʾ
//    PAGE_ID_OTHERCFG                        =95,                            //��������
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