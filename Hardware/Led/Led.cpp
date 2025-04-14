#include "Led.h"

/*******************************************************************************
  * @file    Led.cpp
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

Led::Led()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT ,&GPIO_InitStructure);

	#if defined LED_PULL_UP
	GPIO_SetBits(LED_PORT,LED_PIN);
	#elif defined LED_PULL_DOWN
	GPIO_ResetBits(LED_PORT,LED_PIN);
	#endif
}
void Led::SetFlashLed(u8 status)
{
	#ifdef LED_PULL_UP
	status =! status;
	#endif
	if (LED_SLEEP != status)
	{
		LED_SLEEP = status;
	}
}
void Led::SetDeepSleepLed(u8 status)
{
	#ifdef LED_PULL_UP
	status =! status;
	#endif
	if (LED_WORRING != status)
	{
		LED_WORRING = status;
	}
}
