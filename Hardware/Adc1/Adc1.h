#ifndef _Adc1_H
#define _Adc1_H

/*******************************************************************************
  * @file    Adc1.h
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

extern "C"
{
	#include "System.h"
	#include "SysTick.h"
}
#include "Configuration.h"

class Adc1
{
private:
	u16 GetValue(u8 ch,u8 times);
public:
  Adc1(void);
  int GetVolateAfterDCDC(void);
  int GetVolateInput(void);
  int GetTempertureInside(void);
};

#endif
