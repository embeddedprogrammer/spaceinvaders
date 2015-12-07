/*
 * timers.c
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "timers.h"
#include <stdio.h>
#include "spaceInvaders.h"

#define TIMER_INTERUPT_PERIOD 10
#define MAX_TIMERS 10

customTimer_t timer[MAX_TIMERS];

int addTimer(int timeRemainingInMs, bool repeat, timerFncPtr_t fncPtr)
{
	int i;
	for (i = 0; i < MAX_TIMERS; i++)
		if (!timer[i].enabled) {
			timer[i].fncPtr = fncPtr;
			timer[i].enabled = true;
			timer[i].repeatEnabled = repeat;
			timer[i].repeatPeriod = repeat ? timeRemainingInMs : 0;
			timer[i].timeRemaining = timeRemainingInMs;
			return i;
		}
	xil_printf("ERROR: No timer left.\n\r");
	return -1;
}

void removeTimer(int i)
{
	timer[i].enabled = false;
}

void removeAllTimers()
{
	int i;
	for(i = 0; i < MAX_TIMERS; i++)
		timer[i].enabled = false;
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler()  //10ms
{
	int i;
	frame_t frame = getFrame(); // disable all other timers but gpio when showing screen capture
	for(i = 0; i < MAX_TIMERS; i++)
		if (timer[i].enabled && (frame == frame_gameScreen || timer[i].fncPtr == gpioFunc)) {
			timer[i].timeRemaining -= TIMER_INTERUPT_PERIOD;
			if (timer[i].timeRemaining <= 0) {
				if(timer[i].repeatEnabled)
					timer[i].timeRemaining = timer[i].repeatPeriod;
				else
					timer[i].enabled = false;
				timer[i].fncPtr();
			}
		}
}
