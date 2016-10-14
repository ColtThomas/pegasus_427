/*
 * screen.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */


#include "screen.h"
#include <stdio.h>
#include "tank.h"
#include "aliens.h"
#include "bunkers.h"
#include "bullets.h"
#include "bulletHandler.h"
#include "text.h"
#include "globals.h"
#include<stdint.h>
#include "alienHandler.h"
#include "core.h"

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 10000000;


uint32_t *framePointer;
int32_t frameIndex = 0;
XAxiVdma videoDMAController;

// initializes the screen.
// mostly copy-paste from lab website
void screen_init() {
	init_platform();                   // Necessary for all programs.
	int32_t Status;                        // Keep track of success/failure of system function calls.

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
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
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
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	//  myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
			myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	framePointer = (uint32_t *) FRAME_BUFFER_0_ADDR;
	// framePointer1 = ((uint *) FRAME_BUFFER_0_ADDR) + 640*480;

	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
	}
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}

}

// draws every pixel black.
void screen_clear() {
	int32_t row, col;
	for(row = 0; row < SCREEN_PIXELS_DOWN; row++) {
		for(col = 0; col < SCREEN_PIXELS_ACROSS; col++) {
			framePointer[row*SCREEN_PIXELS_ACROSS + col] = SCREEN_BLACK;
		}
	}
}

//draws a single pixel on a 640x480 grid
void screen_draw_pixel(int32_t x, int32_t y, int32_t color) {
	framePointer[y*SCREEN_PIXELS_ACROSS + x] = color;
}

// some #defines used by draw_double_pixel
#define twice(x) (x)*2
#define p_after(x) twice((x))+1

// draws a 2x2 pixel. X and Y should be values from a 320x240 grid, not a 640x480.
// this allowed us to map everything we draw onto a smaller grid
void screen_draw_double_pixel(int32_t x, int32_t y, int32_t color) {
	screen_draw_pixel(twice(x), twice(y), color);
	screen_draw_pixel(p_after(x), p_after(y), color);
	screen_draw_pixel(p_after(x), twice(y), color);
	screen_draw_pixel(twice(x), p_after(y), color);
}

// some #defines used by run_test
#define KILL_ALIEN '2'
#define FIRE_A_BULLET '3'
#define FIRE_T_BULLET '5'
#define MOVE_T_LEFT '4'
#define MOVE_T_RIGHT '6'
#define ERODE_BUNKER '7'
#define UPDATE_ALIENS '8'
#define UPDATE_BULLETS '9'
#define CHAR_TO_INT '0'
#define times_ten(x) (x)*10

// draws space_invaders basics to screen, then awaits test input from UART
void screen_run_test() {
	char input;
//	int32_t i;
	uint8_t input_number;

	// initial draws
	core_draw_initial();

	//point_t testPoint;
	// input/response loop
	while (1) {
		input = getchar();
		switch(input) {
		case '1': //debug
			text_increment_score();
//			tank_remove_life();
//			bunker_destroy_row(0);
			break;
		case KILL_ALIEN:
			input = getchar();
			input -= CHAR_TO_INT; // turns the character digit into the value of digit
			input_number = times_ten(input); // puts first input into high-order digit of two-digit number
			input = getchar();
			input -= CHAR_TO_INT;
			input_number += input; // adds in low-order digit
			aliens_kill_alien(input_number);
			break;
		case FIRE_A_BULLET: // fire random alien missile
			bullets_fire_aliens();
			break;
		case FIRE_T_BULLET: // fire tank bullet
			bullets_fire_tank();
			break;
		case MOVE_T_LEFT: // move tank left
			tank_move_left();
			break;
		case MOVE_T_RIGHT:// move tank right
			tank_move_right();
			break;
		case ERODE_BUNKER: // erode bunker
//			input = getchar();
//			input -= CHAR_TO_INT;
//			for(i=0;i<GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER;i++){
//				bunker_damage(input,i);
//			}
//			bunkers_update();
			break;
		case UPDATE_ALIENS: // update alien position
			alienHandler_tick();

			break;
		case UPDATE_BULLETS: // update all bullets
			bulletHandler_tick();
			break;
		}
		if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
			xil_printf("vdma parking failed\n\r");
		}
	}
}
