/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "control.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define MAX_TANK_BULLETS 1
#define MAX_ALIEN_BULLETS 4
#define FIRST_ALIEN_BULLET 1
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

// kills an alien then updates the rows and columns that are still viable
void control_killAlien(uint alienIdx)
{
	if (alienIdx >= TOTAL_ALIENS) return;
	bool* aliensAlive = getAliensAliveArrayGlobal();
	aliensAlive[alienIdx] = false; // kill alien
	/* update aliens matrix dimensions */
	int i;
	int j;
	for (i = 0; i < ALIEN_FLEET_ROWS; i++)
		for (j = 0; j < ALIEN_FLEET_COLS; j++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetTopRowNumGlobal(i);
				goto topAlienExit;
			}
	topAlienExit:
	for (i = ALIEN_FLEET_ROWS - 1; i >= 0; i--)
		for (j = 0; j < ALIEN_FLEET_COLS; j++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetBottomRowNumGlobal(i);
				goto bottomAlienExit;
			}

	bottomAlienExit:
	for (j = 0; j < ALIEN_FLEET_COLS; j++)
		for (i = 0; i < ALIEN_FLEET_ROWS; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetLeftColNumGlobal(j);
				goto leftAlienExit;
			}
	leftAlienExit:
	for (j = ALIEN_FLEET_COLS - 1; j >= 0; j--)
		for (i = 0; i < ALIEN_FLEET_ROWS; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetRightColNumGlobal(j);
				goto rightAlienExit;
			}
	rightAlienExit:
	{// undraw the alien
		point_t alienToKillPos;
		point_t alienPos = getAlienFleetPositionGlobal();
		alienToKillPos.row = alienPos.row + (alienIdx / ALIEN_FLEET_COLS)*(ALIEN_VERTICAL_DISTANCE);
		alienToKillPos.col = alienPos.col + (alienIdx % ALIEN_FLEET_COLS)*ALIEN_HORIZONTAL_DISTANCE;
		draw_rectangle(alienToKillPos, ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR);
	}
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
}

//advances the bullet specified by index i
void control_moveBullet(int i)
{
	erase_bullet(bullets[i]);
	if(bullets[i].bulletType == bullet_tank)
		bullets[i].location.row--;
	else if(bullets[i].bulletType == bullet_alien1 || bullets[i].bulletType == bullet_alien2)
		bullets[i].location.row++;
	if(bullets[i].location.row < 0 || bullets[i].location.row >= GAMEBUFFER_HEIGHT)
	{
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
	bool* alienAlive = getAliensAliveArrayGlobal();
	int i;
	int alienCount = 0;
	int bottomRowStart = bottomAlienRow*ALIEN_FLEET_COLS + leftAlienCol;
	int bottomRowArray[ALIEN_FLEET_COLS];
	for (i = bottomRowStart; i < bottomRowStart + (rightAlienCol - leftAlienCol + 1); i++ )// find which aliens are alive
		if (alienAlive[i])
			bottomRowArray[alienCount++] = i;

	return draw_getAlienPosition(bottomAlienRow, bottomRowArray[rand() % alienCount] % ALIEN_FLEET_COLS);// random alien
}

//fires a alien bullet if one is avalible
void control_fireAlienBullet(uint alienIdx)
{
	int i;
	for(i = FIRST_ALIEN_BULLET; i < FIRST_ALIEN_BULLET + MAX_ALIEN_BULLETS; i++)
		if(bullets[i].bulletType == bullet_none)
		{
			bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;// random bullet
			point_t alienPos = getAlienBottomRow();
			bullets[i].location.row = alienPos.row + ALIEN_BITMAP_HEIGHT;
			bullets[i].location.col = alienPos.col + BULLET_ALIEN_OFFSET;
			draw_bullet(bullets[i]);
			return;
		}
	xil_printf("Cannot fire - Maxed out alien bullets\n\r");
}

// fires a tank bullet and places it at the end of its turret
void control_fireTankBullet()
{
	int i;
	for(i = 0; i < MAX_TANK_BULLETS; i++)
		if(bullets[i].bulletType == bullet_none)
		{
			bullets[i].bulletType = bullet_tank;
			point_t tankPos = getTankPositionGlobal();
			bullets[i].location.row = tankPos.row - BULLET_HEIGHT;
			bullets[i].location.col = tankPos.col + BULLET_TANK_OFFSET;

			draw_bullet(bullets[i]);
			return;
		}
	xil_printf("Cannot fire - Maxed out tank bullets\n\r");
}

// ******** Bunkers ********

//erodes a particular bunker section based on bunker row and column
void control_erodeBunkerSection(int bunker, int row, int col)
{
	byte damage = getBunkerDamage(bunker, row, col);
	damage++;
	draw_BunkerDamageAtIndex(bunker, row, col, damage - 1);
	setBunkerDamage(bunker, row, col, damage);
}

//erodes the entire bunker
void control_erodeBunker(int bunker)
{
	int i, j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 4; j++)
			control_erodeBunkerSection(bunker, i, j);
}


