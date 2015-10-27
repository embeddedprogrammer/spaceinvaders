/*
 * sound.c
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "sound.h"
#include "timers.h"

static int tankHitSoundTimer;
static int tankFireSoundTimer;
static int saucerFlyingSoundTimer;
static int alienMarchingSoundTimer;
static int saucerHitSoundTimer;
static int alienKilledSoundTimer;

void sound_playTankHit()
{
	tankHitSoundTimer = addTimer(0, true, &soundCallback_playTankHit);
}
void sound_playTankFire()
{
	tankFireSoundTimer = addTimer(0, true, &soundCallback_playTankFire);
}
void sound_playSaucerFlying()
{
	saucerFlyingSoundTimer = addTimer(0, true, &soundCallback_playSaucerFlying);
}
void sound_playAlienMarching()
{
	alienMarchingSoundTimer = addTimer(0, true, &soundCallback_playAlienMarching);
}
void sound_playSaucerHit()
{
	saucerHitSoundTimer = addTimer(0, true, &soundCallback_playSaucerHit);
}
void sound_playAlienKilled()
{
	alienKilledSoundTimer = addTimer(0, true, &soundCallback_playAlienKilled);
}

void soundCallback_playTankHit()
{
}
void soundCallback_playTankFire()
{

}
void soundCallback_playSaucerFlying()
{

}
void soundCallback_playAlienMarching()
{

}
void soundCallback_playSaucerHit()
{

}
void soundCallback_playAlienKilled()
{

}
