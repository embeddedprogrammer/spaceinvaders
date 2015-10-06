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

//Draw a 2x2 pixel using the 320x240 pixel location
int getPixel(point_t location)
{
	uint* frameBuffer = getFrameBuffer();
	int row = TO_SCREENSIZE(location.row);
	int col = TO_SCREENSIZE(location.col);
	return frameBuffer[row*SCREENBUFFER_WIDTH + col];
}

// Return the pixel of the bitmap of the specified location
bool getBitmapPixel(const int* shapeBuffer, const uint width, int row, int col)
{
	uint shapeWord = shapeBuffer[row];
	int amountToShiftWord = width - 1 - col;
	return shapeWord & (0x01 << amountToShiftWord);
}

// Draw a bitmap at the specified location
void draw_bitmap(bitmap_t bitmap, bool eraseForeground, bool eraseBackground, point_t pos)
{
	int row, col;
	for (row = 0; row < bitmap.height; row++)
	{
		for (col = 0; col < bitmap.width; col++)
		{
			bool pixel = getBitmapPixel(bitmap.shapeBuffer, bitmap.width, row, col);
			point_t pixelLocation = (point_t){pos.col + col, pos.row + row};
			if(pixel)
				drawPixel(pixelLocation, eraseForeground ? BACKGROUND_COLOR : bitmap.shapeColor);
			else if(eraseBackground)
				drawPixel(pixelLocation, BACKGROUND_COLOR);
		}
	}
}

void draw_Saucer(point_t position) {
	draw_bitmap(bitmapSaucer, false, true, position);
}

void draw_AlienExplosion(point_t position) {
	draw_bitmap(bitmapExplosion, false, true, position);
}

void draw_AlienTop(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienTopIn : bitmapAlienTopOut, false, true, position);
}

void draw_AlienMiddle(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienMiddleIn : bitmapAlienMiddleOut, false, true, position);
}

void draw_AlienBottom(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienBottomIn : bitmapAlienBottomOut, false, true, position);
}

void draw_Tank(point_t position) {
	draw_bitmap(bitmapTank, false, true, position);
}

void draw_Bunker(point_t position) {
	draw_bitmap(bitmapBunker, false, true, position);
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

//point_t getAlienIndex()
//{
//
//}

//given row and column returns aliens position
point_t draw_getAlienPosition(int row, int col)
{
	point_t alienPos;
	point_t alienFleetPos = getAlienFleetPositionGlobal();
	alienPos.row = alienFleetPos.row + row*(ALIEN_VERTICAL_DISTANCE);
	alienPos.col = alienFleetPos.col + col*ALIEN_HORIZONTAL_DISTANCE;
	return alienPos;
}

//draws the whole alien fleet
void draw_AlienFleet(bool in)
{
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
			if(isAlienAlive(i, j))
			{
				switch (i)
				{
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
			}
			else
			{
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
	bitmap_t damagePtr;
	switch (damage)
	{
	case 0:
		damagePtr = bitmapBunkerDamage0;
		break;
	case 1:
		damagePtr = bitmapBunkerDamage1;
		break;
	case 2:
		damagePtr = bitmapBunkerDamage2;
		break;
	case 3:
	default:
		damagePtr = bitmapBunkerDamage3;
		break;
	}
	draw_bitmap(damagePtr, true, false, position);
}

void draw_BunkerDamageAtIndex(int bunker, int row, int col, byte damage)
{
	point_t pos = getBunkerLocation(bunker);
	pos.row += row * BUNKER_DAMAGE_HEIGHT;
	pos.col += col * BUNKER_DAMAGE_WIDTH;
	draw_BunkerDamageAtLocation(pos, damage);
}

void draw_bullet_color(bullet_t bullet, bool erase)
{
	bitmap_t bullet_ptr;
	if(bullet.bulletType == bullet_tank)
		bullet_ptr = bitmapBulletTank;
	else if(bullet.bulletType == bullet_alien1)
		bullet_ptr = bitmapBulletAlien1;
	else // bullet_alien2
		bullet_ptr = bitmapBulletAlien2;
	draw_bitmap(bullet_ptr, erase, false, bullet.location);
}

void draw_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, false);
}


void erase_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, true);
}
