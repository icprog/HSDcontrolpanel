#include "BSP.h"

extern void I2C_SYS_PageWrite(INT8U* pBuffer, INT8U WriteAddr, INT8U NumByteToWrite);
//extern void I2C_EE_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead);
extern void I2C_SYS_BufferRead(INT8U* pBuffer, INT8U ReadAddr, INT8U NumByteToRead);

extern void EE_SaveSetVol(INT16U lVol);						//保存设置电压
extern void EE_SaveSetCur(INT16U lCur);						//保存设置的电流
extern void EE_SaveSetMode(INT8U lMode);					//保存设置的模式
extern void EE_ReadSetVol(INT16U *ptrlVol);						//读取设置电压
extern void EE_ReadSetCur(INT16U *ptrlCur);						//读取设置的电流
extern void EE_ReadSetMode(INT8U *ptrlMode);					//读取设置的模式
