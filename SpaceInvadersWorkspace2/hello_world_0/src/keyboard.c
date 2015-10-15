/*
 * keyboard.c
 *
 *  Created on: Oct 15, 2015
 *      Author: superman
 */

#include <stdio.h>
#include "tank.h"
#include "aliens.h"
#include "bullets.h"

#define KEY_KILL_ALIEN '2'
#define KEY_TANK_LEFT '4'
#define KEY_TANK_RIGHT '6'
#define KEY_MOVE_ALIEN '8'
#define KEY_TANK_FIRE_BULLET '5'
#define KEY_MOVE_BULLETS '9'
#define KEY_ALIEN_FIRE_BULLET '3'
#define KEY_ERODE_BUNKER '7'
#define KEY_RESTART 'r'
#define KEY_SAUCER 's'

int getNumber()
{
	xil_printf("Please enter a number:\r\n");
	int num = 0;
	char input;
	while (true)
	{
		input = getchar();
		if(input >= '0' && input <= '9')
		{
			xil_printf("%c", input);
			num = num*10 + (input - '0');
		}
		else //if any other key is pressed, exit
		{
			xil_printf("\r\n");
			return num;
		}
	}
}

bool pollKeyboard()
{
	char input;
	input = getchar();
	switch (input) {
	case KEY_TANK_LEFT:
		tank_moveTankLeft();
		break;
	case KEY_TANK_RIGHT:
		tank_moveTankRight();
		break;
	case KEY_KILL_ALIEN:
		xil_printf("Kill alien - ");
		aliens_killAlien(getNumber());
		xil_printf("Alien killed\r\n");
		break;
	case KEY_MOVE_ALIEN:
		aliens_shiftAlienFleet();
		break;
	case KEY_TANK_FIRE_BULLET:
		tank_fireBullet();
		break;
	case KEY_MOVE_BULLETS:
		bullets_moveAllBullets();
		break;
	case KEY_ALIEN_FIRE_BULLET:
		bullets_fireAlienBullet();
		break;
	case KEY_ERODE_BUNKER:
		xil_printf("Erode bunker - ");
		bunkers_erodeBunker(getNumber());
		xil_printf("Bunker eroded\r\n");
		break;
	case KEY_RESTART:
		restart();
		break;
	case KEY_SAUCER:
		aliens_startSaucer();
		break;
	case ' ':
		draw_rectangle((point_t){0, (GAMEBUFFER_HEIGHT*3)/4}, GAMEBUFFER_WIDTH, BUNKER_HEIGHT*3, BACKGROUND_COLOR, true);
		break;
	case 'x':
		return false;
		break;
	default:
		xil_printf("Key pressed: %c (code %d)\r\n", input, (int)input);
	}
	return true;
}
