/*
 * screen.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include<stdint.h>

#ifndef SCREEN_H_
#define SCREEN_H_

// these define the screen width and height when using double pixels
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define SCREEN_PIXELS_ACROSS 640
#define SCREEN_PIXELS_DOWN 480

#define SCREEN_BLACK 0x00000000
#define SCREEN_RED 0x00FF0000
#define SCREEN_GREEN 0x0000FF00
#define SCREEN_BLUE 0x000000FF
#define SCREEN_WHITE 0x00FFFFFF
#define SCREEN_HOTPINK 0xFF69B4
#define SCREEN_SKYBLUE 0x7ec0ee

// moved all the init stuff into here.
void screen_init();

// draws every pixel black.
void screen_clear();

// draws a 2x2 pixel. X and Y should be values from a 320x240 grid, not a 640x480.
void screen_draw_double_pixel(int32_t x, int32_t y, int32_t color);

//draws a single pixel on a 640x480 grid
void screen_draw_pixel(int32_t x, int32_t y, int32_t color);

//returns the color of the current pixel
int32_t screen_double_color_pixel(int32_t x, int32_t y);

// draws space_invaders basics to screen, then awaits test input from UART
// uses a busy loop; replaced by core.h functions for FIT timer game implementation.
void screen_run_test();

// takes a screenshot using software
void screen_shot();

// takes a screenshot using hardware
void screen_shot_dma();

// displays the last saved screenshot instead of the active screen
void screen_display_screenshot();

// displays the active screen instead of the screenshot
void screen_display_active();

#endif /* SCREEN_H_ */
