// CanBms.cpp : ʵ���ļ�
//

#include "../../app/global.h"
#include "../../app/includes.h"

#include "CanBms.h"
#include "canbusBms.h"
//#include "dcsInfolib.h"
//#include "dcsDlg.h"
#include "mcp2515_reg.h"


#define HIGH      1
#define LOW       0

#define ALARM					0X01  //�澯
#define NOALARM					0X00  //����

// CCanBms




// CCanBms ��Ϣ�������

BMS_MSG gBMS_msg;
CCS_MSG gCCS_msg;

uint8  wzlMsgBuf[40];  //������BMSͨ�Ż�����
WZL_BMS_INFO g_wzlBmsInfo;
WZL_CCS_INFO g_wzlCcsInfo;

uint8 BMS_ID_PGN_CTL[4] = {0xC0, 0x0A, 0xF4, 0xE5};  //BRS������
uint8 CCS_ID_PGN_CTL[4] = {0xC0, 0x0A, 0xF4, 0xE5};  //���������ѹ����״̬
//extern _DCSInfo gDCS_Info; //������׮��Ϣ�ṹ��




/*******************************************************************************************
��������: CDcsDlg::BMS_init
��    ��: BMS��ʼ������
�������: 
�������: ��
��    ��: ��
********************************************************************************************/
uint8 Mcp2515_init(void)
{	
//ORD dwThreadId1,dwThreadId2,dwThreadId3;
//	HANDLE handle1,handle2,handle3;	

	//gDCS_Info.charge_ctl=1;  //test
//	SPIInit();	//SPI��ʼ��
//	CanInit();	//BMS_CAN��ʼ��
//	m_CCanBms.bms_pdu_format_init();  //������BMS��ʽ��ʼ��	

	return 1;
}

/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  CanInit(void)
{
	uint8 byte_IR;

    Reset_2515(); //��λ    

    Write_BYTE_2515(CANCTRL, 0x80); //CANCTRL�Ĵ���������������ģʽ ����DATASHEET 58ҳ

	byte_IR = Read_BYTE_2515(CANCTRL);

	//�������õĲ����� 5K 10K 15K 20K 25K 40K 50K 80K 100K 125K 200K 400K 500K 667K 800K 1M
    Write_BYTE_2515(CNF1, CNF1_250K); //CNF1λ��ʱ���ü���   ����DATASHEET 41-42ҳ
	byte_IR = Read_BYTE_2515(CNF1);

    Write_BYTE_2515(CNF2, CNF2_250K); //CNF2λ��ʱ���ü���   ����DATASHEET 41-42ҳ
	byte_IR = Read_BYTE_2515(CNF2);

     Write_BYTE_2515(CNF3, CNF3_250K); //CNF3λ��ʱ���ü���   ����DATASHEET 41-43ҳ
	byte_IR = Read_BYTE_2515(CNF3);

    Write_BYTE_2515(CANINTE, 0x03); //CANINTE�ж�ʹ�ܼĴ���  ����DATASHEET 50 ҳ  //ֻ����ʹ�ܽ����ж�
	byte_IR = Read_BYTE_2515(CANINTE);
//rxb0
    Write_BYTE_2515(RXB0CTRL, 0x40); //RXB0CTRL���ջ�����0 ���ƼĴ��� ����DATASHEET 27 ҳ  ʹ�����Σ�ֻ������չ֡�� // ��ֹ��Ļ,������������
	byte_IR = Read_BYTE_2515(RXB0CTRL);

	Write_BYTE_2515(CANCTRL, 0x80); //CANCTRL�Ĵ���������������ģʽ ����DATASHEET 58ҳ
	byte_IR = Read_BYTE_2515(CANCTRL);


////Write_BYTE_2515(0x60, 0x60); //RXB0CTRL���ջ�����0 ���ƼĴ��� ����DATASHEET 27 ҳ�� ��ֹ����
    //Write_BYTE_2515(0x60, 0x64); //��������ȣ���ָ���ʾʹ�û���Ĵ���������һ���Ĵ�������֮������ת�浽�ڶ����Ĵ����У�Ҳ������ν�Ĺ���
//rxb1
    Write_BYTE_2515(RXB1CTRL, 0x42); //RXB1CTRL���ջ�����1 ���ƼĴ��� ʹ�����Σ�ֻ������չ֡��// ��ֹ����,������������
	byte_IR = Read_BYTE_2515(RXB1CTRL);
    MODIFY_2515(BFPCTRL, 0x0f, 0x0f); //BFPCTRL_RXnBF ���ſ��ƼĴ�����״̬�Ĵ��� ����DATASHEET 29 ҳ 

////���μĴ�����1--ʹ�ܸ�����λ��0--��ֹ������λ
////M0
    Write_BYTE_2515(RXM0SIDH, 0xff); //�����˲����μĴ���0��׼��ʶ���ĸ�λ,��Ҫ�����θ���λ
    Write_BYTE_2515(RXM0SIDL, 0xff); //�����˲����μĴ���0��׼��ʶ���ĵ�λ��������
    Write_BYTE_2515(RXM0EID8, 0xff); //�����˲����μĴ���0��չ��ʶ���ĸ�λ��ȫ����
    Write_BYTE_2515(RXM0EID0, 0xff); //�����˲����μĴ���0��չ��ʶ���ĵ�λ��ȫ����
////M1
    Write_BYTE_2515(RXM1SIDH, 0xf0); //�����˲����μĴ���0��׼��ʶ���ĸ�λ,��Ҫ�����θ���λ
    Write_BYTE_2515(RXM1SIDL, 0x00); //�����˲����μĴ���0��׼��ʶ���ĵ�λ��������
    Write_BYTE_2515(RXM1EID8, 0xff); //�����˲����μĴ���0��չ��ʶ���ĸ�λ��ȫ����
    Write_BYTE_2515(RXM1EID0, 0xff); //�����˲����μĴ���0��չ��ʶ���ĵ�λ��ȫ����
//
////�˲��Ĵ�����������Ҫ�˲���ID������ֻ��ĳһλ��
////RXB0 FILter
    Write_BYTE_2515(RXF0SIDH, 0xE7); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF0SIDL, 0x4B); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF0EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF0EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ

    Write_BYTE_2515(RXF1SIDH, 0xE7); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF1SIDL, 0x68); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF1EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF1EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ
