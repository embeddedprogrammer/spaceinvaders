/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "bullets.h"
#include "drawShape.h"
#include "timers.h"
#include "tank.h"
#include "aliens.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define FIRST_TANK_BULLET 0
#define MAX_TANK_BULLETS 1
#define FIRST_ALIEN_BULLET MAX_TANK_BULLETS
#define MAX_ALIEN_BULLETS 3
#define MAX_BULLETS_COUNT (MAX_TANK_BULLETS + MAX_ALIEN_BULLETS)
static bullet_t bullets[MAX_BULLETS_COUNT];
#define BULLET_ADVANCE_TIME 10

static byte bunkerDamage[4][3][4];

byte getBunkerDamage(int bunker, int row, int col)
{
	return bunkerDamage[bunker][row][col];
}

void setBunkerDamage(int bunker, int row, int col, byte damage)
{
	bunkerDamage[bunker][row][col] = damage;
}

void bullets_init()
{
	int i;
	for (i = 0; i < MAX_BULLETS_COUNT; i++) {
		bullets[i].bulletType = bullet_none;
	}
	addTimer(BULLET_ADVANCE_TIME, true, &bullets_moveAllBullets);
}

void bunker_init(bool newGame)
{
	int b, r, c;
	if (newGame) {
		for(b = 0; b < TOTAL_BUNKERS; b++)
			for(r = 0; r < BUNKER_SECTION_ROWS; r++)
				for(c = 0; c < BUNKER_SECTION_COLS; c++)
					setBunkerDamage(b, r, c, 0);
	} else {
		for(b = 0; b < TOTAL_BUNKERS; b++)
			for(r = 0; r < BUNKER_SECTION_ROWS; r++)
				for(c = 0; c < BUNKER_SECTION_COLS; c++) {
					byte damage = getBunkerDamage(b, r, c);
					if (damage)
						draw_BunkerDamageAtIndex(b, r, c, damage - 1);
				}
	}
}

//erodes a particular bunker section based on bunker row and column
void bunkers_erodeBunkerSection(int bunker, int row, int col)
{
	if (row >= BUNKER_DAMAGE_ARRAY_ROWS || row < 0 || col >= BUNKER_DAMAGE_ARRAY_COLS || col < 0)
		return;

	byte damage = getBunkerDamage(bunker, row, col);
	damage++;
	draw_BunkerDamageAtIndex(bunker, row, col, damage - 1);
	setBunkerDamage(bunker, row, col, damage);
}

//erodes the entire bunker
void bunkers_erodeBunker(int bunker)
{
	int i, j;
	for(i = 0; i < BUNKER_SECTION_ROWS; i++)
		for(j = 0; j < BUNKER_SECTION_COLS; j++)
			bunkers_erodeBunkerSection(bunker, i, j);
}

