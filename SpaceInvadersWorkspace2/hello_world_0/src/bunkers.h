/*
 * bunkers.h
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "drawShape.h"

#ifndef BUNKERS_H_
#define BUNKERS_H_

void bunkers_erodeSection(int bunker, int row, int col);
void bunkers_erode(int bunker);
bool bunkers_erodeIfCollision(point_t location);

#endif /* BUNKERS_H_ */
