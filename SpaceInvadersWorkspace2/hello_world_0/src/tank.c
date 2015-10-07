/*
 * tank.c
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "tank.h"
#include "timers.h"
#include "globals.h"
#include "drawShape.h"
#include <stdio.h>

#define ERODE_TANK_TIME 1000
#define DISAPPEAR_TANK_TIME 2000

bool tankAlive = true;

//move tank left
void tank_moveTankLeft()
{
	if(!tankAlive)
		return;
	point_t tankPos = getTankPositionGlobal();
	tankPos.col -= 2;
	setTankPositionGlobal(tankPos);
	draw_tank(tankPos, false);
}

//move tank right
void tank_moveTankRight()
{
	if(!tankAlive)
		return;
	point_t tankPos = getTankPositionGlobal();
	tankPos.col += 2;
	setTankPositionGlobal(tankPos);
	draw_tank(tankPos, false);
}

void tank_fireBullet()
{
	if(tankAlive)
		control_fireTankBullet();
}

void tank_reviveTank()
{
	draw_tank(getTankPositionGlobal(), false);
	tankAlive = true;
}

void tank_disappearTank()
{
    draw_tank(getTankPositionGlobal(), true);
    addTimer(DISAPPEAR_TANK_TIME, false, &tank_reviveTank);
}

void tank_killTank()
{
	if(tankAlive)
	{
		tankAlive = false;
		draw_erodeTank(getTankPositionGlobal());
		addTimer(ERODE_TANK_TIME, false, &tank_disappearTank);
	}
}
