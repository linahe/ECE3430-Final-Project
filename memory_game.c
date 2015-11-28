#include "memory_game.h"


Direction testPattern[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};

void startGame(Game * Game)
{
	WaitLowButton(&gPushButton);
	WaitHighButton(&gPushButton);
	//Change the gameState of Game struct to DisplayPattern
	int i;
	for(i = 0; i < PATTERN_LENGTH; i++) {
		Game->pattern[i] = testPattern[i];
	}
}

void displayPattern()
{

}

void receiveUserInput()
{

}

void checkInput()
{

}

void winGame()
{

	LightLED(0x5D); //smiley face
}

void loseGame()
{

}

void continueGame()
{

}
