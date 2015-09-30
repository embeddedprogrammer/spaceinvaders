/*
 * drawShape.h
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#ifndef DRAWSHAPE_H_
#define DRAWSHAPE_H_

#include "globals.h"
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>

void draw_Bunkers();
void draw_Tank(point_t position);
void draw_AlienFleet(bool in);
void draw_rectangle(point_t pos, int width, int height, int color);
void draw_bullet(point_t position);
void erase_bullet(point_t position);

#endif /* DRAWSHAPE_H_ */
