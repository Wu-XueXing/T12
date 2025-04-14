#include "ST7571.h"

/*******************************************************************************
  * @file    ST7571.cpp
  * @author  SGW
  * @version V1.0
  * @date    01-October-2024
  * @brief   For T12 controler V1.0
  * 
  ******************************************************************************
  * @attention
  *
  * 该代码只供交流、学习之用，未经作者允许不得用于其他任何用途。
  *
  ******************************************************************************/

extern "C"
{
	#include "ST7571Font.h"
}
/******************************************************************************
*  display note: top is 7bit, bottom is 0bit.
*  					screen pages are 12, page0 is on the bottom, page12 is on the top.
*  					page0 address is 0xB4, page12 address is 0xC0.
******************************************************************************/
ST7571::ST7571()
{
	GPIO_InitTypeDef  GPIO_InitStructure;  
	
	RCC_APB2PeriphClockCmd(ST7571_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin=ST7571_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ST7571_PORT,&GPIO_InitStructure);
	
	ST7571_CS = 1;
	ST7571_SCK = 1;
	ST7571_SDA = 1;
	
	SetBackLight(1);

	Reset();
	Init12896();
	ClearScreen();

	delay_ms(100);
}
void ST7571::SetBackLight(u8 status)
{
	#ifndef SCREEN_BG_HIGH_OPEN
	status =! status;
	#endif
	ST7571_BG = status;
}
u8 ST7571::GetBackLight()
{
	u8 status = ST7571_BG;
	#ifndef SCREEN_BG_HIGH_OPEN
	status =! status;
	#endif
	return status;
}

