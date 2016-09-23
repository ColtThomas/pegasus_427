/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "screen.h"
#define DEBUG
void print(char *str);



int main()
{
	screen_init();
	//screen_clear(); // now called inside screen_init();
     // Just paint some large red, green, blue, and white squares in different
     // positions of the image for each frame in the buffer (framePointer0 and framePointer1).
     int row=0, col=0;
     for( row=0; row<480; row++) {
    	 for(col=0; col<640; col++) {
    	 if(row < 240) {
    		 if(col<320) {
    			 // upper left corner.
    			 screen_draw_pixel(0,col,row,SCREEN_RED);
    			 screen_draw_pixel(1,col,row,SCREEN_GREEN);

    		 } else {
    			 // upper right corner.
    			 screen_draw_pixel(0,col,row,SCREEN_BLUE);
    			 screen_draw_pixel(1,col,row,SCREEN_RED);
    		 }
    	 } else {
    		 if(col<320) {
    			 // lower left corner.
    			 screen_draw_pixel(0,col,row,SCREEN_GREEN);
    			 screen_draw_pixel(1,col,row,SCREEN_WHITE);
    		 } else {
    			 // lower right corner.
    			 screen_draw_pixel(0,col,row,SCREEN_WHITE);
    	   		 screen_draw_pixel(1,col,row,SCREEN_BLUE);
    		 }
    	 }
       }
     }

     // Oscillate between frame 0 and frame 1.
     screen_run_loop();
     cleanup_platform();

    return 0;
}
