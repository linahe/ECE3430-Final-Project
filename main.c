#include <msp430.h> 
#include "init.h"

/*
 * main.c
 *
 * Authors: Deanna Buttaro dlb3un, Lina He lh3su
 *
 * This program works as an electronic level using an ADC to take readings of how the board is positioned.
 * The board must be calibrated by indicating the x,y,z max and mins, then the LEDs that are in the direction
 * of the tilt will light up in varying brightness depending on the severity of tilt.
 */

#define TOLERANCE 15 //how many degrees of tolerance for the board to be considered level


Game GameObj;
LEDStruct LEDControl; //overall info stored about which leds to light and what brightness, etc
unsigned char CurrentLEDCodeValue; //as interrupts progress, only light LEDs whose duty cycles have not been reached yet
int i;
CalibrationState myCalibrationState; //used at beginning to calibrate board
int X0, Y0, Z0, XAvg, YAvg, ZAvg; //used as externs in led_accel.c to calculate 0s
calculations calc;

volatile int theta, phi; //for debugging
int main(void) {

	InitializeHardware(&LEDControl); //set up ports, timers, interrupts
	ConfigureADC(); //get ADC set up to start reading values

	BlinkLEDs(&LEDControl);

	myCalibrationState = XMaxState; //change to init later?
	StartCalibration(myCalibrationState, &gPushButton, &LEDControl);

	LightLED(LEDControl.LEDStatus);

	//after calibration finished, need user to press button to indicate they are ready to begin game
	BlinkLEDs(&LEDControl);

	//METHOD HERE TO WAIT FOR BUTTON PRESS TO START GAME
	startGame();



    while(1) {

    	//take cordic angle input
    	//put x-x0, y-y0, and z-z0 in calcs
    	//get theta and phi
    	//use those to change brightness of leds
    	calc.x = XAvg - X0;
    	calc.y = YAvg - Y0;
    	calc.z = ZAvg - Z0;
    	calculateArcHypZ(&calc);
    	theta = (long) calc.angleTheta >> 8;	//divide by 256 to see angles 0-360
    	phi = (long) calc.anglePhi >> 8;

    	if(phi > 90) {
    		phi = 360 - phi;
    	}


    	if (phi >= 90-TOLERANCE) { //2 degrees a little too sensitive
    		LEDControl.LEDDir = Flat;
    	} else {
    		LEDControl.LEDDir = DetermineDirection(theta);
    	}
    	SetAllLEDs(&LEDControl, OFF);
    	SetAllLEDs(&LEDControl, TIMER0PD);
    	LightLEDsByDirection(&LEDControl);
    }

	return 0;
}


#pragma vector = TIMER0_A1_VECTOR			// Timer 1 interrupt service routine
	__interrupt void TimerA1_routine(void) {
		switch(__even_in_range(TAIV, 10)) {
			case 2: //capture control flag - turn off LED (CCIFG1)
				//CCIFG1 = 10 in binary. transfer triggered when this flag set
				break;
		}

	TA0CCTL1 &= ~CCIFG; //clear capture control interrupt flag
}

#pragma vector = TIMER0_A0_VECTOR  				// Timer 0 interrupt service routine
__interrupt void TimerA0_routine(void) {

	//debounce
	g1mSTimer++;  //increment the Timer, every time the interrupt occurs, to use for debouncing

	//adc_accel
	ReadADC();
	Filter(); //store latest adc value read in a buffer

}
