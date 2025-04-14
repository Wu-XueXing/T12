#ifndef _Led_H
#define _Led_H

/*******************************************************************************
  * @file    Led.h
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

class Led
{
public:
	Led();
	// set 1 to open, set 0 to close.
	void SetFlashLed(u8 status);
	void SetDeepSleepLed(u8 status);
};

#endif
