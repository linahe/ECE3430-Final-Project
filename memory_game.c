#include "memory_game.h"


const Direction testPattern[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};

void startGame()
{
	WaitLowButton(&gPushButton);
	WaitHighButton(&gPushButton);
	//Change the gameState of Game struct to DisplayPattern
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
