#include "memory_game.h"

extern Game GameObj;
Direction testPattern[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};

GameState update(Game *game) {
	switch (game->currentGameState) {
		case StartGame:
			startGame();
			game->currentGameState = DisplayPattern;
		break;
		case DisplayPattern:
			displayPattern();
			game->currentGameState = UserInput;
		break;
		case UserInput:
			receiveUserInput();
			game->currentGameState = CheckInput;
		break;
		case CheckInput:
			int correct = checkInput();
			if (correct && game->index == PATTERN_LENGTH) { //MAKE SURE TO INCREMENT INDEX
				game->currentGameState = WinGame;
			}
			else if(correct) {
				game->currentGameState = UserInput;
			} else {
				game->currentGameState = LoseGame;
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
