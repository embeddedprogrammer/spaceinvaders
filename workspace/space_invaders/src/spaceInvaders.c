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
#include "time.h"
#include "unistd.h"
#include "globals.h"
#include "drawShape.h"
#include "control.h"
#define DEBUG
void print(char *str);

#define MAX_SILLY_TIMER 10000000;
#define KEY_KILL_ALIEN '2'
#define KEY_TANK_LEFT '4'
#define KEY_TANK_RIGHT '6'
#define KEY_MOVE_ALIEN '8'
#define KEY_KILL_COMPLETE 'k'
#define KEY_TANK_FIRE_BULLET '5'

int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
    XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
    // Step 2: Initialize the memory structure and the hardware.
    if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
    	xil_printf("VideoDMA Did not initialize.\r\n");
    }
    // Step 3: (optional) set the frame store number.
    if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
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
	   if(Status == XST_VDMA_MISMATCH_ERROR)
		   xil_printf("DMA Mismatch Error\r\n");
    }
    // Now we tell the driver about the geometry of our frame buffer and a few other things.
    // Our image is 480 x 640.
    XAxiVdma_DmaSetup myFrameBuffer;
    myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
    myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
    myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
    myFrameBuffer.FrameDelay = 0;
    myFrameBuffer.EnableCircularBuf=1;
    myFrameBuffer.EnableSync = 0;
    myFrameBuffer.PointNum = 0;
    myFrameBuffer.EnableFrameCounter = 0;
    myFrameBuffer.FixedFrameStoreAddr = 0;
    if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
    	xil_printf("DMA Config Failed\r\n");
     }
    // We need to give the frame buffer pointers to the memory that it will use. This memory
    // is where you will write your video data. The vdma IP/driver then streams it to the HDMI
    // IP.
     myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_ADDR;

     if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
    		               myFrameBuffer.FrameStoreStartAddr)) {
    	 xil_printf("DMA Set Address Failed Failed\r\n");
     }
     // Print a sanity message if you get this far.
     xil_printf("Woohoo! I made it through initialization.\n\r");
     // Now, let's get ready to start displaying some stuff on the screen.
     // The variables framePointer and framePointer1 are just pointers to the base address
     // of frame 0 and frame 1.
     uint* framePointer = getFrameBuffer();
     // Just paint some large red, green, blue, and white squares in different
     // positions of the image for each frame in the buffer (framePointer0 and framePointer1).
     int row=0, col=0;
     for( row=0; row<480; row++) {
    	 for(col=0; col<640; col++) {
    		 framePointer[row*640 + col] = 0x00000000;
       }
     }
     draw_Bunkers();
     point_t alienFleetPos;
	  alienFleetPos.row = GAMEBUFFER_HEIGHT/7;
	  alienFleetPos.col = GAMEBUFFER_WIDTH/6;
	  setAlienFleetPositionGlobal(alienFleetPos);
     draw_AlienFleet(true);


     point_t tankPos = getTankPositionGlobal();
	  tankPos.row = TANK_ROW;
	  tankPos.col = TANK_INTIAL_COL;
	  draw_Tank(tankPos);
	  setTankPositionGlobal(tankPos);
     // This tells the HDMI controller the resolution of your display (there must be a better way to do this).
     XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

     // Start the DMA for the read channel only.
     if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
    	 xil_printf("DMA START FAILED\r\n");
     }
     int frameIndex = 0;
     // We have two frames, let's park on frame 0. Use frameIndex to index them.
     // Note that you have to start the DMA process before parking on a frame.
     if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
    	 xil_printf("vdma parking failed\n\r");
     }
     // Oscillate between frame 0 and frame 1.
     // int sillyTimer = MAX_SILLY_TIMER;  // Just a cheap delay between frames.

    bool in = false;
	while (1) {
		char input;
		input = getchar();
		switch (input) {
		case KEY_TANK_LEFT:
			control_moveTankLeft();
			break;
		case KEY_TANK_RIGHT:
			control_moveTankRight();
			break;
		case KEY_KILL_ALIEN:
		{
			xil_printf("waiting for kill\r\n");
			int idx = 0;
			while ((input = getchar()) != 'k') {
				idx *= 10;
				idx += input - '0';
			}
			xil_printf("killed %d\r\n", idx);
			control_killAlien(idx);
			draw_AlienFleet(in);
			break;
		}
		case KEY_MOVE_ALIEN:
			xil_printf("shift alien fleet\r\n");
			control_shiftAlienFleet();
			in = !in;
			draw_AlienFleet(in);
		case KEY_TANK_FIRE_BULLET:
			xil_printf("fire bullet\r\n");
			control_fireBullet();
		default:
			xil_printf("Key pressed: %c (code %d)\r\n", input, (int)input);
		}

		//    	while (sillyTimer) sillyTimer--;    // Decrement the timer.
		//    	sillyTimer = MAX_SILLY_TIMER;       // Reset the timer.
		if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, 0,
				XAXIVDMA_READ)) {
			xil_printf("vdma parking failed\n\r");
		}
	}
	cleanup_platform();

	return 0;
}
