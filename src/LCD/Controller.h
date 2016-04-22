#ifndef __CONTROLLE_H__
#define __CONTROLLE_H__
#include "stm32f10x.h"
///////////////////////////////

#define MCU_RST_SET GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define MCU_RST_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_6);

#define MCU_CS_SET GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define MCU_CS_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_7);

#define MCU_RD_SET GPIO_SetBits(GPIOB, GPIO_Pin_9);
#define MCU_RD_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_9);

#define MCU_WR_SET GPIO_SetBits(GPIOB, GPIO_Pin_8);
#define MCU_WR_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_8);

#define MCU_RS_SET GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define MCU_RS_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_5);


//===== LCD Panel Size =====
#define P800x480
#define Horizontal 800
#define Vertical 480   
/**********************************************/

#define	cSetD0		     0x01
#define	cSetD1		     0x02
#define	cSetD2		     0x04
#define	cSetD3		     0x08
#define	cSetD4		     0x10
#define	cSetD5		     0x20
#define	cSetD6		     0x40
#define	cSetD7		     0x80

#define	cClrD0		     0xfe
#define	cClrD1		     0xfd
#define	cClrD2		     0xfb
#define	cClrD3		     0xf7
#define	cClrD4		     0xef
#define	cClrD5		     0xdf
#define	cClrD6		     0xbf
#define	cClrD7		     0x7f

#define cClrD1D0         0xfc
#define cClrD1SetD0	     0x01
#define cSetD1ClrD0	     0x02
#define cSetD1D0		 0x03
#define cClrD3D2         0xf3
#define cClrD3SetD2      0x04
#define cSetD3ClrD2      0x08
#define cSetD3D2		 0x0c

#define cClrD2D1D0		 0xf8
#define cClrD2D1SetD0	 0x01
#define cClrD2SetD1ClrD0 0x02
#define cClrD2SetD1D0	 0x03
#define cSetD2ClrD1D0	 0x04
#define cSetD2ClrD1SetD0 0x05

#define cClrD6D5D4	     0x8f
#define cClrD6D5SetD4    0x10
#define cClrD6SetD5ClrD4 0x20
#define cClrD6SetD5D4	 0x30
#define cSetD6ClrD5D4	 0x40
#define cSetD6ClrD5SetD4 0x50
#define cSetD6D5ClrD4	 0x60
#define cSetD6D5D4		 0x70


///////////////////////////////////////////
#define Color65k

#define color_black   0x0000
#define color_white   0xffff
#define color_red     0xf800
#define color_green   0x07e0
#define color_blue    0x001f
#define color_yellow  color_red|color_green
#define color_cyan    color_green|color_blue
#define color_purple  color_red|color_blue

#define color_grayscale1    2113
#define color_grayscale2    2113*2
#define color_grayscale3    2113*3
#define color_grayscale4    2113*4
#define color_grayscale5    2113*5
#define color_grayscale6    2113*6
#define color_grayscale7    2113*7
#define color_grayscale8    2113*8
#define color_grayscale9    2113*9
#define color_grayscale10   2113*10
#define color_grayscale11   2113*11
#define color_grayscale12   2113*12
#define color_grayscale13   2113*13
#define color_grayscale14   2113*14
#define color_grayscale15   2113*15
#define color_grayscale16   2113*16
#define color_grayscale17   2113*17
#define color_grayscale18   2113*18
#define color_grayscale19   2113*19
#define color_grayscale20   2113*20
#define color_grayscale21   2113*21
#define color_grayscale22   2113*22
#define color_grayscale23   2113*23
#define color_grayscale24   2113*24
#define color_grayscale25   2113*25
#define color_grayscale26   2113*26
#define color_grayscale27   2113*27
#define color_grayscale28   2113*28
#define color_grayscale29   2113*29
#define color_grayscale30   2113*30


