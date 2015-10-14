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
#include "spaceInvaders.h"

bool isAlienAlive(int row, int col);
void aliens_init();
void aliens_moveSaucer();
void aliens_startSaucer();
void aliens_killAlien(uint alienIdx);
void aliens_shiftAlienFleet();
bool aliens_fleetDirectionIsRight();
void aliens_removeExplosion();
void aliens_killSaucer();
void aliens_killAlienRC(short row, short col);
point_t aliens_getSaucerLocation();
point_t getAlienBottomRow();
point_t getAlienFleetPositionGlobal();

int getAlienFleetLeftColNumGlobal();
int getAlienFleetRightColNumGlobal();
int getAlienFleetTopRowNumGlobal();
int getAlienFleetBottomRowNumGlobal();

#endif /* ALIENS_H_ */
