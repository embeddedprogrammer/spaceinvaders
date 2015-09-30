/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "control.h"

static int leftAlienCol = 0;
static int topAlienRow = 0;
static int rightAlienCol = 10;
static int bottomAlienRow = 4;

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
//	int i;
//	int j;
//	for (i = 0; i < 5; i++)
//		for (j = 0; j < 11; j++)
//			if (aliensAlive[ARRAY_2D(i,j)])
//				topAlienRow = i;
//
//	for (i = 4; i >= 0; i++)
//		for (j = 0; j < 11; j++)
//			if (aliensAlive[ARRAY_2D(i,j)])
//				bottomAlienRow = i;
//
//
//	for (j = 0; j < 11; j++)
//		for (i = 0; i < 5; i++)
//			if (aliensAlive[ARRAY_2D(i,j)])
//				leftAlienCol = j;
//
//	for (j = 10; j >= 0; j--)
//		for (i = 0; i < 5; i++)
//			if (aliensAlive[ARRAY_2D(i,j)])
//				rightAlienCol = j;
}

bool alienFleetAtRightScreenEdge()
{
	int alienFleetWidth = (rightAlienCol - leftAlienCol + 1)*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMMOUNT > GAMEBUFFER_WIDTH);
}

bool alienFleetAtLeftScreenEdge()
{
	int alienFleetLeftOffset = leftAlienCol*ALIEN_BITMAP_WIDTH;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMMOUNT < 0);
}

void shiftAlienFleetDown()
{
	point_t alienPos = getAlienFleetPositionGlobal();
	alienPos.row -= ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
	setAlienFleetPositionGlobal(alienPos);
}

//void shiftDownErase(point_t alienPos)
//{
//	int i;
//	int row = alienPos.row;
//	int col = alienPos.col;
//	for (i = topAlienRow; i < bottomAlienRow; i++ ) {
//		point_t erasePos;
//		erasePos.row =
//	}
//}

static bool directionRight = true;
bool control_AlienDirectionIsRight() { return directionRight; }

void control_shiftAlienFleet()
{
	bool shiftDown = (directionRight) ? alienFleetAtRightScreenEdge() : alienFleetAtLeftScreenEdge();

	point_t alienPos = getAlienFleetPositionGlobal();

	if (shiftDown) {
		directionRight = !directionRight;
		alienPos.row += ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
		point_t erasePos;

	} else {
		point_t erasePos;
		if (directionRight) {
			alienPos.col = ALIEN_SHIFT_AMMOUNT;
			erasePos.col = alienPos.col - ALIEN_SHIFT_AMMOUNT;
		} else {
			alienPos.col = -ALIEN_SHIFT_AMMOUNT;
			erasePos.col = alienPos.col + ALIEN_BITMAP_WIDTH*11;
		}
		erasePos.row = alienPos.row;
		draw_rectangle(erasePos, ALIEN_SHIFT_AMMOUNT, ALIEN_BITMAP_HEIGHT*5 + ALIEN_VERTICAL_SPACER*4, BACKGROUND_COLOR);
	}



	setAlienFleetPositionGlobal(alienPos);
}




