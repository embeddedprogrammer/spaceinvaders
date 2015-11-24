/*
 * drawShape.h
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#ifndef DRAWSHAPE_H_
#define DRAWSHAPE_H_

#include "globals.h"
#include "aliens.h"
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>

#define SCOREBOARD_SPACING 10
#define GRASS_ROW GAMEBUFFER_HEIGHT-5

typedef struct
{
	enum bulletTypes {bullet_none, bullet_tank, bullet_alien1, bullet_alien2} bulletType;
	point_t location;
} bullet_t;

void draw_clearScreen();
int getPixel(point_t location);
void draw_Saucer(point_t position, bool erase);
void draw_AlienExplosion(point_t position, bool erase);
void draw_Bunkers();
void draw_tank(point_t position, bool erase);
void draw_erodeTank(point_t position);
void draw_AlienFleet(bool in);
void draw_rectangle(point_t pos, int width, int height, int color, bool topLayer);
void draw_bullet(bullet_t position);
void erase_bullet(bullet_t position);
point_t draw_getAlienPosition(int i, int j);
void draw_BunkerDamageAtLocation(point_t position, byte damage);
void draw_BunkerDamageAtIndex(int bunker, int i, int j, byte damage);
point_t draw_getBunkerLocation(int i);
void draw_character(char c, int fontColor, point_t position, bool erase);
void draw_string(const char* s, int fontColor, point_t position, bool erase);
void draw_number(int num, int fontColor, point_t position, bool erase);

#endif /* DRAWSHAPE_H_ */
