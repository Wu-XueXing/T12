#include "Pwm.h"

/*******************************************************************************
  * @file    pwm.cpp
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

PWM* _pwm;
Adc2* _adc2;
Data* _data;
Safty* _safty;
Switch* _sw;
void (*_pwm_func)(Data* data, PWM* pwm, Switch* sw, Adc2* adc2, Safty* safty);
void (*_pwm_func1)(Data *data, Adc2* adc2);

PWM::PWM(double kp, double ki, double kd)
{
  	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(T12PWM_PORT_RCC | FAN_PORT_RCC | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin=T12PWM_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(T12PWM_PORT ,&GPIO_InitStructure);

	AFIO->MAPR&=0XFFFFF3FF;

	TIM_TimeBaseInitStructure.TIM_Period=2000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200 - 1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);

    GPIO_InitStructure.GPIO_Pin=FAN_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(FAN_PORT ,&GPIO_InitStructure);
	
	#if defined FAN_HIGH_OPEN
	GPIO_ResetBits(FAN_PORT,FAN_PIN);
	#else
	GPIO_SetBits(FAN_PORT,LED_PIN);
	#endif

	GPIO_InitStructure.GPIO_Pin=T12PWM_SCAN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(T12PWM_PORT,&GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitStructure.EXTI_Line=EXTI_Line7; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

  _kp = kp;
	_ki = ki;
	_kd = kd;

	_errSum = 0;
	_errOld = 0;
}
void PWM::SetDealFunction(Data *data, PWM *pwm, Switch *sw, Adc2* adc2, Safty* safty, 
	void (*func)(Data *data, PWM *pwm, Switch *sw, Adc2* adc2, Safty* safty),
	void (*func1)(Data *data, Adc2* adc2))
{
	_data = data;
	_pwm = pwm;
	_sw = sw;
	_adc2 = adc2;
	_safty = safty;
	_pwm_func = func;
	_pwm_func1 = func1;
	SetPWM(0);
	safty->FeedDog();
}
void PWM::SetFan(u8 status)
{
    #if defined FAN_LOW_OPEN
	status =! status;
	#endif
	FAN = status;
}
int PWM::Calculate(int aim, int temperature)
{
	double output;
	double err = aim - temperature/100;
	if (err < PID_WORK_SECTION * 100)
	{
		_errSum += err;
		if (_errSum > KI_SUM_MAX)
			_errSum = KI_SUM_MAX;
		if (_errSum < KI_SUM_MIN)
			_errSum = KI_SUM_MIN;
		
		output = _kp*err + _ki*_errSum + _kd*(err - _errOld);
		_errOld = err;

		if (output > MAX_PWM)
			output = MAX_PWM;
		if (output < MIN_PWM && output != 0)
			output = MIN_PWM;
	}
	else
	{
		output = MAX_PWM;
	}
	return (int)output;
}
void PWM::SetPWM(u16 p)
{
	p = p > MAX_PWM ? MAX_PWM : p;
	if (p < MIN_PWM && p != 0)
	{
		p = MIN_PWM;
	}
	TIM_SetCompare1(TIM3, (int)(p * 20));
}
extern "C"
{
	void TIM3_IRQHandler(void)
	{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update))
		{
			// start output.
			(*_pwm_func)(_data, _pwm, _sw, _adc2, _safty);
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	void EXTI9_5_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line7)==1)
		{
			(*_pwm_func1)(_data, _adc2);
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

