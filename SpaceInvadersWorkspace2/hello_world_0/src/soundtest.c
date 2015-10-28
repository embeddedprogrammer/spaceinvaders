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
#include <sys/types.h>
#include <stdio.h>
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

#include <stdbool.h>


typedef enum {
	enum_ufo_lowpitch_sound = 0,
	enum_explosion_sound,
	enum_fastinvader1_sound,
	enum_fastinvader2_sound,
	enum_fastinvader3_sound,
	enum_fastinvader4_sound,
	enum_invaderkilled_sound,
	enum_shoot_sound,
	enum_ufo_highpitch_sound
} soundEnum_t;

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

static int currentSoundPosition;
sound_t currentSound;
bool playingSound = false;

void writeSoundDataToFifo(int maxCount)
{
	int i;
	int levelBefore = XAC97_getInFIFOLevel(SOUNDCHIP_BASEADDR);
	for(i = 0; i < maxCount; i++)
	{
		// If there is no sound playing, push zeros into the queue.
		uint16_t data = playingSound ? (128 + currentSound.soundData[currentSoundPosition]) : 0;

		// Bits 0-15 are right channel, 16-31 are left channel. Our audio only has one channel, so we will use it for both channels.
		XAC97_mSetInFifoData(SOUNDCHIP_BASEADDR, (data | data << 16));

		// Advance position
		currentSoundPosition++;
		if(currentSoundPosition >= currentSound.numberOfSamples)
			playingSound = false;
	}
	int levelAfter = XAC97_getInFIFOLevel(SOUNDCHIP_BASEADDR);
	//xil_printf("%d %d\n\r", levelBefore, levelAfter);
}

void interrupt_handler_dispatcher2(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK)
	{
		writeSoundDataToFifo(100);
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	}
	else
		xil_printf("Unknown interupt: %x\n\r", intc_status);
}

void initInterrupts2()
{
	microblaze_register_handler(interrupt_handler_dispatcher2, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

void startPlayingNewSound(sound_t sound)
{
	currentSound = sound;
	currentSoundPosition = 0;
	playingSound = true;
}

void testSoundInterrupt()
{
	initInterrupts2();

	XAC97_HardReset(SOUNDCHIP_BASEADDR);
	XAC97_ClearFifos(SOUNDCHIP_BASEADDR);
	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_ExtendedAudioStat, AC97_EXTENDED_AUDIO_CONTROL_VRA);
	XAC97_AwaitCodecReady(SOUNDCHIP_BASEADDR);
	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_AwaitCodecReady(SOUNDCHIP_BASEADDR);
	XAC97_mSetControl(SOUNDCHIP_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
	XAC97_Delay(100000);

	sound_t soundFile;
	soundEnum_t soundEnum;
	int repeat = 0;

	xil_printf("starting sound loop \n\r");

	while (true)
	{
		if(!playingSound)
		{
			repeat++;
			if(repeat > 10)
			{
				soundEnum++;
				repeat = 0;
			}
			soundFile = getSoundFile(soundEnum);
			xil_printf("Play sound %d\n\r", soundEnum);
			if (soundEnum > enum_ufo_highpitch_sound) {
				soundEnum = 0;
			}
			startPlayingNewSound(soundFile);
		}
		XAC97_Delay(10000);
	}
}

