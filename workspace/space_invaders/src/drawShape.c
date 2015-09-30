/*
 * draw_Shape.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "drawShape.h"
#include "shapebitmap.h"
#include "control.h"
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

//draws a shape based on game coordinates and converts them to screen coordinates
void draw_Shape(const uint width, const uint height, int shapeColor, point_t pos, const int* shapeBuffer) {
	uint* frameBuffer = getFrameBuffer();

	uint row_start = TO_SCREENSIZE(pos.row);
	uint col_start = TO_SCREENSIZE(pos.col);
	uint maxCol = col_start + TO_SCREENSIZE(width);
	uint maxRow = row_start + TO_SCREENSIZE(height);
	uint row;
	uint col;
	uint y = 0;
	int inc_y = 0;
	for (row = row_start; row < maxRow; row++) {
		uint x = 0;
		uint shapeWord = shapeBuffer[y];
		for (col = col_start; col < maxCol; x++) {
			int point = shapeWord & (0x01 << (width - 1 - x));
			int color = (point) ? shapeColor : BACKGROUND_COLOR;
			frameBuffer[row*640 + col++] = color;
			frameBuffer[row*640 + col++] = color;
		}
		y += (inc_y++) % 2;// increment every other line.
	}
}

//draws a shape based on game coordinates and converts them to screen coordinates
void draw_Damage(const uint width, const uint height, point_t pos, const int* shapeBuffer) {
	uint* frameBuffer = getFrameBuffer();

	uint row_start = TO_SCREENSIZE(pos.row);
	uint col_start = TO_SCREENSIZE(pos.col);
	uint maxCol = col_start + TO_SCREENSIZE(width);
	uint maxRow = row_start + TO_SCREENSIZE(height);
	uint row;
	uint col;
	uint y = 0;
	int inc_y = 0;
	for (row = row_start; row < maxRow; row++) {
		uint x = 0;
		uint shapeWord = shapeBuffer[y];
		for (col = col_start; col < maxCol; x++) {
			int buffer_point = shapeWord & (0x01 << (width - 1 - x));
			int onScreen_point = frameBuffer[row*SCREENBUFFER_WIDTH + col];
			int color = (buffer_point | onScreen_point) ? BACKGROUND_COLOR : BACKGROUND_COLOR;
			frameBuffer[row*SCREENBUFFER_WIDTH + col++] = color;
			frameBuffer[row*SCREENBUFFER_WIDTH + col++] = color;
		}
		y += (inc_y++) % 2;// increment every other line.
	}
}

void draw_Saucer(point_t position) {
	draw_Shape(SAUCER_WIDTH, SAUCER_HEIGHT, SAUCER_COLOR, position, ARRAY_PTR(saucer_16x7));
}

void draw_AlienExplosion(point_t position) {
	draw_Shape(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ALIEN_COLOR, position, ARRAY_PTR(alien_explosion_14x10));
}

void draw_AlienTop(point_t position, bool in) {
	draw_Shape(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, position, (in) ? ARRAY_PTR(alien_top_in_14x8) : ARRAY_PTR(alien_top_out_14x8));
}

void draw_AlienMiddle(point_t position, bool in) {
	draw_Shape(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, position, (in) ? ARRAY_PTR(alien_middle_in_14x8) : ARRAY_PTR(alien_middle_out_14x8));
}

void draw_AlienBottom(point_t position, bool in) {
	draw_Shape(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, position, (in) ? ARRAY_PTR(alien_bottom_in_14x8) : ARRAY_PTR(alien_bottom_out_14x8));
}

void draw_Tank(point_t position) {
	draw_Shape(TANK_BITMAP_WIDTH, TANK_BITMAP_HEIGHT, TANK_COLOR, position, ARRAY_PTR(tank_19x8));
}

void draw_Bunker(point_t position) {
	draw_Shape(BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR, position, ARRAY_PTR(bunker_24x18));
}

void draw_Bunkers() {
	int bunkerWidthSpacing = GAMEBUFFER_WIDTH/TOTAL_BUNKERS;
	int topOfBunerHeight = (GAMEBUFFER_HEIGHT*3)/4;
	point_t pos;
	pos.col = bunkerWidthSpacing/2 - BUNKER_WIDTH/2;
	pos.row = topOfBunerHeight;
	int i;
	for (i = 0; i < TOTAL_BUNKERS; i++) {
		draw_Bunker(pos);
		pos.col += bunkerWidthSpacing;
	}

}

void draw_rectangle(point_t pos, int width, int height, int color) {
	uint* frameBuffer = getFrameBuffer();
	uint row_start = TO_SCREENSIZE(pos.row);
	uint col_start = TO_SCREENSIZE(pos.col);
	uint maxCol = col_start + TO_SCREENSIZE(width);
	uint maxRow = row_start + TO_SCREENSIZE(height);
	uint row;
	uint col;
	for (row = row_start; row < maxRow; row++) {
		for (col = col_start; col < maxCol;) {
			frameBuffer[row*SCREENBUFFER_WIDTH + col++] = color;
			frameBuffer[row*SCREENBUFFER_WIDTH + col++] = color;
		}
	}
}


void drawEraseBuffer(point_t pos) {
	const int horizontalEraseBuffer [ALIEN_BITMAP_HEIGHT] = {0,0,0,0,0,0,0,0};
	point_t eraseBufferPos;
	eraseBufferPos.row = pos.row;
	if(control_AlienDirectionIsRight()) {
		eraseBufferPos.col = pos.col - ALIEN_SHIFT_AMMOUNT;
	} else {
		eraseBufferPos.col = pos.col + ALIEN_BITMAP_WIDTH*11;
	}

	draw_Shape(ALIEN_SHIFT_AMMOUNT, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR, eraseBufferPos, ARRAY_PTR(horizontalEraseBuffer));
}
void draw_AlienFleet(bool in)
{
	bool* aliensAlive = getAliensAliveArrayGlobal();
	int i;
	int row= 0;
	point_t alienFleetPos = getAlienFleetPositionGlobal();
	point_t alienPos;
	const int startCol = alienFleetPos.col;
	alienPos.row = alienFleetPos.row;
	alienPos.col = startCol;
	for (i = 0; i < TOTAL_ALIENS; i++) {
		if (i % 11 == 0) {
			if (i != 0) {
				row++;
				alienPos.row += ALIEN_BITMAP_HEIGHT + ALIEN_VERTICAL_SPACER;
			}
			alienPos.col = startCol;
		} else {
			alienPos.col += ALIEN_BITMAP_WIDTH;
		}
		if (aliensAlive[i]) {
			switch (row) {
			case 0:
				draw_AlienTop(alienPos, in);
				break;
			case 1:
				draw_AlienMiddle(alienPos, in);
				break;
			case 2:
				draw_AlienMiddle(alienPos, in);
				break;
			case 3:
				draw_AlienBottom(alienPos, in);
				break;
			case 4:
				draw_AlienBottom(alienPos, in);
				break;
			}
		} else {
			draw_Shape(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR, alienPos, ARRAY_PTR(alien_erase_14x8));
		}
	}
}

void draw_BunkerDamage(point_t position) {
//
//	int bunker_6x6[6] = {0,0,0,0,0,0};
//	//read a 6x6 of buffer memory at location. really a 12x12
//	uint* frameBuffer = getFrameBuffer();
//	uint row;
//	uint col;
//	uint frame_row = position.row;
//	uint frame_col = position.col;
//	for (row = 0; row < 12; ++row) {
//
//			for (col = 0; col < 12; ++col) {
//				bool screenPoint = (frameBuffer[row * 640 + col] != BLACK);
//
//			}
//		}
//
//
//	for (row = 0; row < BUNKER_DAMAGE_ARRAY_ROWS; ++row) {
//		for (col = 0; col < BUNKER_DAMAGE_ARRAY_COLS; ++col) {
//
//		}
//	}
}


