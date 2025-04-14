#include "Data.h"

/*******************************************************************************
  * @file    Data.cpp
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

Data::Data()
{
	sleepMode = WAKE;
	sleepCount = 0;
	// line first.
	inputPower = 0;
	DCDCPower = 0;
	// line secound.
	aim = (int)(HEATER_MAX / 2 + HEATER_MIN / 2);
	croe = 0;
	// line main.
	mainSign = LOCKED;
	t12Temperature = 0;
	t12NTC = 0;
	// line third.
	current = 0;
	fan = false;
	// line forth.
	p = 0;
	lowPowerMode = false;
	// %
	pwm = 0;
	// running time.
	runningTime_temp = 0;
	runningTime_s = 0;
}
// function to record the running time.
void Data::RecordTime()
{
	runningTime_temp ++;
	if (runningTime_temp >= 5)
	{
		runningTime_temp = 0;
		runningTime_s += 1;
		sleepCount += 1;
	}
}
unsigned int Data::GetRunningTime()
{
	return runningTime_s;
}

