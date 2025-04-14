#include "Encoder.h"

/*******************************************************************************
  * @file    Encoder.cpp
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

Encoder::Encoder()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_APB2PeriphClockCmd(EC11_AB_PORT_RCC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//GPIO_InitStructure.GPIO_Pin=EC11_AB_PIN;
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	//GPIO_Init(EC11_AB_PORT,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=0xffff;
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitStructure.TIM_ICFilter=EC11_SENSITIVITY;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	//TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);

	//TIM_SetCounter(TIM4,ENCODER_DEFAULE_VALUE);
	SetValue(0);
	TIM_Cmd(TIM4,ENABLE);
}
int Encoder::ReadData()
{
	int value;
	value = (int)(TIM_GetCounter(TIM4)/EC11_COUNTPERSTEP);
	value -= ENCODER_ORIGN;
	return value;
}
int Encoder::SetValue(int value)
{
	if (value <= ENCODER_VALUE_MIN || value >= ENCODER_VALUE_MAX)
	{
		return 0;
	}
	value += ENCODER_ORIGN;
	value *= EC11_COUNTPERSTEP;
	TIM_SetCounter(TIM4,value);
	return 1;
}

extern "C"
{
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_Update);
}
}

