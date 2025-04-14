#include "Safty.h"

/*******************************************************************************
  * @file    Safty.cpp
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

Safty::Safty(u16 time_ms)
{
	double temp = (time_ms / 1000) * 625;
  u16 temp2 = (int)temp;
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(4);
	IWDG_SetReload(temp2);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void Safty::FeedDog()
{
	IWDG_ReloadCounter();
}
