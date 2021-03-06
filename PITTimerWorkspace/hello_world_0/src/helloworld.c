/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "xintc_l.h"
#include "platform.h"
#include "pit.h"

#include "xparameters.h"
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.


XGpio gpPB;   // This is a handle for the push-button GPIO block.

static int counter = 0;

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_PIT_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_INTERRUPT_MASK);
		counter++;
		xil_printf("%d\n\r", counter);
	}
}

void initInterrupts()
{
	// Initialize the GPIO peripherals.
	XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_INTERRUPT_MASK);

	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

int main()
{
    init_platform();
    initInterrupts();

    xil_printf("Control reg: %d\n\r", PIT_mReadControlReg(XPAR_PIT_0_BASEADDR));
    xil_printf("Delay reg  : %d\n\r", PIT_mReadDelayReg(XPAR_PIT_0_BASEADDR));

    //Write to delay and control register
    PIT_mWriteDelayReg(XPAR_PIT_0_BASEADDR, 100000000); // 1 second
    PIT_mWriteControlReg(XPAR_PIT_0_BASEADDR, PIT_ENABLE_DECREMENT | PIT_ENABLE_INTERRUPTS);

    xil_printf("Control reg: %d\n\r", PIT_mReadControlReg(XPAR_PIT_0_BASEADDR));
    xil_printf("Delay reg  : %d\n\r", PIT_mReadDelayReg(XPAR_PIT_0_BASEADDR));

    while(1);

    cleanup_platform();

    return 0;
}
