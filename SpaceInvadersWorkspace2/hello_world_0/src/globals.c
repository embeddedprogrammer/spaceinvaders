/*
 * globals.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "globals.h"

static uint* framePointer = (uint*) FRAME_BUFFER_ADDR;

uint* getFrameBuffer()
{
	return framePointer;
}











