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
#define MAX_ALIEN_BULLETS 10
#define FIRST_ALIEN_BULLET MAX_TANK_BULLETS
#define MAX_BULLETS_COUNT (MAX_TANK_BULLETS + MAX_ALIEN_BULLETS)
static bullet_t bullets[MAX_BULLETS_COUNT];

#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8

void control_moveTankLeft()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col -= 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

void control_moveTankRight()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col += 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

void control_killAlien(uint alienIdx)
{
	if (alienIdx >= TOTAL_ALIENS) return;
	bool* aliensAlive = getAliensAliveArrayGlobal();
	aliensAlive[alienIdx] = false;
	/* update aliens matrix dimensions */
	int i;
	int j;
	for (i = 0; i < 5; i++)
		for (j = 0; j < 11; j++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetTopRowNumGlobal(i);
				goto topAlienExit;
			}
	topAlienExit:
	for (i = 4; i >= 0; i--)
		for (j = 0; j < 11; j++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetBottomRowNumGlobal(i);
				goto bottomAlienExit;
			}

	bottomAlienExit:
	for (j = 0; j < 11; j++)
		for (i = 0; i < 5; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetLeftColNumGlobal(j);
				goto leftAlienExit;
			}
	leftAlienExit:
	for (j = 10; j >= 0; j--)
		for (i = 0; i < 5; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				setAlienFleetRightColNumGlobal(j);
				return;
			}
	point_t alienPos = getAlienFleetPositionGlobal();
//	alienPos.row = alienIdx
//	alienPos.row
//	draw_Shape(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR, alienPos, ARRAY_PTR(alien_erase_14x8));

}

bool alienFleetAtRightScreenEdge()
{
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMOUNT > GAMEBUFFER_WIDTH);
}

bool alienFleetAtLeftScreenEdge()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int alienFleetLeftOffset = leftAlienCol*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
//	{
//		xil_printf("alien col = %d\n\r", alienPos.col);
//		xil_printf("alien row = %d\n\r", alienPos.row);
//		xil_printf("offset = %d\n\r", alienFleetLeftOffset);
//		xil_printf("param = %d\n\r", alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMMOUNT);
//	}
	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMOUNT < 0);
}

void shiftAlienFleetDown()
{
	point_t alienPos = getAlienFleetPositionGlobal();
	alienPos.row -= ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
	setAlienFleetPositionGlobal(alienPos);
}

void shiftDownErase(point_t alienPos)
{
	int i;
	point_t erasePos;
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int topAlienRow = getAlienFleetTopRowNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	erasePos.col = alienPos.col + leftAlienCol*ALIEN_BITMAP_WIDTH;
	erasePos.row = alienPos.row - ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
	for (i = topAlienRow; i < bottomAlienRow + 1; i++ ) {
		draw_rectangle(erasePos, ALIEN_BITMAP_WIDTH*(rightAlienCol - leftAlienCol + 1), ALIEN_FLEET_SHIFT_DOWN_AMOUNT, BACKGROUND_COLOR);
		erasePos.row += ALIEN_BITMAP_HEIGHT + ALIEN_VERTICAL_SPACER;

	}
}

static bool directionRight = true;
bool control_AlienDirectionIsRight() { return directionRight; }

void control_shiftAlienFleet()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	point_t alienPos = getAlienFleetPositionGlobal();
	bool shiftDown = (directionRight) ? alienFleetAtRightScreenEdge() : alienFleetAtLeftScreenEdge();
	if (alienFleetAtLeftScreenEdge()) {
		xil_printf("fleet at left screen edge\n\r");
	}

	if (shiftDown) {
		directionRight = !directionRight;
		alienPos.row += ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
		shiftDownErase(alienPos);

	} else {
		point_t erasePos;
		if (directionRight) {
			alienPos.col += ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + leftAlienCol*ALIEN_BITMAP_WIDTH - ALIEN_SHIFT_AMOUNT;

		} else {
			alienPos.col -= ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + (rightAlienCol+1)*ALIEN_BITMAP_WIDTH;
		}
		erasePos.row = alienPos.row;
		draw_rectangle(erasePos, ALIEN_SHIFT_AMOUNT, ALIEN_BITMAP_HEIGHT*5 + ALIEN_VERTICAL_SPACER*4, BACKGROUND_COLOR);

	}
	setAlienFleetPositionGlobal(alienPos);
}

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

void control_moveAllBullets()
{
	int i;
	for(i = 0; i < MAX_BULLETS_COUNT; i++)
		if(bullets[i].bulletType != bullet_none)
			control_moveBullet(i);
}

void control_fireAlienBullet(uint alienIdx)
{
	int i;
	for(i = FIRST_ALIEN_BULLET; i < FIRST_ALIEN_BULLET + MAX_ALIEN_BULLETS; i++)
		if(bullets[i].bulletType == bullet_none)
		{
			bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;
			//TODO : Dead aliens can't fire bullets
			bullets[i].location = draw_getAlienPosition(4, rand() % 11);
			draw_bullet(bullets[i]);
			return;
		}
	xil_printf("Cannot fire - Maxed out alien bullets\n\r");
}

void control_fireTankBullet()
{
	int i;
	for(i = 0; i < MAX_TANK_BULLETS; i++)
		if(bullets[i].bulletType == bullet_none)
		{
			bullets[i].bulletType = bullet_tank;
			bullets[i].location = getTankPositionGlobal();
			draw_bullet(bullets[i]);
			return;
		}
	xil_printf("Cannot fire - Maxed out tank bullets\n\r");
}

// ******** Bunkers ********


void control_erodeBunkerSection(int bunker, int row, int col)
{
	byte damage = getBunkerDamage(bunker, row, col);
	damage++;
	draw_BunkerDamageAtIndex(bunker, row, col, damage - 1);
	setBunkerDamage(bunker, row, col, damage);
}

void control_erodeBunker(int bunker)
{
	int i, j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 4; j++)
			control_erodeBunkerSection(bunker, i, j);
}


