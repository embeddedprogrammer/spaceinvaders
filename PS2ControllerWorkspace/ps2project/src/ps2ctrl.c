/*****************************************************************************
* Filename:          C:\Users\superman\Documents\HinckWhite\SpaceInvaderXPS_HW2/drivers/ps2ctrl_v1_00_a/src/ps2ctrl.c
* Version:           1.00.a
* Description:       ps2ctrl Driver Source File
* Date:              Thu Nov 19 08:30:54 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "ps2ctrl.h"
#include "xparameters.h"
#include <stdbool.h>
#include <stdio.h>

/************************** Function Definitions ***************************/

unsigned char lastCmd = 0x0;

void init()
{
	enableReporting();
}

void writeCmd(unsigned char cmd)
{
	lastCmd = cmd;
	PS2CTRL_mWriteSlaveReg0(XPAR_PS2CTRL_0_BASEADDR, cmd);
}

void enableReporting()
{
	xil_printf("0xF4 Enable reporting\n\r");
	writeCmd(0xF4);
}

void resetErrors()
{
	xil_printf("Reset errors\n\r");
	PS2CTRL_mWriteSlaveReg2(XPAR_PS2CTRL_0_BASEADDR, 0);
}

void reset()
{
	xil_printf("0xFF Reset\n\r");
	writeCmd(0xFF);
}

bool reportingEnabled = false;

bool getBit2(Xuint32 num, int i)
{
	return (num >> i) & 0x1;
}

void printBinary2(Xuint32 num)
{
    int i;
    for(i = 0; i < 8; i++)
    {
        if(i % 4 == 0)
            xil_printf(" ");
        if(i % 8 == 0)
            xil_printf("  ");
        xil_printf("%d", getBit2(num, 7 - i));
    }
}

int x = 0;
int y = 0;
int b = 0;

int getXMovement()
{
	xil_printf("x: %d ", x);
	return x;
	x = 0;
}

int getYMovement()
{
	xil_printf("y: %d ", y);
	return y;
	y = 0;
}

int getMouseButtons()
{
	if(getBit2(b, 0))
		xil_printf("Left Click");
	if(getBit2(b, 1))
		xil_printf("Right Click");
	if(getBit2(b, 2))
		xil_printf("Middle Click");
	return b;
}

int reportedCharacter = 0;


void mouseStateMachine(unsigned char readVal)
{
	if(lastCmd != 0x00 && (readVal == 0xFA || readVal == 0xF4)) // Todo: Somehow some of our FA's are shifted to be F4's
	{
		xil_printf("0x%x Acknowledge\n\r", readVal);
		if(lastCmd == 0xF4)
		{
			reportedCharacter = 0;
			reportingEnabled = true;
		}
		if(lastCmd == 0xFF)
			reportingEnabled = false;
		lastCmd = 0x00;
	}
	else if(reportingEnabled)
	{
		//printBinary2(readVal);
		//xil_printf(" Reporting enabled");
		if(reportedCharacter == 0)
		{
			if(getBit2(readVal, 3) != 1)
			{
				xil_printf("b");
				return;
			}
			b = readVal & 0x7;
		}
		else if(reportedCharacter == 1)
			x += ((char)readVal);
		else if(reportedCharacter == 2)
			y += ((char)readVal);
		reportedCharacter = (reportedCharacter + 1) % 3;
	}
	else if(readVal == 0xAA)
		xil_printf("0x%x Self test passed\n\r", readVal);
	else if(readVal == 0x00)
		xil_printf("0x%x Mouse ID\n\r", readVal);
	else
		xil_printf("0x%x ??\n\r", readVal);
}
