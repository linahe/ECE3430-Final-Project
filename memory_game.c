#include "memory_game.h"

extern Game GameObj;
Direction testPattern[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};

void update() {
	switch (GameObj.currentGameState) {
		case StartGame:
			startGame();
			GameObj.currentGameState = DisplayPattern;
		break;
		case DisplayPattern:
			displayPattern();
			GameObj.currentGameState = UserInput;
		break;
		case UserInput:
			receiveUserInput();
			GameObj.currentGameState = CheckInput;
		break;
		case CheckInput:
			if (checkInput() && GameObj.inputIndex == PATTERN_LENGTH) { //MAKE SURE TO INCREMENT INDEX
				GameObj.currentGameState = WinGame;
			}
			else if(checkInput()) {
				GameObj.currentGameState = UserInput;
			} else {
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
	int i;
	for(i = 0; i < PATTERN_LENGTH; i++) {
		GameObj.pattern[i] = testPattern[i];
	}
}

void displayPattern()
{
	int i;
	for (i = 0; i < GameObj.patternIndex; i++)
	{
		LightLEDsByDirection(GameObj.pattern[i]);
		__delay_cycles(500000);
	}
	//each time, will need to increment how much of the array is iterated through


}

void receiveUserInput()
{
	//just ONE input at a time, then CHECK that input
	//check where they are tilted to pass threshold of time
	//then must return to flat for threshold of time


}

int checkInput()
{
	//return 1 if correct
	//return 0 if incorrect

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
