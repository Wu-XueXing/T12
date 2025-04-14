#ifndef _Screen_H
#define _Screen_H

/*******************************************************************************
  * @file    Screen.h
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
#include "ST7571.h"

class Screen
{
private:
    ST7571 _st7571;
    bool _fan;
public:
	Screen(ST7571 st7571);
	
	void UpdateLineFirst(int power, int midPower);
	void UpdateLineSecound(int aimTemperature, int creoTemperature);
    void UpdateMainLine(int temperature, int sign);
    void UpdateLineThird(int i, bool fan);
    void UpdateLineForth(int p, bool mode);

    void EntrySleepMode();
};

#endif
