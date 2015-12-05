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
#include "xparameters.h"
#include "xgpio.h"
#include "dma_controller.h"
#include "xintc_l.h"

XGpio xgpio_pushButtons;
XGpio xgpio_LEDs;
XGpio xgpio_switches;

void initGPIO()
{
	XGpio_Initialize(&xgpio_pushButtons, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_pushButtons, 1, 0x0000001F);

	XGpio_Initialize(&xgpio_LEDs, XPAR_LEDS_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_LEDs, 1, 0x00000000);

	XGpio_Initialize(&xgpio_switches, XPAR_SLIDE_SWITCHES_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_switches, 1, 0x000000FF);
}

int testSwitchesAndLEDs()
{
    init_platform();
    initGPIO();

	unsigned int switch_val = XGpio_DiscreteRead(&xgpio_switches, 1);
	xil_printf("Switch val: %x \n\r", switch_val);
    while(1)
    {
		switch_val = XGpio_DiscreteRead(&xgpio_switches, 1);
		XGpio_DiscreteWrite(&xgpio_LEDs, 1, switch_val);
    }

    cleanup_platform();

    return 0;
}

void printArray(int* array, int length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		printf("%d: %x\r\n", i, array[i]);
	}
}

void setArray(int* array, int length, int rst)
{
	int i;
	for(i = 0; i < length; i++)
	{
		array[i] = rst ? 0 : 0xDEADBEEF + i;
	}
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr)
{
	static int boolPrintedFIT = 0;
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	//Check the PS2 Interrupt first
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		if(!boolPrintedFIT)
		{
			xil_printf("FIT Timer Interrupt\n\r");
			boolPrintedFIT = 1;
		}
	}
	if (intc_status & XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK);
		printf("DMA Interrupt\n\r");
	}
}

void initInterrupts()
{
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK
		   | XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

int main()
{
    init_platform();
    initInterrupts();

	int source_word[16];
	int destination_word[16];
	setArray(source_word, 16, 0);
	setArray(destination_word, 16, 1);

    printf("Printing value before DMA transfer.\n\r");
    printArray(destination_word, 16);

    DMA_CONTROLLER_InitiateTransfer(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word, (Xuint32) &destination_word, 4 * 10);

    printf("Printing value after DMA transfer.\n\r");
    printArray(destination_word, 16);

	setArray(source_word, 6, 1);
    DMA_CONTROLLER_InitiateTransfer(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word, (Xuint32) &destination_word, 4 * 10);

    printf("Printing value after 2nd DMA transfer.\n\r");
    printArray(destination_word, 16);

    cleanup_platform();

    return 0;
}


