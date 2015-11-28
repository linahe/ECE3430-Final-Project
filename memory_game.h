#ifndef MEMORY_GAME_H_
#define MEMORY_GAME_H_

#include <msp430.h>
#include "led_accel.h"

#define PATTERN_LENGTH 10


typedef enum {
	StartGame, DisplayPattern, UserInput, CheckInput, WinGame, LoseGame
} GameState;

typedef struct {
	Direction pattern[PATTERN_LENGTH];
	Direction userInput[PATTERN_LENGTH];
	unsigned int index;
	GameState currentGameState;

} Game;

void startGame(Game * Game);

void displayPattern();

void receiveUserInput();

void checkInput();

void winGame();

void loseGame();

void continueGame();


#endif /* MEMORY_GAME_H_ */
