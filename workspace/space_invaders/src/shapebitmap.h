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

#define packword3(b2,b1,b0)                                                 \
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

//colors
#define BUNKER_COLOR     0x0000FF00
#define TANK_COLOR       0x000000FF
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

static const int alien_erase_14x8[] = {
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0),
	packword14(0,0,0,0,0,0,0,0,0,0,0,0,0,0)
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
// strikes one of these blocks, you erod the block as you sequence through
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

static const int bullet_3x5[] = {
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(1,1,1),
	packword3(0,1,0)};

#endif /* SHAPEBITMAP_C_ */
