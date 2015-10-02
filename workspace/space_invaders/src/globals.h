/*
 * globals.h
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>
#include <sys/types.h>
#include "stdbool.h"
#include "shapebitmap.h"

#define SCREENBUFFER_WIDTH  640
#define SCREENBUFFER_HEIGHT 480

#define GAMEBUFFER_WIDTH  SCREENBUFFER_WIDTH/2
#define GAMEBUFFER_HEIGHT SCREENBUFFER_HEIGHT/2

#define TO_SCREENSIZE(X) (X<<1)
#define TO_GAMESIZE(X) (X>>1)

#define ARRAY_2D(i,j) (i*11 + j)

#define FRAME_BUFFER_ADDR 0xC1000000 // Starting location in DDR where we will store the images that we display.

#define BUNKER_DAMAGE_ARRAY_ROWS 3
#define BUNKER_DAMAGE_ARRAY_COLS 4

#define TOTAL_BUNKERS 4

#define TOTAL_ALIENS 55

#define ALIEN_FLEET_COLS 11
#define ALIEN_FLEET_ROWS 5

#define ALIEN_SHIFT_AMOUNT 4
#define ALIEN_VERTICAL_SPACER 6

#define ALIEN_HORIZONTAL_DISTANCE (ALIEN_BITMAP_WIDTH)
#define ALIEN_VERTICAL_DISTANCE (ALIEN_BITMAP_HEIGHT + ALIEN_VERTICAL_SPACER)

#define TANK_INTIAL_COL GAMEBUFFER_WIDTH/2
#define TANK_ROW GAMEBUFFER_HEIGHT*8/9

#define ARRAY_PTR(X) (&X[0])
#define ARRAY_PTR_2D(X) (&X[0][0])

typedef char byte;

typedef struct
{
	short col;
	short row;
} point_t;

typedef struct
{
	ushort width;
	ushort height;
} dimension_t;

typedef enum {
	bunker_0,
	bunker_1,
	bunker_2,
	bunker_3
}bunker_t;

uint* getFrameBuffer();

byte getBunkerDamage(int bunker, int row, int col);

void setBunkerDamage(int bunker, int row, int col, byte damage);

point_t getTankPositionGlobal();

void setTankPositionGlobal(point_t tankPos);

bool* getAliensAliveArrayGlobal();

point_t getAlienFleetPositionGlobal();

void setAlienFleetPositionGlobal(point_t alienFleetPos);

void setAlienFleetLeftColNumGlobal(int left);

void setAlienFleetRightColNumGlobal(int right);

void setAlienFleetTopRowNumGlobal(int top);

void setAlienFleetBottomRowNumGlobal(int bottom);

int getAlienFleetLeftColNumGlobal();

int getAlienFleetRightColNumGlobal();

int getAlienFleetTopRowNumGlobal();

int getAlienFleetBottomRowNumGlobal();










#endif /* GLOBALS_H_ */