extern void LCD_Reset(void);
extern void PLL_ini(void);
extern void IC_initial(void);
extern void LCD_CmdWrite(unsigned char r);
extern void LCD_DataWrite(unsigned char d);
extern unsigned char LCD_CmdRead(void);
extern unsigned char LCD_DataRead(void);
extern unsigned char LCD_StatusRead(void);
extern void LCD_REGWrite(unsigned char r,unsigned char d);
extern unsigned char LCD_REGRead(unsigned char r);

//STATUS 
extern void Chk_Busy(void);
extern void Chk_Busy_BTE(void);
extern void Chk_Busy_DIR_Access(void);

//REG[BFh]
extern void Chk_DMA_Busy(void);

//REG[01h]
extern void Display_ON(void);
extern void Display_OFF(void);
extern void Normal_Mode_exitSleep(void);
extern void Sleep_Mode(void);
extern void Software_Reset(void);

//REG[04h]
extern void PCLK_inversion(void);
extern void PCLK_non_inversion(void);
extern void PCLK_width(unsigned char setx);

//REG[05h] 
extern void Serial_ROM_select0(void);//bit7
extern void Serial_ROM_select1(void);//bit7

extern void Serial_ROM_Address_set_24bit(void);//bit6
extern void Serial_ROM_Address_set_32bit(void);//bit6

extern void Select_Serial_Waveform_mode0(void);//bit5
extern void Select_Serial_Waveform_mode3(void);//bit5

extern void SERIAL_ROM_Read_Cycle_4bus(void); //bit4~3
extern void SERIAL_ROM_Read_Cycle_5bus(void); //bit4~3
extern void SERIAL_ROM_Read_Cycle_6bus(void); //bit4~3

extern void SERIAL_ROM_Font_mode(void); //bit2
extern void SERIAL_ROM_DMA_mode(void);  //bit2

extern void SERIAL_ROM_Signal_mode(void); //bit1~0
extern void SERIAL_ROM_Dual_mode0(void);  //bit1~0
extern void SERIAL_ROM_Dual_mode1(void);  //bit1~0

//REG[06h]
extern void SROM_CLK_DIV(unsigned char CLK_DIV);

//REG[10h]
extern void Color_256(void);
extern void Color_64K(void);
extern void MPU_8bit_Interface(void);
extern void MPU_16bit_Interface(void);

//REG[12h] 
extern unsigned char GPI_data(void);
//REG[13h] 
extern void GPO_data(unsigned char setx);

//REG[20h]
extern void One_Layer(void);
extern void Two_Layers(void);
extern void HDIR_SEG0_SEGn(void);
extern void HDIR_SEGn_SEG0(void);
extern void VDIR_COM0_COMn(void);
extern void VDIR_COMn_COM0(void);

//REG[21h] 
extern void CGROM_Font(void);
extern void CGRAM_Font(void);
extern void Internal_CGROM(void);
extern void External_CGROM(void);
extern void ISO8859_1(void);
extern void ISO8859_2(void);
extern void ISO8859_3(void);
extern void ISO8859_4(void);

//REG[22h]
extern void No_FullAlignment(void);
extern void FullAlignment(void);
extern void Font_with_BackgroundColor(void);
extern void Font_with_BackgroundTransparency(void);

extern void Horizontal_FontEnlarge_x1(void);
extern void Horizontal_FontEnlarge_x2(void);
extern void Horizontal_FontEnlarge_x3(void);
extern void Horizontal_FontEnlarge_x4(void);
extern void Vertical_FontEnlarge_x1(void);
extern void Vertical_FontEnlarge_x2(void);
extern void Vertical_FontEnlarge_x3(void);
extern void Vertical_FontEnlarge_x4(void);

//REG[23h]	
extern void CGRAM_Select_Number(unsigned char number);

//REG[24h]REG[25h]REG[26h]REG[27h]	
extern void Scroll_Offset(unsigned int X,unsigned int Y);

//REG[29h]
extern void Line_distance(unsigned char setx);

//REG[2Ah]~REG[2Dh]
extern void Font_Coordinate(unsigned int X,unsigned int Y);

//REG[2Eh]
extern void Font_size_16x16_8x16(void);
extern void Font_size_24x24_12x24(void);
extern void Font_size_32x32_16x32(void);
extern void Font_spacing_set(unsigned char setx); //bit[5:0]

