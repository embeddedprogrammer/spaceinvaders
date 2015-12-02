/*
 * draw_Shape.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "drawShape.h"
#include "tank.h"
#include "shapeBitmap.h"
#include "fontBitmap.h"
#include "bullets.h"
#include "aliens.h"
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

// Clear the entire screen
void draw_clearScreen()
{
	uint* framePointer = getFrameBuffer();
	// Clear screen
	int row, col;
	for (row = 0; row < SCREENBUFFER_HEIGHT; row++)
		for (col = 0; col < SCREENBUFFER_WIDTH; col++) {
			framePointer[row * SCREENBUFFER_WIDTH + col] = BACKGROUND_COLOR;
		}
}

// Return the pixel of the bitmap of the specified location
bool getBitmapPixel(bitmap_t bitmap, int row, int col)
{
	uint shapeWord = bitmap.shapeBuffer[row];
	int amountToShiftWord = bitmap.width - 1 - col;
	return shapeWord & (0x01 << amountToShiftWord);
}

int getBunkerOrTankPixel(point_t location)
{
	int bunker;
	for (bunker = 0; bunker < TOTAL_BUNKERS; bunker++)
	{
		point_t bunkerPos = draw_getBunkerLocation(bunker);
		if ((location.row >= bunkerPos.row) && location.row < (bunkerPos.row + BUNKER_HEIGHT) &&
		    (location.col >= bunkerPos.col) && location.col < (bunkerPos.col + BUNKER_WIDTH))
		{
			int bunkerPixelRow = location.row - bunkerPos.row;
			int bunkerPixelCol = location.col - bunkerPos.col;
			int bunkerErodeSectionRow = bunkerPixelRow / BUNKER_DAMAGE_HEIGHT;
			int bunkerErodeSectionCol = bunkerPixelCol / BUNKER_DAMAGE_WIDTH;
			int erodeSectionPixelRow = bunkerPixelRow % BUNKER_DAMAGE_HEIGHT;
			int erodeSectionPixelCol = bunkerPixelCol % BUNKER_DAMAGE_WIDTH;
			bool bunkerPixel = getBitmapPixel(bitmapBunker, bunkerPixelRow, bunkerPixelCol);
			if(bunkerPixel)
			{
				int damage = getBunkerDamage(bunker, bunkerErodeSectionRow, bunkerErodeSectionCol);
				if(damage == 0)
					return BUNKER_COLOR;
				else if(damage >= 1 && getBitmapPixel(bitmapBunkerDamage0, erodeSectionPixelRow, erodeSectionPixelCol))
					return BACKGROUND_COLOR;
				else if(damage >= 2 && getBitmapPixel(bitmapBunkerDamage1, erodeSectionPixelRow, erodeSectionPixelCol))
					return BACKGROUND_COLOR;
				else if(damage >= 3 && getBitmapPixel(bitmapBunkerDamage2, erodeSectionPixelRow, erodeSectionPixelCol))
					return BACKGROUND_COLOR;
				else if(damage >= 4 && getBitmapPixel(bitmapBunkerDamage3, erodeSectionPixelRow, erodeSectionPixelCol))
					return BACKGROUND_COLOR;
				else
					return BUNKER_COLOR;
			}
			else
				return BACKGROUND_COLOR;
		}
	}
    point_t tankPosition = getTankPositionGlobal();
    if(location.row >= tankPosition.row && location.row < (tankPosition.row + TANK_BITMAP_HEIGHT) &&
       location.col >= tankPosition.col && location.col < (tankPosition.col + TANK_BITMAP_WIDTH))
    {
		int tankPixelRow = location.row - tankPosition.row;
		int tankPixelCol = location.col - tankPosition.col;
		bool tankPixel = getBitmapPixel(bitmapTank, tankPixelRow, tankPixelCol);
		return tankPixel ? TANK_COLOR : BACKGROUND_COLOR;
    }
	return BACKGROUND_COLOR;
}

// Draw a 2x2 pixel using the 320x240 pixel location
// Use top layer only for aliens drawing because when it erases
// it will check and see if the pixel is part of a bunker.
void drawPixel(point_t location, int color, bool topLayer)
{
	if(location.col < 0 || location.row < 0 || location.col >= GAMEBUFFER_WIDTH || location.row >= GAMEBUFFER_HEIGHT)
		return;//Out of bounds
	if(topLayer && color == BACKGROUND_COLOR)
		color = getBunkerOrTankPixel(location);
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

// Draw a bitmap at the specified location
void draw_bitmap(bitmap_t bitmap, bool eraseForeground, bool eraseBackground, point_t pos, bool topLayer)
{
	int row, col;
	for (row = 0; row < bitmap.height; row++)
	{
		for (col = 0; col < bitmap.width; col++)
		{
			bool pixel = getBitmapPixel(bitmap, row, col);
			point_t pixelLocation = (point_t){pos.col + col, pos.row + row};
			if(pixel)
				drawPixel(pixelLocation, eraseForeground ? BACKGROUND_COLOR : bitmap.shapeColor, topLayer);
			else if(eraseBackground)
				drawPixel(pixelLocation, BACKGROUND_COLOR, topLayer);
		}
	}
}

void draw_Saucer(point_t position, bool erase)
{
	draw_bitmap(bitmapSaucer, erase, true, position, false);
}

void draw_AlienExplosion(point_t position, bool erase) {
	draw_bitmap(bitmapExplosion, erase, true, position, true);
}

void draw_AlienTop(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienTopIn : bitmapAlienTopOut, false, true, position, true);
}

void draw_AlienMiddle(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienMiddleIn : bitmapAlienMiddleOut, false, true, position, true);
}

void draw_AlienBottom(point_t position, bool in) {
	draw_bitmap(in ? bitmapAlienBottomIn : bitmapAlienBottomOut, false, true, position, true);
}

void draw_tank(point_t position, bool erase)
{
	draw_bitmap(bitmapTank, erase, true, position, false);
}

void draw_erodeTank(point_t position)
{
	int i;
	for(i = 0; i < 3; i++)
		draw_BunkerDamageAtLocation((point_t){position.col + i * BUNKER_DAMAGE_WIDTH, position.row}, 0);
}

void draw_Bunker(point_t position) {
	draw_bitmap(bitmapBunker, false, true, position, false);
}

point_t draw_getBunkerLocation(int i)
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
		draw_Bunker(draw_getBunkerLocation(i));
	draw_rectangle((point_t){0, GRASS_ROW}, GAMEBUFFER_WIDTH, 1, BUNKER_COLOR, false);
}

// useful for erasing sections of the screen
void draw_rectangle(point_t pos, int width, int height, int color, bool topLayer)
{
	int row, col;
	for (row = pos.row; row < pos.row + height; row++)
		for (col = pos.col; col < pos.col + width; col++)
			drawPixel((point_t){col, row}, color, topLayer);
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
				draw_rectangle(alienPos, ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, BACKGROUND_COLOR, true);//undraw alien location
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
	draw_bitmap(damagePtr, true, false, position, false);
}

void draw_BunkerDamageAtIndex(int bunker, int row, int col, byte damage)
{
	point_t pos = draw_getBunkerLocation(bunker);
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
	draw_bitmap(bullet_ptr, erase, false, bullet.location, false);
}

void draw_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, false);
}

void erase_bullet(bullet_t bullet)
{
	draw_bullet_color(bullet, true);
}

void draw_character(char c, int fontColor, point_t position, bool erase)
{
	int offset = 0;
	if(c >= 'A' && c <= 'Z')
		offset = c - 'A' + CHARACTER_OFFSET_UPPERCASE_A;
	else if(c >= 'a' && c <= 'z')
		offset = c - 'a' + CHARACTER_OFFSET_LOWERCASE_A;
	else if(c >= '0' && c <= '9')
		offset = c - '0' + CHARACTER_OFFSET_NUMBER_0;
	else if(c == ' ')
	{
		draw_rectangle(position, FONT_WIDTH, FONT_HEIGHT, BACKGROUND_COLOR, false);
		return;
	}
	else
	{
		xil_printf("Character %c not found in font map\n\r", c);
		return;
	}
	draw_bitmap((bitmap_t){FONT_WIDTH, FONT_HEIGHT, fontColor, characters[offset]}, erase, true, position, false);
}

void draw_string(const char* s, int fontColor, point_t position, bool erase)
{
	int i = 0;
	while(s[i] != '\0')
	{
		draw_character(s[i], fontColor, position, erase);
		position.col += FONT_COLS_OFFSET;
		i++;
	}
}

void draw_number(int num, int fontColor, point_t position, bool erase)
{
	int maxLength = 4;
	char buffer[] = "    ";
	int pos = 0;
	int digitValue = 1;
	while(digitValue*10 <= num)
		digitValue*=10;
	int digit;
	while(digitValue > 0 && pos < maxLength)
	{
		digit = num / digitValue;
		num -= (digit * digitValue);
		digitValue /= 10;
		buffer[pos] = '0' + digit;
		pos++;
	}
	draw_string(buffer, SCORE_123_COLOR, position, erase);
}

void draw_captureScreen()
{
	int row, col;
	uint* framePointer = getFrameBuffer();
	uint* screenCapture = getScreenCaptureFramePointer();
	for (row = 0; row < SCREENBUFFER_HEIGHT; row++)
		for (col = 0; col < SCREENBUFFER_WIDTH; col++) {
			int index = row * SCREENBUFFER_WIDTH + col;
			screenCapture[index] = framePointer[index];
		}
}
