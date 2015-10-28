/*
 * sound.h
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "soundData.h"
#ifndef SOUND_H_
#define SOUND_H_

void sound_playTankHit();
void sound_playTankFire();
void sound_playSaucerFlying();
void sound_playAlienMarching();
void sound_playSaucerHit();
void sound_playAlienKilled();


void soundCallback_playTankHit();
void soundCallback_playTankFire();
void soundCallback_playSaucerFlying();
void soundCallback_playAlienMarching();
void soundCallback_playSaucerHit();
void soundCallback_playAlienKilled();

void sound_volumeUp();
void sound_volumeDown();

#endif /* SOUND_H_ */
