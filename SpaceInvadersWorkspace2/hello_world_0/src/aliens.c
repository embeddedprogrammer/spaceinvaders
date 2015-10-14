/*
 * aliens.c
 *
 *  Created on: Oct 13, 2015
 *      Author: superman
 */

#include "aliens.h"
#include "tank.h"
#include "bullets.h"
#include "fontBitmap.h"
#include "timers.h"
#include "tank.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define EXPLOSION_TIME 100
#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8
#define ALIEN_ADVANCE_TIME 500
#define SAUCER_ADVANCE_TIME 100
#define MAX_CONCURRENT_EXPLOSIONS 3
#define ALIEN_FIRE_TIME ALIEN_ADVANCE_TIME*2
#define SAUCER_START_TIME 1000*30

static bool aliens_alienLegsIn = false;
point_t explosions[MAX_CONCURRENT_EXPLOSIONS];
bool spacecraftTravelingRight;
point_t saucerLocation;

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

void aliens_init()
{
	point_t alienFleetPos;
	alienFleetPos.row = GAMEBUFFER_HEIGHT / 7;
	alienFleetPos.col = GAMEBUFFER_WIDTH / 6;
	setAlienFleetPositionGlobal(alienFleetPos);
	draw_AlienFleet(true);
	int r, c;
	for(r = 0; r < ALIEN_FLEET_ROWS; r++)
		for(c = 0; c < ALIEN_FLEET_COLS; c++)
			setAlienAlive(r, c, true);
	spacecraftTravelingRight = true;
	saucerLocation = (point_t){0, 0};
	setAlienFleetTopRowNumGlobal(0);
	setAlienFleetBottomRowNumGlobal(ALIEN_FLEET_ROWS-1);
	setAlienFleetLeftColNumGlobal(0);
	setAlienFleetRightColNumGlobal(ALIEN_FLEET_COLS-1);
	addTimer(ALIEN_ADVANCE_TIME, true, &aliens_shiftAlienFleet);
	addTimer(ALIEN_FIRE_TIME, true, &bullets_fireAlienBullet);
	addTimer(SAUCER_ADVANCE_TIME, true, &aliens_moveSaucer);
	addTimer(SAUCER_START_TIME, true, &aliens_startSaucer);
}

//get a viable alien on the bottom row that can fire a bullet
point_t getAlienBottomRow()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	int i;
	int alienCount = 0;
	int bottomRowArray[ALIEN_FLEET_COLS];
	for (i = leftAlienCol; i < rightAlienCol + 1; i++) // find which aliens are alive
		if (isAlienAlive(bottomAlienRow, i))
			bottomRowArray[alienCount++] = i;

	return draw_getAlienPosition(bottomAlienRow, bottomRowArray[rand() % alienCount]);// random alien
}

point_t aliens_getSaucerLocation()
{
	return saucerLocation;
}

void aliens_moveSaucer()
{
	if(saucerLocation.row != 0)
	{
		if(saucerLocation.col < 0 || saucerLocation.col > GAMEBUFFER_WIDTH - SAUCER_WIDTH)
		{
			draw_Saucer(saucerLocation, true);
			saucerLocation.row = 0;
			return;
		}
		point_t erasePos = saucerLocation;
		if(spacecraftTravelingRight)
		{
			erasePos.col = saucerLocation.col;
			saucerLocation.col += ALIEN_SHIFT_AMOUNT;
		}
		else
		{
			saucerLocation.col -= ALIEN_SHIFT_AMOUNT;
			erasePos.col = saucerLocation.col + SAUCER_WIDTH;
		}
		draw_rectangle(erasePos, ALIEN_SHIFT_AMOUNT, SAUCER_HEIGHT, BACKGROUND_COLOR);
		draw_Saucer(saucerLocation, false);
	}
}

void aliens_startSaucer()
{
	xil_printf("Start flying saucer\n\r");
	if(saucerLocation.row != 0)
	{
		xil_printf("Already exists a flying saucer\n\r");
		return;
	}
	saucerLocation.row = SCOREBOARD_SPACING + TANK_BITMAP_HEIGHT;
	if(rand() % 2)
	{
		spacecraftTravelingRight = true;
		saucerLocation.col = 0;
	}
	else
	{
		spacecraftTravelingRight = false;
		saucerLocation.col = GAMEBUFFER_WIDTH - SAUCER_WIDTH;
	}
}

void aliens_removeExplosion()
{
	int i;
	for(i = 0; i < MAX_CONCURRENT_EXPLOSIONS; i++)
		if(explosions[i].row != 0)
		{
			draw_AlienExplosion(explosions[i], true);
			explosions[i].row = 0;
		}
}

void aliens_explode(point_t location)
{
	draw_AlienExplosion(location, false);
	int i;
	for(i = 0; i < MAX_CONCURRENT_EXPLOSIONS; i++)
		if(explosions[i].row == 0)
		{
			draw_AlienExplosion(location, false);
			explosions[i] = location;
			addTimer(EXPLOSION_TIME, false, &aliens_removeExplosion);
			return;
		}
	xil_printf("Too many explosions\n\r");
}

