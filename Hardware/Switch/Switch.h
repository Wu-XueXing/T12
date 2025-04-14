#ifndef _Switch_H
#define _Switch_H

/*******************************************************************************
  * @file    Switch.h
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
	#include "SysTick.h"
}
#include "Data.h"
#include "AT24C02.h"
#include "Configuration.h"
#include "Configuration_adv.h"

class Switch
{
private:
    /* data */
public:
    Switch(Data* data, AT24C02* eeprom, void (*func)(Data *data, AT24C02* eeprom));
    // u8 GetEC11SW();
    u8 GetHandleSW();
};

#endif
