/**********************************************************
*	操作对象为一24C02
**********************************************************/
#include "Global.h"

#define EEPROM_ADDRESS 	0xA0+(1<<1)		//设置参数存储的页,1 block
#define EEPROM_SYSTEM 	0xA0+(2<<1)		//系统参数存储的页,2 block
#define I2C_USER		I2C1			//定义使用的哪个IIC功能

/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : 等待I2C，直到处于稳定状态
*******************************************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  	do
  	{	I2C_GenerateSTART(I2C_USER, ENABLE);   										//产生开始条件
    	I2C_ReadRegister(I2C_USER, I2C_Register_SR1);								// Read I2C1 SR1 registe
    	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Transmitter);  	//Send EEPROM address for write
  	}while(!(I2C_ReadRegister(I2C_USER, I2C_Register_SR1) & 0x0002));	 
  	I2C_ClearFlag(I2C_USER, I2C_FLAG_AF);	  										//清除AF标志位
}
		 
/*******************************************************************************
* Function Name  : 写一个字节
* 
*******************************************************************************/
void I2C_EE_ByteWrite(INT8U pBuffer, INT8U WriteAddr)
{
  	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//产生起始条件
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT)); 				// Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 	//Send EEPROM address for write
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Test on EV6 and clear it 
  	I2C_SendData(I2C_USER, WriteAddr);												//Send the EEPROM's internal address to write to
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 			//Test on EV8 and clear it
  	I2C_SendData(I2C_USER, pBuffer); 												//Send the byte to be written
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Test on EV8 and clear it
  	I2C_GenerateSTOP(I2C_USER, ENABLE);  											//Send STOP condition 
}

/*******************************************************************************
* Function Name  : 写一个字符串，最大16个字节
*
*******************************************************************************/
void I2C_EE_PageWrite(INT8U* pBuffer, INT8U WriteAddr, INT8U NumByteToWrite)
{
  	if(NumByteToWrite > 16)	return ;
	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//Send START condition
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT)); 				// Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Transmitter);	  	// Send EEPROM address for write
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 	// Test on EV6 and clear it
  	I2C_SendData(I2C_USER, WriteAddr);   											//Send the EEPROM's internal address to write to
  	while(! I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			// Test on EV8 and clear it 
  	while(NumByteToWrite--)  	  													// While there is data to be written
  	{
    	I2C_SendData(I2C_USER, *pBuffer); 											//Send the current byte
    	pBuffer++; 		 															//Point to the next byte to be written
    	while (!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  		//Test on EV8 and clear it
  	}
  	I2C_GenerateSTOP(I2C_USER, ENABLE);												//Send STOP condition
}

/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : 读取一个数组.
*******************************************************************************/
void I2C_EE_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead)
{  
  	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	   										//Send START condition 
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT));					//Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Transmitter);		//Send EEPROM address for write
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Test on EV6 and clear it 
  	I2C_Cmd(I2C_USER, ENABLE);  													//Clear EV6 by setting again the PE bit
  	I2C_SendData(I2C_USER, ReadAddr);  												//Send the EEPROM's internal address to write to
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Test on EV8 and clear it
  	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//Send STRAT condition a second time
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT)); 				//Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Receiver);			//Send EEPROM address for read
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); 		//Test on EV6 and clear it
  	while(NumByteToRead)  															//While there is data to be read
  	{
    	if(NumByteToRead == 1)
    	{
			I2C_AcknowledgeConfig(I2C_USER, DISABLE);	 							//Disable Acknowledgement
      		I2C_GenerateSTOP(I2C_USER, ENABLE); 									// Send STOP Condition
    	}
    	if(I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_RECEIVED)) 				//Test on EV7 and clear it
    	{      
      		*pBuffer = I2C_ReceiveData(I2C_USER);									//Read a byte from the EEPROM
     		pBuffer++; 					//Point to the next location where the byte read will be saved
      		NumByteToRead--;      		//Decrement the read bytes counter  
    	}   
  	}
  	I2C_AcknowledgeConfig(I2C_USER, ENABLE);//Enable Acknowledgement to be ready for another reception
}

