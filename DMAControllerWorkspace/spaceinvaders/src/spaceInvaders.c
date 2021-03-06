/*
 * spaceInvaders.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include "mouse.h"
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
#include "sound.h"

#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "xac97_l.h"        // Provides the functions for the sounds controller
#include "pit.h"
#include "dma_controller.h"

#include <stdbool.h>

#define CLOCK_FREQ_HZ 100000000 //100MHz
#define INTERRUPTS_PER_SECOND 100 //Every 10 ms

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
XGpio gpswitches;

static const int enable = 1;
static const int disable = 0;

#define DEBUG
void print(char *str);

#define MAX_SILLY_TIMER 10000000;
#define BUTTON_RESPONSE_TIME 20
#define TANK_MOUSE_DIVIDER 1

XAxiVdma videoDMAController;


static frame_t frameIndex = frame_gameScreen;

frame_t getFrame()
{
	return frameIndex;
}

void displayScreenCapture()
{
	frameIndex = frame_screenCapture;
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		 xil_printf("vdma parking failed\n\r");
	}
}

void displayGame()
{
	frameIndex = frame_gameScreen;
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		 xil_printf("vdma parking failed\n\r");
	}
}

XTmrCtr TmrCtrInstance;

void initTimer()
{
	if (XTmrCtr_Initialize(&TmrCtrInstance, XPAR_AXI_TIMER_0_DEVICE_ID) != XST_SUCCESS)
	{
		xil_printf("Error initializing timer\n\r");
	    return;
	}
}

void startTimer()
{
	XTmrCtr_Reset(&TmrCtrInstance, XPAR_AXI_TIMER_0_DEVICE_ID);
	XTmrCtr_Start(&TmrCtrInstance, XPAR_AXI_TIMER_0_DEVICE_ID);
}

void printTime()
{
    int timeInMs = (XTmrCtr_GetValue(&TmrCtrInstance, XPAR_AXI_TIMER_0_DEVICE_ID) / (XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ / 1000));
    xil_printf("Total time to capture screen: %d ms\n\r", timeInMs);
}

void captureScreenHW()
{
	startTimer();
    DMA_CONTROLLER_InitiateTransfer(XPAR_DMA_CONTROLLER_0_BASEADDR, getFrameBuffer(), getScreenCaptureFramePointer(),
    		4 * SCREENBUFFER_HEIGHT * SCREENBUFFER_WIDTH);
}

void captureScreenSW()
{
	startTimer();
	int row, col;
	uint* framePointer = getFrameBuffer();
	uint* screenCapture = getScreenCaptureFramePointer();
	for (row = 0; row < SCREENBUFFER_HEIGHT; row++)
		for (col = 0; col < SCREENBUFFER_WIDTH; col++) {
			int index = row * SCREENBUFFER_WIDTH + col;
			screenCapture[index] = framePointer[index];
		}
	printTime();
}

static int switchState = 0x40;
void respondToGPIOInput()
{
	const int PUSH_BUTTONS_CENTER = 0x01;
    const int PUSH_BUTTONS_RIGHT  = 0x02;
    const int PUSH_BUTTONS_LEFT   = 0x08;
    const int PUSH_BUTTONS_UP     = 0x10;
    const int PUSH_BUTTONS_DOWN   = 0x04;

	int buttonState = XGpio_DiscreteRead(&gpPB, 1);

	if (buttonState & PUSH_BUTTONS_LEFT)
		tank_moveTankLeft();
	else if (buttonState & PUSH_BUTTONS_RIGHT)
		tank_moveTankRight();

	if (buttonState & PUSH_BUTTONS_UP)
		tank_fireBullet();

	if (buttonState & PUSH_BUTTONS_CENTER)
		sound_volumeUp();
	else if (buttonState & PUSH_BUTTONS_DOWN)
		sound_volumeDown();

	int x = mouse_getXMovement();
	tank_moveTank(x / TANK_MOUSE_DIVIDER);
	if(mouse_getMouseButtons() == MOUSE_LEFT_BUTTON)
		tank_fireBullet();

	// switches
	const int SWITCH_SW7 = 0x80;
	const int SWITCH_SW6 = 0x40;
	const int SWITCH_SW5 = 0x20;
	int oldSwitchState = switchState;
	switchState = XGpio_DiscreteRead(&gpswitches, 1);

	if ((switchState & SWITCH_SW5) != (oldSwitchState & SWITCH_SW5)) { // switch changed position
		if (switchState & SWITCH_SW5)
			displayScreenCapture();
		else
			displayGame();

	}
	if ((switchState & SWITCH_SW7) && !(oldSwitchState & SWITCH_SW7)) { // switch flipped up
		captureScreenHW();
	}
	if ((switchState & SWITCH_SW6) && !(oldSwitchState & SWITCH_SW6)) { // switch flipped up
		captureScreenSW();
	}
}

int low_priority_intc_status;

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr)
{
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	//Check the PS2 Interrupt first
	if (intc_status & XPAR_PS2CTRL_0_INTERRUPT_MASK)
	{
		mouse_stateMachine(mouse_ps2ctrlReadValue());
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PS2CTRL_0_INTERRUPT_MASK);
	}
	if (intc_status & XPAR_PIT_0_INTERRUPT_MASK)
	{
		low_priority_intc_status = low_priority_intc_status | XPAR_PIT_0_INTERRUPT_MASK;
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_INTERRUPT_MASK);
	}
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK)
	{
		low_priority_intc_status = low_priority_intc_status | XPAR_AXI_AC97_0_INTERRUPT_MASK;
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	}
	if (intc_status & XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK)
	{
		printTime();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK);
	}
}

void lowPriorityInterruptHandler()
{
	if (low_priority_intc_status & XPAR_PIT_0_INTERRUPT_MASK)
	{
		timer_interrupt_handler();
		low_priority_intc_status = low_priority_intc_status & ~XPAR_PIT_0_INTERRUPT_MASK;
	}
	if (low_priority_intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK)
	{
		sound_writeToFifo(100);
		low_priority_intc_status = low_priority_intc_status & ~XPAR_AXI_AC97_0_INTERRUPT_MASK;
	}
}

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
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount = 2;
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
	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_ADDR + 4*640*480;

	if (XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController,
			XAXIVDMA_READ, myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.

	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if (XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)) {
		xil_printf("DMA START FAILED\r\n");
	}
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,
			XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}

static bool isNewGame;
static int gameLevel = 1;


void buttons_init()
{
	addTimer(BUTTON_RESPONSE_TIME, true, &respondToGPIOInput);
	gpioFunc = &respondToGPIOInput;
}

void initGameScreen()
{
	draw_clearScreen();
	draw_Bunkers();
	bunker_init(isNewGame);
	bullets_init();
	aliens_init();
	tank_init(isNewGame);
	buttons_init();
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

	XGpio_Initialize(&gpswitches, XPAR_SLIDE_SWITCHES_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&gpswitches, 1, 0x000000FF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_PIT_0_INTERRUPT_MASK
		   | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK
		   | XPAR_AXI_AC97_0_INTERRUPT_MASK
		   | XPAR_PS2CTRL_0_INTERRUPT_MASK
		   | XPAR_DMA_CONTROLLER_0_INTERRUPT_MASK));

	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

#define ENTER 13
#define BACKSPACE 8

int getNumber2()
{
	xil_printf("\r\nPlease enter a number to load into the PIT Timer Register:\r\n");
	int num = 0;
	char input;
	while (true)
	{
		input = getchar();
		if(input >= '0' && input <= '9')
			num = num*10 + (input - '0');
		else if(input == BACKSPACE)
		{
			num = num / 10;
			if(num > 0) // Erase space
				xil_printf("\r%d ", num);
			else
				xil_printf("\r ");
		}
		if(num > 0)
			xil_printf("\r%d", num);
		else
			xil_printf("\r");

		if(input == ENTER)
		{
			xil_printf("\r\n");
			return num;
		}
	}
}

int main()
{
	initTimer();
	initVideo();
	sound_initAC97();
	mouse_init();
	initInterrupts();
	isNewGame = true;
	initGameScreen();

	PIT_startRecurringTimer(XPAR_PIT_0_BASEADDR, CLOCK_FREQ_HZ / INTERRUPTS_PER_SECOND);

	while(1) // Program never ends.
		if(low_priority_intc_status != 0)
			lowPriorityInterruptHandler();

	cleanup_platform();
	return 0;
}
