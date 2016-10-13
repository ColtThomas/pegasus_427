/*
 * saucer.c
 *
 *  Created on: Oct 13, 2016
 *      Author: superman
 */

#include "saucer.h"
#include "screen.h"
#include "bunkers.h"
#include "text.h"
#include "globals.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b15 << 15) |(b14 << 14) |(b13 << 13) |(b12 << 12) |(b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define SAUCER_LEFT_X 0
#define SAUCER_LEFT_Y 12
#define SAUCER_RIGHT_X SCREEN_WIDTH
#define SAUCER_RIGHT_Y 12
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7

#define BIT_MASK 1

static const uint32_t saucer_16x7[] =
{
packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

static bool leftBound;
static bool isSpawned = false;
bool saucer_randBool() {
	return rand() % 1;	// Random number generated to add random sequence square
}

void saucer_redraw(bool direction) {
//	int32_t x, y;
	point_t pos = globals_getSaucerPosition();
	for(pos.x = 0; pos.x < SAUCER_WIDTH; pos.x++) {
		for(pos.y = 0; pos.y < SAUCER_HEIGHT; pos.y++) {
			if(saucer_16x7[pos.y] & (BIT_MASK << pos.x)) {
				screen_draw_double_pixel(pos.x,pos.y,SCREEN_RED);
			}
		}
	}
}
void saucer_update() {
	// check to see if it has spawned
		// see which side it spawned from

		// increment/decrement position

		// if off the screen, raise flag for spawn enable. Make sure saucer doesn't roll around
}


void saucer_spawn() {

	// check to see if the flag is raised
	if(!isSpawned) {
		xil_printf("\r\nSaucer Spawned");
		// randomly pick left or right
		leftBound =saucer_randBool();
		point_t initPos = {0,0};
		globals_setSaucerPosition(initPos);
		// draw alien at that position
		saucer_redraw(isSpawned);
	}


}
