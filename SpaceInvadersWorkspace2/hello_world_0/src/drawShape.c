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

//Draw a 2x2 pixel using the 320x240 pixel location
void drawPixel(point_t location, int color)
{
	uint* frameBuffer = getFrameBuffer();
	int row = TO_SCREENSIZE(location.row);
	int col = TO_SCREENSIZE(location.col);
	int subRow, subCol;
	for(subRow = 0; subRow < 2; subRow++)
		for(subCol = 0; subCol < 2; subCol++)
			frameBuffer[(row + subRow)*SCREENBUFFER_WIDTH + (col + subCol)] = color;
}

// Return the pixel of the bitmap of the specified location
bool getBitmapPixel(const int* shapeBuffer, const uint width, int row, int col)
{
	uint shapeWord = shapeBuffer[row];
	int amountToShiftWord = width - 1 - col;
	return shapeWord & (0x01 << amountToShiftWord);
}

// Draw a bitmap at the specified location
void draw_bitmap(const uint width, const uint height, int shapeColor, bool eraseBackground, point_t pos, const int* shapeBuffer)
{
	int row, col;
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			bool pixel = getBitmapPixel(shapeBuffer, width, row, col);
			point_t pixelLocation = (point_t){pos.col + col, pos.row + row};
			if(pixel)
				drawPixel(pixelLocation, shapeColor);
			else if(eraseBackground)
				drawPixel(pixelLocation, BACKGROUND_COLOR);
		}
	}
}

void draw_Saucer(point_t position) {
	draw_bitmap(SAUCER_WIDTH, SAUCER_HEIGHT, SAUCER_COLOR, true, position, ARRAY_PTR(saucer_16x7));
}

void draw_AlienExplosion(point_t position) {
	draw_bitmap(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ALIEN_COLOR, true, position, ARRAY_PTR(alien_explosion_14x10));
}

void draw_AlienTop(point_t position, bool in) {
	draw_bitmap(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, true, position, (in) ? ARRAY_PTR(alien_top_in_14x8) : ARRAY_PTR(alien_top_out_14x8));
}

void draw_AlienMiddle(point_t position, bool in) {
	draw_bitmap(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, true, position, (in) ? ARRAY_PTR(alien_middle_in_14x8) : ARRAY_PTR(alien_middle_out_14x8));
}

void draw_AlienBottom(point_t position, bool in) {
	draw_bitmap(ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, true, position, (in) ? ARRAY_PTR(alien_bottom_in_14x8) : ARRAY_PTR(alien_bottom_out_14x8));
}

void draw_Tank(point_t position) {
	draw_bitmap(TANK_BITMAP_WIDTH, TANK_BITMAP_HEIGHT, TANK_COLOR, true, position, ARRAY_PTR(tank_19x8));
}

void draw_Bunker(point_t position) {
	draw_bitmap(BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR, true, position, ARRAY_PTR(bunker_24x18));
}

point_t getBunkerLocation(int i)
{
	int bunkerWidthSpacing = GAMEBUFFER_WIDTH/TOTAL_BUNKERS;
	int topOfBunerHeight = (GAMEBUFFER_HEIGHT*3)/4;
	point_t pos;
	pos.col = bunkerWidthSpacing/2 + i*bunkerWidthSpacing - BUNKER_WIDTH/2;
	pos.row = topOfBunerHeight;
	return pos;
}

void draw_Bunkers()
{
	int i;
	for (i = 0; i < TOTAL_BUNKERS; i++)
		draw_Bunker(getBunkerLocation(i));
}

// useful for erasing sections of the screen
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

//given row and column returns aliens position
point_t draw_getAlienPosition(int i, int j)
{
	point_t alienPos;
	point_t alienFleetPos = getAlienFleetPositionGlobal();
	alienPos.row = alienFleetPos.row + i*(ALIEN_VERTICAL_DISTANCE);
	alienPos.col = alienFleetPos.col + j*ALIEN_HORIZONTAL_DISTANCE;
	return alienPos;
}

//draws the whole alien fleet
void draw_AlienFleet(bool in)
{
	bool* aliensAlive = getAliensAliveArrayGlobal();
	int i;
	int j;
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int topAlienRow = getAlienFleetTopRowNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	point_t alienFleetPos = getAlienFleetPositionGlobal();
	point_t alienPos;
	const int startCol = alienFleetPos.col + leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	alienPos.row = alienFleetPos.row + topAlienRow*(ALIEN_VERTICAL_DISTANCE);
	alienPos.col = startCol;

	for (i = topAlienRow; i <= bottomAlienRow; i++) {
		for (j = leftAlienCol; j <= rightAlienCol; j++) {
			if (aliensAlive[ARRAY_2D(i,j)]) {
				switch (i) {
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
				draw_rectangle(alienPos, ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR);//undraw alien location
			}
			alienPos.col += ALIEN_HORIZONTAL_DISTANCE;
		}
		alienPos.col = startCol;
		alienPos.row += ALIEN_VERTICAL_DISTANCE;
	}
}

void draw_BunkerDamageAtLocation(point_t position, byte damage)
{
	const int* damagePtr;
	switch (damage)
	{
	case 0:
		damagePtr = bunkerDamage0_6x6;
		break;
	case 1:
		damagePtr = bunkerDamage1_6x6;
		break;
	case 2:
		damagePtr = bunkerDamage2_6x6;
		break;
	case 3:
	default:
		damagePtr = bunkerDamage3_6x6;
		break;
	}
	draw_bitmap(BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, false, position, damagePtr);
}

void draw_BunkerDamageAtIndex(int bunker, int row, int col, byte damage)
{
	point_t pos = getBunkerLocation(bunker);
	pos.row += row * BUNKER_DAMAGE_HEIGHT;
	pos.col += col * BUNKER_DAMAGE_WIDTH;
	draw_BunkerDamageAtLocation(pos, damage);
}

void draw_bullet_color(bullet_t bullet, int shapeColor)
{
	const int* bullet_ptr = NULL;
	if(bullet.bulletType == bullet_tank)
		bullet_ptr = ARRAY_PTR(bullet_tank_3x5);
	else if(bullet.bulletType == bullet_alien1)
		bullet_ptr = ARRAY_PTR(bullet_alien1_3x5);
	else // bullet_alien2
		bullet_ptr = ARRAY_PTR(bullet_alien2_3x5);
	draw_bitmap(BULLET_WIDTH, BULLET_HEIGHT, shapeColor, true, bullet.location, bullet_ptr);
}

void draw_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, BULLET_COLOR);
}


void erase_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, BACKGROUND_COLOR);
}
