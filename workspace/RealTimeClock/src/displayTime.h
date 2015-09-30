/*
 * displayTime.h
 *
 *  Created on: Sep 18, 2015
 *      Author: superman
 */

#ifndef DISPLAYTIME_H_
#define DISPLAYTIME_H_

void displayTime_setButtonsVal(int debouncedButtons);

// prints out the current time and sets the
// carriage return to the start of the line
void displayTime_display();

// This function checks to see if the user is modifying the time with the buttons.
// It first checks the buttons state from the debouncer then uses a state machine
// to determine whether this a single change or a auto-adjusting
void displayTime_modifyTick();

// increments the display time and
// keeps it in 24 hour format
void displayTime_increment();

// Tracks normal time operations
void displayTime_tick();

#endif /* DISPLAYTIME_H_ */
