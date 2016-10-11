/*
 * tank.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "tank.h"
#include "screen.h"
#include "globals.h"
#include "bullets.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>

#define packWord15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define	TANK_Y 220 // out of a 240 height
#define TANK_HEIGHT 8
#define TANK_WIDTH 15
#define TANK_MOVEMENT 3
#define TANK_NUM_BITMAPS 5
#define LIVES_Y 10
#define BIT_MASK 1
#define X_OFFSET 1
#define TANK_RESPAWN_POS 158
#define NUMBER_OF_LIVES 3

// Indexes for bitmaps
#define TANK_INIT 0
#define TANK_DMG_A 1
#define TANK_DMG_B 2
#define TANK_DMG_C 3
#define TANK_GHOST 4


// Tank Directions
#define TANK_LEFT 0
#define TANK_RIGHT 1
#define TANK_NO_MOTION 2

static const long tank_15x8[] =
{
packWord15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packWord15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const long tank_destroyed_a_15x8[] =
{
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,1,0,0,0,0,0,1,0,1,0,1,0),
packWord15(0,0,0,0,0,0,1,0,1,0,0,0,0,0,0),
packWord15(0,0,1,0,1,0,0,0,0,0,0,0,1,0,0),
packWord15(0,0,0,0,0,1,0,0,1,1,0,0,0,0,0),
packWord15(1,0,0,1,1,1,1,1,1,1,1,1,1,0,0),
packWord15(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const long tank_destroyed_b_15x8[] =
{
packWord15(0,0,0,0,0,0,0,0,0,0,1,0,0,0,0),
packWord15(0,1,0,0,0,0,0,1,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,1,0,0,0,0,0,0,1,0,0),
packWord15(1,0,0,1,0,0,0,1,0,0,0,0,0,0,1),
packWord15(0,0,0,0,0,1,0,0,1,1,0,0,0,0,0),
packWord15(0,1,0,1,1,1,1,1,1,1,1,1,1,0,0),
packWord15(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const long tank_destroyed_c_15x8[] =
{
packWord15(0,0,0,0,0,0,0,0,1,0,0,0,0,0,0),
packWord15(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0),
packWord15(0,0,0,0,0,1,0,0,0,1,0,0,0,0,0),
packWord15(0,1,0,0,0,0,0,0,0,0,0,1,0,1,0),
packWord15(0,0,1,0,0,1,0,0,1,1,0,0,0,0,1),
packWord15(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0),
packWord15(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const long tank_ghost15x8[] =
{
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
};

static const int32_t tank_lives_positions[NUMBER_OF_LIVES] = {250, 270, 290};
static const long *tank_bitmap_array[TANK_NUM_BITMAPS] = {tank_15x8,tank_destroyed_a_15x8,tank_destroyed_b_15x8,tank_destroyed_c_15x8,tank_ghost15x8};
// draws the tank near the center bottom of the screen
void tank_draw_initial() {
	int32_t x, y;
	for(x = 0; x < TANK_WIDTH; x++) {
		for(y = 0; y < TANK_HEIGHT; y++) {
			if(tank_15x8[y] & (BIT_MASK << x)) {
				screen_draw_double_pixel(x+globals_getTankPosition(),y+TANK_Y,SCREEN_GREEN);
			}
		}
	}
}

void tank_erase(uint32_t bitmap){
	uint32_t x,y;

	for(x = 0; x < TANK_WIDTH; x++) {
		for(y = 0; y < TANK_HEIGHT; y++) {
			if(tank_bitmap_array[bitmap][y] & (BIT_MASK << x)) {
				screen_draw_double_pixel(x+globals_getTankPosition(),y+TANK_Y,SCREEN_BLACK);
			}
		}
	}
}

// draws 3 copies of the tank in the top right corner of the screen
// to represent the number of remaining lives
void tank_draw_lives_initial() {
	int32_t x, y, i;
	for(i = 0; i < NUMBER_OF_LIVES; i++) {
		for(x = 0; x < TANK_WIDTH; x++) {
			for(y = 0; y < TANK_HEIGHT; y++) {
				if(tank_15x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+tank_lives_positions[i],y+LIVES_Y,SCREEN_GREEN);
				}
			}
		}
	}
}


void tank_redraw(uint32_t pre_image,uint32_t post_image,uint32_t direction){

	uint32_t x,y;
	uint32_t xBitShift = (direction!=TANK_LEFT)? 0:TANK_MOVEMENT;
	uint32_t offset = (direction!=TANK_NO_MOTION)? TANK_MOVEMENT:0;
	uint32_t rightShift = (direction==TANK_RIGHT)? TANK_MOVEMENT:0;

	for(y = 0; y < TANK_HEIGHT; y++) {
		for(x = 0; x < TANK_WIDTH+offset; x++) {
			if((tank_bitmap_array[pre_image][y] & (BIT_MASK<<x)) != ((tank_bitmap_array[post_image][y]<<offset) & (BIT_MASK<<(x)))) { //some changes has occured to this pixel
					if((tank_bitmap_array[post_image][y]<<xBitShift) &(BIT_MASK<<x)) {
						screen_draw_double_pixel(globals_getTankPosition()+rightShift+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_GREEN);
					}
					else {
						screen_draw_double_pixel(globals_getTankPosition()+rightShift+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_BLACK);
					}
			}
		}
	}
}


// moves the tank to the left
void tank_move_left() {
	if(globals_getTankPosition()-TANK_MOVEMENT < 0) return;
	tank_redraw(TANK_INIT,TANK_INIT,TANK_LEFT);
	globals_setTankPosition(globals_getTankPosition()-TANK_MOVEMENT);
}

// moves the tank to the right
void tank_move_right() {
	if(globals_getTankPosition()+TANK_MOVEMENT >= SCREEN_WIDTH - TANK_WIDTH) return;
	tank_redraw(TANK_INIT,TANK_INIT,TANK_RIGHT);
	globals_setTankPosition(globals_getTankPosition()+TANK_MOVEMENT);
}

// COME BACK AND DO THIS MORE EFFICIENTLY!!!!!
void tank_animate() {
	int32_t j;

	for(j=0;j<50000;j++){}
	xil_printf("\r\nAnimate change");
	tank_redraw(TANK_DMG_A,TANK_DMG_B,TANK_NO_MOTION);

	for(j=0;j<50000;j++){}
	xil_printf("\r\nAnimate change");
	tank_redraw(TANK_DMG_B,TANK_DMG_C,TANK_NO_MOTION);
	for(j=0;j<50000;j++){}
//	tank_redraw(TANK_DMG_C,TANK_GHOST,TANK_NO_MOTION);

}

void tank_demolish(){
		if(globals_getTankPosition() >= SCREEN_WIDTH - TANK_WIDTH){
			xil_printf("\r\nMEOW");
			return;
		}
		xil_printf("\r\nAnimate change");
		tank_redraw(TANK_INIT,TANK_DMG_A,TANK_NO_MOTION);
		tank_animate();
}


bool tank_check_hit(point_t pos){
	xil_printf("\r\npos: %d %d",pos.x,pos.y);
	pos.y += bullets_get_height();
	if((pos.y>=TANK_Y) && (pos.y <= TANK_Y+TANK_HEIGHT)) {
		tank_demolish();
		return true;
	}
	return false;
}

void tank_respawn() {
	// Erase debris
	xil_printf("\r\nrespawn");
	tank_redraw(TANK_DMG_C,TANK_GHOST,TANK_NO_MOTION);
	globals_setTankPosition(TANK_RESPAWN_POS);
	// Draw initial tank; verify the position coordinates
	tank_redraw(TANK_GHOST,TANK_INIT,TANK_NO_MOTION);

}
