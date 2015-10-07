/*
 * bullets.c
 *
 *  Created on: Oct 7, 2015
 *      Author: Jacob White and Troy Hinckley
 */

#include "bullets.h"
#include "bunkers.h"
#include "drawShape.h"
#include "aliens.h"
#include "tank.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define FIRST_TANK_BULLET 0
#define MAX_TANK_BULLETS 10
#define FIRST_ALIEN_BULLET MAX_TANK_BULLETS
#define MAX_ALIEN_BULLETS 10
#define MAX_BULLETS_COUNT (MAX_TANK_BULLETS + MAX_ALIEN_BULLETS)
static bullet_t bullets[MAX_BULLETS_COUNT];

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
	if(bullets[i].location.row < 0 || bullets[i].location.row >= GAMEBUFFER_HEIGHT - BULLET_HEIGHT)
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
				collision = aliens_killIfCollision(location) || bunkers_erodeIfCollision(location);
			else if (bullets[i].bulletType != bullet_tank)
				collision = bunkers_erodeIfCollision(location) || tank_killIfCollision(location) || aliens_killIfCollision(location);
			if(collision == false)
				xil_printf("Bullet hit unknown pixel\n\r");
			bullets[i].bulletType = bullet_none;
			return;
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

int bullets_getFirstEmptyBulletPosition(int start, int length)
{
	int i = 0;
	for(i = start; i < start + length; i++)
		if(bullets[i].bulletType == bullet_none)
			return i;
	return -1;
}

void bullets_tick()
{
	bullets_moveAllBullets();
}

//fires an alien bullet if one is avalible
void bullets_fireAlienBullet()
{
	xil_printf("stuck\n\r");
	int i = bullets_getFirstEmptyBulletPosition(FIRST_ALIEN_BULLET, MAX_ALIEN_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;
		point_t alienPos = aliens_getAlienBottomRow();
		bullets[i].location.row = alienPos.row + ALIEN_BITMAP_HEIGHT;
		bullets[i].location.col = alienPos.col + BULLET_ALIEN_OFFSET;
		draw_bullet(bullets[i]);
	}
	else
		xil_printf("Cannot fire - Maxed out alien bullets\n\r");
	xil_printf("Not stuck\n\r");
}

// fires a tank bullet and places it at the end of its turret
void bullets_fireTankBullet()
{
	int i = bullets_getFirstEmptyBulletPosition(FIRST_TANK_BULLET, MAX_TANK_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = bullet_tank;
		point_t tankPos = getTankPositionGlobal();
		bullets[i].location.row = tankPos.row - BULLET_HEIGHT;
		bullets[i].location.col = tankPos.col + BULLET_TANK_OFFSET;
		draw_bullet(bullets[i]);
	}
	else
		xil_printf("Cannot fire - Maxed out tank bullets\n\r");
}
