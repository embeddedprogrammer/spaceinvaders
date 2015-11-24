/*
 * tank.c
 *
 *  Created on: Oct 7, 2015
 *      Author: superman
 */

#include "tank.h"
#include "timers.h"
#include "globals.h"
#include "drawShape.h"
#include "bullets.h"
#include "fontBitmap.h"
#include "spaceInvaders.h"
#include "sound.h"
#include "soundData.h"
#include <stdio.h>

#define ERODE_TANK_TIME 1000
#define DISAPPEAR_TANK_TIME 2000
#define START_LIVES 5

bool tankAlive;

#define SCORE_TEXT_COL    0
#define SCORE_TEXT_ROW    0
#define SCORE_DISPLAY_COL (SCORE_TEXT_COL+FONT_COLS_OFFSET*6)
#define SCORE_DISPLAY_ROW 0
#define LIVES_TEXT_COL    GAMEBUFFER_WIDTH/2
#define LIVES_TEXT_ROW    0
#define LIVES_DISPLAY_COL (LIVES_TEXT_COL+FONT_COLS_OFFSET*6)
#define LIVES_DISPLAY_ROW 0
#define LIVES_SPACER      2

static int score = 0;
static int lives = 0;

static point_t g_tankPos;

point_t getTankPositionGlobal() {
	return g_tankPos;
}

void setTankPositionGlobal(point_t tankPos) {
	g_tankPos = tankPos;
}

void draw_scoreAndLifeText(bool erase)
{
	draw_string("SCORE", SCORE_ABC_COLOR, (point_t){SCORE_TEXT_COL, SCORE_TEXT_ROW}, erase);
	draw_string("LIVES", SCORE_ABC_COLOR, (point_t){LIVES_TEXT_COL, LIVES_TEXT_ROW}, erase);
}

void draw_tankLife(int life, bool erase)
{
	draw_tank((point_t){LIVES_DISPLAY_COL + (TANK_BITMAP_WIDTH + LIVES_SPACER)*(life - 1), LIVES_DISPLAY_ROW}, erase);
}

void draw_score(int score, bool erase)
{
	draw_number(score, SCORE_123_COLOR, (point_t){SCORE_DISPLAY_COL, SCORE_DISPLAY_ROW}, erase);
}

void updateScore(int newScore)
{
	if(newScore == -1)
	{
		draw_score(score, true);
		score = 0;
	}
	score = newScore;
	draw_score(score, false);
}

void addPoints(int points)
{
	updateScore(score + points);
}

void updateLives(int newLives)
{
	while(lives < newLives)
	{
		lives++;
		draw_tankLife(lives, false);
	}
	while(lives > newLives)
	{
		draw_tankLife(lives, true);
		lives--;
	}
	tankAlive = true;
}

void tank_init(bool newGame)
{
	draw_scoreAndLifeText(false);
	int prevLives = lives;
	updateLives(0);
	if (newGame) {
		updateScore(0);
		updateLives(3);
	} else {
		updateScore(score);
		updateLives(prevLives);
	}

	point_t tankPos = {TANK_INTIAL_COL, TANK_ROW};
	draw_tank(tankPos, false);
	setTankPositionGlobal(tankPos);
	tankAlive = true;
}

//move tank left
void tank_moveTankLeft()
{
	if(!tankAlive)
		return;
	point_t tankPos = getTankPositionGlobal();
	if(tankPos.col > 2)
		tankPos.col -= 2;
	setTankPositionGlobal(tankPos);
	draw_tank(tankPos, false);
}

//move tank right
void tank_moveTankRight()
{
	if(!tankAlive)
		return;
	point_t tankPos = getTankPositionGlobal();
	if(tankPos.col < GAMEBUFFER_WIDTH - TANK_BITMAP_WIDTH - 2)
		tankPos.col += 2;
	setTankPositionGlobal(tankPos);
	draw_tank(tankPos, false);
}

void tank_fireBullet()
{
	if(tankAlive)
		bullets_fireTankBullet();
}

void tank_reviveTank()
{
	draw_tank(getTankPositionGlobal(), false);
	tankAlive = true;
}

void tank_disappearTank()
{
    draw_tank(getTankPositionGlobal(), true);
	if(lives >= 1)
	{
		updateLives(lives - 1);
		addTimer(DISAPPEAR_TANK_TIME, false, &tank_reviveTank);
	}
	else
	{
		gameOver();
	}
}

void tank_killTank()
{
	if(tankAlive)
	{
		tankAlive = false;
		draw_erodeTank(getTankPositionGlobal());
		addTimer(ERODE_TANK_TIME, false, &tank_disappearTank);
		sound_play(explosion_sound);
	}
}
