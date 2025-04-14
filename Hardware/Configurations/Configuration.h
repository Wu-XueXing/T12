#ifndef _Configuration_H
#define _Configuration_H

/*******************************************************************************
  * @file    Configuration.h
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

/****************************************************************************
 * define the parameter setting
 ****************************************************************************/
// #define debug
// the sensitivity of ec11 must choose one of : LOW, MEDIA, HIGH
#define EC11_SENSITIVITY		MEDIA
#define EC11_COUNTPERSTEP		4
// the pid parameters.
#define KP						2.3774//5.1717//10
#define KI						0.24949//0.8941//0.25
#define KD						-3.9653//4.7295//0.15
// the no action times to entry the light sleep mode (/secounds).
#define LIGHTSLEEP_MODE			300
#define DEEPSLEEP_MODE			3*LIGHTSLEEP_MODE
#define LIGHTSLEEP_TAIM			120
/****************************************************************************
 * some important parameter settings.
 ****************************************************************************/
// about the heater temperature settings.
#define HEATER_MAX				500.0
#define HEATER_MIN				100.0
#define EC11_HEATER_DIVIDED		1

#define MAX_VOLATE				24.0
#define VOLATE_DIV				5.0
#define MAX_CURRENT				3.0			//A
#define MAX_POWER				72.0		//W
// about the NTC settings, it must be 100 times of the true value....
#define MAX_NTC_TEMP      120*100
#define MIN_NTC_TEMP      -50*100
// about the pwm output.
#define MIN_PWM					3
#define MAX_PWM					96
// about the pid.
#define PID_WORK_SECTION		5
#define KI_SUM_MAX				3000
#define KI_SUM_MIN				-3000
// about the t12 sensor software softy.
#define T12_T_MAX				550
#define T12_T_MIN				-10
/****************************************************************************
 * about the IWDG settings...
 ****************************************************************************/
// the value should be longer enough...
 #define IWDG_DELAY_TIME		2000 //ms
/****************************************************************************
 * the display settings
 ****************************************************************************/
// about the first line.
#define FIRSTLINE_PAGE			10
#define POWER_SIGN_START		0
#define POWER_NUMBER_START		POWER_SIGN_START + 16
#define DCDC_SIGN_START			74 - 3
#define DCDC_NUMBER_START		DCDC_SIGN_START + 16 + 1
#define MAIN_T_USENTC			2500	//50*100
// about the secound line.
#define SECOUND_PAGE			8
#define AIM_SIGN_START			0
#define AIM_SPACE_ADJUST		AIM_SIGN_START + 17
#define AIM_NUMBER_START		AIM_SIGN_START + 21
#define CREO_SIGN_START			74 - 3
#define CREO_SPACE_ADJUST		CREO_SIGN_START + 17
#define CREO_NUMBER_START		CREO_SIGN_START + 21
// about the main line.
#define MAINLINE_PAGE			4
#define MAINLINE_START			2
#define MAINLINE_SPACE_ADJUST	0
// about the third line.
#define THIRDLINE_PAGE			2
#define I_SIGN_START			0
#define I_NUMBER_START			65
// about the forth line.
#define FORTHLINE_PAGE			0
#define P_SIGN_START			0
#define P_NUMBER_START			65
// other settings.
#define CHAR_WIDTH				9
#define MAINCHAR_WIDTH			17
/****************************************************************************
 * define the hardware connection 
 ****************************************************************************/
// define the setting of ec11 switch.
#define EC11_SW_PULL_UP
#define EC11_SW					PAin(0)
#define EC11_SW_PORT			GPIOA
#define EC11_SW_PIN				GPIO_Pin_0
#define EC11_SW_PORT_RCC		RCC_APB2Periph_GPIOA

// define the connection of POWER SCRAN.
#define POWERSCAN_PORT			GPIOB
#define POWERSCAN_PIN			(GPIO_Pin_0|GPIO_Pin_1)
#define POWERSCAN_PORT_RCC		RCC_APB2Periph_GPIOB

