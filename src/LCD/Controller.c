
#include"controller.h"
#include  <ucos_ii.h>
//#include "stm32f10x_it.h"
/**********************************************/
void LCD_Reset(void)
{
	OSTimeDlyHMSM(0,0,0,100);
	MCU_RST_RESET; 
	OSTimeDlyHMSM(0,0,0,10);
	MCU_RST_SET;
	OSTimeDlyHMSM(0,0,0,10);
}
/**********************************************/
void PLL_ini(void)
{

    LCD_REGWrite(0x88,0x0b);    
    Delay1ms(1);
    LCD_REGWrite(0x89,0x02);	//SYS_CLK=17.19M
    Delay1ms(1);
}	
/**********************************************/
void IC_initial(void)
{
	PLL_ini();
	Software_Reset();
	LCD_REGWrite(0x10,0x0c);	//bit[3:2]=00 256 colok,bit[1:0]=00 8bit mcu,65K,8bit MCU


/***************	Display Window	800x480 *************/	
	LCD_REGWrite(0x04,0x81);  //PCLK inverse----17.19MHZ

	//Horizontal set 		  //928
	LCD_REGWrite(0x14,0x63);  //800/HDWR//Horizontal Display Width Setting Bit[6:0] Horizontal display width(pixels) = (HDWR + 1)*8     
	LCD_REGWrite(0x15,0x00);  //0/Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR) Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0] 
	LCD_REGWrite(0x16,0x04);  //40/HNDR//Horizontal Non-Display Period Bit[4:0] Horizontal Non-Display Period (pixels) = (HNDR + 1)*8 
	LCD_REGWrite(0x17,0x04);  //40/HSTR//HSYNC Start Position[4:0] HSYNC Start Position(PCLK) = (HSTR + 1)*8
	LCD_REGWrite(0x18,0x05);  //48/HPWR//HSYNC Polarity ,The period width of HSYNC.HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8  

    //Vertical set 			  //525
	LCD_REGWrite(0x19,0xdf);  //480/VDHR0 //Vertical Display Height Bit [7:0] Vertical pixels = VDHR + 1
	LCD_REGWrite(0x1A,0x01);  //VDHR1 //Vertical Display Height Bit [8] Vertical pixels = VDHR + 1
	LCD_REGWrite(0x1B,0x1c);  //29/VNDR0 //Vertical Non-Display Period Bit [7:0] Vertical Non-Display area = (VNDR + 1) 
	LCD_REGWrite(0x1C,0x00);  //VNDR1 //Vertical Non-Display Period Bit [8] Vertical Non-Display area = (VNDR + 1) 
	LCD_REGWrite(0x1D,0x0c);  //13/VSTR0 //VSYNC Start Position[7:0] VSYNC Start Position(PCLK) = (VSTR + 1) 
	LCD_REGWrite(0x1E,0x00);  //0/VSTR1 //VSYNC Start Position[8]  VSYNC Start Position(PCLK) = (VSTR + 1) 
    LCD_REGWrite(0x1F,0x02);  //3/VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0] VSYNC Pulse Width(PCLK) = (VPWR + 1)  

	//Active window  set 
    //setting active window X
	LCD_REGWrite(0x30,0x00);  //Horizontal Start Point 0 of Active Window (HSAW0) Horizontal Start Point of Active Window [7:0]
	LCD_REGWrite(0x31,0x00);  //Horizontal Start Point 1 of Active Window (HSAW1) Horizontal Start Point of Active Window [9:8]
	LCD_REGWrite(0x34,0x1f);  //40/Horizontal End Point 0 of Active Window (HEAW0) Horizontal End Point of Active Window [7:0]
	LCD_REGWrite(0x35,0x03);  //Horizontal End Point 1 of Active Window (HEAW1) Horizontal End Point of Active Window [9:8]

    //setting active window Y
	LCD_REGWrite(0x32,0x00);  //Vertical Start Point 0 of Active Window (VSAW0) Vertical Start Point of Active Window [7:0]
	LCD_REGWrite(0x33,0x00);  //Vertical Start Point 1 of Active Window (VSAW1) Vertical Start Point of Active Window [8] 	
	LCD_REGWrite(0x36,0xdf);  //f0/Vertical End Point of Active Window 0 (VEAW0) Vertical End Point of Active Window [7:0] 
	LCD_REGWrite(0x37,0x01);  //Vertical End Point of Active Window 1 (VEAW1) Vertical End Point of Active Window [8]	

/**********************************************/
}


