#ifndef _AT24C02_H
#define _AT24C02_H

/*******************************************************************************
  * @file    AT24C02.h
  * @author  SGW
  * @version V1.0
  * @date    03-October-2025
  * @brief   For T12 controler V1.1
  * 
  ******************************************************************************
  * @attention
  *
  * 该代码只供交流、学习之用，未经作者允许不得用于其他任何用途。
  *
  ******************************************************************************/

extern "C"
{
	#include "System.h"
	#include "SysTick.h"
    #include "Usart.h"
}
#include "Data.h"
#include "Configuration.h"

#define DATA_LENGTH 2

class AT24C02
{
private:
    Data* _data;
    union
    {
        struct
        {
            u16 aim;
        }data;
        u8 str[DATA_LENGTH];
    }savedData, savedData_old;

    void SDA_IN();
    void SDA_OUT();
    void IIC_Start();
    void IIC_Stop();
    u8 IIC_Wait_Ack();
    void IIC_Ack();
    void IIC_NAck();
    void IIC_Send_Byte(u8 txd);
    u8 IIC_Read_Byte(u8 ack);

    u8 readOneByte(u16 readAddr);
    void writeOneByte(u16 writeAddr, u8 dataToWrite);

    void readLenByte(u8* readData, u8 len);
    void writeLenByte(u8* writeData, u8 len);

    u8 check();
public:
    AT24C02(Data *data);
    void AutoSave();
    void AutoRead();
};



#endif