//REG[2Fh]
extern void GT_serial_ROM_select_GT21L16T1W(void);
extern void GT_serial_ROM_select_GT23L16U2W(void);
extern void GT_serial_ROM_select_GT23L24T3Y(void);
extern void GT_serial_ROM_select_GT23L24M1Z(void);
extern void GT_serial_ROM_select_GT23L32S4W(void);
extern void Font_code_GB2312(void);
extern void Font_code_GB12345(void);
extern void Font_code_BIG5(void);
extern void Font_code_UNICODE(void);
extern void Font_code_ASCII(void);
extern void Font_code_UNIJIS(void);
extern void Font_code_JIS0208(void);
extern void Font_code_LATIN(void);
extern void Font_Standard(void);
extern void Font_Arial(void);
extern void Font_Roman(void);
extern void Font_Bold(void); //for ASCII

//REG[30h]~REG[37h]
extern void Active_Window(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB);

//REG[38h]~REG[3Fh]
extern void Scroll_Window(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB);

//REG[40h]
extern void Graphic_Mode(void);
extern void Text_Mode(void);
extern void Text_Cursor_Disable(void);
extern void Text_Cursor_Enable(void);
extern void Text_Cursor_Blink_Disable(void);
extern void Text_Cursor_Blink_Enable(void);
extern void Memory_Write_LeftRight_TopDown(void);
extern void Memory_Write_RightLeft_TopDown(void);
extern void Memory_Write_DownTop_LeftRight(void);
extern void Memory_Write_TopDown_LeftRight(void);
extern void MemoryWrite_Cursor_autoIncrease(void);
extern void MemoryWrite_Cursor_NoautoIncrease(void);
extern void MemoryRead_Cursor_autoIncrease(void);
extern void MemoryRead_Cursor_NoautoIncrease(void);

//REG[41h]
extern void No_Graphic_Cursor(void);
extern void Graphic_Cursor(void);
extern void Graphic_Cursor_Set1(void);
extern void Graphic_Cursor_Set2(void);
extern void Graphic_Cursor_Set3(void);
extern void Graphic_Cursor_Set4(void);
extern void Graphic_Cursor_Set5(void);
extern void Graphic_Cursor_Set6(void);
extern void Graphic_Cursor_Set7(void);
extern void Graphic_Cursor_Set8(void);
extern void Write_To_Bank1and2(void);
extern void Write_To_CGRAM(void);
extern void Write_To_GraphicCursor(void);
extern void Write_To_Pattern(void);
extern void Write_To_Bank1(void);
extern void Write_To_Bank2(void);

//REG[44h]
extern void Text_Blink_Time(unsigned char setx);

//REG[45h]  
extern void Memory_read_LeftRight_TopDown(void);
extern void Memory_read_RightLeft_TopDown(void);
extern void Memory_read_TopDown_LeftRight(void);
extern void Memory_read_DownTop_LeftRight(void);

//REG[46h]~REG[49h]
extern void XY_Coordinate(unsigned int X,unsigned int Y);

//REG[4Ah]~REG[4Dh]
extern void Memory_read_Coordinate(unsigned int X,unsigned int Y);

//REG[4Eh]
extern void Text_Cursor_Horizontal_Size(unsigned char setx);//bit[3:0]

//REG[4Fh] 
extern void Text_Cursor_Vertical_Size(unsigned char setx);//bit[3:0]

//REG[50h] 
extern void no_BTE_write(void);
extern void BTE_enable(void);
extern void BTE_contiguous_Data(void);
extern void BTE_rectangular_Data(void);

//REG[51h] 
extern void BTE_ROP_Code(unsigned char setx);

//REG[52h] 
extern void Layer1_Visible(void);
extern void Layer2_Visible(void);
extern void Transparent_Mode(void);
extern void Lighten_Overlay_Mode(void);
extern void Boolean_OR(void);
extern void Boolean_AND(void);
extern void Floating_window_mode(void);
extern void Floating_Window_transparent_with_BGTR_enable(void);
extern void Floating_Window_transparent_with_BGTR_disable(void);

