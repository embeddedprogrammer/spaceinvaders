/*
 * shapebitmap.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#ifndef SHAPEBITMAP_C_
#define SHAPEBITMAP_C_

#include <sys/types.h>

//dimensions
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7
#define EXPLOSION_WIDTH 12
#define EXPLOSION_HEIGHT 10
#define EXPOLSION_BITMAP_WIDTH 14
#define EXPOLSION_BITMAP_HEIGHT 10
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define ALIEN_BITMAP_WIDTH 14
#define ALIEN_BITMAP_HEIGHT 8
#define TANK_BITMAP_WIDTH 19
#define TANK_BITMAP_HEIGHT 8
#define TANK_WIDTH 17
#define TANK_HEIGHT 8
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_DAMAGE_WIDTH 6
#define BUNKER_DAMAGE_HEIGHT 6
#define BULLET_WIDTH 3
#define BULLET_HEIGHT 5
#define BULLET_TANK_OFFSET 8
#define BULLET_ALIEN_OFFSET 5
#define BUNKER_SECTION_ROWS 3
#define BUNKER_SECTION_COLS 4

//colors
#define BUNKER_COLOR     0x0000FF00
#define TANK_COLOR       0x0000FF00
#define ALIEN_COLOR      0x00FFFFFF
#define BULLET_COLOR     0x00FFFFFF
#define SAUCER_COLOR     0x00FF0000
#define BACKGROUND_COLOR 0x00000000
#define GAME_OVER_COLOR  0x00FF0000
#define SCORE_ABC_COLOR  0x00FFFFFF
#define SCORE_123_COLOR  0x0000FF00
#define ALIEN_PTS_COLOR  0x00FFFFFF

//bitmaps

typedef struct
{
	uint width;
	uint height;
	int shapeColor;
	const int* shapeBuffer;
} bitmap_t;

extern const bitmap_t bitmapSaucer;
extern const bitmap_t bitmapExplosion;
extern const bitmap_t bitmapAlienTopIn;
extern const bitmap_t bitmapAlienTopOut;
extern const bitmap_t bitmapAlienMiddleIn;
extern const bitmap_t bitmapAlienMiddleOut;
extern const bitmap_t bitmapAlienBottomIn;
extern const bitmap_t bitmapAlienBottomOut;
extern const bitmap_t bitmapTank;
extern const bitmap_t bitmapBunker;
extern const bitmap_t bitmapBunkerDamage0;
extern const bitmap_t bitmapBunkerDamage1;
extern const bitmap_t bitmapBunkerDamage2;
extern const bitmap_t bitmapBunkerDamage3;
extern const bitmap_t bitmapBulletAlien1;
extern const bitmap_t bitmapBulletAlien2;
extern const bitmap_t bitmapBulletTank;

#endif /* SHAPEBITMAP_C_ */
