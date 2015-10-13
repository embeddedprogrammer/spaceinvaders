/*
 * shapebitmap.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#ifndef SHAPEBITMAP_C_
#define SHAPEBITMAP_C_

#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)        \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) | \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)                                        \
((b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword19(b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)             \
((b18 << 18) | (b17 << 17) | (b16 << 16) | (b15 << 15) | (b14 << 14) | (b13 << 13) |              \
 (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) |              \
 (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword14(b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)                                   \
((b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) |  \
 (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)                            \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword6(b5,b4,b3,b2,b1,b0)                                                 \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword3(b2,b1,b0)              \
((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

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
#define FONT_COLOR     0x0000FF00
#define BUNKER_COLOR     0x0000FF00
#define TANK_COLOR       0x0000FF00
#define ALIEN_COLOR      0x00FFFFFF
#define BULLET_COLOR     0x00FFFFFF
#define SAUCER_COLOR     0x00FF0000
#define BACKGROUND_COLOR 0x00000000

// Must define packword for each of the different bit-widths.
static const int saucer_16x7[] = {
	packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
	packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
	packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
	packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};


static const int alien_explosion_14x10[] = {
	packword14(0,0,0,0,0,0,0,1,0,0,0,0,0,0),
	packword14(0,0,0,0,1,0,0,1,0,0,0,1,0,0),
	packword14(0,1,0,0,1,0,0,0,0,0,1,0,0,0),
	packword14(0,0,1,0,0,1,0,0,0,1,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,1,1,0),
	packword14(0,1,1,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,1,0,0,0,1,0,0,1,0,0),
	packword14(0,0,0,1,0,0,0,0,0,1,0,0,1,0),
	packword14(0,0,1,0,0,0,1,0,0,1,0,0,0,0),
	packword14(0,0,0,0,0,0,1,0,0,0,0,0,0,0)
};


static const int alien_top_in_14x8[] = {
	packword14(0,0,0,0,0,0,1,1,0,0,0,0,0,0),
	packword14(0,0,0,0,0,1,1,1,1,0,0,0,0,0),
	packword14(0,0,0,0,1,1,1,1,1,1,0,0,0,0),
	packword14(0,0,0,1,1,0,1,1,0,1,1,0,0,0),
	packword14(0,0,0,1,1,1,1,1,1,1,1,0,0,0),
	packword14(0,0,0,0,1,0,1,1,0,1,0,0,0,0),
	packword14(0,0,0,1,0,0,0,0,0,0,1,0,0,0),
	packword14(0,0,0,0,1,0,0,0,0,1,0,0,0,0)
};

static const int alien_top_out_14x8[] = {
	packword14(0,0,0,0,0,0,1,1,0,0,0,0,0,0),
	packword14(0,0,0,0,0,1,1,1,1,0,0,0,0,0),
	packword14(0,0,0,0,1,1,1,1,1,1,0,0,0,0),
	packword14(0,0,0,1,1,0,1,1,0,1,1,0,0,0),
	packword14(0,0,0,1,1,1,1,1,1,1,1,0,0,0),
	packword14(0,0,0,0,0,1,0,0,1,0,0,0,0,0),
	packword14(0,0,0,0,1,0,1,1,0,1,0,0,0,0),
	packword14(0,0,0,1,0,1,0,0,1,0,1,0,0,0)
};
static const int alien_middle_in_14x8[] = {
	packword14(0,0,0,0,1,0,0,0,0,0,1,0,0,0),
	packword14(0,0,0,0,0,1,0,0,0,1,0,0,0,0),
	packword14(0,0,0,0,1,1,1,1,1,1,1,0,0,0),
	packword14(0,0,0,1,1,0,1,1,1,0,1,1,0,0),
	packword14(0,0,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,0,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,0,1,0,1,0,0,0,0,0,1,0,1,0),
	packword14(0,0,0,0,0,1,1,0,1,1,0,0,0,0)
};
static const int alien_middle_out_14x8[] = {
	packword14(0,0,0,0,1,0,0,0,0,0,1,0,0,0),
	packword14(0,0,1,0,0,1,0,0,0,1,0,0,1,0),
	packword14(0,0,1,0,1,1,1,1,1,1,1,0,1,0),
	packword14(0,0,1,1,1,0,1,1,1,0,1,1,1,0),
	packword14(0,0,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,0,0,1,1,1,1,1,1,1,1,1,0,0),
	packword14(0,0,0,0,1,0,0,0,0,0,1,0,0,0),
	packword14(0,0,0,1,0,0,0,0,0,0,0,1,0,0)
};
static const int alien_bottom_in_14x8[] = {
	packword14(0,0,0,0,0,1,1,1,1,0,0,0,0,0),
	packword14(0,0,1,1,1,1,1,1,1,1,1,1,0,0),
	packword14(0,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,1,1,1,0,0,1,1,0,0,1,1,1,0),
	packword14(0,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,0,0,1,1,1,0,0,1,1,1,0,0,0),
	packword14(0,0,1,1,0,0,1,1,0,0,1,1,0,0),
	packword14(0,0,0,1,1,0,0,0,0,1,1,0,0,0)
};
static const int alien_bottom_out_14x8[] = {
	packword14(0,0,0,0,0,1,1,1,1,0,0,0,0,0),
	packword14(0,0,1,1,1,1,1,1,1,1,1,1,0,0),
	packword14(0,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,1,1,1,0,0,1,1,0,0,1,1,1,0),
	packword14(0,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword14(0,0,0,0,1,1,0,0,1,1,0,0,0,0),
	packword14(0,0,0,1,1,0,1,1,0,1,1,0,0,0),
	packword14(0,1,1,0,0,0,0,0,0,0,0,1,1,0)
};

static const int tank_19x8[] = {
	packword19(0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0),
	packword19(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
	packword19(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
	packword19(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword19(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword19(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword19(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword19(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0)
};


// Shape of the entire bunker.
static const int bunker_24x18[] = {
	packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};


// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const int bunkerDamage0_6x6[] = {
	packword6(0,1,1,0,0,0),
	packword6(0,0,0,0,0,1),
	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,0,0,0,1,0)
};

static const int bunkerDamage1_6x6[] = {
	packword6(1,1,1,0,1,0),
	packword6(1,0,1,0,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(0,1,1,1,0,1),
	packword6(0,1,1,0,1,0)
};

static const int bunkerDamage2_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,0,1,1,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),
	packword6(0,1,1,1,0,1),
	packword6(1,1,1,1,1,1)
};

static const int bunkerDamage3_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1)
};

static const int bullet_alien1_3x5[] = {
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(1,1,1),
	packword3(0,1,0)};

static const int bullet_alien2_3x5[] = {
	packword3(0,1,0),
	packword3(0,0,1),
	packword3(0,1,0),
	packword3(1,0,0),
	packword3(0,1,0)};

static const int bullet_tank_3x5[] = {
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0)};

static const int number_0_6x7[] = {
	packword6(0,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,1,1,0),
	packword6(1,0,1,0,1,0),
	packword6(1,1,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0)};

static const int number_1_6x7[] = {
	packword6(0,0,1,0,0,0),
	packword6(0,1,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,1,1,1,0,0)};


static const int number_2_6x7[] = {
	packword6(0,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(0,0,0,0,1,0),
	packword6(0,0,0,1,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,1,0,0,0,0),
	packword6(1,1,1,1,1,0)};

static const int number_3_6x7[] = {
	packword6(1,1,1,1,1,0),
	packword6(0,0,0,1,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,0,1,0,0),
	packword6(0,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0)};


static const int number_4_6x7[] = {
	packword6(0,0,0,1,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,1,0,1,0,0),
	packword6(1,0,0,1,0,0),
	packword6(1,1,1,1,1,0),
	packword6(0,0,0,1,0,0),
	packword6(0,0,0,1,0,0)};

static const int number_5_6x7[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(1,1,1,1,0,0),
	packword6(0,0,0,0,1,0),
	packword6(0,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0)};

static const int number_6_6x7[] = {
	packword6(0,0,1,1,0,0),
	packword6(0,1,0,0,0,0),
	packword6(1,0,0,0,0,0),
	packword6(1,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0)};

static const int number_7_6x7[] = {
	packword6(1,1,1,1,1,0),
	packword6(0,0,0,0,1,0),
	packword6(0,0,0,1,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,0)};

static const int number_8_6x7[] = {
	packword6(0,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,0,0)};

static const int number_9_6x7[] = {
	packword6(0,1,1,1,0,0),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,0,1,0),
	packword6(0,1,1,1,1,0),
	packword6(0,0,0,0,1,0),
	packword6(0,0,0,1,0,0),
	packword6(0,1,1,0,0,0)};

typedef struct
{
	uint width;
	uint height;
	int shapeColor;
	const int* shapeBuffer;
} bitmap_t;

static const bitmap_t bitmapSaucer         = {SAUCER_WIDTH, SAUCER_HEIGHT, SAUCER_COLOR, saucer_16x7};
static const bitmap_t bitmapExplosion      = {EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ALIEN_COLOR, alien_explosion_14x10};
static const bitmap_t bitmapAlienTopIn     = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_top_in_14x8};
static const bitmap_t bitmapAlienTopOut    = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_top_out_14x8};
static const bitmap_t bitmapAlienMiddleIn  = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_middle_in_14x8};
static const bitmap_t bitmapAlienMiddleOut = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_middle_out_14x8};
static const bitmap_t bitmapAlienBottomIn  = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_bottom_in_14x8};
static const bitmap_t bitmapAlienBottomOut = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_bottom_out_14x8};
static const bitmap_t bitmapTank           = {TANK_BITMAP_WIDTH, TANK_BITMAP_HEIGHT, TANK_COLOR, tank_19x8};
static const bitmap_t bitmapBunker         = {BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR, bunker_24x18};
static const bitmap_t bitmapBunkerDamage0  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage0_6x6};
static const bitmap_t bitmapBunkerDamage1  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage1_6x6};
static const bitmap_t bitmapBunkerDamage2  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage2_6x6};
static const bitmap_t bitmapBunkerDamage3  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage3_6x6};
static const bitmap_t bitmapBulletAlien1   = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_alien1_3x5};
static const bitmap_t bitmapBulletAlien2   = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_alien2_3x5};
static const bitmap_t bitmapBulletTank     = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_tank_3x5};

#endif /* SHAPEBITMAP_C_ */
