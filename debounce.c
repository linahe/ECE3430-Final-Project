/*
 * debounce.c
 *
 *  Created on: Oct 16, 2015
 *      Author: // Deanna Buttaro dlb3un, Lina He lh3su, David Zekoski djz5qj
 */


#include "debounce.h"
#include <msp430.h>

//Get the status of the current switch (either high or low)
SwitchStatus GetSwitch(SwitchDefine *Switch) {
	//0 is high, l is low
	unsigned int shifted_val = (*(Switch->SwitchPort) & Switch->SwitchPin); //Getting the bit value at the Button Bit for the Switch's current port
	if(shifted_val == 0) { //a 0, active low, so button was pressed
		Switch->CurrentSwitchReading = High; //we have read a high value
		return High; //active when 0
	} else { //a 1
		Switch->CurrentSwitchReading = Low; //we have read a low value
		return Low; //inactive when 1
	}
}


//Function that debounces
SwitchStatus Debouncer(SwitchDefine *Switch) {
//Code must be added to access the internal variable through the SwitchDefine pointer
	SwitchStatus Current_Button_Reading = GetSwitch(Switch);
	switch (Switch->ControlState) {
		case DbExpectHigh: //waiting for button to be pressed
			if(Current_Button_Reading == High) {
				TimeStamp = g1mSTimer; //record the current time to check if button's state stays high long enough
				Switch->ControlState = DbValidateHigh; //ready to move on to the next state to ensure button was actually pressed
			}
		break ;
		case DbValidateHigh: //ensure the button's time in high state exceeds the threshold
			if(Current_Button_Reading == High)
				{
				if(CheckDebounceTime(g1mSTimer, TimeStamp, Switch->HighDelayTime)) {
					P1OUT ^= GLED; //toggle led on or off
					Switch->ControlState = DbExpectLow; //ready to move onto the next state
					Switch->CurrentValidState = High; //the button is confirmed to be in the high state
				}
			}
			else
				Switch->ControlState = DbExpectHigh; //if low from button "bouncing" go to previous state to wait until high again

		break ;
		case DbExpectLow: //button was confirmed to be high, waiting for button to be "unpressed" or low again
			if(Current_Button_Reading == Low) {
				TimeStamp = g1mSTimer; //record current time to check if button's state stays low long enough
				Switch->ControlState = DbValidateLow; //ready to move on to the next state to ensure button was actually released
			}
		break ;
		case DbValidateLow: //ensure button's time in low state exceeds the threshold
			if(Current_Button_Reading == Low)
				{
					if(CheckDebounceTime(g1mSTimer, TimeStamp, Switch->LowDelayTime))
					{

						gButtonPresses++; //global counter for number of times button was pressed
						Switch->ControlState = DbExpectHigh; //if button was truly released, go to state to check for next button press
						Switch->CurrentValidState = Low; //definitely in low state now
					}
				}
			else
				Switch->ControlState = DbExpectLow; //button bouncing or maybe was not truly released yet, go back to check for button to be low again

		break ;
		default: Switch->ControlState = DbExpectHigh ;
		// error, so just go to default state
	}
//The internal state should be updated here. It should also be returned as a debugging aid....
	return Switch->CurrentValidState;
}

//accounts for when button is pressed /released right before the 500ms has been reached and the timer resets to 0
//need a way to check if threshold for debounce has still been passed
unsigned int CheckDebounceTime(unsigned int currentTime, unsigned int startTime, unsigned int bounceTime) {
	unsigned int overlapDifference = MAXINT - startTime + currentTime; //the true difference between TimeStamp and g1mSTimer, if startTime is larger than current time(means reset has occured)
	unsigned int overlapThreshold = MAXINT - bounceTime; //if time is recorded at time higher than this, we know timer will reset before we can determine if debounce threshold has been passed
	if(currentTime - startTime > bounceTime) { //passed the threshold
		return 1;
	} else if (startTime > overlapThreshold && overlapDifference >= bounceTime) {
		//this means timer has reset from 500, but the actual time elapsed still surpasses the threshold
		return 1;
	} else { //threshold has not been passed
		return 0;
	}

}
