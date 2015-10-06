/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "control.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define FIRST_TANK_BULLET 0
#define MAX_TANK_BULLETS 10
#define FIRST_ALIEN_BULLET MAX_TANK_BULLETS
#define MAX_ALIEN_BULLETS 10
#define MAX_BULLETS_COUNT (MAX_TANK_BULLETS + MAX_ALIEN_BULLETS)
static bullet_t bullets[MAX_BULLETS_COUNT];

#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8

//move tank left
void control_moveTankLeft()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col -= 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

//move tank right
void control_moveTankRight()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col += 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

static bool alienLegsIn = false;

// kills an alien then updates the rows and columns that are still viable
void control_killAlienRC(short row, short col)
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

void control_killAlien(uint alienIdx)
{
	control_killAlienRC(alienIdx / ALIEN_FLEET_COLS, alienIdx % ALIEN_FLEET_COLS);
}

//checks to see if the fleet is at right screen edge
bool alienFleetAtRightScreenEdge()
{
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMOUNT > GAMEBUFFER_WIDTH);
}

// checks to see if the fleet is at the left screen edge
bool alienFleetAtLeftScreenEdge()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int alienFleetLeftOffset = leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();

	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMOUNT < 0);
}

// erase the parts of aliens that get left behind by a redraw
void shiftDownErase(point_t alienPos)
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
bool control_AlienDirectionIsRight() { return directionRight; }

