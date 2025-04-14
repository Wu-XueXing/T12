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
  * �ô���ֻ��������ѧϰ֮�ã�δ���������������������κ���;��
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
