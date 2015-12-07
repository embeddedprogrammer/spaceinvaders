/*
 * spaceInvaders.h
 *
 *  Created on: Oct 14, 2015
 *      Author: superman
 */

#ifndef SPACEINVADERS_H_
#define SPACEINVADERS_H_

#include "timers.h"

void gameOver();
void restart();
void nextLevel();
void printStats();

timerFncPtr_t gpioFunc;

typedef enum
{
	frame_gameScreen = 0,
	frame_screenCapture = 1
} frame_t;

frame_t getFrame();

#endif /* SPACEINVADERS_H_ */
