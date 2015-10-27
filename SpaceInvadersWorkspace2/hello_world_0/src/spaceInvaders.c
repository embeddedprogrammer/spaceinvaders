/*
 * spaceInvaders.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include <stdio.h>
#include "platform.h"
#include "keyboard.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "fontBitmap.h"
#include "time.h"
#include "unistd.h"
#include "globals.h"
#include "drawShape.h"
#include "tank.h"
#include "bullets.h"
#include "timers.h"
#include "aliens.h"

#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "xac97_l.h"        // Provides the functions for the sounds controller

#include <stdbool.h>

#include "soundtest.h"

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

static const int enable = 1;
static const int disable = 0;

#define DEBUG
void print(char *str);

#define MAX_SILLY_TIMER 10000000;
#define BUTTON_RESPONSE_TIME 20

void respondToButtonInput()
{
    const int PUSH_BUTTONS_RIGHT  = 0x02;
    const int PUSH_BUTTONS_LEFT   = 0x08;
    const int PUSH_BUTTONS_UP     = 0x10;
	int buttonState = XGpio_DiscreteRead(&gpPB, 1);
	if(buttonState & PUSH_BUTTONS_LEFT)
		tank_moveTankLeft();
	else if(buttonState & PUSH_BUTTONS_RIGHT)
		tank_moveTankRight();
	if(buttonState & PUSH_BUTTONS_UP)
		tank_fireBullet();
}

u32 totalTimeSpentInInterrupts = 0;
XTmrCtr TmrCtrInstance;

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		u32 timerValBegin = XTmrCtr_GetValue(&TmrCtrInstance, 0);
		timer_interrupt_handler();
		u32 timerValEnd = XTmrCtr_GetValue(&TmrCtrInstance, 0);
		u32 timeInInterrupt = timerValEnd - timerValBegin;
		totalTimeSpentInInterrupts += timeInInterrupt;
	}
}

void initSound()
{
	XAC97_HardReset(SOUNDCHIP_BASEADDR);

	XAC97_ClearFifos(SOUNDCHIP_BASEADDR);

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_ExtendedAudioStat, AC97_EXTENDED_AUDIO_CONTROL_VRA);

	XAC97_AwaitCodecReady(SOUNDCHIP_BASEADDR);

	XAC97_WriteReg(SOUNDCHIP_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
}

XAxiVdma videoDMAController;

void initVideo()
{
	init_platform(); // Necessary for all programs.
	int Status; // Keep track of success/failure of system function calls.
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(
			XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if (XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController,
			VideoDMAConfig, XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
	}
	// Step 3: (optional) set the frame store number.
	if (XST_FAILURE == XAxiVdma_SetFrmStore(&videoDMAController, 2,
			XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 1;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount = 1;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if (Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is 480 x 640.
	XAxiVdma_DmaSetup myFrameBuffer;
	myFrameBuffer.VertSizeInput = 480; // 480 vertical pixels.
	myFrameBuffer.HoriSizeInput = 640 * 4; // 640 horizontal (32-bit pixels).
	myFrameBuffer.Stride = 640 * 4; // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf = 1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if (XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ,
			&myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
	}
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_ADDR;

	if (XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController,
			XAXIVDMA_READ, myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuf4f on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.

	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if (XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)) {
		xil_printf("DMA START FAILED\r\n");
	}
	int frameIndex = 0;
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,
			XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}
static bool isNewGame;
static int gameLevel = 1;

void initGameScreen()
{
	draw_clearScreen();
	draw_Bunkers();
	bunker_init(isNewGame);
	bullets_init();
	aliens_init();
	tank_init(isNewGame);
	addTimer(BUTTON_RESPONSE_TIME, true, &respondToButtonInput);
}

void restart()
{
	removeAllTimers();
	isNewGame = true;
	isNewGame = 1;
	initGameScreen();
}

void gameOver()
{
	removeAllTimers();
	draw_string("GAME OVER", GAME_OVER_COLOR, (point_t){GAMEBUFFER_WIDTH/2-(9/2*FONT_COLS_OFFSET), GAMEBUFFER_HEIGHT/2}, false);
	isNewGame = true;
	gameLevel = 1;
	addTimer(10000, false, &initGameScreen);
}

void nextLevel()
{
	removeAllTimers();
	char* nextLevelText = "Level   ";
	gameLevel++;
	char tens = gameLevel % 10;
	nextLevelText[6] = ((tens) ? (tens) : ' ') + '0';
	nextLevelText[7] = (gameLevel - tens*10) + '0';
	draw_string(nextLevelText, SCORE_ABC_COLOR, (point_t){GAMEBUFFER_WIDTH/2-(9/2*FONT_COLS_OFFSET), GAMEBUFFER_HEIGHT/2}, false);
	isNewGame = false;
	addTimer(3000, false, &initGameScreen);
}

void initInterrupts()
{
	// Initialize the GPIO peripherals. NOTE: We wait to do this till after the HDMI to ensure that nothing happens before the HDMI is enabled.
	XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);

	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

void initTimers()
{
	if (XTmrCtr_Initialize(&TmrCtrInstance, XPAR_AXI_TIMER_0_DEVICE_ID) != XST_SUCCESS)
	{
		xil_printf("Error initializing timer\n\r");
	    return;
	}
	XTmrCtr_Start(&TmrCtrInstance, 0);
}

void printStats()
{
    int runningTimeInMs = (XTmrCtr_GetValue(&TmrCtrInstance, 0) / (XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ / 1000));
    int interruptsTimeInMs = (totalTimeSpentInInterrupts / (XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ / 1000));

    xil_printf("Total time running: %d\n\r", runningTimeInMs);
    xil_printf("Total time spent in interrupts: %d\n\r", interruptsTimeInMs);
    xil_printf("CPU utilization: %d%%\n\r", interruptsTimeInMs*100/runningTimeInMs);
}


int main()
{
//	initTimers();
//	initVideo();
//	initSound();
//	initInterrupts();
//	isNewGame = true;
//	initGameScreen();
//	while(true);
//	cleanup_platform();
//	return 0;
	testSound();
}
