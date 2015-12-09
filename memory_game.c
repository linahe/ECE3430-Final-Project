#include "memory_game.h"
#include "cordic.h"
#include "debounce.h"
extern Game GameObj;

calculations calc;
volatile int theta, phi; //for debugging
extern int X0, Y0, Z0, XAvg, YAvg, ZAvg;
Direction LEDDir;


void update() {
	switch (GameObj.currentGameState) {
		case StartGame:
			startGame();
			GameObj.currentGameState = DisplayPattern;
		break;
		case DisplayPattern:
			GameObj.inputIndex = 0; // reset index at beginning of each display so correct entire pattern again
			displayPattern();
			GameObj.currentGameState = UserInput;
		break;
		case UserInput:
			P1OUT &= ~GLED;
			GameObj.userInput[GameObj.inputIndex] =  receiveUserInput();
			GameObj.currentGameState = CheckInput;
		break;
		case CheckInput:
			GameObj.pass = checkInput();
			if (GameObj.pass && GameObj.inputIndex == (PATTERN_LENGTH-1)) { //MAKE SURE TO INCREMENT INDEX
				P1OUT |= GLED;
				GameObj.currentGameState = WinGame;
			} else if(GameObj.pass && (GameObj.inputIndex == GameObj.patternIndex)) {
				P1OUT |= GLED;
				BlinkLEDs();
				GameObj.patternIndex += 2;
				GameObj.currentGameState = DisplayPattern;
			}	else if(GameObj.pass) {
				GameObj.inputIndex++;
				GameObj.currentGameState = UserInput;
			} else {
				P1OUT &= ~GLED;
				GameObj.currentGameState = LoseGame;
			}
		break;
		case WinGame:
			winGame();
		break;
		case LoseGame:
			loseGame();
		break;
	}
}

void startGame()
{
	WaitLowButton(&gPushButton);
	WaitHighButton(&gPushButton);
	//Change the gameState of Game struct to DisplayPattern

}

void displayPattern()
{
	int i;
	for (i = 0; i <= GameObj.patternIndex; i++)
	{
		LightLEDsByDirection(GameObj.pattern[i]);
		__delay_cycles(500000);
	}
	//each time, will need to increment how much of the array is iterated through


}

Direction receiveUserInput()
{
	//just ONE input at a time, then CHECK that input
	//check where they are tilted to pass threshold of time
	//then must return to flat for threshold of time
	Direction userInput = Flat;
	Direction currentDirection = Flat;
	unsigned int inputTimestamp = 0;
	while(1)
	{
		calc.x = XAvg - X0;
		calc.y = YAvg - Y0;
		calc.z = ZAvg - Z0;
		calculateArcHypZ(&calc);
		theta = (long) calc.angleTheta >> 8;	//divide by 256 to see angles 0-360
		phi = (long) calc.anglePhi >> 8;

		if(phi > 90) {
			phi = 360 - phi;
		}


		if(phi >= 90-TOLERANCE)
			LEDDir = Flat;
		else
			LEDDir = DetermineDirection(theta);
		LightLEDsByDirection(LEDDir);

		if (userInput == Flat && LEDDir != Flat)
		{
			if (inputTimestamp == 0)
			{
				inputTimestamp = g1mSTimer;
				currentDirection = LEDDir;
			}
			else if (currentDirection != LEDDir && g1mSTimer - inputTimestamp < TIMER_THRESHOLD) //resetting if direction switched before time threshold
			{
				inputTimestamp = g1mSTimer;
				currentDirection = LEDDir;
			}
			else if (g1mSTimer - inputTimestamp >= TIMER_THRESHOLD) //going in here when flat
			{
				userInput = currentDirection;
			}
			else
				continue;
		}
		else
		{
			if(LEDDir != Flat ) //put in new direction
			{
				inputTimestamp = g1mSTimer;
				currentDirection = LEDDir;

			} else if (LEDDir == Flat && currentDirection != Flat)
			{
				inputTimestamp = g1mSTimer;
				currentDirection = Flat;
			}
			else if (LEDDir != Flat && g1mSTimer - inputTimestamp < TIMER_THRESHOLD)
			{
				inputTimestamp = 0;
				userInput = Flat;
				currentDirection = Flat;

			}
			else if (g1mSTimer - inputTimestamp >= TIMER_THRESHOLD && userInput != Flat && LEDDir == Flat)
			{
				return userInput;
			}
			else
				continue; //goes here if flat at beginning
		}

	}


}

int checkInput()
{
	//return 1 if correct
	//return 0 if incorrect
	if(GameObj.pattern[GameObj.inputIndex] == GameObj.userInput[GameObj.inputIndex]) {
		return 1;
	} else {
		return 0;
	}

}

void winGame()
{
	LightLED(0x5D); //smiley face
}

void loseGame()
{
	LightLED(0x80);
	__delay_cycles(500000);
	LightLED(0xC0);
	__delay_cycles(500000);
	LightLED(0xE0);
	__delay_cycles(500000);
	LightLED(0xF0);
	__delay_cycles(500000);
	LightLED(0xF8);
	__delay_cycles(500000);
	LightLED(0xFC);
	__delay_cycles(500000);
	LightLED(0xFE);
	__delay_cycles(500000);
	LightLED(0xFF);
	__delay_cycles(500000);
}

void continueGame()
{

}
