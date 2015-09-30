/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "control.h"
#include <stdio.h>

static int leftAlienCol = 0;
static int topAlienRow = 0;
static int rightAlienCol = 10;
static int bottomAlienRow = 4;
static point_t bulletLocation = (point_t){10, 10};

#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8
#define ARRAY_2D(i,j) (i*11 + j)

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
				topAlienRow = i;
				goto topAlienExit;
			}
	topAlienExit:
	for (i = 4; i >= 0; i--)
		for (j = 0; j < 11; j++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				bottomAlienRow = i;
				goto bottomAlienExit;
			}

	bottomAlienExit:
	for (j = 0; j < 11; j++)
		for (i = 0; i < 5; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				leftAlienCol = j;
				goto leftAlienExit;
			}
	leftAlienExit:
	for (j = 10; j >= 0; j--)
		for (i = 0; i < 5; i++)
			if (aliensAlive[ARRAY_2D(i,j)]) {
				rightAlienCol = j;
				return;
			}
}

bool alienFleetAtRightScreenEdge()
{
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMMOUNT > GAMEBUFFER_WIDTH);
}

bool alienFleetAtLeftScreenEdge()
{
	int alienFleetLeftOffset = leftAlienCol*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
//	{
//		xil_printf("alien col = %d\n\r", alienPos.col);
//		xil_printf("alien row = %d\n\r", alienPos.row);
//		xil_printf("offset = %d\n\r", alienFleetLeftOffset);
//		xil_printf("param = %d\n\r", alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMMOUNT);
//	}
	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMMOUNT < 0);
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
		bool needEraseBuffer = true;
		if (directionRight) {
			alienPos.col += ALIEN_SHIFT_AMMOUNT;
			if (leftAlienCol != 0) {
				needEraseBuffer = false;
			} else {
				erasePos.col = alienPos.col - ALIEN_SHIFT_AMMOUNT;
			}
		} else {
			alienPos.col -= ALIEN_SHIFT_AMMOUNT;
			if (rightAlienCol != 10) {
				needEraseBuffer = false;
			} else {
				erasePos.col = alienPos.col + ALIEN_BITMAP_WIDTH*11;
			}
		}
		if (needEraseBuffer) {
			erasePos.row = alienPos.row;
			draw_rectangle(erasePos, ALIEN_SHIFT_AMMOUNT, ALIEN_BITMAP_HEIGHT*5 + ALIEN_VERTICAL_SPACER*4, BACKGROUND_COLOR);
		}
	}
	setAlienFleetPositionGlobal(alienPos);
}

void control_fireBullet()
{
	xil_printf("firing bullet\r\n");
	erase_bullet(bulletLocation);
	bulletLocation.row++;
	draw_bullet(bulletLocation);
}