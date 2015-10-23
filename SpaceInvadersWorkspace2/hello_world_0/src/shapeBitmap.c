/*
 * shapeBitmap.c
 *
 *  Created on: Oct 23, 2015
 *      Author: Jacob White and Troy Hinckley
 */

#include "shapeBitmap.h"

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

const bitmap_t bitmapSaucer         = {SAUCER_WIDTH, SAUCER_HEIGHT, SAUCER_COLOR, saucer_16x7};
const bitmap_t bitmapExplosion      = {EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ALIEN_COLOR, alien_explosion_14x10};
const bitmap_t bitmapAlienTopIn     = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_top_in_14x8};
const bitmap_t bitmapAlienTopOut    = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_top_out_14x8};
const bitmap_t bitmapAlienMiddleIn  = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_middle_in_14x8};
const bitmap_t bitmapAlienMiddleOut = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_middle_out_14x8};
const bitmap_t bitmapAlienBottomIn  = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_bottom_in_14x8};
const bitmap_t bitmapAlienBottomOut = {ALIEN_BITMAP_WIDTH, ALIEN_BITMAP_HEIGHT, ALIEN_COLOR, alien_bottom_out_14x8};
const bitmap_t bitmapTank           = {TANK_BITMAP_WIDTH, TANK_BITMAP_HEIGHT, TANK_COLOR, tank_19x8};
const bitmap_t bitmapBunker         = {BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR, bunker_24x18};
const bitmap_t bitmapBunkerDamage0  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage0_6x6};
const bitmap_t bitmapBunkerDamage1  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage1_6x6};
const bitmap_t bitmapBunkerDamage2  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage2_6x6};
const bitmap_t bitmapBunkerDamage3  = {BUNKER_DAMAGE_WIDTH, BUNKER_DAMAGE_HEIGHT, BACKGROUND_COLOR, bunkerDamage3_6x6};
const bitmap_t bitmapBulletAlien1   = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_alien1_3x5};
const bitmap_t bitmapBulletAlien2   = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_alien2_3x5};
const bitmap_t bitmapBulletTank     = {BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR, bullet_tank_3x5};
