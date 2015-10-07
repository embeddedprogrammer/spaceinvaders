/*
 * bunkers.c
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "drawShape.h"

//erodes a particular bunker section based on bunker row and column
void bunkers_erodeSection(int bunker, int row, int col)
{
	if (row >= BUNKER_DAMAGE_ARRAY_ROWS || row < 0 || col >= BUNKER_DAMAGE_ARRAY_COLS || col < 0)
	{
		xil_printf("Error: Cannot erode bunker section %d %d %d \n\r", bunker, row, col);
		return;
	}

	byte damage = getBunkerDamage(bunker, row, col);
	damage++;
	draw_BunkerDamageAtIndex(bunker, row, col, damage - 1);
	setBunkerDamage(bunker, row, col, damage);
}

//erodes the entire bunker
void bunkers_erode(int bunker)
{
	int i, j;
	for(i = 0; i < BUNKER_SECTION_ROWS; i++)
		for(j = 0; j < BUNKER_SECTION_COLS; j++)
			bunkers_erodeSection(bunker, i, j);
}

bool bunkers_erodeIfCollision(point_t location)
{
	int i;
	for (i = 0; i < TOTAL_BUNKERS; i++ ) {
		point_t bunkerPos = draw_getBunkerLocation(i);
		if ((location.row >= bunkerPos.row) && location.row < (bunkerPos.row + BUNKER_HEIGHT) &&
		    (location.col >= bunkerPos.col) && location.col < (bunkerPos.col + BUNKER_WIDTH))
		{
			int row = (location.row - bunkerPos.row) / BUNKER_DAMAGE_HEIGHT;
			int col = (location.col - bunkerPos.col) / BUNKER_DAMAGE_WIDTH;
			bunkers_erodeSection(i, row, col);
			return true;
		}
	}
	return false;
}
