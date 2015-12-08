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


Game GameObj;
unsigned char CurrentLEDCodeValue; //as interrupts progress, only light LEDs whose duty cycles have not been reached yet
int i;
CalibrationState myCalibrationState; //used at beginning to calibrate board
int X0, Y0, Z0, XAvg, YAvg, ZAvg; //used as externs in led_accel.c to calculate 0s



int main(void) {

	InitializeHardware(); //set up ports, timers, interrupts //FIX!!!!
	ConfigureADC(); //get ADC set up to start reading values

	BlinkLEDs();

	myCalibrationState = XMaxState; //change to init later?
	StartCalibration(myCalibrationState, &gPushButton);

	//after calibration finished, need user to press button to indicate they are ready to begin game
	BlinkLEDs();

	//METHOD HERE TO WAIT FOR BUTTON PRESS TO START GAME
	//startGame(); //NEED TO PUSH BUTTON TO START


    while(1) {

    	update(&GameObj); //calls all different states of game

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
