/*
 * tank.h
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "drawShape.h"

#ifndef TANK_H_
#define TANK_H_

void tank_tick();

//move tank left
void tank_moveLeft();

//move tank right
void tank_moveRight();

void tank_kill();

bool tank_killIfCollision(point_t location);

#endif /* TANK_H_ */
