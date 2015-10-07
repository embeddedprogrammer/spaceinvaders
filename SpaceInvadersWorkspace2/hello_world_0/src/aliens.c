/*
 * aliens.c
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */
#include "aliens.h"
#include "globals.h"
#include "drawShape.h"
#include "stdio.h"
#include <stdlib.h> //for rand()

#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8

static bool alienLegsIn = false;

// kills an alien then updates the rows and columns that are still viable
void aliens_killAlienRC(short row, short col)
{
	setAlienAlive(row, col, false);
	point_t alienPos = draw_getAlienPosition(row, col);
	draw_rectangle(alienPos, ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR);
	draw_AlienFleet(alienLegsIn);

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

	xil_printf("Bounds: %d %d %d %d\n\r", maxRow ,minRow, maxCol, minCol);

	setAlienFleetTopRowNumGlobal(minRow);
	setAlienFleetBottomRowNumGlobal(maxRow);
	setAlienFleetLeftColNumGlobal(minCol);
	setAlienFleetRightColNumGlobal(maxCol);
}

void aliens_killAlien(uint alienIdx)
{
	aliens_killAlienRC(alienIdx / ALIEN_FLEET_COLS, alienIdx % ALIEN_FLEET_COLS);
}

//checks to see if the fleet is at right screen edge
bool aliens_FleetAtRightScreenEdge()
{
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMOUNT > GAMEBUFFER_WIDTH);
}

// checks to see if the fleet is at the left screen edge
bool aliens_FleetAtLeftScreenEdge()
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
bool aliens_AlienDirectionIsRight()
{
	return directionRight;
}

// shifts the whole alien fleet left right or down depending on position
void aliens_shiftAlienFleet()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	point_t alienPos = getAlienFleetPositionGlobal();
	bool shiftDown = (directionRight) ? aliens_FleetAtLeftScreenEdge() : aliens_FleetAtLeftScreenEdge();// at screen edge

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
	alienLegsIn = !alienLegsIn;
	draw_AlienFleet(alienLegsIn);
}

//get a viable alien on the bottom row that can fire a bullet
point_t aliens_getAlienBottomRow()
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

bool aliens_killIfCollision(point_t location)
{
	point_t fleetPosition = getAlienFleetPositionGlobal();
	if(location.row > fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
	   location.col > fleetPosition.col && location.col < (fleetPosition.col + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
	{
		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
		aliens_killAlienRC(row, col);
		return true;
	}
	return false;
}
