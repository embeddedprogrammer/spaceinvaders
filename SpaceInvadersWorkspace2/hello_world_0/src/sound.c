/*
 * sound.c
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "sound.h"
#include "timers.h"
#include "xac97_l.h"
#include "globals.h"
#include <stdio.h>

static int tankHitSoundTimer;
static int tankFireSoundTimer;
static int saucerFlyingSoundTimer;
static int alienMarchingSoundTimer;
static int saucerHitSoundTimer;
static int alienKilledSoundTimer;

typedef enum {
	enum_AC97_VOL_ATTN_0_DB = 0,
	enum_AC97_VOL_ATTN_1_5_DB,
	enum_AC97_VOL_ATTN_3_0_DB,
	enum_AC97_VOL_ATTN_4_5_DB,
	enum_AC97_VOL_ATTN_6_0_DB,
	enum_AC97_VOL_ATTN_7_5_DB,
	enum_AC97_VOL_ATTN_9_0_DB,
	enum_AC97_VOL_ATTN_10_0_DB,
	enum_AC97_VOL_ATTN_11_5_DB,
	enum_AC97_VOL_ATTN_13_0_DB,
	enum_AC97_VOL_ATTN_14_5_DB,
	enum_AC97_VOL_ATTN_16_0_DB,
	enum_AC97_VOL_ATTN_17_5_DB,
	enum_AC97_VOL_ATTN_19_0_DB,
	enum_AC97_VOL_ATTN_20_5_DB,
	enum_AC97_VOL_ATTN_22_0_DB,
	enum_AC97_VOL_ATTN_23_5_DB,
	enum_AC97_VOL_ATTN_25_0_DB,
	enum_AC97_VOL_ATTN_26_5_DB,
	enum_AC97_VOL_ATTN_28_0_DB,
	enum_AC97_VOL_ATTN_29_5_DB,
	enum_AC97_VOL_ATTN_31_0_DB,
	enum_AC97_VOL_ATTN_32_5_DB,
	enum_AC97_VOL_ATTN_34_0_DB,
	enum_AC97_VOL_ATTN_35_5_DB,
	enum_AC97_VOL_ATTN_37_0_DB,
	enum_AC97_VOL_ATTN_38_5_DB,
	enum_AC97_VOL_ATTN_40_0_DB,
	enum_AC97_VOL_ATTN_41_5_DB,
	enum_AC97_VOL_ATTN_43_0_DB,
	enum_AC97_VOL_ATTN_44_5_DB,
	enum_AC97_VOL_ATTN_46_0_DB
} volume_t;

static const volume_t max_volume = enum_AC97_VOL_ATTN_46_0_DB;
static const volume_t min_volume = enum_AC97_VOL_ATTN_0_DB;
static volume_t volume = enum_AC97_VOL_ATTN_22_0_DB;

int getVolumeValue(volume_t vol)
{
	switch(vol) {
		case enum_AC97_VOL_ATTN_0_DB    : return AC97_VOL_ATTN_0_DB;
		case enum_AC97_VOL_ATTN_1_5_DB  : return AC97_VOL_ATTN_1_5_DB;
		case enum_AC97_VOL_ATTN_3_0_DB  : return AC97_VOL_ATTN_3_0_DB;
		case enum_AC97_VOL_ATTN_4_5_DB  : return AC97_VOL_ATTN_4_5_DB;
		case enum_AC97_VOL_ATTN_6_0_DB  : return AC97_VOL_ATTN_6_0_DB;
		case enum_AC97_VOL_ATTN_7_5_DB  : return AC97_VOL_ATTN_7_5_DB;
		case enum_AC97_VOL_ATTN_9_0_DB  : return AC97_VOL_ATTN_9_0_DB;
		case enum_AC97_VOL_ATTN_10_0_DB : return AC97_VOL_ATTN_10_0_DB;
		case enum_AC97_VOL_ATTN_11_5_DB : return AC97_VOL_ATTN_11_5_DB;
		case enum_AC97_VOL_ATTN_13_0_DB : return AC97_VOL_ATTN_13_0_DB;
		case enum_AC97_VOL_ATTN_14_5_DB : return AC97_VOL_ATTN_14_5_DB;
		case enum_AC97_VOL_ATTN_16_0_DB : return AC97_VOL_ATTN_16_0_DB;
		case enum_AC97_VOL_ATTN_17_5_DB : return AC97_VOL_ATTN_17_5_DB;
		case enum_AC97_VOL_ATTN_19_0_DB : return AC97_VOL_ATTN_19_0_DB;
		case enum_AC97_VOL_ATTN_20_5_DB : return AC97_VOL_ATTN_20_5_DB;
		case enum_AC97_VOL_ATTN_22_0_DB : return AC97_VOL_ATTN_22_0_DB;
		case enum_AC97_VOL_ATTN_23_5_DB : return AC97_VOL_ATTN_23_5_DB;
		case enum_AC97_VOL_ATTN_25_0_DB : return AC97_VOL_ATTN_25_0_DB;
		case enum_AC97_VOL_ATTN_26_5_DB : return AC97_VOL_ATTN_26_5_DB;
		case enum_AC97_VOL_ATTN_28_0_DB : return AC97_VOL_ATTN_28_0_DB;
		case enum_AC97_VOL_ATTN_29_5_DB : return AC97_VOL_ATTN_29_5_DB;
		case enum_AC97_VOL_ATTN_31_0_DB : return AC97_VOL_ATTN_31_0_DB;
		case enum_AC97_VOL_ATTN_32_5_DB : return AC97_VOL_ATTN_32_5_DB;
		case enum_AC97_VOL_ATTN_34_0_DB : return AC97_VOL_ATTN_34_0_DB;
		case enum_AC97_VOL_ATTN_35_5_DB : return AC97_VOL_ATTN_35_5_DB;
		case enum_AC97_VOL_ATTN_37_0_DB : return AC97_VOL_ATTN_37_0_DB;
		case enum_AC97_VOL_ATTN_38_5_DB : return AC97_VOL_ATTN_38_5_DB;
		case enum_AC97_VOL_ATTN_40_0_DB : return AC97_VOL_ATTN_40_0_DB;
		case enum_AC97_VOL_ATTN_41_5_DB : return AC97_VOL_ATTN_41_5_DB;
		case enum_AC97_VOL_ATTN_43_0_DB : return AC97_VOL_ATTN_43_0_DB;
		case enum_AC97_VOL_ATTN_44_5_DB : return AC97_VOL_ATTN_44_5_DB;
		case enum_AC97_VOL_ATTN_46_0_DB : return AC97_VOL_ATTN_46_0_DB;
		default: return 0;
	}
}

void sound_volumeUp()
{
	xil_printf("volume Up \n\r");
	if (++volume > max_volume)
		volume = max_volume;

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_MasterVol, getVolumeValue(volume));
}

void sound_volumeDown()
{
	xil_printf("volume Down \n\r");
	if (--volume < min_volume)
		volume = min_volume;

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_MasterVol, getVolumeValue(volume));
}



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