bool killAlienIfAlienCollision(point_t location)
{
	point_t saucerLocation = aliens_getSaucerLocation();

	point_t fleetPosition = getAlienFleetPositionGlobal();
	if(location.row >= fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
	   location.col >= fleetPosition.col && location.col < (fleetPosition.col + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
	{
		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
		bool alive = isAlienAlive(row, col);
		if(alive)
			aliens_killAlienRC(row, col);
		return alive;
	}
	else if(location.row >= saucerLocation.row && location.row < (saucerLocation.row + SAUCER_HEIGHT) &&
	        location.col >= saucerLocation.col && location.col < (saucerLocation.col + SAUCER_WIDTH))
	{
		aliens_killSaucer();
	}
	return false;
}

bool erodeBunkerIfCollision(point_t location)
{
	int i;
	for (i = 0; i < TOTAL_BUNKERS; i++ ) {

		point_t bunkerPos = draw_getBunkerLocation(i);
		if ((location.row >= bunkerPos.row) && location.row < (bunkerPos.row + BUNKER_HEIGHT) &&
		    (location.col >= bunkerPos.col) && location.col < (bunkerPos.col + BUNKER_WIDTH))
		{
			int row = (location.row - bunkerPos.row) / BUNKER_DAMAGE_HEIGHT;
			int col = (location.col - bunkerPos.col) / BUNKER_DAMAGE_WIDTH;
			bunkers_erodeBunkerSection(i, row, col);
			return true;
		}
	}
	return false;
}

bool control_killTankIfCollision(point_t location)
{
    point_t tankPosition = getTankPositionGlobal();
    if(location.row >= tankPosition.row && location.row < (tankPosition.row + TANK_HEIGHT) &&
       location.col >= tankPosition.col && location.col < (tankPosition.col + TANK_WIDTH))
    {
		tank_killTank();
        return true;
    }
    return false;
}

//advances the bullet specified by index i
void bullets_moveBullet(int i)
{
	erase_bullet(bullets[i]);

	//Move location
	if(bullets[i].bulletType == bullet_tank)
		bullets[i].location.row--;
	else if(bullets[i].bulletType == bullet_alien1 || bullets[i].bulletType == bullet_alien2)
		bullets[i].location.row++;

	//Check for off screen
	if(bullets[i].location.row < SCOREBOARD_SPACING + TANK_BITMAP_HEIGHT || bullets[i].location.row >= GRASS_ROW - BULLET_HEIGHT)
	{
		bullets[i].bulletType = bullet_none;
		return;
	}
	short checkRow = bullets[i].location.row;
	short checkCol = bullets[i].location.col;
	if(bullets[i].bulletType == bullet_alien1 || bullets[i].bulletType == bullet_alien2)
		checkRow += BULLET_HEIGHT - 1;

    //Check for collisions
    int c;
    for(c = 0; c <= 2; c++)
    {
        point_t location = (point_t){checkCol + c, checkRow};
        if(getPixel(location) != 0)
        {
            bool collision = false;
            if (bullets[i].bulletType == bullet_tank)
                collision = killAlienIfAlienCollision(location) || erodeBunkerIfCollision(location);
            else if (bullets[i].bulletType != bullet_tank)
                collision = erodeBunkerIfCollision(location) || control_killTankIfCollision(location) || control_killTankIfCollision(location);
            if(collision)
            {
            	bullets[i].bulletType = bullet_none;
            	return;
            }
        }
    }

	draw_bullet(bullets[i]);
}

//advances all on screen bullets
void bullets_moveAllBullets()
{
	int i;
	for(i = 0; i < MAX_BULLETS_COUNT; i++)
		if(bullets[i].bulletType != bullet_none)
			bullets_moveBullet(i);
}



int control_getFirstEmptyBulletPosition(int start, int length)
{
	int i = 0;
	for(i = start; i < start + length; i++)
		if(bullets[i].bulletType == bullet_none)
			return i;
	return -1;
}

//fires a alien bullet if one is available
void bullets_fireAlienBullet()
{
	xil_printf("");
	int i = control_getFirstEmptyBulletPosition(FIRST_ALIEN_BULLET, MAX_ALIEN_BULLETS);
	point_t alienPos = getAlienBottomRow();
	if(i != -1 && (alienPos.row + ALIEN_BITMAP_HEIGHT < GRASS_ROW - BULLET_HEIGHT))
	{
		bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;
		bullets[i].location.row = alienPos.row + ALIEN_BITMAP_HEIGHT;
		bullets[i].location.col = alienPos.col + BULLET_ALIEN_OFFSET;
		draw_bullet(bullets[i]);
	}
	xil_printf("");
	addTimer(randNum(ALIEN_MIN_FIRE_TIME, ALIEN_MAX_FIRE_TIME), false, &bullets_fireAlienBullet);
}

// fires a tank bullet and places it at the end of its turret
void bullets_fireTankBullet()
{
	int i = control_getFirstEmptyBulletPosition(FIRST_TANK_BULLET, MAX_TANK_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = bullet_tank;
		point_t tankPos = getTankPositionGlobal();
		bullets[i].location.row = tankPos.row - BULLET_HEIGHT;
		bullets[i].location.col = tankPos.col + BULLET_TANK_OFFSET;
		draw_bullet(bullets[i]);
	}
}






