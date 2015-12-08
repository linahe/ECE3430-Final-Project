#ifndef GAME_PATTERNS_H_
#define GAME_PATTERNS_H_

#include "led_accel.h"

#define PATTERN_LENGTH 10

Direction pattern1[PATTERN_LENGTH] = {North, South, East, West, North, South, East, West, North, South};
Direction pattern2[PATTERN_LENGTH] = {North, North, West, South, East, East, East, South, West, North};
Direction pattern3[PATTERN_LENGTH] = {South, West, East, West, East, North, South, South, North, West};



#endif /* GAME_PATTERNS_H_ */
