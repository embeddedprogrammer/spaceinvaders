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

XGpio xgpio_pushButtons;
XGpio xgpio_LEDs;
XGpio xgpio_switches;

void initGPIO()
{
	XGpio_Initialize(&xgpio_pushButtons, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_pushButtons, 1, 0x0000001F);

	XGpio_Initialize(&xgpio_LEDs, XPAR_SLIDE_SWITCHES_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_LEDs, 1, 0x0000001F);

	XGpio_Initialize(&xgpio_switches, XPAR_LEDs_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&xgpio_switches, 1, 0x0000001F);
}

int main()
{
    init_platform();
    initGPIO();

    xil_printf("Hello World\n\r");

    cleanup_platform();

    return 0;
}


