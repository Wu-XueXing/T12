#ifndef _Encoder_H
#define _Encoder_H

/*******************************************************************************
  * @file    Encoder.h
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
	#include "SysTick.h"
}
#include "Configuration.h"
#include "Configuration_adv.h"

class Encoder
{
private:

public:
    Encoder();
    int ReadData();
    int SetValue(int value);
};


#endif
