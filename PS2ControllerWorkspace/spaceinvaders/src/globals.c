/*
 * globals.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "globals.h"

static uint* framePointer =   (uint*) FRAME_BUFFER_ADDR;
static uint* framePointer1 = ((uint*) FRAME_BUFFER_ADDR) + 640*480;

uint* getFrameBuffer()
{
	return framePointer;
}

uint* getScreenCaptureFramePointer()
{
	return framePointer1;
}











