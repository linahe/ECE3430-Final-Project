/*
 * led_accel.c
 *
 *  Created on: Oct 27, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su
 */
#include <msp430.h>
#include "led_accel.h"
#include "adc_accel.h"


int XMax, XMin, YMax, YMin, ZMax, ZMin;
extern int X0, Y0, Z0, XAvg, YAvg, ZAvg;


void StartCalibration(CalibrationState myCalibrationState, SwitchDefine * mySwitch)
{
	myCalibrationState = XMaxState;
	while(myCalibrationState != CalibrationDone)
	{
		IndicateLED(myCalibrationState); //LED stuff
		WaitLowButton(mySwitch);
		WaitHighButton(mySwitch);
		StoreValue(myCalibrationState);
		myCalibrationState = GetNextCalibrationState(myCalibrationState);
	}

	//turn off all leds when done
	LightLED(0x00);

	CalculateZeros();
}

void WaitLowButton(SwitchDefine * mySwitch)
{
	while(mySwitch->CurrentValidState != Low)
		mySwitch->CurrentValidState = Debouncer(mySwitch);	//update current valid state of button
}

void WaitHighButton(SwitchDefine * mySwitch)
{
	while(mySwitch->CurrentValidState != High)
		mySwitch->CurrentValidState = Debouncer(mySwitch);	//update current valid state of button
}

//Given max and min of axis, find the middle point
void CalculateZeros()
{
	X0 = (XMax + XMin) >> 1;
	Y0 = (YMax + YMin) >> 1;
	Z0 = (ZMax + ZMin) >> 1;
}

void StoreValue(CalibrationState myCalibrationState)
{
	switch(myCalibrationState)
	{
		case XMaxState:
			XMax = XAvg;
			break;
		case XMinState:
			XMin = XAvg;
			break;
		case YMaxState:
			YMax = YAvg;
			break;
		case YMinState:
			YMin = YAvg;
			break;
		case ZMaxState:
			ZMax = ZAvg;
			break;
		case ZMinState:
			ZMin = ZAvg;
			break;
		case CalibrationDone:
			break;

	}

}

CalibrationState GetNextCalibrationState(CalibrationState myCalibrationState)
{
	switch(myCalibrationState)
	{
		case XMaxState:
			return XMinState;
		case XMinState:
			return YMaxState;
		case YMaxState:
			return YMinState;
		case YMinState:
			return ZMaxState;
		case ZMaxState:
			return ZMinState;
		case ZMinState:
			return CalibrationDone;
		case CalibrationDone:
			return CalibrationDone;
	}

	return CalibrationDone;

}


//display a specified number on the segment led screens
void LightLED(unsigned char LEDNum) {
	Send8Bits(LEDNum); //send values to the SPI
	P2OUT |= LATCH; // rising edge of latch
	P2OUT &= ~LATCH; //need to tick latch to put value on 7 segment display
}


void IndicateLED(CalibrationState myCalibrationState) {

    switch(myCalibrationState)
    {
            case XMaxState:
            	LightLED(BIT3);
				break;
            case XMinState:
            	LightLED(BIT7);
                break;
            case YMaxState:
            	LightLED(BIT5);
				break;
            case YMinState:
            	LightLED(BIT1);
				break;
            case ZMaxState:
            	LightLED(0xFF);
				break;
            case ZMinState:
            	LightLED(0xAA);
				break;
            case CalibrationDone:
				break;

    }
}

void Send8Bits(unsigned char data) { //send 8 bit value to GPIO
	unsigned int i;
	for(i = 0; i < NUMLEDS; i++) {
		P1OUT &= ~SCLK; //bring clock low
		P1OUT &= ~SIN; //default value to send is 0
		if( data & 0x80 )
			P1OUT |= SIN; //checking for a 1 in most significant bit, if there, make SI = 1 to send
		data <<= 1; //bitshift to prepare to read next value
		P1OUT |= SCLK; //bring clock high
	}
	P1OUT &= ~SIN; //"turn off" SI
}

Direction DetermineDirection(long theta) {
	if (theta >= 45  && theta <= 134) {
		return North;
	} else if (theta >= 135  && theta <= 224) {
		return West;
	} else if (theta >= 225  && theta <= 314) {
		return South;
	} else if (theta >= 315  && theta <= 360) {
		return East;
	} else { //btw 0 and 44 degrees inclusive
		return East;
	}

}

//Based off of what direction the board is tilted, light the LEDs in those directions
void LightLEDsByDirection(Direction LEDDir) {

    switch(LEDDir)
    {
            case West:
            	LightLED(BIT3);
				break;
            case North: //LED1
            	LightLED(BIT1);
				break;
            case East: //LED8 (top LED)
            	LightLED(BIT7);
                break;
            case South:
            	LightLED(BIT5);
				break;
            case Flat:
            	LightLED(0xFF);
                break;
    }
}



void BlinkLEDs()
{
	LightLED(0xFF);		//Blink LEDs twice
	__delay_cycles(500000);
	LightLED(0x00);
	__delay_cycles(500000);
	LightLED(0xFF);
	__delay_cycles(500000);
	LightLED(0x00);
	__delay_cycles(500000);
}
