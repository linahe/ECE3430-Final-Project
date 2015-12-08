#ifndef MEMORY_GAME_H_
#define MEMORY_GAME_H_

#include <msp430.h>
#include "led_accel.h"

#define PATTERN_LENGTH 10
#define TOLERANCE 15 //how many degrees of tolerance for the board to be considered level


//Direction pattern1[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};
//Direction pattern2[PATTERN_LENGTH] = {North, North, West, South, East, East, East, South, West, North};
//Direction pattern3[PATTERN_LENGTH] = {South, West, East, West, East, North, South, South, North, West};

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

void update();

void startGame();

void displayPattern();

Direction receiveUserInput();

int checkInput();

void winGame();

void loseGame();

void continueGame();


#endif /* MEMORY_GAME_H_ */