// connection connected like: + -- R1 --(pin)-- R2 -- GND.
// the next two magnification is: (R1 + R2)/R2
#define MAGNIFICATION_MAIN		13
#define POWERSCAN_MAIN			ADC_Channel_8
#define MAGNIFICATION_9V		6
#define POWERSCAN_9V			ADC_Channel_9

// define the temperature inside of tem32.(connected to adc1)
#define TEMPERATURE_INSIDE		ADC_Channel_16

// define the connection of T12 SENSOR SCRAN.
#define T12SNESOR_PORT			GPIOA
#define T12SNESOR_PIN			(GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4)
#define T12SNESOR_PORT_RCC		RCC_APB2Periph_GPIOA

// define the current sensor Channel of ADC2.
// define the res to scan current and the magnification.
#define T12CURRENT_RES			0.25
#define T12CURRENT_MAGN			2
#define T12CURRENT				ADC_Channel_2

// define the NTC sensor settings.
#define T12NTC_PULL_UP
//#define T12NTC_PULL_DOWN
#define T12NTC_RES_NORMAL		10.0	//k
#define T12NTC_RES_CONNECTED	4.7		//k
#define T12NTC_B				3950
#define T12NTC					ADC_Channel_4

// define the temperature inside of T12 handle.
#define T12TEMPERATURE_DIVIDE	0.19	// adc value, must be double type.
#define T12TEMPERATURE_BASE		24.0	//must be double type.
#define T12TEMPERATURE			ADC_Channel_3

// define the PWM pin.
//#define T12PWM_LOW_OPEN
#define T12PWM_PORT				GPIOA
#define T12PWM_PIN				(GPIO_Pin_6)
#define T12PWM_PORT_RCC			RCC_APB2Periph_GPIOA
#define T12PWM_SCAN       (GPIO_Pin_7)
#define T12PWM_SCAN_PIN   PAin(7)

// define the sleep switch pin.
#define T12SW_PULL_UP
#define T12SW					PAin(1)
#define T12SW_PORT				GPIOA
#define T12SW_PIN				GPIO_Pin_1
#define T12SW_PORT_RCC			RCC_APB2Periph_GPIOA

// define the connection of IIC.
#define IIC_SDA_PORT		GPIOA
#define IIC_SDA_PIN			(GPIO_Pin_15)
#define IIC_SDA					PAout(15)
#define IIC_SDA_READ    PAin(15)

#define IIC_SCK_PORT		GPIOB
#define IIC_SCK_PIN			(GPIO_Pin_3)
#define IIC_SCK					PBout(3)
#define IIC_PORT_RCC			(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB)

// define the connection of fan.
#define FAN_HIGH_OPEN
//#define FAN_LOW_OPEN
#define FAN						PAout(8)
#define FAN_PORT				GPIOA
#define FAN_PIN					GPIO_Pin_8
#define FAN_PORT_RCC			RCC_APB2Periph_GPIOA

// define the setting of ec11 A and B.
//#define EC11_AB_PULL_UP
//#define EC11_AB_PORT			GPIOB
//#define EC11_AB_PIN				(GPIO_Pin_6|GPIO_Pin_7)
//#define EC11_AB_PORT_RCC		RCC_APB2Periph_GPIOB

// define the connection of leds.
#define LED_PULL_UP
//#define LED_PULL_DOWN
#define LED_PORT				GPIOB
#define LED_PIN					(GPIO_Pin_4|GPIO_Pin_5)
#define LED_SLEEP				PBout(4)
#define LED_WORRING				PBout(5)
#define LED_PORT_RCC			RCC_APB2Periph_GPIOB

// define the connection of screen.
#define SCREEN_PORT				GPIOB
#define SCREEN_PIN				(GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)
#define SCREEN_PORT_RCC			RCC_APB2Periph_GPIOB

#define SCREEN_BG_HIGH_OPEN
//#define SCREEN_BG_LOW_OPEN
#define SCREEN_BG				PBout(15)

#define SCREEN_CS				PBout(11)
#define SCREEN_RST				PBout(10)
#define SCREEN_DC				PBout(12)
#define SCREEN_SCK				PBout(13)
#define SCREEN_SDA				PBout(14)

#endif

