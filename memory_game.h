#ifndef MEMORY_GAME_H_
#define MEMORY_GAME_H_

#include <msp430.h>
#include "led_accel.h"
#include "game_patterns.h"

typedef enum {
	StartGame, DisplayPattern, UserInput, CheckInput, WinGame, LoseGame
} GameState;

typedef struct {
	Direction pattern[PATTERN_LENGTH];
	Direction userInput[PATTERN_LENGTH];
	unsigned int patternIndex;
	unsigned int inputIndex;
	GameState currentGameState;

} Game;

GameState update();

void startGame();

void displayPattern();

void receiveUserInput();

int checkInput();

void winGame();

void loseGame();

void continueGame();


#endif /* MEMORY_GAME_H_ */
