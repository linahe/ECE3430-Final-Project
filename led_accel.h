/*
 * led_accel.h
 *
 *  Created on: Oct 27, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su
 *
 */

#include "debounce.h"

#ifndef LED_ACCEL_H_
#define LED_ACCEL_H_

#define SIN BIT7 //line to send data from spi (Master out, slave in)
#define BLANK BIT4
#define SCLK BIT5 //clock to tick: serial clock. synced between master and slave
#define LATCH BIT0 //PORT 2!!! XLAT is port 2 bit 0
#define NUMLEDS 8 //equals number of directions
#define ALLLEDS 0xFF
#define ALTLEDS 0x55 //light every other led
//#define HALFWAY 0x7D //hex value for halfway point in display
//#define HALFWAYDEC 125 //decimal value for halfway point in LED brightness

#define XMAXLED 3
#define XMINLED 7
#define YMAXLED 5
#define YMINLED 1

#define OFF 0

#define CONTROLPD 50
#define TIMER0PD 500//700


typedef enum
{
	XMaxState, XMinState, YMaxState, YMinState, ZMaxState, ZMinState, CalibrationDone

} CalibrationState;

typedef enum {
	Flat, North, West, South, East
} Direction;


void StartCalibration(CalibrationState myCalibrationState, SwitchDefine * mySwitch);
void WaitLowButton(SwitchDefine * mySwitch);
void WaitHighButton(SwitchDefine * mySwitch);
void StoreValue(CalibrationState myCalibrationState);
CalibrationState GetNextCalibrationState(CalibrationState myCalibrationState);
void CalculateZeros();

void LightLED(unsigned char LEDNum); //make two byte value appear on the 7 segment displays
void IndicateLED(CalibrationState myCalibrationState);
void Send8Bits(unsigned char data); //send a byte to the 7 segment display
//void SetAllLEDs(LEDStruct * myLEDStruct, int brightness);
void BlinkLEDs();

Direction DetermineDirection(long theta);
void LightLEDsByDirection(Direction LEDDir);



#endif /* LED_ACCEL_H_ */
