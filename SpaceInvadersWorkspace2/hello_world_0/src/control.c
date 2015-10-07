/*
 * control.c
 *
 *  Created on: Sep 23, 2015
 *      Author: superman
 */

#include "control.h"
#include "timers.h"
#include "tank.h"
#include <stdio.h>
#include <stdlib.h> //for rand()

#define FIRST_TANK_BULLET 0
#define MAX_TANK_BULLETS 10
#define FIRST_ALIEN_BULLET MAX_TANK_BULLETS
#define MAX_ALIEN_BULLETS 10
#define MAX_BULLETS_COUNT (MAX_TANK_BULLETS + MAX_ALIEN_BULLETS)
static bullet_t bullets[MAX_BULLETS_COUNT];
#define EXPLOSION_TIME 100
#define ALIEN_FLEET_SHIFT_DOWN_AMOUNT 8

static bool aliens_alienLegsIn = false;

#define MAX_CONCURRENT_EXPLOSIONS 3
point_t explosions[MAX_CONCURRENT_EXPLOSIONS];

point_t spacecraftLocation = (point_t){0, 0};
bool spacecraftTravelingRight = true;

void aliens_moveSaucer()
{
	if(spacecraftLocation.row != 0)
	{
		if(spacecraftLocation.col < 0 || spacecraftLocation.col > GAMEBUFFER_WIDTH - SAUCER_WIDTH)
		{
			draw_Saucer(spacecraftLocation, true);
			spacecraftLocation.row = 0;
			return;
		}
		point_t erasePos = spacecraftLocation;
		if(spacecraftTravelingRight)
		{
			erasePos.col = spacecraftLocation.col;
			spacecraftLocation.col += ALIEN_SHIFT_AMOUNT;
		}
		else
		{
			spacecraftLocation.col -= ALIEN_SHIFT_AMOUNT;
			erasePos.col = spacecraftLocation.col + SAUCER_WIDTH;
		}
		draw_rectangle(erasePos, ALIEN_SHIFT_AMOUNT, SAUCER_HEIGHT, BACKGROUND_COLOR);
		draw_Saucer(spacecraftLocation, false);
	}
}

void aliens_startSaucer()
{
	xil_printf("Start flying saucer\n\r");
	spacecraftLocation.row = 10;
	if(rand() % 2)
	{
		spacecraftTravelingRight = true;
		spacecraftLocation.col = 0;
	}
	else
	{
		spacecraftTravelingRight = false;
		spacecraftLocation.col = GAMEBUFFER_WIDTH - SAUCER_WIDTH;
	}
}

void aliens_removeExplosion()
{
	int i;
	for(i = 0; i < MAX_CONCURRENT_EXPLOSIONS; i++)
		if(explosions[i].row != 0)
		{
			draw_AlienExplosion(explosions[i], true);
			explosions[i].row = 0;
		}
}

void aliens_explode(point_t location)
{
	draw_AlienExplosion(location, false);
	int i;
	for(i = 0; i < MAX_CONCURRENT_EXPLOSIONS; i++)
		if(explosions[i].row == 0)
		{
			draw_AlienExplosion(location, false);
			explosions[i] = location;
			addTimer(EXPLOSION_TIME, false, &aliens_removeExplosion);
			return;
		}
	xil_printf("Too many explosions\n\r");
}

// kills an alien then updates the rows and columns that are still viable
void aliens_killAlienRC(short row, short col)
{
	setAlienAlive(row, col, false);
	point_t alienPos = draw_getAlienPosition(row, col);
	point_t explosionPos = (point_t){alienPos.col + (ALIEN_BITMAP_WIDTH  / 2) - (EXPLOSION_WIDTH  / 2),
									 alienPos.row + (ALIEN_BITMAP_HEIGHT / 2) - (EXPLOSION_HEIGHT / 2)};
	aliens_explode(explosionPos);

	//Update indexes:
	int minRow = ALIEN_FLEET_ROWS;
	int maxRow = 0;
	int minCol = ALIEN_FLEET_COLS;
	int maxCol = 0;
	int r = 0;
	int c = 0;

	for (r = 0; r < ALIEN_FLEET_ROWS; r++)
		for (c = 0; c < ALIEN_FLEET_COLS; c++)
			if (isAlienAlive(r, c))
			{
				if(r > maxRow)
					maxRow = r;
				if(r < minRow)
					minRow = r;
				if(c > maxCol)
					maxCol = c;
				if(c < minCol)
					minCol = c;
			}

	setAlienFleetTopRowNumGlobal(minRow);
	setAlienFleetBottomRowNumGlobal(maxRow);
	setAlienFleetLeftColNumGlobal(minCol);
	setAlienFleetRightColNumGlobal(maxCol);
}

