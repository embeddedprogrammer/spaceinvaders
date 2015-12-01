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

int main()
{
	int source_word = 0xDEADBEEF;
	int destination_word = 0x0;

    init_platform();

    print("Hello World\n\r");
    cleanup_platform();
    printf("Printing value before DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    DMA_CONTROLLER_MasterRecvWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word);
    DMA_CONTROLLER_MasterSendWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &destination_word);

    printf("Printing value after DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

//    cleanup_platform();

    return 0;
}


