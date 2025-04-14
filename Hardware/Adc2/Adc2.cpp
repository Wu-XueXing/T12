#include "Adc2.h"

/*******************************************************************************
  * @file    Adc2.cpp
  * @author  SGW
  * @version V2.0
  * @date    20-March-2025
  * @brief   For T12 controler V2.0
  * 
  ******************************************************************************
  * @attention
  *
  * 该代码只供交流、学习之用，未经作者允许不得用于其他任何用途。
  *
  ******************************************************************************/

Adc2::Adc2(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	ADC_InitTypeDef		ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(T12SNESOR_PORT_RCC|RCC_APB2Periph_ADC2,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=T12SNESOR_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(T12SNESOR_PORT,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	
	ADC_Cmd(ADC2, ENABLE);
	
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));

	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}
u16 Adc2::GetValue(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_239Cycles5);
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
		temp_val+=ADC_GetConversionValue(ADC2);
		delay_us(15);
	}
	return temp_val/times;
}
int Adc2::GetT12Current()
{
	u32 adc_value;
	int temp;
 	double current;
	adc_value=GetValue(T12CURRENT,10);
	current=adc_value*T12_CV_CHANGE;
	if (current >= MAX_CURRENT * 2)
	{
		current = 0;
	}
	else
	{
		temp=current*100;
	}
	return temp;
}
int Adc2::GetT12NTC(void)
{
	u32 adc_value;
 	double temperture;
	adc_value=GetValue(T12NTC,10);
	temperture=GetNTCTemp(adc_value);
	temperture=GetTemperature(temperture);
	return (int)(temperture * 100);
}
int Adc2::GetT12Temperture(int ntc_t)
{
	u32 adc_value;
	int temp;
 	double temperture;
	adc_value=GetValue(T12TEMPERATURE,20);
	temperture = ADC_TO_VOLATE(adc_value);
	temperture = (temperture / Amplifier_TIME) / T12VOLPERTEM;
	if (temperture >= T12_T_MAX || temperture <= T12_T_MIN || ntc_t >= MAX_NTC_TEMP || ntc_t <= MIN_NTC_TEMP)
	{
		temperture = 0;
	}
	else
	{
		temperture += ntc_t / 100;
	}
	temp=temperture*100;
	return temp;
}