//    
////RXB1 FILter
    Write_BYTE_2515(RXF2SIDH, 0xC0); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF2SIDL, 0x0A); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF2EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF2EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ

    Write_BYTE_2515(RXF3SIDH, 0xA0); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF3SIDL, 0x49); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF3EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF3EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ

    Write_BYTE_2515(RXF4SIDH, 0xC0); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF4SIDL, 0x0A); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF4EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF4EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ

    Write_BYTE_2515(RXF5SIDH, 0xA0); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��׼��ʶ���ĸ�λ
    Write_BYTE_2515(RXF5SIDL, 0xC9); //RXFnSIDL�����˲��Ĵ���0��׼��ʶ���ĵ�λ
    Write_BYTE_2515(RXF5EID8, 0xE5); //RXFnSIDH//RXF0SIDH-�����˲��Ĵ���0��չ��ʶ���ĸ�λ
    Write_BYTE_2515(RXF5EID0, 0xF4); //RXFnSIDL�����˲��Ĵ���0��չ��ʶ���ĵ�λ
    
    Write_BYTE_2515(CANCTRL, 0x00); //CAN���ƼĴ�����������ģʽ  ����Ϊ��������ģʽ
}
/*******************************************************************************************
��������: SPIInit
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  SPIInit(void)
{
//	m_hFileSpi= INVALID_HANDLE_VALUE;
//	m_hFileSpi_Eint= INVALID_HANDLE_VALUE;

/*	//��SPI����
	m_hFileSpi = CreateFile(TEXT("SPI1:"), GENERIC_READ | GENERIC_WRITE, 0,  NULL, OPEN_EXISTING, 0, 0);
	
	if ( m_hFileSpi== INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("Cannot Open spi1!"));				
	}

	//��SPI�����ж�����
	m_hFileSpi_Eint = CreateFile(TEXT("INT1:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	
	if ( m_hFileSpi== INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("Cannot Open INT1!"));				
	}
*/
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ���SPI���߷���һ���ֽ�
uint8  Write_SPI(uint8 Write_Byte)
{
	char ret;
	uint64 wbuff;
	uint64 len=1,wactlen=0;

	wbuff=Write_Byte;
//	if ( m_hFileSpi != INVALID_HANDLE_VALUE)
//	{
//		ret= ::WriteFile(m_hFileSpi, &wbuff,len ,&wactlen, NULL);
//	}
   
	return ret ;
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ���SPI���߶�ȡһ���ֽ�
uint8  Read_SPI(void)
{
	unsigned long wactlen=0,ractlen=0,len=1;
	char  rbuff=0;
	char ret;
	
//	if ( m_hFileSpi != INVALID_HANDLE_VALUE)
//	{
//		ret = ::ReadFile(m_hFileSpi, &rbuff, len, &ractlen, NULL); /* ��ȡ���� */
//		if(ractlen>0)
//		{
//		
//		}
//	}
	
    return rbuff;
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ���λָ��������³�ʼ��MCP2515 ���ڲ��Ĵ���������������ģʽ
void  Reset_2515(void)
{
    //CS_SPI = 0; //��λ
	CS_enable( LOW );
	//delay(20);
	Write_SPI(MCP2515_RESET); 
	//CS_SPI=1; 
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ�ֻ�޸ļĴ����е�ĳЩλ
//��ڲ�����Addr:�ķ�����ַ  MASK��������  dat:�����ֽ�
void  MODIFY_2515(uint8 Addr, uint8 MASK,uint8 dat)
{
    //CS_SPI = 0 ;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_BIT_MODIFY);
    Write_SPI(Addr) ;
    Write_SPI(MASK) ;
    Write_SPI(dat) ;
    //CS_SPI = 1 ;
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ���ָ����ַ��ʼ�ļĴ�����ȡ���ݡ�
uint8  Read_BYTE_2515(uint8 Addr)
{
    uint8 ret;
    //CS_SPI = 0;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_READ);
    Write_SPI(Addr);
	//delay(2000);
    ret = Read_SPI();//��һ��Ԥ��
	ret = Read_SPI();//��ʽ������
	
    //CS_SPI = 1;
	//delay(2000);
	CS_enable( HIGH );

    return(ret);
}
/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
//���ܣ�������д��ָ����ַ��ʼ�ļĴ�����
void  Write_BYTE_2515(uint8 Addr,uint8 dat)
{
    //CS_SPI = 0;
	CS_enable( LOW );
	//delay(20);
    Write_SPI(MCP2515_WRITE);
    Write_SPI(Addr);
    Write_SPI(dat);
    //CS_SPI = 1;
	//delay(2000);
	CS_enable( HIGH );
}
/*******************************************************************************************
��������: TaskCanBusSend
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  TaskCanBusSend(void)
{

}
/*******************************************************************************************
��������: TaskCanBusRev
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  TaskCanBusRev(void)
{    
    
}


/*******************************************************************************************
��������: 
��    ��: 
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  Can_Send(uint8 ID[], uint8 tx_buff[], uint8 num, uint8 Fream)
{
    uint8 i,ret;
	static uint8 err_cnt=0;

//	CSingleLock singelLock(semaphore_spi);			//�����ź���
//	singelLock.Lock();								//�ź�������

    uint16 j=0;

	byte_EFLG = Read_BYTE_2515(EFLG);				//�������?
	if(bit_RX0OVR==1 || bit_RX1OVR==1 )
	{
		Write_BYTE_2515(EFLG, 0x00);				//�������Ĵ���
	}

	byte_IR = Read_BYTE_2515(CANINTF);					//���жϼĴ���
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//����
    {
        Write_BYTE_2515(CANINTF, 0x00);				//ȫ��
		ret=3;
        //return(3); //����
    }	
	else if (bit_TI == 1)							//�����ж�
	{
	    MODIFY_2515(CANINTF, 0x04, 0x00);				//��ַ�����Σ����ݡ������ֽ�ȷ�������޸ļĴ����е���һλ��											
		ret=3;		
	}

    do 
    {
        ret = Read_BYTE_2515(TXB0CTRL);					//TXB0CTRL���ͻ�����0���ƼĴ���
        if(++j > 1000)
		{
			err_cnt++;
			//MODIFY_2515(0x30, 0x08, 0x01);				//��ַ�����Σ����ݡ������ֽ�ȷ�������޸ļĴ����е���һλ��	
			if(err_cnt>2)									//�����ʹ�������
			{						
				err_cnt=0;				
				CanInit();									//��λ�����³�ʼ��MCP2515
			}
			goto LastLine;
		}
    } while (ret&0x08);										//���ͻ����е��������Ƿ�����ϣ�

	err_cnt=0;
	ret = Read_BYTE_2515(EFLG);
	if(ret !=0)
	{
		Write_BYTE_2515(EFLG, 0);						//���ͻ�����0��׼��ʶ����λ
	}

    if (Fream==STANDARD_FRAME_TYPE)						//��׼����֡
    {
        Write_BYTE_2515(TXB0SIDH, ID[0]);				//���ͻ�����0��׼��ʶ����λ,
		ID[1]=ID[1]&0xe0;
        Write_BYTE_2515(TXB0SIDL, ID[1]);				//���ͻ�����0��׼��ʶ����λ 
        Write_BYTE_2515(TXB0DLC, num);					//���ͻ�����0���ݳ�����--------------------д�뷢�͵�������
    }
	else if (Fream==EXTEND_FRAME_TYPE)									//��չ����֡
	{        
        Write_BYTE_2515(TXB0SIDH, ID[0]);				//���ͻ�����0��׼��ʶ����λ     (���ȼ�3�ֽ�+R1+DP+PF8~PF6) 
        ID[1]=(ID[1]&0xe3)|0x08;  
        Write_BYTE_2515(TXB0SIDL, ID[1]);				//���ͻ�����0��׼��ʶ����λ     (PF5~PF1)��������
        Write_BYTE_2515(TXB0EID8, ID[2]);				//���ͻ�����0��չ��ʶ����λ		(PS8~PS1)Ŀ���ַ
        Write_BYTE_2515(TXB0EID0, ID[3]);				//���ͻ�����0��չ��ʶ����λ     (SA8~SA1)Դ��ַ
        Write_BYTE_2515(TXB0DLC, num);					//���ͻ�����0���ݳ�����--------------------д�뷢�͵�������
	}
	
  //  CS_SPI = 0;
	CS_enable( LOW );
    Write_SPI(MCP2515_WRITE);
    Write_SPI(TXB0D0);
    for(i = 0; i < num; i++)  Write_SPI(tx_buff[i]);
  //  CS_SPI = 1;  //��CS_SPIΪ�͵�ƽ������¿�������д���ֽڣ���ַָ���Զ���һ��ֱ��CS_SPI=1Ϊֹ��
	CS_enable( HIGH );
    //Write_BYTE_2515(0x35,0x08);
  //  CS_SPI = 0;
	CS_enable( LOW );
	Write_SPI(MCP_RTS_TX0);  //0x1000 0nnn�����ͱ���  nnn=001���ʾ���ͻ�����1�еı��ġ�
//	CS_SPI = 1;//��������
	CS_enable( HIGH );

LastLine: 										//���һ�У�ʲôҲ��ִ�С�

//	singelLock.Unlock();						//�ź�������
}

/*******************************************************************************************
��������: Can_Receive1
��    ��: CAN���յ�֡��������,��������RXB1������˲��Ĵ��������յ�֡
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8  Can_Receive1(uint8 ID[], uint8 rx_buff[], uint8 *ptr)
{
    uint8 i;
	uint8 tmp;
	uint8 ret=0;

//	CSingleLock singelLock(semaphore_spi);	//�����ź���
//	singelLock.Lock();	//�ź�������

    byte_IR = Read_BYTE_2515(CANINTF);			//��ȡ�ж�״̬
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//����
    {
        Write_BYTE_2515(CANINTF, 0x00);
		ret=3;
       // return(3); //����
    }    
  //   if (byte_IR == 0xe0)				//������
  //  {
  //      Write_BYTE_2515(0x2c, 0x00);
		//ret=0;
  //     // return(0);//������
  //  }
     if (bit_RI_1 == 1)					//RXB1�����ж�
    {
      //  CS_SPI = 0;
		CS_enable( LOW );
        Write_SPI(MCP2515_READ);
        Write_SPI(RXB1SIDH);

		tmp=Read_SPI();						//Ԥ��һ���ֽ�,��Ϊ���ֽ�Ϊ0XFF��Ч�ֽ�
        for(i = 0; i < 4; i++)				//��ȡID
			ID[i] = Read_SPI();
		byte_RXBnSIDL = ID[1];
		byte_RXBnDLC = Read_SPI();				//Ӧ���Ǳ�ʾ�������ݵģ�ͨ�����֪�����յ���������
        *ptr = (byte_RXBnDLC & 0x0f);			//��ȡ���ݵĸ���
		if(*ptr>8)							//��ֹ�ڴ�й©
			goto END_RXB1;
        for(i = 0; i < *ptr; i++) 
			rx_buff[i] = Read_SPI();
      //  CS_SPI = 1;
		CS_enable( HIGH );
        MODIFY_2515(CANINTF,0x02,0x00);		//����ж�
		
		if ((bit_IDE==0)&&(bit_SRR==0))				//���ձ�׼����֡��
		{
			ret=1;
			//return(1);//���ձ�׼����֡
		}
 		else if ((bit_IDE==1)&&(bit_RTR==0))		//������չ����֡��
		{
			ret=2;
			//return(2);//������չ����֡
		}
    }

	 Write_BYTE_2515(CANINTF, 0x00);

END_RXB1:

//	singelLock.Unlock();  //�ź�������
    return ret;
}

/*******************************************************************************************
��������: Can_Receive0
��    ��: CAN���ն�֡��������,��������RXB0������˲��Ĵ��������ն�֡
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8  Can_Receive0(uint8 ID[], uint8 rx_buff[], uint8 *ptr)
{	
    uint8 i;
	uint8 tmp;
	uint8 ret=0;

//	CSingleLock singelLock(semaphore_spi);	//�����ź���
//	singelLock.Lock();	//�ź�������

    byte_IR = Read_BYTE_2515(CANINTF);
    if (byte_IR == 0xff || byte_IR == 0xe0 || bit_MERRF==1 || bit_WAKIF==1 || bit_ERRIF==1)			//����
    {
        Write_BYTE_2515(CANINTF, 0x00);			//ȫ��
		ret=3;
        //return(3); //����
    }    
  
     if (bit_RI_0 == 1)						//RXB0���յ�����
    {
      //  CS_SPI = 0;
		CS_enable( LOW );
        Write_SPI(MCP2515_READ);
        Write_SPI(RXB0SIDH);
		tmp=Read_SPI();

        for(i = 0; i < 4; i++) ID[i] = Read_SPI();
		byte_RXBnSIDL = ID[1];
		byte_RXBnDLC = Read_SPI();					//Ӧ���Ǳ�ʾ�������ݵģ�ͨ�����֪�����յ���������
        *ptr = (byte_RXBnDLC & 0x0f);				//��ȡ���ݵĸ���
		if(*ptr>8)
			goto END_RXB0;
        for(i = 0; i < *ptr; i++)  rx_buff[i] = Read_SPI();
       // CS_SPI = 1;
		CS_enable( HIGH );
        MODIFY_2515(CANINTF,0x01,0x00);
		Write_BYTE_2515(CANINTF, 0x00);
		if ((bit_IDE==0)&&(bit_SRR==0))					//���ձ�׼����֡
		{	
			ret=1;
			//return(1);//���ձ�׼����֡
		}
 		else if ((bit_IDE==1)&&(bit_RTR==0))			//������չ����֡
		{
			ret=2;
			//return(2);//������չ����֡
		}
    }
   
    //Write_BYTE_2515(0x2c, 0x00);

END_RXB0:
//	singelLock.Unlock();  //�ź�������
    return ret;

}

/*******************************************************************************************
��������: Task_Init
��    ��: BMS_CAN�����ʼ��
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8  Task_Init(void)
{
	return 1;
}

/*******************************************************************************************
��������: CS_enable
��    ��: CS
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
uint8  CS_enable(uint8 enable)
{	
	uint8 ret=1;
	uint8 inbuf,outbuf;
//	DWORD len;
	
//	ret = ::DeviceIoControl(m_hFileSpi, enable, &inbuf, 0, &outbuf, 0, &len, NULL);
//	if (ret != TRUE)
//		ret=0;

	return ret;
}
/*******************************************************************************************
��������: CS_enable
��    ��: CS
�������: ��
�������: ��
��    ��: ��
********************************************************************************************/
void  delay(int us)
{
	while(--us)
	{
		;
	}
}
/*******************************************************************************************
��������: waitSpiRecciveIntr
��    ��: �ȴ�SPI�����ж�
�������: ��
�������: ��
��    ��: �� 1 �ɹ�        0 ʧ��
********************************************************************************************/
uint8  waitSpiRecciveIntr(void)
{

	unsigned long wactlen=0,ractlen=0,len=1;
	char  rbuff=0;
	char ret;
	
//	if ( m_hFileSpi_Eint != INVALID_HANDLE_VALUE)
//	{
//		ret = ::ReadFile(m_hFileSpi_Eint, &rbuff, len, &ractlen, NULL); /* ��ȡ���� */
//		if(ractlen>0)
//		{
//		
//		}
//	}

	return rbuff;
}

/*******************************************************************************************
��������: waitSpiRecciveIntr
��    ��: �ȴ�SPI�����ж�
�������: ��
�������: ��
��    ��: �� 1 �ɹ�        0 ʧ��
********************************************************************************************/
void  closeSpi(void)
{
//	if(m_hFileSpi_Eint != INVALID_HANDLE_VALUE)
//		CloseHandle(m_hFileSpi_Eint);				/* �رս����߳��˳��¼���� */	
//	if(m_hFileSpi != INVALID_HANDLE_VALUE)
//		CloseHandle(m_hFileSpi);				    /* �رս����߳��˳��¼���� */
//
//	m_hFileSpi = INVALID_HANDLE_VALUE ;
//	m_hFileSpi_Eint = INVALID_HANDLE_VALUE ;
	
}

/*******************************************************************************************
��������: BmsComAffairManage
��    ��: BMSͨ���������
�������: ��
�������: ��
��    ��: �� 
********************************************************************************************/
void  BmsComAffairManage(void)
{
	static uint8 bmsComErr_cnt=0;

	
}

/*******************************************************************************************
��������: data_callback
��    ��: ���ݴ���
�������: ��
�������: ��
��    ��: �� 
********************************************************************************************/
void  data_callback(uint8 cmd)
{
	switch(cmd)
	{	
	case 1:
		break;
	}

}

/*******************************************************************************************
��������: bms_pdu_format_init
��    ��: Э���ʽת����ת��ΪMCP2515�Ĵ������Ӧ�ı�ʶ��
�������: ��
�������: ��
��    ��: �� 
********************************************************************************************/
void  bms_pdu_format_init(void)
{
	uint8 priorty,pf,ps,sa,tmp;	

	//ID:0X1806E5F4   ������BMS����
	priorty=0x06;
	pf=0x01;
	ps=0xE5;
	sa=0xF4;
	BMS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	BMS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	BMS_ID_PGN_CTL[2]=ps;
	BMS_ID_PGN_CTL[3]=sa;


	//ID:0X1806E5F4   ������BMS����
	priorty=0x06;
	pf=0x06;
	ps=0xE5;
	sa=0xF4;
	BMS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	BMS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	BMS_ID_PGN_CTL[2]=ps;
	BMS_ID_PGN_CTL[3]=sa;

	//ID:0X18FF50E5  ��������������
	priorty=0x06;
	pf=0xFF;
	ps=0x50;
	sa=0xE5;
	tmp=(pf&0x1c);
	tmp=((pf&0x1c)<<3);
	tmp=((pf&0x1c)<<3)+(1<<3);
	tmp=(pf&0x03);
	CCS_ID_PGN_CTL[0]=((pf&0xe0)>>5)+(priorty<<5);
	CCS_ID_PGN_CTL[1]=((pf&0x1c)<<3)+(1<<3)+(pf&0x03);
	CCS_ID_PGN_CTL[2]=ps;
	CCS_ID_PGN_CTL[3]=sa;

}
/*******************************************************************************************
��������: CCS_info
��    ��: ������Ϣ
�������: ��
�������: ��
��    ��: �� 
********************************************************************************************/
void  CCS_info(void)
{
	uint32 tmp; 

	g_wzlCcsInfo.status=0;
//	g_wzlCcsInfo.out_volt=gDCS_Info.pduInfo.pdu_volt;
//	g_wzlCcsInfo.out_cur=gDCS_Info.pduInfo.pdu_curr;
//
//	tmp=(uint32)(g_wzlCcsInfo.out_volt*10);
//	wzlMsgBuf[OUT_VOL_H]= (tmp&0xff00)>>8;
//	wzlMsgBuf[OUT_VOL_L]= tmp&0xff;
//
//	tmp=(uint32)(g_wzlCcsInfo.out_cur*10);
//	wzlMsgBuf[OUT_CUR_H]= (tmp&0xff00)>>8;
//	wzlMsgBuf[OUT_CUR_L]= tmp&0xff;
//
//	wzlMsgBuf[STATUS]= g_wzlCcsInfo.status;
}
