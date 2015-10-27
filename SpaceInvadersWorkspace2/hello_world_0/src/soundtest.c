/*
 * soundtest.c
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include <stdio.h>
#include "soundtest.h"
#include "globals.h"
#include "xparameters.h"
#include "xac97_l.h"
#include "soundData.h"

enum sound_files {
	enum_ufo_lowpitch_sound = 0,
	enum_explosion_sound,
	enum_fastinvader1_sound,
	enum_fastinvader2_sound,
	enum_fastinvader3_sound,
	enum_fastinvader4_sound,
	enum_invaderkilled_sound,
	enum_shoot_sound,
	enum_ufo_highpitch_sound
};

sound_t getSoundFile(int file)
{
	switch (file) {
	case enum_ufo_lowpitch_sound : return ufo_lowpitch_sound;
	case enum_explosion_sound    : return explosion_sound;
	case enum_fastinvader1_sound : return fastinvader1_sound;
	case enum_fastinvader2_sound : return fastinvader2_sound;
	case enum_fastinvader3_sound : return fastinvader3_sound;
	case enum_fastinvader4_sound : return fastinvader4_sound;
	case enum_invaderkilled_sound: return invaderkilled_sound;
	case enum_shoot_sound        : return shoot_sound;
	case enum_ufo_highpitch_sound: return ufo_highpitch_sound;
	}
	return (sound_t){0,0,0};
}
void testSound()
{
	XAC97_HardReset(SOUNDCHIP_BASEADDR);

	XAC97_ClearFifos(SOUNDCHIP_BASEADDR);

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_ExtendedAudioStat, AC97_EXTENDED_AUDIO_CONTROL_VRA);

	XAC97_AwaitCodecReady(SOUNDCHIP_BASEADDR);

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_Delay(100000);
	unsigned int itr = 0;
	unsigned int repeats = 0;
	xil_printf("starting sound loop \n\r");
	int sound_enum = enum_ufo_lowpitch_sound;
	sound_t soundFile = getSoundFile(sound_enum);

	while (true) {
		while (!XAC97_isInFIFOFull(SOUNDCHIP_BASEADDR)) {
			if (itr >= soundFile.numberOfSamples) {
				itr = 0;
				if (repeats++ >= 4) {
					xil_printf("playing new sound \n\r");
					repeats = 0;
					if (++sound_enum > enum_ufo_highpitch_sound) {
						sound_enum = 0;
					}
					soundFile = getSoundFile(sound_enum);
				}
			}
			XAC97_mSetInFifoData(SOUNDCHIP_BASEADDR, (soundFile.soundData[itr] | soundFile.soundData[itr] << 16 ));
			itr++;
		}
		XAC97_Delay(10000);
	}
}

