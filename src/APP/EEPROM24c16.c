/**********************************************************
*	��������Ϊһ24C02
**********************************************************/
#include "Global.h"

#define EEPROM_ADDRESS 	0xA0+(1<<1)		//���ò����洢��ҳ,1 block
#define EEPROM_SYSTEM 	0xA0+(2<<1)		//ϵͳ�����洢��ҳ,2 block
#define I2C_USER		I2C1			//����ʹ�õ��ĸ�IIC����

/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : �ȴ�I2C��ֱ�������ȶ�״̬
*******************************************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  	do
  	{	I2C_GenerateSTART(I2C_USER, ENABLE);   										//������ʼ����
    	I2C_ReadRegister(I2C_USER, I2C_Register_SR1);								// Read I2C1 SR1 registe
    	I2C_Send7bitAddress(I2C_USER, EEPROM_ADDRESS, I2C_Direction_Transmitter);  	//Send EEPROM address for write
  	}while(!(I2C_ReadRegister(I2C_USER, I2C_Register_SR1) & 0x0002));	 
  	I2C_ClearFlag(I2C_USER, I2C_FLAG_AF);	  										//���AF��־λ
}
		 
/*******************************************************************************
* Function Name  : дһ���ֽ�
* 
*******************************************************************************/
void I2C_EE_ByteWrite(INT8U pBuffer, INT8U WriteAddr)
{
  	I2C_EE_WaitEepromStandbyState();
	I2C_GenerateSTART(I2C_USER, ENABLE);	 										//������ʼ����
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
* Function Name  : дһ���ַ��������16���ֽ�
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
* Description    : ��ȡһ������.
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
* 	Function Name  : дһ���ַ��������16���ֽ�,ϵͳ�������������ڴӻ���ַ��һ��
* 	ʵ�ʱ������˫�ֽ����ݣ����ô��ģʽ�����ֽ���ǰ�����ֽ��ں�
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
* Description    : ��ȡһ������.�������ڴӻ���ַ��һ��
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
* 	Function Name  : �������õ�ѹ
* 
*******************************************************************************/
void EE_SaveSetVol(INT16U lVol)
{
	INT8U Tmp[2];											//������ʱ����
	Tmp[0] = lVol/256;										//���λ
	Tmp[1] = lVol%256; 										//���λ
//	I2C_EE_PageWrite(Tmp, ADDR_SETVOL, 2);					//�������ݵ�EEPROM
}

/*******************************************************************************
* 	Function Name  : ��ȡ���õ�ѹ
* 
*******************************************************************************/
void EE_ReadSetVol(INT16U *ptrlVol)
{
	INT8U Tmp[2];											//������ʱ����
//	I2C_EE_BufferRead(Tmp, ADDR_SETVOL, 2);					//��ȡ����
	*ptrlVol = Tmp[0]*256+Tmp[1];					
}

/*******************************************************************************
* 	Function Name  : �������õ���
* 
*******************************************************************************/
void EE_SaveSetCur(INT16U lCur)
{
	INT8U Tmp[2];											//������ʱ����
	Tmp[0] = lCur/256;										//���λ
	Tmp[1] = lCur%256; 										//���λ
//	I2C_EE_PageWrite(Tmp, ADDR_SETCUR, 2);					//�������ݵ�EEPROM
}
/*******************************************************************************
* 	Function Name  : �������õ���
* 
*******************************************************************************/
void EE_ReadSetCur(INT16U *ptrlCur)
{
	INT8U Tmp[2];											//������ʱ����
//	I2C_EE_BufferRead(Tmp, ADDR_SETCUR, 2);					//��ȡ����
	*ptrlCur = Tmp[0]*256+Tmp[1];					
}
/*******************************************************************************
* 	Function Name  : ��������ģʽ
* 
*******************************************************************************/
void EE_SaveSetMode(INT8U lMode)
{
//	I2C_EE_ByteWrite(lMode, ADDR_CHARGEMODE);				//�������ݵ�EEPROM
}
/*******************************************************************************
* 	Function Name  : ��ȡ����ģʽ
* 
*******************************************************************************/
void EE_ReadSetMode(INT8U *ptrlMode)
{
//	I2C_EE_BufferRead(ptrlMode,ADDR_CHARGEMODE,1);			//��ȡ����
}


 /*********************************************************************************************************************
��������: 	EE_WriteVoltAdj          
��������: 	д��ѹУ������
�䡡��:   	��

�䡡��:   	��	                 
*********************************************************************************************************************/
void EE_WriteData(INT8U adr,INT8U *buf)
{
	INT8U Tmp[2];											//������ʱ����	
	
	switch(adr)
	{
	case ADDR_FIRSTFLAG:	 																//�洢��־��ַ
	    //*buf=0xA5;
	    I2C_EE_PageWrite(buf, ADDR_FIRSTFLAG, 1);											//�������ݵ�EEPROM
		break;
	
	}									
	
}
/*********************************************************************************************************************
��������: 	EE_ReadVoltAdj          
��������: 	��ȡ��ѹУ������
�䡡��:   	��

�䡡��:     ��                 
*********************************************************************************************************************/
void EE_ReadData(INT8U adr,INT8U *buf)
{ 	
	switch(adr)
	{
	case ADDR_FIRSTFLAG:	 																		//�洢��־��ַ
		I2C_EE_BufferRead(buf, ADDR_FIRSTFLAG, 1);													//��ȡ����
		break;

	
	}					
} 