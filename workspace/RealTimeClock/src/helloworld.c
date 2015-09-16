#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include <stdbool.h>

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

#define PUSH_BUTTONS_CENTER 0x01
#define PUSH_BUTTONS_RIGHT 0x02
#define PUSH_BUTTONS_DOWN 0x04
#define PUSH_BUTTONS_LEFT 0x08
#define PUSH_BUTTONS_UP 0x10



int hours = 0;
int minutes = 0;
int seconds = 0;

int debounceTimer = 0;
bool debounceTimerEnable = false;
int debouncedButtonState = 0;

void debounceButtons_tick()
{
	if (debounceTimerEnable)
	{
		debounceTimer++;
		if (debounceTimer == 5) //50ms
		{
			debounceTimerEnable = false;
			debouncedButtonState = XGpio_DiscreteRead(&gpPB, 1);
		}
	}
}

void modifyTime_tick()
{
	static enum modifyTimer_states {modifyTimer_state_waitForTimeButton, modifyTimer_state_waitForUpDown, modifyTimer_state_AutoIncrement}
		modifyTimer_state = modifyTimer_state_waitForTimeButton;
	bool modifyTime = false;


	// Wait for user to push a time button (left, center, or right)
	int timeButton = debouncedButtonState & (PUSH_BUTTONS_LEFT | PUSH_BUTTONS_CENTER | PUSH_BUTTONS_RIGHT);
	static int pushedTimeButton = 0;
	if(modifyTimer_state == modifyTimer_state_waitForTimeButton && timeButton)
	{
		pushedTimeButton = timeButton; // Lock in the button. Don't allow user to push different time button
		modifyTimer_state = modifyTimer_state_waitForUpDown;
	}
	else if(modifyTimer_state > modifyTimer_state_waitForTimeButton && !(timeButton & pushedTimeButton)) // Go backwards if released
		modifyTimer_state = modifyTimer_state_waitForTimeButton;


	// Wait for user to push up and down buttons
	int upDown = debouncedButtonState & (PUSH_BUTTONS_UP | PUSH_BUTTONS_DOWN);
	static int pushedUpDown = 0;
	static int autoIncrementTimer = 0;
	if(modifyTimer_state == modifyTimer_state_waitForUpDown && upDown)
	{
		pushedUpDown = upDown; // Lock in the button. Don't allow user to push different up down button
		modifyTimer_state = modifyTimer_state_AutoIncrement;
		modifyTime = true; // Modify time on initial press
		autoIncrementTimer = 0;
	}
	else if(modifyTimer_state > modifyTimer_state_waitForUpDown && !(upDown & pushedUpDown)) // Go backwards if released
		modifyTimer_state = modifyTimer_state_waitForUpDown;


	// Auto increment time if held
	if(modifyTimer_state == modifyTimer_state_AutoIncrement)
	{
		autoIncrementTimer++;
		if(autoIncrementTimer > 100 && autoIncrementTimer % 50 == 0) // 1 sec
		{
			modifyTime = true;
		}
	}


	// Modify time if in correct state.
	if(modifyTime)
	{
		// Up or down?
		int incVal;
		if(pushedUpDown & PUSH_BUTTONS_UP)
			incVal = 1;
		else if(pushedUpDown & PUSH_BUTTONS_DOWN)
			incVal = -1;

		// Hours, minutes, or seconds?
		if(pushedTimeButton & PUSH_BUTTONS_LEFT)
			hours = (hours + incVal + 24) % 24;
		else if(pushedTimeButton & PUSH_BUTTONS_CENTER)
			minutes = (minutes + incVal + 60) % 60;
		else if(pushedTimeButton & PUSH_BUTTONS_RIGHT)
			seconds = (seconds + incVal + 60) % 60;
		displayTime_display();
	}
}

int displayTime_timer = 0;

void displayTime_increment()
{
	seconds++;
	if (seconds > 59) {
		seconds = 0;
		minutes++;
		if (minutes > 59) {
			minutes = 0;
			hours++;
			if (hours > 23) {
				hours = 0;
			}
		}
	}
}

void displayTime_display()
{
	xil_printf("%02d:%02d:%02d \r", hours, minutes, seconds);
}

void displayTime_tick()
{
	displayTime_timer++;
	if (displayTime_timer == 100)
	{
		displayTime_timer = 0;
		displayTime_increment();
		displayTime_display();
	}
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() //10ms
{
	debounceButtons_tick();
	modifyTime_tick();
	displayTime_tick();
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler()
{
	// Clear the GPIO interrupt.
	XGpio_InterruptGlobalDisable(&gpPB);                    // Turn off all PB interrupts for now.
	int currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	static int previousButtonState;
	if (previousButtonState != currentButtonState)
	{
		debounceTimer = 0;
		debounceTimerEnable = true;
		previousButtonState = currentButtonState;
	}

	// You need to do something here.
	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
	XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}

int main (void)
{
	init_platform();
	// Initialize the GPIO peripherals.
	int success;
	print("hello world\n\r");
	success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	while(1);  // Program never ends.

	cleanup_platform();

	return 0;
}
