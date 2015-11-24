/*****************************************************************************
* Filename:          C:\Users\superman\Documents\HinckWhite\SpaceInvaderXPS_HW2/drivers/ps2ctrl_v1_00_a/src/ps2ctrl.c
* Version:           1.00.a
* Description:       ps2ctrl Driver Source File
* Date:              Thu Nov 19 08:30:54 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#include "mouse.h"
#include "xparameters.h"
#include <stdbool.h>
#include <stdio.h>

unsigned char lastCmd = 0x0;
static bool reportingEnabled = false;
static int x = 0;
static int y = 0;
static int b = 0;
int reportedCharacter = 0;

void mouse_init()
{
	mouse_enableReporting();
}

void mouse_writeCmd(unsigned char cmd)
{
	lastCmd = cmd;
	PS2CTRL_mWriteSlaveReg0(XPAR_PS2CTRL_0_BASEADDR, cmd);
}

void mouse_enableReporting()
{
	xil_printf("0xF4 Enable reporting\n\r");
	mouse_writeCmd(0xF4);
}

void mouse_resetErrors()
{
	xil_printf("Reset errors\n\r");
	PS2CTRL_mWriteSlaveReg2(XPAR_PS2CTRL_0_BASEADDR, 0);
}

void mouse_reset()
{
	xil_printf("0xFF Reset\n\r");
	mouse_writeCmd(0xFF);
}

bool mouse_getBit(Xuint32 num, int i)
{
	return (num >> i) & 0x1;
}

int mouse_getXMovement()
{
	int retVal = x;
	x = 0;
	return retVal;
}

int mouse_getYMovement()
{
	int retVal = y;
	y = 0;
	return retVal;
}

int mouse_getMouseButtons()
{
	return b;
}

void mouse_stateMachine(unsigned char readVal)
{
	if(lastCmd != 0x00 && (readVal == 0xFA || readVal == 0xF4))
	{
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
		if(reportedCharacter == 0)
		{
			if(mouse_getBit(readVal, 3) != 1)
				return;
			b = readVal & MOUSE_ALL_BUTTONS;
		}
		else if(reportedCharacter == 1)
			x += ((char)readVal);
		else if(reportedCharacter == 2)
			y += ((char)readVal);
		reportedCharacter = (reportedCharacter + 1) % 3;
	}
}
