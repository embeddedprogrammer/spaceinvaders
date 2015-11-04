/*
 * fontBitmap.h
 *
 *  Created on: Oct 13, 2015
 *      Author: superman
 */

#ifndef FONTBITMAP_H_
#define FONTBITMAP_H_

#define FONT_WIDTH 5
#define FONT_HEIGHT 8
#define FONT_ROWS_OFFSET (FONT_HEIGHT + 1)
#define FONT_COLS_OFFSET (FONT_WIDTH + 1)

#define CHARACTER_OFFSET_UPPERCASE_A 0
#define CHARACTER_OFFSET_LOWERCASE_A 26
#define CHARACTER_OFFSET_NUMBER_0 26+26

extern const int* characters[];

#endif /* FONTBITMAP_H_ */
