#include "Switch.h"

/*******************************************************************************
  * @file    Switch.cpp
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

Data* _data1;
AT24C02* _eeprom;
void (*_switch_func)(Data *data, AT24C02* eeprom);

Switch::Switch(Data* data, AT24C02* eeprom, void (*func)(Data *data, AT24C02* eeprom))
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(T12SW_PORT_RCC | EC11_SW_PORT_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = EC11_SW_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EC11_SW_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(T12SW_PORT, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
#ifdef EC11_SW_PULL_UP
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
#else
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
#endif
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    _data1 = data;
    _eeprom = eeprom;
    _switch_func = func;
}
u8 Switch::GetHandleSW()
{
    u8 result = T12SW;
#ifdef T12SW_PULL_UP
    result = !result;
#endif
//     if (result == 1)
//     {
//         delay_ms(10);
// #ifdef T12SW_PULL_UP
//         if (T12SW != 0)
//             result = 0;
// #else
//         if (T12SW != 1)
//             result = 0;
// #endif
//     }
    return result;
}
extern "C"
{
    void EXTI0_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line0) == 1)
        {
            (*_switch_func)(_data1, _eeprom);
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
