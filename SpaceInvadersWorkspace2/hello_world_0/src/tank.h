/*
 * tank.h
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#ifndef TANK_H_
#define TANK_H_

#include "globals.h"

void tank_moveTankLeft();
void tank_moveTankRight();
void tank_killTank();
void tank_fireBullet();
void tank_init(bool gameOver);
void tank_cleanup();
point_t getTankPositionGlobal();
void addPoints(int points);

#endif /* TANK_H_ */