/*******************************************************************************
* 	Function Name  : 写一个字符串，最大16个字节,系统参数，区别在于从机地址不一样
* 	实际保存的是双字节数据，采用打断模式，高字节在前，低字节在后
*******************************************************************************/
void I2C_SYS_PageWrite(INT8U* pBuffer, INT8U WriteAddr, INT8U NumByteToWrite)
{
  	if(NumByteToWrite >  16)	return ;
	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//Send START condition
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT)); 				// Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_SYSTEM, I2C_Direction_Transmitter);	  	// Send EEPROM address for write
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 	// Test on EV6 and clear it
  	I2C_SendData(I2C_USER, WriteAddr);   											//Send the EEPROM's internal address to write to
  	while(! I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			// Test on EV8 and clear it 
  	while(NumByteToWrite--)  	  													// While there is data to be written
  	{
    	I2C_SendData(I2C_USER, *pBuffer); 											//Send the current byte
    	pBuffer++; 		 															//Point to the next byte to be written
    	while (!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  		//Test on EV8 and clear it
  	}
  	I2C_GenerateSTOP(I2C_USER, ENABLE);												//Send STOP condition
}



/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : 读取一个数组.区别在于从机地址不一样
*******************************************************************************/
void I2C_SYS_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead)
{  
  	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	   										//Send START condition 
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT));					//Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_SYSTEM, I2C_Direction_Transmitter);		//Send EEPROM address for write
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Test on EV6 and clear it 
  	I2C_Cmd(I2C_USER, ENABLE);  													//Clear EV6 by setting again the PE bit
  	I2C_SendData(I2C_USER, ReadAddr);  												//Send the EEPROM's internal address to write to
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Test on EV8 and clear it
  	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//Send STRAT condition a second time
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_MODE_SELECT)); 				//Test on EV5 and clear it
  	I2C_Send7bitAddress(I2C_USER, EEPROM_SYSTEM, I2C_Direction_Receiver);			//Send EEPROM address for read
  	while(!I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); 		//Test on EV6 and clear it
  	while(NumByteToRead)  															//While there is data to be read
  	{
    	if(NumByteToRead == 1)
    	{
			I2C_AcknowledgeConfig(I2C_USER, DISABLE);	 							//Disable Acknowledgement
      		I2C_GenerateSTOP(I2C_USER, ENABLE); 									// Send STOP Condition
    	}
    	if(I2C_CheckEvent(I2C_USER, I2C_EVENT_MASTER_BYTE_RECEIVED)) 				//Test on EV7 and clear it
    	{      
      		*pBuffer = I2C_ReceiveData(I2C_USER);									//Read a byte from the EEPROM
     		pBuffer++; 					//Point to the next location where the byte read will be saved
      		NumByteToRead--;      		//Decrement the read bytes counter  
    	}   
  	}
  	I2C_AcknowledgeConfig(I2C_USER, ENABLE);//Enable Acknowledgement to be ready for another reception
}


/*******************************************************************************
* 	Function Name  : 保存设置电压
* 
*******************************************************************************/
void EE_SaveSetVol(INT16U lVol)
{
	INT8U Tmp[2];											//定义临时变量
	Tmp[0] = lVol/256;										//存高位
	Tmp[1] = lVol%256; 										//存低位
//	I2C_EE_PageWrite(Tmp, ADDR_SETVOL, 2);					//保存数据到EEPROM
}

/*******************************************************************************
* 	Function Name  : 读取设置电压
* 
*******************************************************************************/
void EE_ReadSetVol(INT16U *ptrlVol)
{
	INT8U Tmp[2];											//定义临时变量
//	I2C_EE_BufferRead(Tmp, ADDR_SETVOL, 2);					//读取数据
	*ptrlVol = Tmp[0]*256+Tmp[1];					
}

/*******************************************************************************
* 	Function Name  : 保存设置电流
* 
*******************************************************************************/
void EE_SaveSetCur(INT16U lCur)
{
	INT8U Tmp[2];											//定义临时变量
	Tmp[0] = lCur/256;										//存高位
	Tmp[1] = lCur%256; 										//存低位
//	I2C_EE_PageWrite(Tmp, ADDR_SETCUR, 2);					//保存数据到EEPROM
}
/*******************************************************************************
* 	Function Name  : 保存设置电流
* 
*******************************************************************************/
void EE_ReadSetCur(INT16U *ptrlCur)
{
	INT8U Tmp[2];											//定义临时变量
//	I2C_EE_BufferRead(Tmp, ADDR_SETCUR, 2);					//读取数据
	*ptrlCur = Tmp[0]*256+Tmp[1];					
}
/*******************************************************************************
* 	Function Name  : 保存设置模式
* 
*******************************************************************************/
void EE_SaveSetMode(INT8U lMode)
{
//	I2C_EE_ByteWrite(lMode, ADDR_CHARGEMODE);				//保存数据到EEPROM
}
/*******************************************************************************
* 	Function Name  : 读取设置模式
* 
*******************************************************************************/
void EE_ReadSetMode(INT8U *ptrlMode)
{
//	I2C_EE_BufferRead(ptrlMode,ADDR_CHARGEMODE,1);			//读取数据
}


 /*********************************************************************************************************************
函数名称: 	EE_WriteVoltAdj          
功能描述: 	写电压校正参数
输　入:   	无

输　出:   	无	                 
*********************************************************************************************************************/
void EE_WriteData(INT8U adr,INT8U *buf)
{
	INT8U Tmp[2];											//定义临时变量	
	
	switch(adr)
	{
	case ADDR_FIRSTFLAG:	 																//存储标志地址
	    //*buf=0xA5;
	    I2C_EE_PageWrite(buf, ADDR_FIRSTFLAG, 1);											//保存数据到EEPROM
		break;
	
	}									
	
}
/*********************************************************************************************************************
函数名称: 	EE_ReadVoltAdj          
功能描述: 	读取电压校正参数
输　入:   	无

输　出:     无                 
*********************************************************************************************************************/
void EE_ReadData(INT8U adr,INT8U *buf)
{ 	
	switch(adr)
	{
	case ADDR_FIRSTFLAG:	 																		//存储标志地址
		I2C_EE_BufferRead(buf, ADDR_FIRSTFLAG, 1);													//读取数据
		break;

	
	}					
} 