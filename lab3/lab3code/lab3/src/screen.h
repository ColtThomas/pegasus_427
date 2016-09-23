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

#ifndef SCREEN_H_
#define SCREEN_H_

#define SCREEN_PIXELS_ACROSS 640
#define SCREEN_PIXELS_DOWN 480

#define SCREEN_BLACK 0x00000000
#define SCREEN_RED 0x00FF0000
#define SCREEN_GREEN 0x0000FF00
#define SCREEN_BLUE 0x000000FF
#define SCREEN_WHITE 0x00FFFFFF

// moved all the init stuff into here.
void screen_init();

// draws every pixel black.
void screen_clear();

void screen_draw_pixel(int buffer, int x, int y, int color);

void screen_run_loop(); // to be deprecated, probably.
// I'm thinking that instead of a busy loop, this should be triggered by a FIT interrupt. but this works for now.

#endif /* SCREEN_H_ */