extern void Layer1_2_scroll(void);
extern void Layer1_scroll(void);
extern void Layer2_scroll(void);
extern void Buffer_scroll(void);

//REG[53h] 
extern void layer2_1_transparency(unsigned char setx);

//REG[54h]~ [5Fh]
extern void BTE_Source_Destination	(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB);
extern void Source_Layer1(void);
extern void Source_Layer2(void);
extern void Destination_Layer1(void);
extern void Destination_Layer2(void);
extern void BTE_Size_setting(unsigned int X,unsigned int Y);

//REG[60h]~REG[67h]
extern void Text_color(unsigned int color);
extern void Background_color(unsigned int color);

extern void Text_Background_Color(unsigned char setR,unsigned char setG,unsigned char setB);//new 20100810
extern void Text_Foreground_Color(unsigned char setR,unsigned char setG,unsigned char setB);//new 20100810

//extern void BTE_Background_color(unsigned char setR,unsigned char setG,unsigned char setB);
//extern void BTE_Foreground_color(unsigned char setR,unsigned char setG,unsigned char setB);

extern void BTE_Background_red(unsigned char setx);
extern void BTE_Background_green(unsigned char setx);
extern void BTE_Background_blue(unsigned char setx);
extern void BTE_Foreground_red(unsigned char setx);
extern void BTE_Foreground_green(unsigned char setx);
extern void BTE_Foreground_blue(unsigned char setx);
extern void Pattern_Set_number(unsigned char setx);

extern void Pattern_Set_8x8(void);
extern void Pattern_Set_16x16(void);
extern void Pattern_Set_number(unsigned char setx);
extern void BackgroundColor_TransparentMode(unsigned char setR,unsigned char setG,unsigned char setB);

//********TP controller*******
//REG[70h]~REG[74h]
extern void Enable_TP(void);
extern void Disable_TP(void);
extern void Enable_Touch_WakeUp(void);
extern void Disable_Touch_WakeUp(void);
extern void TP_manual_mode(void);
extern void TP_auto_mode(void);
extern void TP_IDLE_mode(void);
extern void TP_event_mode(void);
extern void TP_latch_X(void);
extern void TP_latch_Y(void);
unsigned char ADC_X(void);
unsigned char ADC_Y(void);
unsigned char ADC_XY(void);

//REG[80h]~REG[85h]
extern void Graphic_Cursor_Coordinate(unsigned int X,unsigned int Y);
extern void Graphic_Cursor_Color_0(unsigned char setx);
extern void Graphic_Cursor_Color_1(unsigned char setx);

//REG[8Ah]
extern void PWM1_enable(void);
extern void PWM1_disable(void);
extern void PWM1_disable_level_low(void);
extern void PWM1_disable_level_high(void);
extern void PWM1_fnuction_sel(void);
extern void PWM1_system_clk_out(void);
extern void PWM1_clock_ratio(unsigned char setx); //bit0~3 

//REG[8Bh]
extern void PWM1_duty_cycle(unsigned char setx); //bit0~7

//REG[8Ch]
extern void PWM2_enable(void);
extern void PWM2_disable(void);
extern void PWM2_disable_level_low(void);
extern void PWM2_disable_level_high(void);
extern void PWM2_fnuction_sel(void);
extern void PWM2_system_clk_out(void);
extern void PWM2_clock_ratio(unsigned char setx); //bit0~3 
//REG[8Dh]
extern void PWM2_duty_cycle(unsigned char setx); //bit0~7

//REG[8Eh]
extern void Stop_Memory_Clear(void);
extern void Memory_Clear(void);
extern void Clear_Full_Window(void);
extern void Clear_Active_Window(void);

//REG[90h] 					
extern void Draw_line(void);
extern void Draw_square(void);
extern void Draw_square_fill(void);
extern void Draw_circle(void);
extern void Draw_circle_fill(void);
extern void Draw_Triangle(void);
extern void Draw_Triangle_fill(void);

