/*
 * debounce.h
 *
 *  Created on: Oct 16, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su, David Zekoski djz5qj
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#define MAKEDEBOUNCETIME 2		//Definitions for debounce Times for the P1.3 button (ms)
#define RELEASEBOUNCETIME 10
#define MAXINT 65535
#define HALFSECOND 125			//500 ms
#define TENSECONDS 5			//20*HALFSECOND = 10s
#define GLED BIT6

//*******************Type Definitions Section*********************************************
typedef enum { //the 4 states of the Finite State Machine to debounce the button
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

// if you use useful.h, remember to make sure that the case of HIGH/LOW and High/Low is correct.
typedef enum {
	High, Low //status of button. low is 1 (Active low) high is 0
} SwitchStatus;

typedef struct {
	DbState ControlState;
	SwitchStatus CurrentSwitchReading; // {instantaneous, from GetSwitch}
	SwitchStatus CurrentValidState;    // {debounced value of the switch from state machine}
	char* SwitchPort;	//port the current SwitchDefine is a part of
	unsigned int SwitchPin; //pin the current SwitchDefine is on
	unsigned int HighDelayTime;
	unsigned int LowDelayTime;
} SwitchDefine;
//******************End of Type Definitions Section***************************************


//********************** Global Variables Declaration Section ****************************
SwitchDefine gPushButton;    	//Structure to contain all info for the P1.3 button
unsigned int g1mSTimer; 		//This variable is incremented in ManageSoftwareTimers

unsigned int TimeStamp;
unsigned int gButtonPresses;  	//Number of button presses from all switches
//********************** End of Global Variables Declaration Section *********************


//This function returns the instantaneous value of the selected switch
// returns a High or Low
SwitchStatus GetSwitch(SwitchDefine *Switch);

//This function debounces a switch input
SwitchStatus Debouncer(SwitchDefine *Switch);

unsigned int CheckDebounceTime(unsigned int currentTime, unsigned int startTime, unsigned int bounceTime);

#endif /* DEBOUNCE_H_ */