//6800
void LCD_CmdWrite(unsigned char r)
{
	GPIO_InitTypeDef GPIO_InitStructure; 					//声明 GPIO 的结构
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	MCU_WR_RESET;
	MCU_RS_SET;
	GPIO_Write(GPIOE, r);
	MCU_RD_RESET;
	MCU_RD_SET;
}

void LCD_DataWrite(unsigned char d)
{
	GPIO_InitTypeDef GPIO_InitStructure; 					//声明 GPIO 的结构	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	MCU_WR_RESET;
	MCU_RS_RESET;
	GPIO_Write(GPIOE, d);
	MCU_RD_RESET;
	MCU_RD_SET;
}


unsigned char LCD_DataRead(void)
{
    unsigned char d;
	GPIO_InitTypeDef GPIO_InitStructure; 					//声明 GPIO 的结构	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	MCU_WR_SET;
	MCU_RS_RESET;
	MCU_RD_RESET;
	MCU_RD_SET;
	d=(unsigned char)(GPIO_ReadInputData(GPIOE)%256);
	return d;
}

unsigned char LCD_StatusRead(void)
{
    unsigned char d;
	GPIO_InitTypeDef GPIO_InitStructure; 					//声明 GPIO 的结构	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	MCU_WR_SET;
	MCU_RS_SET;
	MCU_RD_RESET;
	MCU_RD_SET;
	d=(unsigned char)(GPIO_ReadInputData(GPIOE)%256);
	return d;
}

////////////////////////////////////

void Delay2us(unsigned char Counter)
{
	unsigned char Counter1;
	while(Counter--)
	{
		Counter1=250;
		while(Counter1--);
		Counter1=250;
		while(Counter1--);
	}

}
void Delay100us(unsigned char Counter)
{
	while(Counter--)
	{	Delay2us(50);
	}
}
void Delay1ms(unsigned char Counter)
{
	while(Counter--)
	{	
	Delay100us(10);			
	}
}
/**********************************************/
void LCD_REGWrite(unsigned char r,unsigned char d)
{
	LCD_CmdWrite(r);
	LCD_DataWrite(d);
}

//STATUS 
void Chk_Busy(void)
{
	unsigned char temp;
	do
	{
        temp=LCD_StatusRead();
	}
	while((temp&0x80)==0x80);	   
}
//REG[BFh]
void Chk_DMA_Busy(void)
{
	unsigned char temp; 	
	do
	{
		LCD_CmdWrite(0xBF);
		temp = LCD_DataRead();
	}
	while((temp&0x01)==0x01);   
}

/**********************************************/
//REG[01h]
void Display_ON(void)
{	
    LCD_REGWrite(0x01,0x80);//PWRR
}


void Software_Reset(void)
{	
    LCD_REGWrite(0x01,0x01);//PWRR
    LCD_REGWrite(0x01,0x00);//PWRR
    Delay1ms(1);// 
}							   

//REG[05h]
void Serial_ROM_select0(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD7;
	LCD_DataWrite(temp);
}

void Serial_ROM_select1(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD7;
	LCD_DataWrite(temp);
}

void Serial_ROM_Address_set_24bit(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD6;
	LCD_DataWrite(temp);
}

