#include "AT24C02.h"

/*******************************************************************************
  * @file    AT24C02.cpp
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
AT24C02::AT24C02(Data *data)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  
	
	RCC_APB2PeriphClockCmd(IIC_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=IIC_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCK_PORT,&GPIO_InitStructure);
	
	IIC_SDA = 1;
	IIC_SCK = 1;

	_data = data;
	delay_ms(10);
}
void AT24C02::AutoSave()
{
    int temp = 0;
    savedData.data.aim = _data->aim;
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        if (savedData.str[i] != savedData_old.str[i])
        {
            temp = 1;
            break;
        }
    }
    if (check() == 1 && temp == 1)
    {
        writeLenByte(savedData.str, DATA_LENGTH);
        for (int i = 0; i < DATA_LENGTH; i++)
        {
            savedData_old.str[i] = savedData.str[i];
        }
    }
}
void AT24C02::AutoRead()
{
    if (check() == 1)
	{
		readLenByte(savedData_old.str, DATA_LENGTH);
		if (savedData_old.data.aim > HEATER_MIN && savedData_old.data.aim < HEATER_MAX)
			_data->aim = savedData_old.data.aim;
		else
			_data->aim = (int)(HEATER_MAX / 2 + HEATER_MIN / 2);
	}
#if defined debug
	printf("check result is: %d; savedData_old.aim: %d; aim is: %d\n", 
			check(), savedData_old.data.aim, _data->aim);
#endif
}
/******************************************************************************
*  the following is private function.
******************************************************************************/
/******** about the iic functions... ********/
void AT24C02::SDA_OUT()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}
void AT24C02::SDA_IN()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}
void AT24C02::IIC_Start()
{
    SDA_OUT();
	IIC_SDA = 1;
	IIC_SCK = 1;
	delay_us(5);
 	IIC_SDA = 0;
	delay_us(6);
	IIC_SCK = 0;
}
void AT24C02::IIC_Stop()
{
	SDA_OUT();
	IIC_SCK = 0;
	IIC_SDA = 0;
 	IIC_SCK = 1;
	delay_us(6);
	IIC_SDA = 1;
	delay_us(6);
}
u8 AT24C02::IIC_Wait_Ack()
{
	u8 tempTime = 0;
	
	IIC_SDA = 1;
	delay_us(1);
	SDA_IN();
	IIC_SCK = 1;
	delay_us(1);
	while(IIC_SDA_READ)
	{
		tempTime++;
		if(tempTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCK = 0;
	return 0;
}
void AT24C02::IIC_Ack()
{
	IIC_SCK = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCK = 1;
	delay_us(5);
	IIC_SCK = 0;
}
void AT24C02::IIC_NAck()
{
	IIC_SCK = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCK = 1;
	delay_us(5);
	IIC_SCK = 0;
}
void AT24C02::IIC_Send_Byte(u8 txd)
{
    u8 t;
	SDA_OUT();
    IIC_SCK = 0;
    for(t = 0; t < 8; t++)
    {
        if(( txd&0x80 ) > 0)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
        txd <<= 1;
		delay_us(2);
		IIC_SCK = 1;
		delay_us(2);
		IIC_SCK = 0;
		delay_us(2);
    }
}
u8 AT24C02::IIC_Read_Byte(u8 ack)
{
    u8 i,receive = 0;
	SDA_IN();
    for( i = 0; i < 8; i++)
	{
        IIC_SCK = 0; 
        delay_us(2);
		IIC_SCK = 1;
        receive <<= 1;
        if (IIC_SDA_READ)
            receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}
/******** about the at24c02 functions... ********/
u8 AT24C02::readOneByte(u16 readAddr)
{
    u8 temp = 0;
    IIC_Start();
	IIC_Send_Byte(0XA0 + ((readAddr/256) << 1));
	IIC_Wait_Ack();
    IIC_Send_Byte(readAddr % 256);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);
	IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);   
    IIC_Stop();
	return temp;
}
void AT24C02::writeOneByte(u16 writeAddr,u8 dataToWrite)
{
    IIC_Start();
	IIC_Send_Byte(0XA0+((writeAddr / 256) << 1));
	IIC_Wait_Ack();
    IIC_Send_Byte(writeAddr % 256);
	IIC_Wait_Ack();
	IIC_Send_Byte(dataToWrite);
	IIC_Wait_Ack();
    IIC_Stop();
	delay_ms(20);
}
void AT24C02::readLenByte(u8* readData, u8 len)
{
	for(int t = 0; t < len; t++)
	{
		readData[t] = readOneByte(t);
	}
}
void AT24C02::writeLenByte(u8* writeData, u8 len)
{  	
	for(int t = 0; t < len; t++)
	{
		writeOneByte(t, writeData[t]);
	}												    
}
u8 AT24C02::check()
{
	u8 temp;
	temp = readOneByte(255);
	if (temp == 0x36)
        return 1;
	else
	{
		writeOneByte(255,0x36);
	    temp = readOneByte(255);
		if (temp == 0x36)
            return 1;
	}
	return 0;
}
