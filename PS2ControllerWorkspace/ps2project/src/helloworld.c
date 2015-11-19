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

void printBinary(Xuint32 num)
{
    int i;
    for(i = 0; i < 32; i++)
    {
        if(i % 4 == 0)
            xil_printf(" ");
        if(i % 8 == 0)
            xil_printf("  ");
        xil_printf("%d", (num >> (31 - i)) & 0x1);
    }
    xil_printf("\n\r");
}

void enableReporting()
{
	PS2CTRL_mWriteSlaveReg0(XPAR_PS2CTRL_0_BASEADDR, 0xF4);
}

void reset()
{
	PS2CTRL_mWriteSlaveReg0(XPAR_PS2CTRL_0_BASEADDR, 0xFF);
}

void test()
{
	xil_printf("Registers:\n\r");
	xil_printf("Reg 0 - bitsToSend  ");
	printBinary(PS2CTRL_mReadSlaveReg0(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 1 - readVal     ");
	printBinary(PS2CTRL_mReadSlaveReg1(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 2 - state       ");
	printBinary(PS2CTRL_mReadSlaveReg2(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 3 - bitsReceived");
	printBinary(PS2CTRL_mReadSlaveReg3(XPAR_PS2CTRL_0_BASEADDR));
	xil_printf("Reg 4 - counter     ");
	printBinary(PS2CTRL_mReadSlaveReg4(XPAR_PS2CTRL_0_BASEADDR));
}

void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the PIT interrupt first.
	if (intc_status & XPAR_PS2CTRL_0_INTERRUPT_MASK)
	{
		unsigned char c = PS2CTRL_mReadSlaveReg1(XPAR_PS2CTRL_0_BASEADDR);
		//if(c == 0xFA)

		xil_printf("0x%x\n\r", c);
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_INTERRUPT_MASK);
	}
}

void initInterrupts()
{
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

int main()
{
    init_platform();
    initInterrupts();
    setvbuf(stdin, NULL, _IONBF, 1024); //This makes it so we don't have to wait for the user to push enter.
    while(true)
    {
    	char c = getchar();
    	if(c == 'p')
    		test();
    	else if(c == 'f')
    		enableReporting();
    	else if(c == 'r')
    		reset();
    	else if(c == 'd')
    		XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, 0);
    	else if(c == 'e')
    		XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
    }
    xil_printf("End\n\r");
    cleanup_platform();
    return 0;
}
