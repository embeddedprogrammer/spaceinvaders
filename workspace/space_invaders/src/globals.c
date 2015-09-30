/*
 * globals.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "globals.h"

static uint* framePointer = (uint*) FRAME_BUFFER_ADDR;

static uint* bunker0FramePointer = 0;
static uint* bunker1FramePointer = 0;
static uint* bunker2FramePointer = 0;
static uint* bunker3FramePointer = 0;

static byte bunker0Damage[3][4] =
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

static byte bunker1Damage[3][4] =
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

static byte bunker2Damage[3][4] =
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

static byte bunker3Damage[3][4] =
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

uint* getFrameBuffer()
{
	return framePointer;
}

byte* getBunkerDamage(bunker_t bunkerNum)
{
	switch (bunkerNum) {
	case bunker_0: return ARRAY_PTR_2D(bunker0Damage);
	case bunker_1: return ARRAY_PTR_2D(bunker1Damage);
	case bunker_2: return ARRAY_PTR_2D(bunker2Damage);
	case bunker_3: return ARRAY_PTR_2D(bunker3Damage);
	default:
		return NULL;
	}
}

uint* getBunkerFramePointer(bunker_t bunkerNum)
{
	switch (bunkerNum) {
	case bunker_0: return bunker0FramePointer;
	case bunker_1: return bunker1FramePointer;
	case bunker_2: return bunker2FramePointer;
	case bunker_3: return bunker3FramePointer;
	default:
		return NULL;
	}
}

static point_t g_tankPos;

point_t getTankPositionGlobal() {
	return g_tankPos;
}

void setTankPositionGlobal(point_t tankPos) {
	g_tankPos = tankPos;
}

static bool g_aliensAlive[] = {true,true,true,true,true,true,true,true,true,true,true,
						   	   true,true,true,true,true,true,true,true,true,true,true,
							   true,true,true,true,true,true,true,true,true,true,true,
							   true,true,true,true,true,true,true,true,true,true,true,
							   true,true,true,true,true,true,true,true,true,true,true};

bool* getAliensAliveArrayGlobal()
{
	return ARRAY_PTR(g_aliensAlive);
}

static point_t g_alienFleetPos;

point_t getAlienFleetPositionGlobal() {
	return g_alienFleetPos;
}

void setAlienFleetPositionGlobal(point_t alienFleetPos) {
	g_alienFleetPos = alienFleetPos;
}

static int leftAlienCol = 0;
static int topAlienRow = 0;
static int rightAlienCol = 10;
static int bottomAlienRow = 4;

void setAlienFleetLeftColNumGlobal(int left)
{
	leftAlienCol = left;
}

void setAlienFleetRightColNumGlobal(int right)
{
	rightAlienCol = right;
}

void setAlienFleetTopRowNumGlobal(int top)
{
	topAlienRow = top;
}

void setAlienFleetBottomRowNumGlobal(int bottom)
{
	bottomAlienRow = bottom;
}

int getAlienFleetLeftColNumGlobal()
{
	return leftAlienCol;
}
int getAlienFleetRightColNumGlobal()
{
	return rightAlienCol;
}
int getAlienFleetTopRowNumGlobal()
{
	return topAlienRow;
}
int getAlienFleetBottomRowNumGlobal()
{
	return bottomAlienRow;
}





