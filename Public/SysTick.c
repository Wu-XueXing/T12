#include "SysTick.h"

static u8  _us=0;
static u16 _ms=0;

void SysTick_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	_us=SYSCLK/8;
	_ms=(u16)_us*1000;
}

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*_us;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

void delay_ms(u16 nms)
{	 		  	  
	u32 temp;
	SysTick->LOAD=(u32)nms*_ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

