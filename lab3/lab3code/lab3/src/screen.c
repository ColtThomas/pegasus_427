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
#include "text.h"
#include "globals.h"

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 10000000;


unsigned int * framePointer;
//unsigned int * framePointer1;
int frameIndex = 0;
XAxiVdma videoDMAController;

void screen_init() {
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.

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
     framePointer = (unsigned int *) FRAME_BUFFER_0_ADDR;
    // framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;

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

void screen_clear() {
	int row, col;
	for(row = 0; row < SCREEN_PIXELS_DOWN; row++) {
		for(col = 0; col < SCREEN_PIXELS_ACROSS; col++) {
			framePointer[row*SCREEN_PIXELS_ACROSS + col] = SCREEN_BLACK;
		}
	}
}

void screen_draw_pixel(int x, int y, int color) {
	framePointer[y*SCREEN_PIXELS_ACROSS + x] = color;
//	temporary delay
//	unsigned int i;
//	for(i = 0; i < 1000000; i++);
}

void screen_draw_double_pixel(int x, int y, int color) {
	screen_draw_pixel(x*2, y*2, color);
	screen_draw_pixel(x*2+1, y*2+1, color);
	screen_draw_pixel(x*2+1, y*2, color);
	screen_draw_pixel(x*2, y*2+1, color);
}

void screen_run_test() {
	//int sillyTimer = MAX_SILLY_TIMER;  // Just a cheap delay between frames.
char input,input1,input2;
int i;
unsigned char input_number;
point_t thing;

tank_draw_initial();
tank_draw_lives_initial();
aliens_draw_initial();
bunkers_draw_initial();
	     while (1) {
	    	// while (sillyTimer) sillyTimer--;    // Decrement the timer.
	    	// sillyTimer = MAX_SILLY_TIMER;       // Reset the timer.
	        // frameIndex = (frameIndex + 1) % 2;  // Alternate between frame 0 and frame 1.
	    	 input = getchar();
	    	 switch(input) {
	    	 case '1':
	    		 text_draw_score();
	    		 break;
	    	 case '2':
	    		 input = getchar();
	    		 input -= '0'; // turns the character digit into the value of digit
	    		 input_number = input * 10; // puts first input into high-order digit of two-digit number
	    		 input = getchar();
	    		 input -= '0';
	    		 input_number += input; // adds in low-order digit
	    		 aliens_kill_alien(input_number);
	    		 break;
	    	 case '3': // fire random alien missile
	    		 bullets_fire_aliens();
	    		 break;
	    	 case '5': // fire tank bullet
	    		 bullets_fire_tank();
	    	 	 break;
	    	 case '4': // move tank left
	    		 tank_move_left();
	    		 break;
	    	 case '6':// move tank right
	    		 tank_move_right();
	    		 break;
	    	 case '7': // erode bunker
				 input1 = getchar();
				 input1 -= '0';
				 for(i=0;i<9;i++){
					 bunker_damage(input1,i);
				 }
				 bunkers_update();
				 break;
	    	 case '8': // update alien position
//	    		 xil_printf("update aliens\r\n");
	    		 aliens_update_position();
	    		 break;
	    	 case '9': // update all bullets
	    		 bullets_update_position();
	    		 break;
	    	 }
	         if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
	        	 xil_printf("vdma parking failed\n\r");
	        }
	     }
}

