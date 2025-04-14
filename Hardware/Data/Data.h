#ifndef _Data_H
#define _Data_H

/*******************************************************************************
  * @file    Data.h
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
	#include "System.h"
}
#include "Configuration.h"

enum Mode
{
	NO_HANDLER = -1,
	LOCKED = 0,
	WORK = 1,
	SLEEP = 2
};
enum SleepMode
{
	WAKE = 0,
	LIGHTSLEEP = 1,
	DEEPSLEEP = 2
};

class Data
{
private:
	// running time.
	u8 runningTime_temp;
	unsigned int runningTime_s;
	
public:
	Data();
	// sleep mode.
	SleepMode sleepMode;
	unsigned int sleepCount;
	// line first.
	int inputPower;
	int DCDCPower;
	// line secound.
	int aim;
	int croe;
	// line main.
	Mode mainSign;
	int t12Temperature;
	int t12NTC;
	// line third.
	int current;
	bool fan;
	// line forth.
	int p;
	bool lowPowerMode;
	// %
	int pwm;
	// loop times, do not change.
	void RecordTime();
	unsigned int GetRunningTime();
};

#endif

