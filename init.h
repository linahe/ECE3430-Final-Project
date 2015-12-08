/*
 * init.h
 *
 *  Created on: Oct 13, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su
 */

#include <msp430.h>
#include "adc_accel.h"
#include "cordic.h"
#include "memory_game.h"


#ifndef ROTARY_ENCODER_DLB3UN_INIT_H_
#define ROTARY_ENCODER_DLB3UN_INIT_H_

#define RLED BIT0 //red LED (not used)
#define GLED BIT6 //green LED
#define BTN BIT3 //push button
#define SMCLK BIT4
//#define ONEMS 125 //Number of ticks for 125khz clock to take one millisecond

void InitializeHardware(); // Initialize all hardware subsystems
void InitializeVariables(void); //Initialize all variables

//initialize a specific switch
void InitSwitch(SwitchDefine *SwitchInput, char* Port, unsigned int Pin, unsigned int Bit, unsigned int UpBounceTime, unsigned int DownBounceTime);

//These are explicitly called from within InitializeHardware()
void InitTimerSystem(); //This should set up a periodic interrupt at a 1 mS rate using SMCLK as the clock source.
void InitPorts(); //This should also be called from ManageSoftwareTimers every 10 seconds
void InitLEDState(); //init the struct to contain all the LED info
void InitLEDDisplay(); //Setting up inputs / latch / blank for 7 segment display
void InitGame();

#endif /* ROTARY_ENCODER_DLB3UN_INIT_H_ */
