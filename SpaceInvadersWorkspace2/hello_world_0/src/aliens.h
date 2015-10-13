/*
 * aliens.h
 *
 *  Created on: Oct 13, 2015
 *      Author: superman
 */

#ifndef ALIENS_H_
#define ALIENS_H_

#include "globals.h"
#include "drawShape.h"
#include "shapebitmap.h"

void aliens_moveSaucer();
void aliens_startSaucer();
void aliens_killAlien(uint alienIdx);
void aliens_shiftAlienFleet();
bool aliens_fleetDirectionIsRight();
void aliens_removeExplosion();
void aliens_killSaucer();
void aliens_killAlienRC(short row, short col);
point_t aliens_getSaucerLocation();

#endif /* ALIENS_H_ */
