#ifndef _Configuration_adv_H
#define _Configuration_adv_H

/*******************************************************************************
  * @file    Configuration_adv.h
  * @author  SGW
  * @version V1.0
  * @date    01-October-2024
  * @brief   For T12 controler V1.0
  * 
  ******************************************************************************
  * @attention
  *
  * �ô���ֻ��������ѧϰ֮�ã�δ���������������������κ���;��
  *
  ******************************************************************************/

extern "C"
{
	#include "System.h"
}
#include "Configuration.h"

#define BUFFER_LENGTH   (PWM_T / 2) * 4

#if EC11_HEATER_DIVIDED < 1
#error "the value must be bigger than 0"
#endif

// about the encoder base data settings.
#define ENCODER_DEFAULE_VALUE	32768
#define ENCODER_ORIGN			(ENCODER_DEFAULE_VALUE/EC11_COUNTPERSTEP)
#define ENCODER_VALUE_MAX		ENCODER_ORIGN
#define ENCODER_VALUE_MIN		-1*ENCODER_ORIGN

#define LOW						0x0011
#define MEDIA					0x0010
#define HIGH					0x0000

#if EC11_SENSITIVITY != LOW && EC11_SENSITIVITY != MEDIA && EC11_SENSITIVITY != HIGH
#error "EC11��������ֻ��ѡ����������֮һ��"
#endif

// T12 inside temperature settings....
// The Rf is 112k, the R1 is 470.
#define Amplifier_TIME			240.0
#define ADC_TO_VOLATE(val)		(3.3 * val / 4.096)
#define T12VOLPERTEM			0.022	// mV

// about the T12 inside current and NTC in the handle.
#define T12_CV_CHANGE			(3.3/4096.0)/(T12CURRENT_RES * T12CURRENT_MAGN)
#define T0						273.15
#define Res						(T12NTC_RES_CONNECTED/T12NTC_RES_NORMAL)
#if defined(T12NTC_PULL_UP)
#define GetNTCTemp(val)			(Res*val/(4096-val))
#elif defined(T12NTC_PULL_DOWN)
#define GetNTCTemp(val)			(Res*(4096-val)/val)
#endif
#define GetTemperature(val) 	((1/(log(val)/T12NTC_B +0.00335))-T0)

/// about the NTC int t12 handl setting.
#if defined(T12NTC_PULL_UP) && defined(T12NTC_PULL_DOWN)
#error "NTCֻ��ѡ�����������������е�һ����"
#elif !defined(T12NTC_PULL_UP) && !defined(T12NTC_PULL_DOWN)
#error "NTCֻ��ѡ�����������������е�һ����"
#endif

/// about the fan setting.
#if defined(FAN_HIGH_OPEN) && defined(FAN_LOW_OPEN)
#error "����ֻ��ѡ��ߵ�ƽ�������ߵ͵�ƽ��ʼ���е�һ����"
#elif !defined(FAN_HIGH_OPEN) && !defined(FAN_LOW_OPEN)
#error "���ȱ���ѡ��ߵ�ƽ�������ߵ͵�ƽ��ʼ���е�һ����"
#endif

/// about the led setting.
#if defined(LED_PULL_UP) && defined(LED_PULL_DOWN)
#error "LEDֻ��ѡ�����������������е�һ����"
#elif !defined(LED_PULL_UP) && !defined(LED_PULL_DOWN)
#error "LED����ѡ�����������������е�һ����"
#endif

/// about the screen backlight setting.
#if defined(SCREEN_BG_HIGH_OPEN) && defined(SCREEN_BG_LOW_OPEN)
#error "��Ļ����ֻ��ѡ��ߵ�ƽ�������ߵ͵�ƽ��ʼ���е�һ����"
#elif !defined(SCREEN_BG_HIGH_OPEN) && !defined(SCREEN_BG_LOW_OPEN)
#error "��Ļ�������ѡ��ߵ�ƽ�������ߵ͵�ƽ��ʼ���е�һ����"
#endif

#if (IWDG_DELAY_TIME < 220) || (IWDG_DELAY_TIME > 6500)
#error "���Ź����ʱ�䲻��̫С������900ms�ᵼ�³������������1�����ϣ�����2�롣�����Ҳ���ܳ���6.5�룡"
#endif

#endif

