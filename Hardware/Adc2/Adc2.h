#ifndef _Adc2_H
#define _Adc2_H

/*******************************************************************************
  * @file    Adc2.h
  * @author  SGW
  * @version V2.0
  * @date    20-March-2025
  * @brief   For T12 controler V2.0
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
	#include "SysTick.h"
}
#include "math.h"
#include "Configuration.h"
#include "Configuration_adv.h"

class Adc2
{
private:
	u16 GetValue(u8 ch,u8 times);
public:
	Adc2(void);
	int GetT12Current(void);
	int GetT12NTC(void);
	int GetT12Temperture(int ntc_t);
};

#endif
