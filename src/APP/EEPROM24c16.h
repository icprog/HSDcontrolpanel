#include "BSP.h"

extern void I2C_SYS_PageWrite(INT8U* pBuffer, INT8U WriteAddr, INT8U NumByteToWrite);
//extern void I2C_EE_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead);
extern void I2C_SYS_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead);

extern void EE_SaveSetVol(INT16U lVol);						//�������õ�ѹ
extern void EE_SaveSetCur(INT16U lCur);						//�������õĵ���
extern void EE_SaveSetMode(INT8U lMode);					//�������õ�ģʽ
extern void EE_ReadSetVol(INT16U *ptrlVol);						//��ȡ���õ�ѹ
extern void EE_ReadSetCur(INT16U *ptrlCur);						//��ȡ���õĵ���
extern void EE_ReadSetMode(INT8U *ptrlMode);					//��ȡ���õ�ģʽ