void ST7571::Reset(void)
{
	ST7571_RST = 0;
	delay_ms(100);
	ST7571_RST = 1;
	delay_ms(100);
}
void ST7571::Init12896(void)
{
	// SOFTWARE RESET.
	WriteCommand(0xE2);
	delay_ms(50);
	// inside rise voltage first step1.
	WriteCommand(0x2C);
	delay_us(50);
	// inside rise voltage first step2.
	WriteCommand(0x2E);
	delay_us(50);
	// inside rise voltage first step3.
	WriteCommand(0x2F);
	delay_us(50);
	
	// display off.
	WriteCommand(0xAE);
	// mode set.
	WriteCommand(0x38);
	// voltage raise three leave.
	WriteCommand(0xB8);
	// adc choose. shl choose.
	WriteCommand(0xA0);
	WriteCommand(0xC8);
	// SET COM0.
	WriteCommand(0x44);
	// SET THE DISPLAY ROW.
	WriteCommand(0x00);
	WriteCommand(0x40);
	WriteCommand(0x00);
	
	// OSC ON.
	WriteCommand(0xAB);
	// DC-DC VOLATE RAISE.
	WriteCommand(0x67);
	// ADJUST THE VOLATE.
	WriteCommand(ContrastRatio);
	// SET THE Reference voltage.
	WriteCommand(0x81);
	// SET THE VOLATE INSIDE.
	WriteCommand(ContrastRatioAdujust);
	
	// SET THE LCD ACTION VOLATE.
	WriteCommand(0x56);
	// SET THE LOW POWER MODE.
	WriteCommand(0xF3);
	WriteCommand(0x04);
	
	// define the show mode:
	//	row is down from top, column is to right from left.
	// start from the first row.
	WriteCommand(0xA6);
	WriteCommand(0xC0);
	WriteCommand(0xA0);
	
	// SET THE FRC AND PWM MODE.
	WriteCommand(0x93);
	// DISPLAY ON.
	WriteCommand(0xAF);
}
void ST7571::ClearScreen(void)
{
	u16 i,j;
	for(i = 0;i < 12;i++)
	{
		WriteCommand(i+0xB4);
		WriteCommand(0x10);
		WriteCommand(0x00);
		for(j = 0;j < 128;j++)
		{
			WriteData(0x00);
			WriteData(0x00);
		}
	}
}
void ST7571::ShowChar(u8 page, u8 column, u8 content)
{
	u16 j;
	content -= ' ';
	WriteCommand((page&0x0F)+0xB4);
	WriteCommand((column >> 4)|0x10);
	WriteCommand(column&0x0F);
	for(j = 8;j < 16;j++)
	{
		WriteData(ST7571_ASCII_1608[content][j]);
		WriteData(ST7571_ASCII_1608[content][j]);
	}
	delay_us(20);
	WriteCommand(((page+1)&0x0F)+0xB4);
	WriteCommand((column >> 4)|0x10);
	WriteCommand(column&0x0f);
	for(j = 0;j < 8;j++)
	{
		WriteData(ST7571_ASCII_1608[content][j]);
		WriteData(ST7571_ASCII_1608[content][j]);
	}
}
void ST7571::ShowMiniSign(u8 page, u8 column, u8 number)
{
	u16 j;
	WriteCommand((page&0x0F)+0xB4);
	WriteCommand((column >> 4)|0x10);
	WriteCommand(column&0x0F);
	for(j = 16;j < 32;j++)
	{
		WriteData(ST7571_MINI_SIGN[number][j]);
		WriteData(ST7571_MINI_SIGN[number][j]);
	}
	delay_us(20);
	WriteCommand(((page+1)&0x0F)+0xB4);
	WriteCommand((column >> 4)|0x10);
	WriteCommand(column&0x0f);
	for(j = 0;j < 16;j++)
	{
		WriteData(ST7571_MINI_SIGN[number][j]);
		WriteData(ST7571_MINI_SIGN[number][j]);
	}
}
void ST7571::ShowBigNumber(u8 page, u8 column, u8 number)
{
	u16 i, j;
	for(i = 0;i < 4;i++)
	{
		WriteCommand(((page+i)&0x0F)+0xB4);
		WriteCommand((column >> 4)|0x10);
		WriteCommand(column&0x0f);
		for(j = 48-i*16;j < 64-i*16;j++)
		{
			WriteData(ST7571_Number_3216_BLACK[number][j]);
			WriteData(ST7571_Number_3216_BLACK[number][j]);
		}
	}
}
void ST7571::ShowSign(u8 page, u8 column, u8 sign)
{
	u16 i, j;
	for(i = 0;i < 4;i++)
	{
		WriteCommand(((page+i)&0x0F)+0xB4);
		WriteCommand((column >> 4)|0x10);
		WriteCommand(column&0x0f);
		for(j = 96-i*32;j < 128-i*32;j++)
		{
			WriteData(ST7571_Sign_3232[sign][j]);
			WriteData(ST7571_Sign_3232[sign][j]);
		}
	}
}
/*********************************************************
 * some special functions.
*********************************************************/
void ST7571::WriteLine(u8 page, u8 data)
{
	u16 j;
	WriteCommand((page&0x0F)+0xB4);
	WriteCommand(0x10);
	WriteCommand(0x00);
	for(j = 0;j < 128;j++)
	{
		WriteData(data);
		WriteData(data);
	}
}
void ST7571::ProgressBar(u8 page, u8 startColumn, u8 endColumn, u8 progress)
{
	u16 j;
	int temp;
	temp = (int)((endColumn - startColumn - 6) * progress / 100);
	temp = temp < 0 ? 0 : temp;

	WriteCommand(((page+1)&0x0F)+0xB4);
	WriteCommand((startColumn >> 4)|0x10);
	WriteCommand(startColumn&0x0f);

	WriteData(FIRST_COLUMN_UP);
	WriteData(FIRST_COLUMN_UP);

	WriteData(SECOUND_COLUMN_UP);
	WriteData(SECOUND_COLUMN_UP);

	WriteData(THIRD_COLUMN_UP);
	WriteData(THIRD_COLUMN_UP);

	for(j = 0;j < temp;j++)
	{
		WriteData(FULL_COLUMN_UP);
		WriteData(FULL_COLUMN_UP);
	}

	for(j = 0; j < endColumn - startColumn - 6 - temp; j++)
	{
		WriteData(EMPTY_COLUMN_UP);
		WriteData(EMPTY_COLUMN_UP);
	}

	WriteData(THIRD_COLUMN_UP);
	WriteData(THIRD_COLUMN_UP);

	WriteData(SECOUND_COLUMN_UP);
	WriteData(SECOUND_COLUMN_UP);

	WriteData(FIRST_COLUMN_UP);
	WriteData(FIRST_COLUMN_UP);

	WriteCommand((page&0x0F)+0xB4);
	WriteCommand((startColumn >> 4)|0x10);
	WriteCommand(startColumn&0x0F);

	WriteData(FIRST_COLUMN_DOWN);
	WriteData(FIRST_COLUMN_DOWN);

	WriteData(SECOUND_COLUMN_DOWN);
	WriteData(SECOUND_COLUMN_DOWN);

	WriteData(THIRD_COLUMN_DOWN);
	WriteData(THIRD_COLUMN_DOWN);

	for(j = 0;j < temp;j++)
	{
		WriteData(FULL_COLUMN_DOWN);
		WriteData(FULL_COLUMN_DOWN);
	}

	for(j = 0; j < endColumn - startColumn - 6 - temp; j++)
	{
		WriteData(EMPTY_COLUMN_DOWN);
		WriteData(EMPTY_COLUMN_DOWN);
	}

	WriteData(THIRD_COLUMN_DOWN);
	WriteData(THIRD_COLUMN_DOWN);

	WriteData(SECOUND_COLUMN_DOWN);
	WriteData(SECOUND_COLUMN_DOWN);

	WriteData(FIRST_COLUMN_DOWN);
	WriteData(FIRST_COLUMN_DOWN);
}
/******************************************************************************
*  the following is private function.
******************************************************************************/
void ST7571::WriteCommand(u8 order)
{
	u8 i;
	ST7571_CS = 0;
	ST7571_DC = 0;
	ST7571_SCK = 1;
	ST7571_SDA = 1;
	delay_us(10);
	
	for(i = 0;i < 8;i++)
	{
		ST7571_SDA = (order&0x80)?1:0;
		delay_us(10);
		ST7571_SCK = 0;
		delay_us(10);
		ST7571_SCK = 1;
		order <<= 1;
	}
	ST7571_CS = 1;
	ST7571_SDA = 1;
	delay_us(10);
}
void ST7571::WriteData(u8 data)
{
	u8 i;
	ST7571_CS = 0;
	ST7571_DC = 1;
	ST7571_SCK = 1;
	ST7571_SDA = 1;
	delay_us(10);
	
	for(i = 0;i < 8;i++)
	{
		ST7571_SDA = (data&0x80)?1:0;
		ST7571_SCK = 0;
		delay_us(10);
		ST7571_SCK = 1;
		delay_us(10);
		data <<= 1;
	}
	ST7571_CS = 1;
	ST7571_SDA = 1;
	delay_us(10);
}
