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

void aliens_killAlien(uint alienIdx);
void aliens_shiftAlienFleet();
bool aliens_fleetDirectionIsRight();
void aliens_removeExplosion();
void control_moveAllBullets();
void control_fireAlienBullet();
void control_fireTankBullet();
void bunkers_erodeBunker(int bunker);


#endif /* CONTROL_H_ */
