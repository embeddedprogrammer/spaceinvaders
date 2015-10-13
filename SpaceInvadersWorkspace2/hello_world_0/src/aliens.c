/*
 * aliens.c
 *
 *  Created on: Oct 13, 2015
 *      Author: superman
 */

#include "aliens.h"
#include "control.h"
#include "timers.h"
#include "tank.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define EXPLOSION_TIME 100


#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8
static bool aliens_alienLegsIn = false;

#define MAX_CONCURRENT_EXPLOSIONS 3
point_t explosions[MAX_CONCURRENT_EXPLOSIONS];

bool spacecraftTravelingRight = true;

point_t saucerLocation = (point_t){0, 0};

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
	saucerLocation.row = 10;
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

	setAlienFleetTopRowNumGlobal(minRow);
	setAlienFleetBottomRowNumGlobal(maxRow);
	setAlienFleetLeftColNumGlobal(minCol);
	setAlienFleetRightColNumGlobal(maxCol);
}

void aliens_killSaucer()
{
	point_t explosionPos = (point_t){saucerLocation.col + (SAUCER_WIDTH  / 2) - (EXPLOSION_WIDTH  / 2),
									 saucerLocation.row + (SAUCER_HEIGHT / 2) - (EXPLOSION_HEIGHT / 2)};
	draw_Saucer(saucerLocation, true);
	aliens_explode(explosionPos);
	saucerLocation.row = 0;
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