//REG[90h]~REG[9Dh]  
extern void Geometric_Coordinate(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB);
extern void Circle_Coordinate_Radius(unsigned int X,unsigned int Y,unsigned int R);

//REG[A0h]~REG[ACh] 
extern void Draw_Ellipse(void);
extern void Draw_Ellipse_Curve(void);
extern void Draw_Ellipse_Curve_Fill(void);
extern void Draw_Ellipse_fill(void);

extern void Draw_Circle_Square(void);
extern void Draw_Circle_Square_fill(void);
extern void Draw_Ellipse_Curve_part(unsigned char setx);

extern void Ellipse_Coordinate_setting(unsigned int X,unsigned int Y,unsigned int ELL_A,unsigned int ELL_B);
extern void Circle_Square_Coordinate_setting(unsigned int XL,unsigned int XR ,unsigned int YT ,unsigned int YB ,unsigned int ELL_A,unsigned int ELL_B);
extern void Draw_Triangle_3point_Coordinate(unsigned int P1_X,unsigned int P1_Y,unsigned int P2_X,unsigned int P2_Y,unsigned int P3_X,unsigned int P3_Y);

//REG[B0]~REG[B3]
extern void DMA_Start_address_setting(unsigned long set_address);

//REG[B4]~REG[B8]
extern void DMA_Continuous_mode_size_setting(unsigned long set_size);
extern void DMA_block_mode_size_setting(unsigned int BWR,unsigned int BHR,unsigned int SPWR);

//REG[BFh]
extern void DMA_Continuous_mode(void);
extern void DMA_Block_mode(void);
extern void DMA_Start_enable(void);

//REG[C0h]
extern void Key_Scan_Enable(void);
extern void Long_Key_Scan_enable(void);
extern void Key_Scan_setting(unsigned char setx);//bit[5:0]
extern void Key_Scan_Wakeup_Function_Enable(void);
extern void Long_Key_Timing_Adjustment(unsigned char setx);

//REG[D0]
extern void Floating_window_start_point(unsigned int X,unsigned int Y);
extern void Floating_window_size(unsigned int width_X,unsigned int high_Y);
extern void Floating_window_display_point(unsigned int X,unsigned int Y);

 //REG[E0]
extern void Serial_ROM_direct_access_mode_enable(void);
extern void Serial_ROM_direct_access_mode_disable(void);
extern void Direct_access_mode_Address_32bits(unsigned long set_address);
extern void Direct_access_mode_Address_24bits(unsigned long set_address);
unsigned char Direct_access_mode_Read_Data_buffer(void);

//REG[F0h] 
extern void KEYSCAN_Interrupt_Enable(void);
extern void KEYSCAN_Interrupt_Disable(void);
extern void DMA_Interrupt_Enable(void);
extern void DMA_Interrupt_Disable(void);
extern void Touch_Panel_Interrupt_Enable(void);
extern void Touch_Panel_Interrupt_Disable(void);
extern void BTE_Interrupt_Enable(void);
extern void BTE_Interrupt_Disable(void);
extern void Font_BTE_read_write_Interrupt_Enable(void);
extern void Font_BTE_read_write_Interrupt_Disable(void);

//REG[F1h] 
extern void Clear_keyscan_interrupt(void);
//bit Keyscan_Status(void);

extern void Clear_DMA_interrupt(void);
//bit DMA_Status(void);

extern void clear_TP_interrupt(void);
//bit Touch_Status(void);

extern void Clear_BTE_interrupt(void);
//bit BTE_Status(void);

extern void Clear_Font_BTE_read_write_Interrupt(void);
//bit Font_BTE_read_write_Status(void);

extern void Chk_Wait(void);
extern void Show_String(unsigned char *str,unsigned char n);
//extern void LCD_Print_J_Str(unsigned char *ptr, int char_num);


extern void Delay1ms(unsigned char Counter);
extern void Delay100ms(unsigned char Counter);

extern void Show_button_DMA_function(void);
extern void DMA_Show_pic(void);

#endif
