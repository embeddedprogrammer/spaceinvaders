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
#include "ps2ctrl.h"


#include <stdbool.h>

bool getBit(Xuint32 num, int i)
{
	return (num >> i) & 0x1;
}

void printBinary(Xuint32 num)
{
    int i;
    for(i = 0; i < 32; i++)
    {
        if(i % 4 == 0)
            xil_printf(" ");
        if(i % 8 == 0)
            xil_printf("  ");
        xil_printf("%d", getBit(num, 31 - i));
    }
    xil_printf("\n\r");
}

void test()
{
	xil_printf("Registers:\n\r");
	xil_printf("Reg 0 - bitsToSend  ");
	printBinary(PS2CTRL_mReadSlaveReg0(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 1 - readVal     ");
	printBinary(PS2CTRL_mReadSlaveReg1(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 2 - state & errs");
	printBinary(PS2CTRL_mReadSlaveReg2(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 3 - bitsReceived");
	printBinary(PS2CTRL_mReadSlaveReg3(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 4 - counter     ");
	printBinary(PS2CTRL_mReadSlaveReg4(XPAR_PS2CTRL_0_BASEADDR));
}

unsigned int circularBuffer[100];
int startIndex = -1;
int endIndex = -1;

int size()
{
	if(startIndex == -1)
		return 0;
	else if(startIndex <= endIndex)
		return endIndex - startIndex + 1;
	else
		return (endIndex + 100) - startIndex + 1;
}

void push(unsigned int c)
{
	if(startIndex == -1)
	{
		startIndex = 0;
		endIndex = 0;
	}
	else if(size() == 100)
		xil_printf("Queue full!");
	else
	{
		endIndex = (endIndex + 1) % 100;
	}
	circularBuffer[endIndex] = c;
}

unsigned int pop()
{
	if(size() == 0)
	{
		xil_printf("Queue empty!\n\r");
		return 0;
	}
	int val = circularBuffer[startIndex];
	if(size() == 1)
	{
		startIndex = -1;
		endIndex = -1;
	}
	else
		startIndex = (startIndex + 1) % 100;
	return val;
}

void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the PIT interrupt first.
	if (intc_status & XPAR_PS2CTRL_0_INTERRUPT_MASK)
	{
		//unsigned int readVal = PS2CTRL_mReadSlaveReg1(XPAR_PS2CTRL_0_BASEADDR);
		unsigned int receivedVal = PS2CTRL_mReadSlaveReg3(XPAR_PS2CTRL_0_BASEADDR);
		push(receivedVal);
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
	}
}

XGpio gpPB;   // This is a handle for the push-button GPIO block.

void initInterrupts()
{
	// Initialize the GPIO peripherals. NOTE: We wait to do this till after the HDMI to ensure that nothing happens before the HDMI is enabled.
	XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

void util_delayMs(int ms)
{
	volatile int i = 0;
	volatile int m = 0;
	for(m = 0; m < ms; m++)
		for(i = 0; i < 10000; i++);
}

void pollButtons()
{
	const int PUSH_BUTTONS_CENTER = 0x01;
    const int PUSH_BUTTONS_RIGHT  = 0x02;
    const int PUSH_BUTTONS_LEFT   = 0x08;
    const int PUSH_BUTTONS_UP     = 0x10;
    const int PUSH_BUTTONS_DOWN   = 0x04;

	int buttonState = XGpio_DiscreteRead(&gpPB, 1);

	if (buttonState & PUSH_BUTTONS_LEFT)
		test();
	else if (buttonState & PUSH_BUTTONS_RIGHT)
	{
		enableReporting();
		util_delayMs(500);
	}
	else if (buttonState & PUSH_BUTTONS_UP)
	{
		reset();
		util_delayMs(500);
	}
	else if (buttonState & PUSH_BUTTONS_CENTER)
		xil_printf("\n\r");
	else if (buttonState & PUSH_BUTTONS_DOWN)
	{
		push(0x1);
		push(0x2);
		push(0x3);
		push(0x4);
		push(0x5);
		push(0x6);
	}
}

char lastCharReceived;

bool isParityValid(unsigned int num)
{
	bool result = 0;
	int i;
	for(i = 1; i <= 9; i++)
		result = result ^ getBit(num, i);
	return result; //If odd parity (what it should be), return true.
}

void printInfo()
{
	int count = 0;
	while(size() > 0)
	{
		unsigned int valReceived = pop();
		unsigned char readVal = (valReceived >> 1) & 0xFF;
		if(valReceived % 2 != 0)
			xil_printf("-");
		else if(!isParityValid(valReceived))
			xil_printf("=");
		mouseStateMachine(readVal);
		count++;
	}
	if(count > 0)
		xil_printf("\n\r");
}

int main()
{
    init_platform();
    initInterrupts();
    setvbuf(stdin, NULL, _IONBF, 1024); //This makes it so we don't have to wait for the user to push enter.
    while(true)
    {
    	pollButtons();
    	printInfo();
    }
    xil_printf("End\n\r");
    cleanup_platform();
    return 0;
}
