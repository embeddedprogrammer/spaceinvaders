/*
 * control.h
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "globals.h"
#include "drawShape.h"
#include "shapebitmap.h"

void control_moveTankLeft();
void control_moveTankRight();
void aliens_killAlien(uint alienIdx);
void aliens_shiftAlienFleet();
bool aliens_fleetDirectionIsRight();
void control_moveAllBullets();
void control_fireAlienBullet();
void control_fireTankBullet();
void control_erodeBunker(int bunker);


#endif /* CONTROL_H_ */