// shifts the whole alien fleet left right or down depending on position
void control_shiftAlienFleet()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	point_t alienPos = getAlienFleetPositionGlobal();
	bool shiftDown = (directionRight) ? alienFleetAtRightScreenEdge() : alienFleetAtLeftScreenEdge();// at screen edge

	if (shiftDown) {
		directionRight = !directionRight;
		alienPos.row += ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
		shiftDownErase(alienPos);// erase remaining alien parts

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

//erodes a particular bunker section based on bunker row and column
void control_erodeBunkerSection(int bunker, int row, int col)
{
	if (row >= BUNKER_DAMAGE_ARRAY_ROWS || row < 0 || col >= BUNKER_DAMAGE_ARRAY_COLS || col < 0)
		return;

	byte damage = getBunkerDamage(bunker, row, col);
	damage++;
	draw_BunkerDamageAtIndex(bunker, row, col, damage - 1);
	setBunkerDamage(bunker, row, col, damage);
}

//erodes the entire bunker
void control_erodeBunker(int bunker)
{
	int i, j;
	for(i = 0; i < BUNKER_SECTION_ROWS; i++)
		for(j = 0; j < BUNKER_SECTION_COLS; j++)
			control_erodeBunkerSection(bunker, i, j);
}

bool killAlienIfAlienCollision(point_t location)
{

	point_t fleetPosition = getAlienFleetPositionGlobal();
	if(location.row > fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
	   location.col > fleetPosition.col && location.col < (fleetPosition.col + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
	{
		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
		control_killAlienRC(row, col);
		return true;
	}
	return false;
}

bool erodeBunkerIfCollision(point_t location)
{
	xil_printf("Check if bunker collision at %d, %d\n\r", location.col, location.row);
	int i;
	for (i = 0; i < TOTAL_BUNKERS; i++ ) {

		point_t bunkerPos = draw_getBunkerLocation(i);
		if ((location.row >= bunkerPos.row) && location.row < (bunkerPos.row + BUNKER_HEIGHT) &&
		    (location.col >= bunkerPos.col) && location.col < (bunkerPos.col + BUNKER_WIDTH))
		{
			int row = (location.row - bunkerPos.row) / BUNKER_DAMAGE_HEIGHT;
			int col = (location.col - bunkerPos.col) / BUNKER_DAMAGE_WIDTH;
			control_erodeBunkerSection(i, row, col);
			return true;
		}
	}
	return false;
}

//void killTankIfTankCollision(point_t location)
//{
//	point_t fleetPosition = getAlienFleetPositionGlobal();
//	if(location.row > fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
//	   location.col > fleetPosition.col && location.col < (fleetPosition.row + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
//	{
//		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
//		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
//		control_killAlienRC(row, col);
//	}
//}
//
//void erodeBunkerIfBunkerCollision(point_t location)
//{
//	point_t fleetPosition = getAlienFleetPositionGlobal();
//	if(location.row > fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
//	   location.col > fleetPosition.col && location.col < (fleetPosition.row + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
//	{
//		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
//		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
//		control_killAlienRC(row, col);
//	}
//}

//advances the bullet specified by index i
void control_moveBullet(int i)
{
	erase_bullet(bullets[i]);

	//Move location
	if(bullets[i].bulletType == bullet_tank)
		bullets[i].location.row--;
	else if(bullets[i].bulletType == bullet_alien1 || bullets[i].bulletType == bullet_alien2)
		bullets[i].location.row++;

	//Check for off screen
	if(bullets[i].location.row < 0 || bullets[i].location.row >= GAMEBUFFER_HEIGHT - BULLET_HEIGHT)
	{
		bullets[i].bulletType = bullet_none;
		return;
	}
	short checkRow = bullets[i].location.row;
	short checkCol = bullets[i].location.col;
	if(bullets[i].bulletType == bullet_alien1 || bullets[i].bulletType == bullet_alien2)
		checkRow += BULLET_HEIGHT - 1;

	//Check for collisions
	if(getPixel((point_t){checkCol, checkRow}) != 0 || getPixel((point_t){checkCol + 1, checkRow}) != 0 || getPixel((point_t){checkCol + 2, checkRow}) != 0)
	{
		point_t location = (point_t){checkCol + 1, checkRow};
		xil_printf("Hit location: %d, %d\n\r", location.col, location.row);
		xil_printf("Pixel hit: %d\n\r", getPixel(location));
		if (bullets[i].bulletType != bullet_tank) {
			erodeBunkerIfCollision(location);
		} else {
			if (!killAlienIfAlienCollision(location))
				erodeBunkerIfCollision(location);
		}

		bullets[i].bulletType = bullet_none;
		return;
	}

	draw_bullet(bullets[i]);
}

//advances all on screen bullets
void control_moveAllBullets()
{
	int i;
	for(i = 0; i < MAX_BULLETS_COUNT; i++)
		if(bullets[i].bulletType != bullet_none)
			control_moveBullet(i);
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

int control_getFirstEmptyBulletPosition(int start, int length)
{
	int i = 0;
	for(i = start; i < start + length; i++)
		if(bullets[i].bulletType == bullet_none)
			return i;
	return -1;
}

//fires a alien bullet if one is avalible
void control_fireAlienBullet()
{
	xil_printf("stuck\n\r");
	int i = control_getFirstEmptyBulletPosition(FIRST_ALIEN_BULLET, MAX_ALIEN_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;
		point_t alienPos = getAlienBottomRow();
		bullets[i].location.row = alienPos.row + ALIEN_BITMAP_HEIGHT;
		bullets[i].location.col = alienPos.col + BULLET_ALIEN_OFFSET;
		draw_bullet(bullets[i]);
	}
	else
		xil_printf("Cannot fire - Maxed out alien bullets\n\r");
	xil_printf("Not stuck\n\r");
}

// fires a tank bullet and places it at the end of its turret
void control_fireTankBullet()
{
	int i = control_getFirstEmptyBulletPosition(FIRST_TANK_BULLET, MAX_TANK_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = bullet_tank;
		point_t tankPos = getTankPositionGlobal();
		bullets[i].location.row = tankPos.row - BULLET_HEIGHT;
		bullets[i].location.col = tankPos.col + BULLET_TANK_OFFSET;

		draw_bullet(bullets[i]);
	}
	else
		xil_printf("Cannot fire - Maxed out tank bullets\n\r");
}

// ******** Bunkers ********






