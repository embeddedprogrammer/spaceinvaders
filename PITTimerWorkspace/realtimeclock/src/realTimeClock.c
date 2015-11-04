#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include <stdbool.h>
#include "displayTime.h"

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

static int  debounceTimer = 0;           // 50 ms timer for debouncing buttons
static bool debounceTimerEnable = false; // only counts when enabled

void custom_XIntc_EnableIntr(BaseAddress, EnableMask)
{
	*(volatile u32 *)(BaseAddress + XIN_IER_OFFSET) = EnableMask;
}

void custom_XIntc_MasterEnable(BaseAddress)
{
	*(volatile u32 *)(BaseAddress + XIN_MER_OFFSET) = XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK;
}

u32 custom_XIntc_GetIntrStatus(BaseAddress)
{
	return (*(volatile u32 *)(BaseAddress + XIN_ISR_OFFSET)) & (*(volatile u32 *)(BaseAddress + XIN_IER_OFFSET));
}

void custom_XIntc_AckIntr(BaseAddress, AckMask)
{
	*(volatile u32 *)(BaseAddress + XIN_IAR_OFFSET) = AckMask;
}

// After 50ms of button stability the buttons are
// considered debounced and this timer is disabled
void debounceButtons_tick() {
	if (debounceTimerEnable) {
		debounceTimer++;
		if (debounceTimer == 5) { // 50 ms
			debounceTimerEnable = false;
			displayTime_setButtonsVal(XGpio_DiscreteRead(&gpPB, 1));
		}
	}
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() { //10ms
	debounceButtons_tick();
	displayTime_modifyTick();
	displayTime_tick();
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
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
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = custom_XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK)
	{
		custom_XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK)
	{
		custom_XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}

int main (void) {
	init_platform();
	// Initialize the GPIO peripherals.
	int success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);

	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	custom_XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	custom_XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	while(1);  // Program never ends.

	cleanup_platform();

	return 0;
}