void aliens_killSaucer()
{
	point_t explosionPos = (point_t){spacecraftLocation.col + (SAUCER_WIDTH  / 2) - (EXPLOSION_WIDTH  / 2),
									 spacecraftLocation.row + (SAUCER_HEIGHT / 2) - (EXPLOSION_HEIGHT / 2)};
	draw_Saucer(spacecraftLocation, true);
	aliens_explode(explosionPos);
	spacecraftLocation.row = 0;
}

void aliens_killAlien(uint alienIdx)
{
	aliens_killAlienRC(alienIdx / ALIEN_FLEET_COLS, alienIdx % ALIEN_FLEET_COLS);
}

//checks to see if the fleet is at right screen edge
bool aliens_fleetAtRightScreenEdge()
{
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int alienFleetWidth = (rightAlienCol + 1)*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();
	return (alienPos.col + alienFleetWidth + ALIEN_SHIFT_AMOUNT > GAMEBUFFER_WIDTH);
}

// checks to see if the fleet is at the left screen edge
bool aliens_fleetAtLeftScreenEdge()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int alienFleetLeftOffset = leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	point_t alienPos = getAlienFleetPositionGlobal();

	return (alienPos.col + alienFleetLeftOffset - ALIEN_SHIFT_AMOUNT < 0);
}

// erase the parts of aliens that get left behind by a redraw
void aliens_shiftDownErase(point_t alienPos)
{
	int i;
	point_t erasePos;
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int topAlienRow = getAlienFleetTopRowNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	erasePos.col = alienPos.col + leftAlienCol*ALIEN_HORIZONTAL_DISTANCE;
	erasePos.row = alienPos.row - ALIEN_FLEET_SHIFT_DOWN_AMOUNT + topAlienRow*(ALIEN_VERTICAL_DISTANCE);
	for (i = topAlienRow; i < bottomAlienRow + 1; i++ ) {
		draw_rectangle(erasePos, ALIEN_HORIZONTAL_DISTANCE*(rightAlienCol - leftAlienCol + 1), ALIEN_FLEET_SHIFT_DOWN_AMOUNT, BACKGROUND_COLOR);
		erasePos.row += ALIEN_VERTICAL_DISTANCE;
	}
}

static bool directionRight = true;
bool aliens_fleetDirectionIsRight()
{
	return directionRight;
}

// shifts the whole alien fleet left right or down depending on position
void aliens_shiftAlienFleet()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	point_t alienPos = getAlienFleetPositionGlobal();
	bool shiftDown = (directionRight) ? aliens_fleetAtRightScreenEdge() : aliens_fleetAtLeftScreenEdge();// at screen edge

	if (shiftDown) {
		directionRight = !directionRight;
		alienPos.row += ALIEN_FLEET_SHIFT_DOWN_AMOUNT;
		aliens_shiftDownErase(alienPos);// erase remaining alien parts

	} else {// shift left or right
		point_t erasePos;
		if (directionRight) {
			alienPos.col += ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + leftAlienCol*ALIEN_HORIZONTAL_DISTANCE - ALIEN_SHIFT_AMOUNT;

		} else {
			alienPos.col -= ALIEN_SHIFT_AMOUNT;
			erasePos.col = alienPos.col + (rightAlienCol+1)*ALIEN_HORIZONTAL_DISTANCE;
		}
		erasePos.row = alienPos.row;
		draw_rectangle(erasePos, ALIEN_SHIFT_AMOUNT, ALIEN_BITMAP_HEIGHT*ALIEN_FLEET_ROWS + ALIEN_VERTICAL_SPACER*4, BACKGROUND_COLOR);// undraw remaing alien pieces

	}
	setAlienFleetPositionGlobal(alienPos);
	aliens_alienLegsIn = !aliens_alienLegsIn;
	draw_AlienFleet(aliens_alienLegsIn);
}