void Select_Serial_Waveform_mode3(void)
{	unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD5;
	LCD_DataWrite(temp);
}
void SERIAL_ROM_Read_Cycle_5bus(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
    temp |=cSetD3;
	LCD_DataWrite(temp);
}
void SERIAL_ROM_Font_mode(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD2;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_DMA_mode(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD2;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Signal_mode(void)
{   unsigned char temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
	LCD_DataWrite(temp);
}
//REG[06h]
void SROM_CLK_DIV(unsigned char CLK_DIV)
{
	LCD_CmdWrite(0x06);
    LCD_DataWrite(CLK_DIV);
}

//REG[21h] 
void CGROM_Font(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}

void External_CGROM(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp |= cSetD5 ;
	LCD_DataWrite(temp);
}
void Font_size_32x32_16x32(void) 
{
 	unsigned char temp;
	LCD_CmdWrite(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	temp |= 0x80; 
	LCD_DataWrite(temp);
}

void GT_serial_ROM_select_GT23L32S4W(void)
{ unsigned char temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x80;
  LCD_DataWrite(temp);
}
/**/
void Font_code_GB2312(void)
{ unsigned char temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  LCD_DataWrite(temp);
}
/*
void Font_code_ASCII(void)
{ unsigned char temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x10;
  LCD_DataWrite(temp);
}
*/
void Font_Standard(void)
{ unsigned char temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  LCD_DataWrite(temp);
}

/**********************************************/
//REG[30h]~REG[37h]
void Active_Window(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB)
{
    //setting active window X
  
    LCD_CmdWrite(0x30);//HSAW0
	LCD_DataWrite(XL); 
    LCD_CmdWrite(0x31);//HSAW1	   
	LCD_DataWrite(XL>>8);

    LCD_CmdWrite(0x34);//HEAW0
	LCD_DataWrite(XR);
    LCD_CmdWrite(0x35);//HEAW1	   
	LCD_DataWrite(XR>>8);

    //setting active window Y
  
    LCD_CmdWrite(0x32);//VSAW0
	LCD_DataWrite(YT);  
    LCD_CmdWrite(0x33);//VSAW1	   
	LCD_DataWrite(YT>>8);
  
    LCD_CmdWrite(0x36);//VEAW0
	LCD_DataWrite(YB); 
    LCD_CmdWrite(0x37);//VEAW1	   
	LCD_DataWrite(YB>>8);
}

//REG[46h]~REG[49h]
void XY_Coordinate(unsigned int X,unsigned int Y)
{
    LCD_CmdWrite(0x46);
	LCD_DataWrite(X);  
    LCD_CmdWrite(0x47);	   
	LCD_DataWrite(X>>8);
 
    LCD_CmdWrite(0x48);
	LCD_DataWrite(Y);  
    LCD_CmdWrite(0x49);	   
	LCD_DataWrite(Y>>8);
}
/**********************************************/
void Text_Background_Color(unsigned char setR,unsigned char setG,unsigned char setB)
{
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite(setB);
} 

void Text_Foreground_Color(unsigned char setR,unsigned char setG,unsigned char setB)
{	    
    LCD_CmdWrite(0x63);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x64);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x65);//BGCR0
	LCD_DataWrite(setB);

}


//REG[8A]
void PWM1_enable(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);  
 }
 void PWM1_fnuction_sel(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	LCD_DataWrite(temp);  
 }
 void PWM1_clock_ratio(unsigned char setx) //bit0~3 
{
	unsigned char temp,temp1;
	temp1= setx&0x0f;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	LCD_DataWrite(temp);  
 }

 void PWM1_duty_cycle(unsigned char setx) //bit0~7
{
  
    LCD_CmdWrite(0x8b);//PTNO
	LCD_DataWrite(setx);
} 	


void PWM2_enable(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);  
 }
/*
void PWM2_disable(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);  
 } */
 void PWM2_fnuction_sel(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	LCD_DataWrite(temp);  
 }
  void PWM2_clock_ratio(unsigned char setx) //bit0~3 
{
	unsigned char temp,temp1;
	temp1= setx&0x0f;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	LCD_DataWrite(temp);  
 }
 void PWM2_duty_cycle(unsigned char setx) //bit0~7
{  
    LCD_CmdWrite(0x8d);//PTNO
	LCD_DataWrite(setx);
} 	

