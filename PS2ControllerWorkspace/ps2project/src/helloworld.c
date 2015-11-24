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
#include "platform.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"

#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "pit.h"
#include "mouse.h"

#include <stdbool.h>

bool tickOccurred = false;

void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the PIT interrupt first.
	if (intc_status & XPAR_PS2CTRL_0_INTERRUPT_MASK)
	{
		unsigned char readVal = PS2CTRL_mReadSlaveReg1(XPAR_PS2CTRL_0_BASEADDR);
		mouse_stateMachine(readVal);
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
	}
	if (intc_status & XPAR_PIT_0_INTERRUPT_MASK)
	{
		tickOccurred = true;
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_INTERRUPT_MASK);
	}
}

XGpio gpPB;   // This is a handle for the push-button GPIO block.

void initInterrupts()
{
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK | XPAR_PIT_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	PIT_startRecurringTimer(XPAR_PIT_0_BASEADDR, 50000000);
	mouse_init();
}

void printInfo()
{
	xil_printf("x: %d, y: %d, b: %d\n\r", mouse_getXMovement(), mouse_getYMovement(), mouse_getMouseButtons());
}

int main()
{
    init_platform();
    initInterrupts();
    setvbuf(stdin, NULL, _IONBF, 1024); //This makes it so we don't have to wait for the user to push enter.
    while(true)
    {
    	if(tickOccurred)
    	{
			printInfo();
			tickOccurred = false;
    	}
    }
    xil_printf("End\n\r");
    cleanup_platform();
    return 0;
}
