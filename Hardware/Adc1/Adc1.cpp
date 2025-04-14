#include "Adc1.h"

/*******************************************************************************
  * @file    Adc1.cpp
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

Adc1::Adc1()
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	ADC_InitTypeDef		ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(POWERSCAN_PORT_RCC|RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=POWERSCAN_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(POWERSCAN_PORT,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
u16 Adc1::GetValue(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
	
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_us(15);
	}
	return temp_val/times;
}
int Adc1::GetVolateAfterDCDC()
{
	u32 adc_value;
	int temp;
 	double volate;
	adc_value=GetValue(POWERSCAN_9V,10);
	volate=adc_value*(3.3/4096)*MAGNIFICATION_9V;
	if (volate >= MAX_VOLATE)
	{
		temp = 0;
	}
	else
	{
		temp=volate*100;
	}
	return temp;
}
int Adc1::GetVolateInput()
{
	u32 adc_value;
	int temp;
 	double volate;
	adc_value=GetValue(POWERSCAN_MAIN,10);
	volate=adc_value*(3.3/4096)*MAGNIFICATION_MAIN;
	if (volate >= MAX_VOLATE * 2)
	{
		temp = 0;
	}
	else
	{
		temp=volate*100;
	}
	return temp;
}
int Adc1::GetTempertureInside()
{
	u32 adc_value;
	int temp;
 	double temperture;
	adc_value=GetValue(TEMPERATURE_INSIDE,10);
	temperture=adc_value*(3.3/4096);
	temperture=(1.43-temperture)/0.0043+25;
	temp=temperture*100;
	return temp;
}
