/*
 * aliens.h
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */
#include "drawShape.h"

#ifndef ALIENS_H_
#define ALIENS_H_

void aliens_tick(short row, short col);

void aliens_killAlienRC(short row, short col);

void aliens_killAlien(uint alienIdx);

//checks to see if the fleet is at right screen edge
bool aliens_FleetAtRightScreenEdge();

// checks to see if the fleet is at the left screen edge
bool aliens_FleetAtLeftScreenEdge();

// erase the parts of aliens that get left behind by a redraw
void aliens_shiftDownErase(point_t alienPos);

bool aliens_AlienDirectionIsRight();

// shifts the whole alien fleet left right or down depending on position
void aliens_shiftAlienFleet();

//get a viable alien on the bottom row that can fire a bullet
point_t aliens_getAlienBottomRow();

bool aliens_killIfCollision(point_t location);

#endif /* ALIENS_H_ */
