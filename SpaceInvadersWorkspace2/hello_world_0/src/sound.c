/*
 * sound.c
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "sound.h"
#include "soundData.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "xac97_l.h"
#include "xintc_l.h"
#include "xparameters.h"

static int volumeLevel = MAX_VOLUME; // The HW is set to max volume (zero attenuation) by default.
static int currentSoundPosition;
static sound_t currentSound;
static bool playingSound = false;

//VOLUME

int sound_getAC97Attn(int volume)
{
	switch(volume)
	{
		case 0  : return AC97_VOL_ATTN_46_0_DB;
		case 1  : return AC97_VOL_ATTN_44_5_DB;
		case 2  : return AC97_VOL_ATTN_43_0_DB;
		case 3  : return AC97_VOL_ATTN_41_5_DB;
		case 4  : return AC97_VOL_ATTN_40_0_DB;
		case 5  : return AC97_VOL_ATTN_38_5_DB;
		case 6  : return AC97_VOL_ATTN_37_0_DB;
		case 7  : return AC97_VOL_ATTN_35_5_DB;
		case 8  : return AC97_VOL_ATTN_34_0_DB;
		case 9  : return AC97_VOL_ATTN_32_5_DB;
		case 10 : return AC97_VOL_ATTN_31_0_DB;
		case 11 : return AC97_VOL_ATTN_29_5_DB;
		case 12 : return AC97_VOL_ATTN_28_0_DB;
		case 13 : return AC97_VOL_ATTN_26_5_DB;
		case 14 : return AC97_VOL_ATTN_25_0_DB;
		case 15 : return AC97_VOL_ATTN_23_5_DB;
		case 16 : return AC97_VOL_ATTN_22_0_DB;
		case 17 : return AC97_VOL_ATTN_20_5_DB;
		case 18 : return AC97_VOL_ATTN_19_0_DB;
		case 19 : return AC97_VOL_ATTN_17_5_DB;
		case 20 : return AC97_VOL_ATTN_16_0_DB;
		case 21 : return AC97_VOL_ATTN_14_5_DB;
		case 22 : return AC97_VOL_ATTN_13_0_DB;
		case 23 : return AC97_VOL_ATTN_11_5_DB;
		case 24 : return AC97_VOL_ATTN_10_0_DB;
		case 25 : return AC97_VOL_ATTN_9_0_DB;
		case 26 : return AC97_VOL_ATTN_7_5_DB;
		case 27 : return AC97_VOL_ATTN_6_0_DB;
		case 28 : return AC97_VOL_ATTN_4_5_DB;
		case 29 : return AC97_VOL_ATTN_3_0_DB;
		case 30 : return AC97_VOL_ATTN_1_5_DB;
		case 31 : return AC97_VOL_ATTN_0_DB;
		default : return AC97_VOL_ATTN_0_DB;
	}
}

void sound_setVolume(int volume)
{
	if(volume > MAX_VOLUME)
		volume = MAX_VOLUME;
	if(volume < MIN_VOLUME)
		volume = MIN_VOLUME;
	volumeLevel = volume;
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, sound_getAC97Attn(volume));
}

int sound_getVolume()
{
	return volumeLevel;
}

void sound_volumeUp()
{
	sound_setVolume(volumeLevel + 1);
}

void sound_volumeDown()
{
	sound_setVolume(volumeLevel - 1);
}

// SOUND

void sound_writeToFifo(int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		// If there is no sound playing, push zeros into the queue.
		uint16_t data = playingSound ? (128 + currentSound.soundData[currentSoundPosition]) : 0;

		// Bits 0-15 are right channel, 16-31 are left channel. Our audio only has one channel, so we will use it for both channels.
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, (data | data << 16));

		// Advance position
		currentSoundPosition++;
		if(currentSoundPosition >= currentSound.numberOfSamples)
			playingSound = false;
	}
}

void sound_interrupt_handler(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK)
	{
		sound_writeToFifo(100);
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	}
	else
		xil_printf("Unknown interupt: %x\n\r", intc_status);
}

void sound_play(sound_t sound)
{
	currentSound = sound;
	currentSoundPosition = 0;
	playingSound = true;
}

// Enable interrupts by using a simple interrupt routine. If more complex
// interrupt routine is needed, you must create your own interrupt handler routine.
void sound_initInterupts()
{
	microblaze_register_handler(sound_interrupt_handler, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

// Inits the AC97. Note: you must also enable interrupts by either calling
// sound_initInterupts, or create your own interrupt handler routine.
void sound_initAC97()
{
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, AC97_EXTENDED_AUDIO_CONTROL_VRA);
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
	XAC97_Delay(100000);
}

void sound_test()
{
	sound_initAC97();
	sound_initInterupts();
	xil_printf("Press a number between 0 and 8 to play a sound\r\n");
	while(true)
	{
		char input;
		input = getchar();
		switch (input) {
		case '0':
			sound_play(explosion_sound);
			break;
		case '1':
			sound_play(fastinvader1_sound);
			break;
		case '2':
			sound_play(fastinvader2_sound);
			break;
		case '3':
			sound_play(fastinvader3_sound);
			break;
		case '4':
			sound_play(fastinvader4_sound);
			break;
		case '5':
			sound_play(invaderkilled_sound);
			break;
		case '6':
			sound_play(shoot_sound);
			break;
		case '7':
			sound_play(ufo_highpitch_sound);
			break;
		case '8':
			sound_play(ufo_lowpitch_sound);
			break;
		case '+':
			sound_volumeUp();
			break;
		case '-':
			sound_volumeDown();
			break;
		default:
			xil_printf("Key pressed: %c (code %d)\r\n", input, (int)input);
			break;
		}
	}
}