void Memory_Clear(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= 0x80 ;
	LCD_DataWrite(temp);
	Chk_Busy(); 

}

void Clear_Full_Window(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);  
}


void Clear_Active_Window(void)
{
	unsigned char temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);  
}

//REG[B0]~REG[B3]
void DMA_Start_address_setting(unsigned long set_address)
{ 
  LCD_CmdWrite(0xB0);
  LCD_DataWrite(set_address);

  LCD_CmdWrite(0xB1);
  LCD_DataWrite(set_address>>8);

  LCD_CmdWrite(0xB2);
  LCD_DataWrite(set_address>>16);

//  LCD_CmdWrite(0xB3);
//  LCD_DataWrite(set_address>>24);

}

void DMA_block_mode_size_setting(unsigned int BWR,unsigned int BHR,unsigned int SPWR)
{
  LCD_CmdWrite(0xB4);
  LCD_DataWrite(BWR);
  LCD_CmdWrite(0xB5);
  LCD_DataWrite(BWR>>8);

  LCD_CmdWrite(0xB6);
  LCD_DataWrite(BHR);
  LCD_CmdWrite(0xB7);
  LCD_DataWrite(BHR>>8);

  LCD_CmdWrite(0xB8);
  LCD_DataWrite(SPWR);
  LCD_CmdWrite(0xB9);
  LCD_DataWrite(SPWR>>8);  
}

void DMA_Block_mode(void)
{ 
  LCD_CmdWrite(0xBF);
  LCD_DataWrite(0x02);
}

void DMA_Start_enable(void)
{  unsigned char temp;
  LCD_CmdWrite(0xBF);
  temp = LCD_DataRead();
  temp |= 0x01;
  LCD_DataWrite(temp);
}


void Show_button_DMA_function(void)
{
	Text_Background_Color(0x00,0x00,0x00);
    Text_Foreground_Color(0xff,0xff,0xff);
	Active_Window(0,Horizontal-1,0,Vertical-1);	
	Clear_Active_Window();
	Memory_Clear();

	SROM_CLK_DIV(2);    
	Serial_ROM_select1(); 
	Select_Serial_Waveform_mode3();
	SERIAL_ROM_Read_Cycle_5bus();
	SERIAL_ROM_DMA_mode();
	SERIAL_ROM_Signal_mode();
	DMA_Block_mode();

	DMA_Show_pic();
}
/********************************************/

void DMA_Show_pic(void)
{
	XY_Coordinate(0,0);
	DMA_Start_address_setting(0x000340000);
	DMA_block_mode_size_setting(800,480,800);
	DMA_Start_enable();
	Chk_DMA_Busy();

/********************************************/
	//backlight on 
	PWM1_enable();
    PWM1_fnuction_sel();
    PWM1_clock_ratio(0x08);		//bit0~3
    PWM1_duty_cycle(0xff); 		//max duty 
/********************************************/

	//OSTimeDlyHMSM(0,0,0,500);

	XY_Coordinate(0,0);
	DMA_Start_address_setting(0x00040000);
	DMA_block_mode_size_setting(800,480,800);
	DMA_Start_enable();
	Chk_DMA_Busy();
	//OSTimeDlyHMSM(0,0,0,500);

	XY_Coordinate(0,0);
	DMA_Start_address_setting(0x00100000);
	DMA_block_mode_size_setting(800,480,800);
	DMA_Start_enable();
	Chk_DMA_Busy();
	//OSTimeDlyHMSM(0,0,0,500);

	XY_Coordinate(0,0);
	DMA_Start_address_setting(0x001C0000);
	DMA_block_mode_size_setting(800,480,800);
	DMA_Start_enable();
	Chk_DMA_Busy();
	//OSTimeDlyHMSM(0,0,0,500);

	XY_Coordinate(0,0);
	DMA_Start_address_setting(0x000280000);
	DMA_block_mode_size_setting(800,480,800);
	DMA_Start_enable();
	Chk_DMA_Busy();
	//OSTimeDlyHMSM(0,0,0,500);

}


		 

