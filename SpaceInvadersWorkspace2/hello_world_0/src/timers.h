/*
 * timers.h
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include <stdbool.h>

#ifndef TIMERS_H_
#define TIMERS_H_

typedef void (*timerFncPtr_t)();

typedef struct
{
	timerFncPtr_t fncPtr;
	bool enabled;
	bool repeatEnabled;
	int repeatPeriod;
	int timeRemaining;
} customTimer_t;

void testTimerFnc();

int addTimer(int timeRemainingInMs, bool repeat, timerFncPtr_t fncPtr);

void timer_interrupt_handler();

#endif /* TIMERS_H_ */
