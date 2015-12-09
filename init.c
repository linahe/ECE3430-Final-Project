/*
 * init.c
 *
 *  Created on: Oct 13, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su
 */
#include "init.h"
#include <msp430.h>
#include <stdlib.h>

extern Game GameObj;


const Direction userInitialPattern[PATTERN_LENGTH] = {Flat, Flat, Flat, Flat, Flat, Flat, Flat, Flat, Flat, Flat};
const Direction testPattern[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};
const Direction gamePatterns[PATTERN_LENGTH][PATTERN_LENGTH] = {
	{ North, North, South, East, South, West, North, West, West, South} , //1
	{ West, East, South, West, North, South, West, West, East, South } , //2
	{ West, North, West, East, East, South, North, South, East, West } , //3
	{ South, North, South, West, West, East, East, South, West, North } , //4
	{ South, North, East, West, East, North, South, North, West, South } , //5
	{ East, East, North, East, South, West, South, East, North, West } , //6
	{ East, West, North, South, West, South, East, North, West, South }, //7
	{ North, West, South, West, East, South, North, East, East, South }, //8
	{ North, East, North, East, West, North, South, West, West, North }, //9
	{ West, North, South, South, West, East, South, North, North, West } //10
};

void InitializeHardware() {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    InitTimerSystem(); //set up the timers interrupts
    InitPorts(); //set up the LED and buttons
    InitializeVariables();
    InitLEDState();
    InitLEDDisplay(); //set up the latch, clock and inputs for the 7 segment display
}

//set up default values of global vars. purposes expained in debounce.h
void InitializeVariables(void) {
	gButtonPresses = 0;
	gPushButton.CurrentValidState = Low;
	gPushButton.ControlState = DbExpectHigh;
	gPushButton.SwitchPort = (char*) &(P1IN); //port one, an input, is the current switchport
	gPushButton.SwitchPin = BTN;
	gPushButton.HighDelayTime = MAKEDEBOUNCETIME;
	gPushButton.LowDelayTime = RELEASEBOUNCETIME;
	g1mSTimer = 0;
}

void InitLEDState() {
	LightLED(0x00);
}


//set up all variables for a switchdefine "object"
void InitSwitch(SwitchDefine *SwitchInput, char* Port, unsigned int Pin, unsigned int Bit, unsigned int UpBounceTime, unsigned int DownBounceTime) {
	SwitchInput->ControlState = DbExpectHigh; //initial state for debounce FSM
	SwitchInput->CurrentValidState = Low; //initial state is non active
	SwitchInput->SwitchPort = Port; //the port to be used for this switch
	SwitchInput->SwitchPin = Pin; //the pin number this reads off of
	SwitchInput->HighDelayTime = UpBounceTime; //debounce threshold for high time
	SwitchInput->LowDelayTime = DownBounceTime; //debounce threshold for low time
}

void InitTimerSystem() {
	DCOCTL = CALDCO_1MHZ;  			// |Set clock speed to 1 MHz|

	TACCR0 = TIMER0PD; //125 for 125khz clock does 1ms //at 8ms now
	TACCR1 = CONTROLPD; //half of max brightness

	TACCTL0 |= CCIE; //ccie is capture compare interrupt enable. enables the interrupt to occur for timer 0
	TACCTL1 |= CCIE; //OUTMOD_7 |//using outmod for PWM


	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR | TAIE;// try later
	//Tassel_2 is so smclock is used,
	//ID_3 = divide by 8 to bring 1MHz period to 125kHz, otherwise number of times to run the clock is over the 16 bit int limit
	//MC_1 means to set in up mode,
	//TACLR will start by clearing the timer

	_BIS_SR(GIE); //Globally enable interrupts

}

void InitPorts() {
	P1DIR |= GLED + SMCLK;				// Make LEDs and clock outputs
	P1OUT |= GLED;
	P1SEL |= SMCLK;

	P1DIR &= ~BTN;	// set direction as input
	P1REN |= BTN;	// enable use of pullup/pulldown resistor
	P1OUT |= BTN;	// indicate that should use pull-up resistor so button output = 1 when not pushed
	//button is active low, so dont want to be active until pushed

}

void InitLEDDisplay() {
	P1DIR |= SCLK + SIN + BLANK; //set as output the clock, input, and blank variable
	P1OUT &= ~BLANK; //set blank to low to enable sending data
	P2DIR |= LATCH; //LATCH high!
	P2OUT &= ~LATCH; //tick latch low
}

void InitGame() {

	int patternToUse = (g1mSTimer % 10);
	int i;
	for(i = 0; i < PATTERN_LENGTH; i++) {
		GameObj.pattern[i] = gamePatterns[patternToUse][i];
		GameObj.userInput[i] = userInitialPattern[i];
	}
	GameObj.patternIndex = 1;
	GameObj.inputIndex = 0;
	GameObj.currentGameState = StartGame;

}


