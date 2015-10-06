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

static byte bunkerDamage[4][3][4];

uint* getFrameBuffer()
{
	return framePointer;
}

byte getBunkerDamage(int bunker, int row, int col)
{
	return bunkerDamage[bunker][row][col];
}

void setBunkerDamage(int bunker, int row, int col, byte damage)
{
	bunkerDamage[bunker][row][col] = damage;
}

static point_t g_tankPos;

point_t getTankPositionGlobal() {
	return g_tankPos;
}

void setTankPositionGlobal(point_t tankPos) {
	g_tankPos = tankPos;
}

static bool g_aliensAlive[5][11] = {{true, true, true, true, true, true, true, true, true, true, true},
									{true, true, true, true, true, true, true, true, true, true, true},
									{true, true, true, true, true, true, true, true, true, true, true},
									{true, true, true, true, true, true, true, true, true, true, true},
									{true, true, true, true, true, true, true, true, true, true, true}};

bool isAlienAlive(int row, int col)
{
	return g_aliensAlive[row][col];
}

void setAlienAlive(int row, int col, bool alive)
{
	g_aliensAlive[row][col] = alive;
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
static int rightAlienCol = ALIEN_FLEET_COLS - 1;
static int bottomAlienRow = ALIEN_FLEET_ROWS - 1;

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





