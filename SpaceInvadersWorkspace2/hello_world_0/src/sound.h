/*
 * sound.h
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "soundData.h"
#ifndef SOUND_H_
#define SOUND_H_

#define MAX_VOLUME 31
#define MIN_VOLUME 1

int sound_getVolume();
void sound_setVolume(int volume);
void sound_volumeUp();
void sound_volumeDown();
void sound_test();
void sound_play(sound_t sound);
void sound_writeToFifo(int count);
void sound_initInterupts();

#endif /* SOUND_H_ */
