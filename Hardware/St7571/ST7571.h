#ifndef _ST7571_H
#define _ST7571_H

/*******************************************************************************
  * @file    ST7571.h
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

#define ST7571_PORT				SCREEN_PORT
#define ST7571_PORT_RCC		    SCREEN_PORT_RCC

#define ST7571_PIN              SCREEN_PIN

#define ST7571_SCK				SCREEN_SCK
#define ST7571_SDA				SCREEN_SDA

#define ST7571_DC    			SCREEN_DC
#define ST7571_RST    		    SCREEN_RST
#define ST7571_CS    			SCREEN_CS

#define ST7571_BG               SCREEN_BG

#define  ContrastRatio          0x27
#define  ContrastRatioAdujust   0x26
#define  DegreeCentigradeSign   0
#define  TemperatureSign        1

//#define USE_BLACK_BODY_ProgressBar
#if defined USE_BLACK_BODY_ProgressBar
// the start and the end of the up part.
#define FIRST_COLUMN_UP         0x7F
#define SECOUND_COLUMN_UP       0x7F
#define THIRD_COLUMN_UP         0x60
// the main of the up part.
#define FULL_COLUMN_UP          0x6F
#define EMPTY_COLUMN_UP         0x60
// the start and the end of the down part.
#define FIRST_COLUMN_DOWN       0xFD
#define SECOUND_COLUMN_DOWN     0xFD
#define THIRD_COLUMN_DOWN       0x0D
// the main of the down part.
#define FULL_COLUMN_DOWN        0xED
#define EMPTY_COLUMN_DOWN       0x0D
#endif

#define USE_THIN_BODY_ProgressBar
#if defined USE_THIN_BODY_ProgressBar
// the start and the end of the up part.
#define FIRST_COLUMN_UP         0x3F
#define SECOUND_COLUMN_UP       0x70
#define THIRD_COLUMN_UP         0x60
// the main of the up part.
#define FULL_COLUMN_UP          0x4F
#define EMPTY_COLUMN_UP         0x40
// the start and the end of the down part.
#define FIRST_COLUMN_DOWN       0xF9
#define SECOUND_COLUMN_DOWN     0x1D
#define THIRD_COLUMN_DOWN       0x0D
// the main of the down part.
#define FULL_COLUMN_DOWN        0xE5
#define EMPTY_COLUMN_DOWN       0x05
#endif

#if defined(USE_BLACK_BODY_ProgressBar)&&defined(USE_THIN_BODY_ProgressBar)
#error "only one can't be chosed."
#endif
#if !defined(USE_BLACK_BODY_ProgressBar)&&!defined(USE_THIN_BODY_ProgressBar)
#error "must choose one of up two."
#endif

class ST7571
{
private:
    void Init12896(void);
    void WriteCommand(u8 order);
    void WriteData(u8 data);

public:
    ST7571();
    void SetBackLight(u8 status);
    u8 GetBackLight();
    void Reset();
    void ClearScreen();
    void ShowChar(u8 page, u8 column, u8 content);
    void ShowMiniSign(u8 page, u8 column, u8 number);
    void ShowBigNumber(u8 page, u8 column, u8 number);
    void ShowSign(u8 page, u8 column, u8 sign);
    /*********************************************************
     * some special functions.
    *********************************************************/
    // write a data to the same page.
    void WriteLine(u8 page, u8 data);
    // progress should be % number.
    void ProgressBar(u8 page, u8 startColumn, u8 endColumn, u8 progress);
};

#endif
