/*
 * spaceInvaders.c
 *
 *  Created on: Sep 22, 2015
 *      Author: superman
 */

#include <stdio.h>
#include "platform.h"
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
#include <stdbool.h>

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

#define DEBUG
void print(char *str);

#define MAX_SILLY_TIMER 10000000;
#define KEY_KILL_ALIEN '2'
#define KEY_TANK_LEFT '4'
#define KEY_TANK_RIGHT '6'
#define KEY_MOVE_ALIEN '8'
#define KEY_TANK_FIRE_BULLET '5'
#define KEY_MOVE_BULLETS '9'
#define KEY_ALIEN_FIRE_BULLET '3'
#define KEY_ERODE_BUNKER '7'
#define KEY_RESTART 'r'
#define KEY_SAUCER 's'

int getNumber()
{
	xil_printf("Please enter a number:\r\n");
	int num = 0;
	char input;
	while (true)
	{
		input = getchar();
		if(input >= '0' && input <= '9')
		{
			xil_printf("%c", input);
			num = num*10 + (input - '0');
		}
		else //if any other key is pressed, exit
		{
			xil_printf("\r\n");
			return num;
		}
	}
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler()
{
	// Clear the GPIO interrupt.
	XGpio_InterruptGlobalDisable(&gpPB);                    // Turn off all PB interrupts for now.

	//Do nothing for now
	//xil_printf("pb interupt\n\r");

	int currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	static int previousButtonState;
	if (previousButtonState != currentButtonState)
	{
		previousButtonState = currentButtonState;
	}

	// You need to do something here.
	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
	XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK)
	{
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
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

void initGameScreen()
{
	uint* framePointer = getFrameBuffer();
	// Clear screen
	int row, col;
	for (row = 0; row < 480; row++)
		for (col = 0; col < 640; col++)
			framePointer[row * 640 + col] = BACKGROUND_COLOR;

	draw_Bunkers();
	bullets_init();
	aliens_init();
	tank_init();
}

void gameOver()
{
	removeAllTimers();
	draw_string("GAME OVER", GAME_OVER_COLOR, (point_t){GAMEBUFFER_WIDTH/2-(9/2*FONT_COLS_OFFSET), GAMEBUFFER_HEIGHT/2}, false);
	addTimer(10000, false, &initGameScreen);
}

void initInterupts()
{
	// Initialize the GPIO peripherals. NOTE: We wait to do this till after the HDMI to ensure that nothing happens before the HDMI is enabled.
	int success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);

	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}

void listenToKeyPresses()
{
	xil_printf("Erode bunker:      %c ", KEY_ERODE_BUNKER);
	xil_printf("Move aliens:       %c ", KEY_MOVE_ALIEN);
	xil_printf("Move bullets:      %c ", KEY_MOVE_BULLETS);
	xil_printf("------\n\r");
	xil_printf("Left:              %c ", KEY_TANK_LEFT);
	xil_printf("Fire tank bullet   %c ", KEY_TANK_FIRE_BULLET);
	xil_printf("Right:             %c ", KEY_TANK_RIGHT);
	xil_printf("------\n\r");
	xil_printf("                   %c ", ' ');
	xil_printf("Kill alien:        %c ", KEY_KILL_ALIEN);
	xil_printf("Fire alien bullet: %c ", KEY_ALIEN_FIRE_BULLET);
	xil_printf("------\n\r");

	while (1) {
		char input;
		input = getchar();
		switch (input) {
		case KEY_TANK_LEFT:
			tank_moveTankLeft();
			break;
		case KEY_TANK_RIGHT:
			tank_moveTankRight();
			break;
		case KEY_KILL_ALIEN:
			xil_printf("Kill alien - ");
			aliens_killAlien(getNumber());
			xil_printf("Alien killed\r\n");
			break;
		case KEY_MOVE_ALIEN:
			aliens_shiftAlienFleet();
			break;
		case KEY_TANK_FIRE_BULLET:
			tank_fireBullet();
			break;
		case KEY_MOVE_BULLETS:
			bullets_moveAllBullets();
			break;
		case KEY_ALIEN_FIRE_BULLET:
			bullets_fireAlienBullet();
			break;
		case KEY_ERODE_BUNKER:
			xil_printf("Erode bunker - ");
			bunkers_erodeBunker(getNumber());
			xil_printf("Bunker eroded\r\n");
			break;
		case KEY_RESTART:
			initGameScreen();
			break;
		case KEY_SAUCER:
			aliens_startSaucer();
			break;
		default:
			xil_printf("Key pressed: %c (code %d)\r\n", input, (int)input);
		}
	}
}

int main()
{
	initVideo();
	initInterupts();
	initGameScreen();
	listenToKeyPresses();

	cleanup_platform();

	return 0;
}
