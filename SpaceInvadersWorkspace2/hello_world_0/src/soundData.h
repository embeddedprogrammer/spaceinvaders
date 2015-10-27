/*
 * soundData.h
 *
 *  Created on: Oct 23, 2015
 *      Author: Jacob White and Troy Hinckley
 */

#ifndef SOUNDDATA_H_
#define SOUNDDATA_H_

typedef struct
{
	const char* soundData;
	int numberOfSamples;
	int sampleRate;
} sound_t;

extern const sound_t explosion_sound;
extern const sound_t fastinvader1_sound;
extern const sound_t fastinvader2_sound;
extern const sound_t fastinvader3_sound;
extern const sound_t fastinvader4_sound;
extern const sound_t invaderkilled_sound;
extern const sound_t shoot_sound;
extern const sound_t ufo_highpitch_sound;
extern const sound_t ufo_lowpitch_sound;

#endif /* SOUNDDATA_H_ */
