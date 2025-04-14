extern "C"
{
	#include "Usart.h"
	#include "System.h"
	#include "SysTick.h"
}
#include "Led.h"
#include "Pwm.h"
#include "Adc1.h"
#include "Adc2.h"
#include "Data.h"
#include "Safty.h"
#include "ST7571.h"
#include "Screen.h"
#include "Switch.h"
#include "AT24C02.h"
#include "Encoder.h"
#include "Configuration.h"
#include "Configuration_adv.h"

/*******************************************************************************
  * @file    main.cpp
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

// the function to open or close heater.
void EC11_switch_touched(Data *data, AT24C02 *eeprom)
{
	delay_ms(15);
#ifdef EC11_SW_PULL_UP
	if (EC11_SW == 0)
	{
		if (data->mainSign >= 0)
		{
			if (data->sleepMode == DEEPSLEEP)
			{
				data->sleepCount = 0;
				data->sleepMode = WAKE;
				if (data->mainSign == SLEEP)
				{
					data->mainSign = WORK;
				}
			}
			else
			{
				data->mainSign = (data->mainSign == LOCKED) ? WORK : LOCKED;
				data->sleepCount = 0;
				data->sleepMode = WAKE;
				// auto save the data...
				eeprom->AutoSave();
			}
		}
	}
#else
	if (EC11_SW == 1)
	{
		if (data->sleepMode == 2)
			{
				data->sleepCount = 0;
				data->sleepMode = 0;
				if (data->mainSign == 2)
				{
					data->mainSign = 1;
				}
			}
			else
			{
				data->mainSign = (data->mainSign == 0) ? 1 : 0;
				data->sleepCount = 0;
				data->sleepMode = 0;
				// auto save the data...
				eeprom->AutoSave();
			}
	}
#endif
}
// scan the t12 temperature between the heater work...
void TemperatureScan(Data *data, Adc2* adc2)
{
	if (T12PWM_SCAN_PIN == 0)
	{
		//delay_us(1800);
		delay_ms(4);
		data->t12Temperature = adc2->GetT12Temperture(data->t12NTC);
#if defined debug
		printf("%d %f\n", data->aim, (float)(data->t12Temperature)/100);
#endif
	}
	else if (T12PWM_SCAN_PIN == 1)
	{
		//delay_us(500);
		delay_ms(2);
		data->t12NTC = adc2->GetT12NTC();
		data->current = adc2->GetT12Current();
	}
}
// the function to deal how to running pwm.
void Deal(Data *data, PWM *pwm, Switch *sw, Adc2* adc2, Safty* safty)
{
	if (data->mainSign > LOCKED && data->sleepMode <= LIGHTSLEEP)
	{
		if (data->sleepMode == 1)
		{
			data->pwm = pwm->Calculate(LIGHTSLEEP_TAIM, data->t12Temperature);
		}
		else
		{
			data->pwm = pwm->Calculate(data->aim, data->t12Temperature);
		}
		pwm->SetPWM(data->pwm);
	}
	else if (T12PWM_SCAN_PIN == 1)
	{
		data->pwm = 0;
		pwm->SetPWM(data->pwm);
	}
	else
	{
		data->current = adc2->GetT12Current();
		data->t12NTC = adc2->GetT12NTC();
		data->t12Temperature = adc2->GetT12Temperture(data->t12NTC);
	}
	data->RecordTime();
	safty->FeedDog();
	// deal the sleep mode.
	if (sw->GetHandleSW() && data->sleepMode <= 1)
	{
		data->sleepCount = 0;
		data->sleepMode = WAKE;
		if (data->mainSign == SLEEP)
		{
			data->mainSign = WORK;
		}
	}
	else if (data->sleepCount >= LIGHTSLEEP_MODE && data->sleepMode == 0)
	{
		data->sleepMode = LIGHTSLEEP;
	}
	else if (data->sleepCount >= DEEPSLEEP_MODE && data->sleepMode == 1)
	{
		data->sleepMode = DEEPSLEEP;
	}
}

int main()
{
	int i = 0;

	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#if defined debug
	USART1_Init(9600);
#endif
	Data data;
	Adc1 adc1;
	Adc2 adc2;
	Led led;
	ST7571 st7571;
	Screen screen(st7571);
	Encoder encoder;
	AT24C02 eeprom(&data);
	Switch sw(&data, &eeprom, EC11_switch_touched);
	eeprom.AutoRead();
	encoder.SetValue((int)((data.aim - (HEATER_MAX / 2 + HEATER_MIN / 2)) * EC11_HEATER_DIVIDED));
	PWM pwm(KP, KI, KD);
	Safty safty(IWDG_DELAY_TIME);
	pwm.SetDealFunction(&data, &pwm, &sw, &adc2, &safty, Deal, TemperatureScan);
	
	pwm.SetFan(1);
	data.fan = true;

	while (1)
	{
		i++;
		if (i % 40 == 0)
			led.SetFlashLed(1);
		if (i % 80 == 0)
		{
			led.SetFlashLed(0);
			// get the sensor data.
			data.inputPower = adc1.GetVolateInput();
			data.DCDCPower = adc1.GetVolateAfterDCDC();
			data.croe = adc1.GetTempertureInside();
			// change the aim temperture.
			data.aim = (int)(HEATER_MAX / 2 + HEATER_MIN / 2 + encoder.ReadData() / EC11_HEATER_DIVIDED);
			if (data.aim > HEATER_MAX)
			{
				data.aim = HEATER_MAX;
				encoder.SetValue((int)((HEATER_MAX - HEATER_MIN) * EC11_HEATER_DIVIDED / 2));
			}
			if (data.aim < HEATER_MIN)
			{
				data.aim = HEATER_MIN;
				encoder.SetValue((int)((HEATER_MIN - HEATER_MAX) * EC11_HEATER_DIVIDED / 2));
			}
			if (data.sleepMode == DEEPSLEEP)
			{
				screen.EntrySleepMode();
				led.SetDeepSleepLed(1);
				delay_ms(30);
			}
			else
			{
				led.SetDeepSleepLed(0);
				// update the first line.
				screen.UpdateLineFirst(data.inputPower, data.DCDCPower);
				delay_ms(5);
				// update the secound line.
				screen.UpdateLineSecound(data.aim, data.croe);
				delay_ms(5);
				if (data.t12NTC <= MIN_NTC_TEMP + 5 || data.t12NTC >= MAX_NTC_TEMP)
				{
					if (data.mainSign >= LOCKED)
						data.mainSign = NO_HANDLER;
				}
				else if (data.mainSign == NO_HANDLER)
					data.mainSign = LOCKED;
				else if (data.mainSign > LOCKED && data.sleepMode == LIGHTSLEEP)
				{
					data.mainSign = SLEEP;
				}
				// update the main line.
				if (data.mainSign == NO_HANDLER)
				{
					screen.UpdateMainLine(0, data.mainSign);
				}
				else
				{
					screen.UpdateMainLine(data.t12Temperature, data.mainSign);
				}
				delay_ms(5);
				// update the third line.
				data.p = (int)(data.pwm * data.inputPower * data.current / 10000);
				screen.UpdateLineThird((int)(data.current * data.pwm / 100), data.fan);
				delay_ms(5);
				// get the current and calculate the output power.
				// update the forth line.
				screen.UpdateLineForth(data.p, data.lowPowerMode);
				delay_ms(5);
			}
			i = 0;
		}
		delay_ms(1);
	}
}
