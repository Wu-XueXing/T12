#include "Screen.h"

/*******************************************************************************
  * @file    Screen.cpp
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

Screen::Screen(ST7571 st7571)
{
    _st7571 = st7571;
    _fan = true;

    _st7571.WriteLine(FIRSTLINE_PAGE,0x01);
    _st7571.WriteLine(SECOUND_PAGE,0x01);
    _st7571.WriteLine(MAINLINE_PAGE,0x01);
    _st7571.WriteLine(THIRDLINE_PAGE,0x01);
    _st7571.WriteLine(FORTHLINE_PAGE,0x01);

    _st7571.SetBackLight(1);
}

// The real volate
void Screen::UpdateLineFirst(int power, int midPower)
{
    int a, b, c, d;

    a = power / 1000;
    b = power / 100 - a * 10;
    c = power / 10 - a * 100 - b * 10;
    d = power - a * 1000 - b * 100 - c * 10;

    if (_st7571.GetBackLight() == 0)
    {
        _st7571.SetBackLight(1);
    }
    
    _st7571.ShowMiniSign(FIRSTLINE_PAGE, POWER_SIGN_START, 0);
    if(a > 0)
        _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + a));
    else
        _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 0,' ');
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 1,(u8)('0' + b));
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 2,'.');
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 3 - 4,(u8)('0' + c));
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 4 - 4,(u8)('0' + d));
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 5 - 4,'V');
    _st7571.ShowChar(FIRSTLINE_PAGE, POWER_NUMBER_START + CHAR_WIDTH * 6 - 5,'|');

    a = midPower / 1000;
    b = midPower / 100 - a * 10;
    c = midPower / 10 - a * 100 - b * 10;
    d = midPower - a * 1000 - b * 100 - c * 10;

	_st7571.ShowMiniSign(FIRSTLINE_PAGE, DCDC_SIGN_START, 1);
    _st7571.ShowChar(FIRSTLINE_PAGE, DCDC_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + b));
    _st7571.ShowChar(FIRSTLINE_PAGE, DCDC_NUMBER_START + CHAR_WIDTH * 1,'.');
    _st7571.ShowChar(FIRSTLINE_PAGE, DCDC_NUMBER_START + CHAR_WIDTH * 2 - 4,(u8)('0' + c));
    _st7571.ShowChar(FIRSTLINE_PAGE, DCDC_NUMBER_START + CHAR_WIDTH * 3 - 4,(u8)('0' + d));
    _st7571.ShowChar(FIRSTLINE_PAGE, DCDC_NUMBER_START + CHAR_WIDTH * 4 - 4,'V');
}
void Screen::UpdateLineSecound(int aimTemperature, int creoTemperature)
{
    int a, b, c;
    
    a = aimTemperature / 100;
    b = aimTemperature / 10 - a * 10;
    c = aimTemperature - a * 100 - b * 10;

    _st7571.ShowMiniSign(SECOUND_PAGE, AIM_SIGN_START, 2);
    _st7571.ShowChar(SECOUND_PAGE, AIM_SPACE_ADJUST,' ');
    if(a > 0)
        _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + a));
    else
        _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 0,' ');
    if(b == 0 && a == 0)
        _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 1,' ');
    else
        _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 1,(u8)('0' + b));
    _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 2,(u8)('0' + c));
    _st7571.ShowMiniSign(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 3, 3);
    _st7571.ShowChar(SECOUND_PAGE, AIM_NUMBER_START + CHAR_WIDTH * 6 - 10,'|');

    if (creoTemperature < 0)
        creoTemperature = 0;
    
    a = creoTemperature / 1000;
    b = creoTemperature / 100 - a * 10;

    _st7571.ShowMiniSign(SECOUND_PAGE, CREO_SIGN_START, 4);
    _st7571.ShowChar(SECOUND_PAGE, CREO_SPACE_ADJUST,' ');
    if(a > 0)
        _st7571.ShowChar(SECOUND_PAGE, CREO_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + a));
    else
        _st7571.ShowChar(SECOUND_PAGE, CREO_NUMBER_START + CHAR_WIDTH * 0,' ');
    _st7571.ShowChar(SECOUND_PAGE, CREO_NUMBER_START + CHAR_WIDTH * 1,(u8)('0' + b));
    _st7571.ShowMiniSign(SECOUND_PAGE, CREO_NUMBER_START + CHAR_WIDTH * 2, 3);
}
void Screen::UpdateMainLine(int temperature, int sign)
{
    int a, b, c, d;

    a = temperature / 10000;
    b = temperature / 1000 - a * 10;
    c = temperature / 100 - a * 100 - b * 10;
    d = temperature / 10 - a * 1000 - b * 100 - c * 10;
    //e = temperature - a * 10000 - b * 1000 - c * 100 - d * 10;

    // show nothings.
    if (sign == 1)
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_SPACE_ADJUST,13);
    // show the sleep sign.
    else if(sign == 2)
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_SPACE_ADJUST,16);
    // show the sign of no handle connected.
    else if(sign == -1)
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_SPACE_ADJUST,15);
    // show the sign of locker.
    else
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_SPACE_ADJUST,14);
    if(a > 0)
	    _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*1,a);
    else
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*1,13);
    if(b == 0 && a == 0)
	    _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*2,13);
    else
        _st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*2,b);
	_st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*3,c);
	_st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*4,10);
	_st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*5 - 8,d);
    //_st7571.ShowBigNumber(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*5 - 8,e);
    _st7571.ShowSign(MAINLINE_PAGE,MAINLINE_START+MAINCHAR_WIDTH*6 - 8,0);
}
void Screen::UpdateLineThird(int i, bool fan)
{
    int a, b, c;

    _st7571.ShowMiniSign(THIRDLINE_PAGE, I_SIGN_START, 5);
    _st7571.ProgressBar(THIRDLINE_PAGE, 16 + CHAR_WIDTH * 0,16 + CHAR_WIDTH * 5,(int)(i/MAX_CURRENT));

    a = i / 100;
    b = i / 10 - a * 10;
    c = i - a * 100 - b * 10;
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + a));
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 1,'.');
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 2 - 4,(u8)('0' + b));
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 3 - 4,(u8)('0' + c));
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 4 - 4,'A');
    _st7571.ShowChar(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 5 - 5,'|');

    if (fan)
        _fan =! _fan;
    if (_fan)
        _st7571.ShowMiniSign(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 6 - 8, 7);
    else
        _st7571.ShowMiniSign(THIRDLINE_PAGE, I_NUMBER_START + CHAR_WIDTH * 6 - 8, 8);
}
void Screen::UpdateLineForth(int p, bool mode)
{
    int a, b, c;

    _st7571.ShowMiniSign(FORTHLINE_PAGE, P_SIGN_START, 6);
    _st7571.ProgressBar(FORTHLINE_PAGE, 16 + CHAR_WIDTH * 0,16 + CHAR_WIDTH * 5,(int)(p/MAX_POWER));

    a = p / 1000;
    b = p / 100 - a * 10;
    c = p / 10 - a * 100 - b * 10;

    if (a > 0)
        _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 0,(u8)('0' + a));
    else
        _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 0,' ');
    _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 1,(u8)('0' + b));
    _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 2,'.');
    _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 3 - 4,(u8)('0' + c));
    _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 4 - 4,'W');

    _st7571.ShowChar(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 5 - 5,'|');

    if (!mode)
        _st7571.ShowMiniSign(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 6 - 8, 9);
    else
        _st7571.ShowMiniSign(FORTHLINE_PAGE, P_NUMBER_START + CHAR_WIDTH * 6 - 8, 10);
}

void Screen::EntrySleepMode()
{
    _st7571.SetBackLight(0);
    _st7571.ClearScreen();
}

