/*
 * displayTime.h
 *
 *  Created on: Sep 18, 2015
 *      Author: superman
 */
#include <stdbool.h>
#include <stdio.h>
#include "displayTime.h"

#define APPLY_ROLLOVER(X,Y) ((X + Y) % Y)

static int hours = 0;   // current value of the hour counter
static int minutes = 0; // current value of the minutes counter
static int seconds = 0; // current value of the seconds counter

int displayTime_debouncedButtonState = 0;    // the value of the button register after debouncing

void displayTime_setButtonsVal(int debouncedButtons)
{
	displayTime_debouncedButtonState = debouncedButtons;
}

// prints out the current time and sets the
// carriage return to the start of the line
void displayTime_display()
{
	xil_printf("\r%02d:%02d:%02d", hours, minutes, seconds);
}

// This function checks to see if the user is modifying the time with the buttons.
// It first checks the buttons state from the debouncer then uses a state machine
// to determine whether this a single change or a auto-adjusting
void displayTime_modifyTick() {
	const int PUSH_BUTTONS_CENTER = 0x01;
	const int PUSH_BUTTONS_RIGHT  = 0x02;
	const int PUSH_BUTTONS_DOWN   = 0x04;
	const int PUSH_BUTTONS_LEFT   = 0x08;
	const int PUSH_BUTTONS_UP     = 0x10;
	static enum modifyTimer_states {
		modifyTimer_state_waitForTimeButton,
		modifyTimer_state_waitForUpDown,
		modifyTimer_state_AutoIncrement
	} modifyTimer_state = modifyTimer_state_waitForTimeButton;

	// Wait for user to push a time button (left, center, or right)
	int timeButton = displayTime_debouncedButtonState & (PUSH_BUTTONS_LEFT | PUSH_BUTTONS_CENTER | PUSH_BUTTONS_RIGHT);
	static int pushedTimeButton = 0;
	if (modifyTimer_state == modifyTimer_state_waitForTimeButton && timeButton) {
		pushedTimeButton = timeButton; // Lock in the button. Don't allow user to push different time button
		modifyTimer_state = modifyTimer_state_waitForUpDown;

	} else if (modifyTimer_state > modifyTimer_state_waitForTimeButton && !(timeButton & pushedTimeButton)) { // Go backwards if released
		modifyTimer_state = modifyTimer_state_waitForTimeButton;
	}

	// Wait for user to push up and down buttons
	int upDown = displayTime_debouncedButtonState & (PUSH_BUTTONS_UP | PUSH_BUTTONS_DOWN);
	static int pushedUpDown = 0;
	static int autoIncrementTimer = 0;
	bool modifyTime = false;

	if (modifyTimer_state == modifyTimer_state_waitForUpDown && upDown) {
		pushedUpDown = upDown; // Lock in the button. Don't allow user to push different up down button
		modifyTimer_state = modifyTimer_state_AutoIncrement;
		modifyTime = true; // Modify time on initial press
		autoIncrementTimer = 0;

	} else if (modifyTimer_state > modifyTimer_state_waitForUpDown && !(upDown & pushedUpDown)) {// Go backwards if released
		modifyTimer_state = modifyTimer_state_waitForUpDown;
	}

	// Auto increment time if held
	if (modifyTimer_state == modifyTimer_state_AutoIncrement) {
		autoIncrementTimer++;
		if (autoIncrementTimer > 100 && autoIncrementTimer % 50 == 0) // 1 sec
			modifyTime = true;
	}

	// Modify time if in correct state.
	if (modifyTime) {
		const int TOTAL_SECONDS = 60;
		const int TOTAL_MINUTES = 60;
		const int TOTAL_HOURS   = 24;

		// Up or down?
		int incVal;
		if (pushedUpDown & PUSH_BUTTONS_UP)
			incVal = 1;
		else if (pushedUpDown & PUSH_BUTTONS_DOWN)
			incVal = -1;
		else return;// There is nothing to modify

		// Hours, minutes, or seconds?
		if (timeButton & PUSH_BUTTONS_LEFT) {
			hours += incVal;
			hours = APPLY_ROLLOVER(hours, TOTAL_HOURS);
		}
		if (timeButton & PUSH_BUTTONS_CENTER) {
			minutes += incVal;
			minutes = APPLY_ROLLOVER(minutes, TOTAL_MINUTES);
		}
		if (timeButton & PUSH_BUTTONS_RIGHT) {
			seconds += incVal;
			seconds = APPLY_ROLLOVER(seconds, TOTAL_SECONDS);
		}
		displayTime_display();
	}
}

// increments the display time and
// keeps it in 24 hour format
void displayTime_increment() {
	const int SECONDS_MAXVAL = 59;
	const int MINUTES_MAXVAL = 59;
	const int HOURS_MAXVAL = 23;
	seconds++;
	if (seconds > SECONDS_MAXVAL) {
		seconds = 0;
		minutes++;
		if (minutes > MINUTES_MAXVAL) {
			minutes = 0;
			hours++;
			if (hours > HOURS_MAXVAL) {
				hours = 0;
			}
		}
	}
}

// Tracks normal time operations
void displayTime_tick() {
	static int displayTime_timer = 0;
	const int DISPLAYTIMER_ONESECOND = 100;
	displayTime_timer++;
	if (displayTime_timer == DISPLAYTIMER_ONESECOND) { // adjust every second
		displayTime_timer = 0; // reset display timer
		displayTime_increment();
		displayTime_display();
	}
}
