/*
 * soundtest.c
 *
 *  Created on: Oct 27, 2015
 *      Author: superman
 */

#include "soundtest.h"
#include "globals.h"
#include "xparameters.h"

static const int enable = 1;
static const int disable = 0;


void testSound()
{
	XAC97_HardReset(SOUNDCHIP_BASEADDR);
	while (!XAC97_isRegisterAccessFinished(SOUNDCHIP_BASEADDR)) {}
	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_ExtendedAudioStat, enable);
	while (!XAC97_isRegisterAccessFinished(SOUNDCHIP_BASEADDR)) {}
	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	while (!XAC97_isRegisterAccessFinished(SOUNDCHIP_BASEADDR)) {}
}