// kills an alien then updates the rows and columns that are still viable
void aliens_killAlienRC(short row, short col)
{
	setAlienAlive(row, col, false);
	if(row == 0)
		addPoints(40);
	else if(row <= 2)
		addPoints(20);
	else //if(row <= 4)
		addPoints(10);
	point_t alienPos = draw_getAlienPosition(row, col);
	point_t explosionPos = (point_t){alienPos.col + (ALIEN_BITMAP_WIDTH  / 2) - (EXPLOSION_WIDTH  / 2),
									 alienPos.row + (ALIEN_BITMAP_HEIGHT / 2) - (EXPLOSION_HEIGHT / 2)};
	aliens_explode(explosionPos);

	//Update indexes:
	int minRow = ALIEN_FLEET_ROWS;
	int maxRow = 0;
	int minCol = ALIEN_FLEET_COLS;
	int maxCol = 0;
	int r = 0;
	int c = 0;

	for (r = 0; r < ALIEN_FLEET_ROWS; r++)
		for (c = 0; c < ALIEN_FLEET_COLS; c++)
			if (isAlienAlive(r, c))
			{
				if(r > maxRow)
					maxRow = r;
				if(r < minRow)
					minRow = r;
				if(c > maxCol)
					maxCol = c;
				if(c < minCol)
					minCol = c;
			}
	if (maxRow < minRow || maxCol < minCol) {
		nextLevel();
		return;
	}
	setAlienFleetTopRowNumGlobal(minRow);
	setAlienFleetBottomRowNumGlobal(maxRow);
	setAlienFleetLeftColNumGlobal(minCol);
	setAlienFleetRightColNumGlobal(maxCol);
}

int alienPoints;
point_t alienPointsPosition;

void aliens_erasePoints()
{
	draw_number(alienPoints, ALIEN_PTS_COLOR, alienPointsPosition, true);
}

void aliens_killSaucer()
{
	int strLen = 3;
	alienPointsPosition = (point_t){saucerLocation.col + (SAUCER_WIDTH  / 2) - ((FONT_COLS_OFFSET * strLen)  / 2),
									saucerLocation.row + (SAUCER_HEIGHT / 2) - (FONT_ROWS_OFFSET / 2)};
	draw_Saucer(saucerLocation, true);
	alienPoints = (rand() % 6 + 1) * 50;
	addPoints(alienPoints);
	draw_number(alienPoints, ALIEN_PTS_COLOR, alienPointsPosition, false);
	saucerLocation.row = 0;
	addTimer(EXPLOSION_TIME, false, &aliens_erasePoints);
}

void aliens_killAlien(uint alienIdx)
{
	aliens_killAlienRC(alienIdx / ALIEN_FLEET_COLS, alienIdx % ALIEN_FLEET_COLS);
}

//checks to see if the fleet is at right screen edge
bool aliens_fleetAtRightScreenEdge()
{
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMOUNT > GAMEBUFFER_WIDTH);
}

// checks to see if the fleet is at the left screen edge
bool aliens_fleetAtLeftScreenEdge()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int alienFleetLeftOffset = leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();

	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMOUNT < 0);
}

// erase the parts of aliens that get left behind by a redraw
void aliens_shiftDownErase(point_t alienPos)
{
	int i;
	point_t erasePos;
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int topAlienRow = getAlienFleetTopRowNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	erasePos.col = alienPos.col + leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	erasePos.row = alienPos.row - ALIEN_FLEET_SHIFT_DOWN_AMOUNT + topAlienRow*(ALIEN_VERTICAL_DISTANCE);
	for (i = topAlienRow; i < bottomAlienRow + 1; i++ ) {
		draw_rectangle(erasePos, ALIEN_HORIZONTAL_DISTANCE*(rightAlienCol - leftAlienCol + 1), ALIEN_FLEET_SHIFT_DOWN_AMOUNT, BACKGROUND_COLOR);
		erasePos.row += ALIEN_VERTICAL_DISTANCE;
	}
}

static bool directionRight = true;
bool aliens_fleetDirectionIsRight()
{
	return directionRight;
}

// shifts the whole alien fleet left right or down depending on position
void aliens_shiftAlienFleet()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	point_t alienPos = getAlienFleetPositionGlobal();
	bool shiftDown = (directionRight) ? aliens_fleetAtRightScreenEdge() : aliens_fleetAtLeftScreenEdge();// at screen edge

	if (shiftDown) {
		directionRight = !directionRight;
		alienPos.row += ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
		aliens_shiftDownErase(alienPos);// erase remaining alien parts

	} else {// shift left or right
		point_t erasePos;
		if (directionRight) {
			alienPos.col += ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + leftAlienCol*ALIEN_HORIZONTAL_DISTANCE - ALIEN_SHIFT_AMOUNT;

		} else {
			alienPos.col -= ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + (rightAlienCol+1)*ALIEN_HORIZONTAL_DISTANCE;
		}
		erasePos.row = alienPos.row;
		draw_rectangle(erasePos, ALIEN_SHIFT_AMOUNT, ALIEN_BITMAP_HEIGHT*ALIEN_FLEET_ROWS + ALIEN_VERTICAL_SPACER*4, BACKGROUND_COLOR);// undraw remaing alien pieces

	}
	setAlienFleetPositionGlobal(alienPos);
	aliens_alienLegsIn = !aliens_alienLegsIn;
	draw_AlienFleet(aliens_alienLegsIn);
}