//BUNKERS

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

	point_t fleetPosition = getAlienFleetPositionGlobal();
	if(location.row > fleetPosition.row && location.row < (fleetPosition.row + ALIEN_VERTICAL_DISTANCE * ALIEN_FLEET_ROWS) &&
	   location.col > fleetPosition.col && location.col < (fleetPosition.col + ALIEN_HORIZONTAL_DISTANCE * ALIEN_FLEET_COLS))
	{
		int row = (location.row - fleetPosition.row) / ALIEN_VERTICAL_DISTANCE;
		int col = (location.col - fleetPosition.col) / ALIEN_HORIZONTAL_DISTANCE;
		aliens_killAlienRC(row, col);
		return true;
	}
	else if(location.row > spacecraftLocation.row && location.row < (spacecraftLocation.row + SAUCER_HEIGHT) &&
	        location.col > spacecraftLocation.col && location.col < (spacecraftLocation.col + SAUCER_WIDTH))
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
    if(location.row > tankPosition.row && location.row < (tankPosition.row + TANK_HEIGHT) &&
       location.col > tankPosition.col && location.col < (tankPosition.row + TANK_WIDTH))
    {
		tank_killTank();
        return true;
    }
    return false;
}

//advances the bullet specified by index i
void control_moveBullet(int i)
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
                collision = killAlienIfAlienCollision(location) || erodeBunkerIfCollision(location);
            else if (bullets[i].bulletType != bullet_tank)
                collision = erodeBunkerIfCollision(location) || control_killTankIfCollision(location) || control_killTankIfCollision(location);
            if(collision == false)
                xil_printf("Bullet hit unknown pixel\n\r");
            bullets[i].bulletType = bullet_none;
            return;
        }
    }

	draw_bullet(bullets[i]);
}

//advances all on screen bullets
void control_moveAllBullets()
{
	int i;
	for(i = 0; i < MAX_BULLETS_COUNT; i++)
		if(bullets[i].bulletType != bullet_none)
			control_moveBullet(i);
}

//get a viable alien on the bottom row that can fire a bullet
point_t getAlienBottomRow()
{
	int leftAlienCol = getAlienFleetLeftColNumGlobal();
	int rightAlienCol = getAlienFleetRightColNumGlobal();
	int bottomAlienRow = getAlienFleetBottomRowNumGlobal();
	int i;
	int alienCount = 0;
	int bottomRowArray[ALIEN_FLEET_COLS];
	for (i = leftAlienCol; i < rightAlienCol + 1; i++) // find which aliens are alive
		if (isAlienAlive(bottomAlienRow, i))
			bottomRowArray[alienCount++] = i;

	return draw_getAlienPosition(bottomAlienRow, bottomRowArray[rand() % alienCount]);// random alien
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
void control_fireAlienBullet()
{
	xil_printf("");
	int i = control_getFirstEmptyBulletPosition(FIRST_ALIEN_BULLET, MAX_ALIEN_BULLETS);
	if(i != -1)
	{
		bullets[i].bulletType = (rand() % 2) ? bullet_alien1 : bullet_alien2;
		point_t alienPos = getAlienBottomRow();
		bullets[i].location.row = alienPos.row + ALIEN_BITMAP_HEIGHT;
		bullets[i].location.col = alienPos.col + BULLET_ALIEN_OFFSET;
		draw_bullet(bullets[i]);
	}
	else
		xil_printf("Cannot fire - Maxed out alien bullets\n\r");
	xil_printf("");
}

// fires a tank bullet and places it at the end of its turret
void control_fireTankBullet()
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
	else
		xil_printf("Cannot fire - Maxed out tank bullets\n\r");
}






