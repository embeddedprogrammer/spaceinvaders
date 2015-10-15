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

void bullets_moveAllBullets();
void bullets_fireAlienBullet();
void bunkers_erodeBunker(int bunker);
void bullets_fireTankBullet();
void bullets_init();
void bunker_init(bool newGame);
byte getBunkerDamage(int bunker, int row, int col);


#endif /* CONTROL_H_ */
