/*
 * tank.c
 *
 *  Created on: Oct 7, 2015
 *      Author: Troy Hinkley and Jacob White
 */

#include "tank.h"
#include "globals.h"
#include <stdio.h>

void tank_tick()
{

}

//move tank left
void tank_moveLeft()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col -= 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

//move tank right
void tank_moveRight()
{
	point_t tankPos = getTankPositionGlobal();
	tankPos.col += 2;
	setTankPositionGlobal(tankPos);
	draw_Tank(tankPos);

}

void tank_kill()
{
	xil_printf("Kill tank\n\r");
}

bool tank_killIfCollision(point_t location)
{
	point_t tankPosition = getTankPositionGlobal();
	if(location.row > tankPosition.row && location.row < (tankPosition.row + TANK_HEIGHT) &&
	   location.col > tankPosition.col && location.col < (tankPosition.row + TANK_WIDTH))
	{
		tank_kill();
		return true;
	}
	return false;
}
