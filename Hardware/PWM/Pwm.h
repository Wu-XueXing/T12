#ifndef _Pwm_H
#define _Pwm_H

/*******************************************************************************
  * @file    pwm.h
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
#include "Adc2.h"
#include "Data.h"
#include "Safty.h"
#include "Switch.h"
#include "Configuration.h"

class PWM
{
private:
	double _kp;
	double _ki;
	double _kd;

	double _errSum;
	double _errOld;
public:
	PWM(double kp, double ki, double kd);
  void SetDealFunction(Data* data, PWM* pwm, Switch* sw, Adc2* adc2, Safty* safty,
    void (*func)(Data* data, PWM* pwm, Switch* sw, Adc2* adc2, Safty* safty),
    void (*func1)(Data *data, Adc2* adc2));
	// set 1 to open, set 0 to close.
	void SetFan(u8 status);
	int Calculate(int aim, int temperature);
	void SetPWM(u16 pwm);
};

#endif
