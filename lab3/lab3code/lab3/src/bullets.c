/*
 * bullets.c
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#include "bullets.h"
#include "screen.h"
#include "globals.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>

#define BULLET_HEIGHT 5
#define BULLET_WIDTH 3
#define BULLET_SPEED 4
#define BULLET_START_X globals_getTankPosition() + 6
#define BULLET_START_Y 220 - 2*BULLET_HEIGHT

// Not good practice... but for the sake of passoff
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define TOP_ROW 11
#define MIDDLE_ROW 33
#define BOTTOM_ROW 55
#define ALIENS_PER_ROW 11
#define ALIEN_SPACING 16
#define BUFFER_WIDTH 10
#define ALIEN_BLOCK_HEIGHT 5*ALIEN_SPACING
#define ALIEN_BLOCK_WIDTH ALIENS_PER_ROW*ALIEN_SPACING
#define ALIEN_MOVEMENT 4
#define ALIEN_ROWS 5
#define ALIEN_COLUMNS 11

#define BULLET_ALIEN_HALFSPACE ALIEN_WIDTH/2 - 1
#define packword3(b2,b1,b0) \
		( (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )



static const int tank_bullet_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const int alien_bullet_up_3x5[] =
{
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const int alien_bullet_down_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0)
};
static point_t tankBulletPos;
static point_t alienBulletPos;

static bool tankFired = false;
static bool alien_bullet_status[GLOBALS_NUMBER_OF_ALIEN_BULLETS];
static u8 alien_bullet_count = 0;


void bullets_fire_tank() {
	if(!tankFired) {
		tankFired = true;

		// Reset global position on bullet
		point_t tankBulletPos;
		tankBulletPos.x = BULLET_START_X;
		tankBulletPos.y = BULLET_START_Y;
		globals_setTankBulletPosition(tankBulletPos);

		// draw the tank bullet
		bullets_draw_tank_bullet();
	}
}

void bullets_erase_tank_bullet() {
	point_t tankBulletPos = globals_getTankBulletPosition();
	int xOffset,yOffset;
//	tankFired = false;
	int x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (tank_bullet_3x5[y]) & (1 << x) ) {
				xOffset = x + tankBulletPos.x;
				yOffset = y + tankBulletPos.y;
				screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
			}

		}
	}
}

void bullets_draw_tank_bullet() {
	point_t tankBulletPos = globals_getTankBulletPosition();
	int xOffset,yOffset;
	int x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (tank_bullet_3x5[y]) & (1 << x) ) {
				xOffset = x + tankBulletPos.x;
				yOffset = y + tankBulletPos.y;
				screen_draw_double_pixel(xOffset,yOffset,SCREEN_WHITE);
			}

		}
	}
}

// Figure out what to do with the alien 0x0 stuff
void bullets_fire_aliens(){
	point_t thing = globals_getAlienBlockPosition();
//	xil_printf("\r\nAlien block position before: %d %d",thing.x,thing.y);
	if(alien_bullet_count<GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
//		alien_bullet_status[0] = true;
		alien_bullet_count++;
		xil_printf("\r\nSpawn ",alien_bullet_count);
		// Reset global position on bullet
		point_t alienBulletPos = globals_getAlienBlockPosition();
		alienBulletPos.x += BULLET_ALIEN_HALFSPACE;
		alienBulletPos.y += ALIEN_BLOCK_HEIGHT;
		globals_setAlienBulletPosition(alienBulletPos,0x0);
		thing = globals_getAlienBulletPosition(0x0);
//		xil_printf("\r\nAlien block position after: %d %d",thing.x,thing.y);
		// draw the tank bullet
		bullets_draw_alien_bullets();
	}
}

void bullets_draw_alien_bullets() {
	point_t alienBulletPos = globals_getAlienBulletPosition(0x0);
//	xil_printf("\r\nAlien block position: %d %d",alienBulletPos.x,alienBulletPos.y);
		int xOffset,yOffset;
		int x, y;
		for(y = 0; y < BULLET_HEIGHT; y++) {
			for(x = 0; x < BULLET_WIDTH; x++) {
				if( (alien_bullet_down_3x5[y]) & (1 << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_WHITE);
				}

			}
		}
}

void bullets_erase_alien_bullet() {
	point_t alienBulletPos = globals_getAlienBulletPosition(0x0);
	int xOffset,yOffset;
//	alienFired = false;
	int x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (alien_bullet_down_3x5[y]) & (1 << x) ) {
				xOffset = x + alienBulletPos.x;
				yOffset = y + alienBulletPos.y;
				screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
			}

		}
	}
}

void bullets_update_position() {
	// Tank bullet update
	bullets_erase_tank_bullet();

	point_t tankBulletPos = globals_getTankBulletPosition();
	tankBulletPos.y -= BULLET_SPEED;
	globals_setTankBulletPosition(tankBulletPos);

	// See if the bullet hit anything, or the edge
	// <add the bunker hit>
//	xil_printf("\r\nCurrent pos: %d %d",tankBulletPos.x,tankBulletPos.y);

	if(tankBulletPos.y>BULLET_START_Y) {
		tankFired = false;
	} else {
		bullets_draw_tank_bullet();
	}

	// Alien bullets update
	bullets_erase_alien_bullet();

	point_t alienBulletPos = globals_getAlienBulletPosition(0x0);
	alienBulletPos.y += BULLET_SPEED;
	globals_setAlienBulletPosition(alienBulletPos,0x0);

	// See if the bullet hit anything, or the edge
	// <add the bunker hit>
	xil_printf("\r\nCurrent pos: %d %d",alienBulletPos.x,alienBulletPos.y);

	if((alienBulletPos.y>SCREEN_HEIGHT) & (alien_bullet_count>0)) {
		alien_bullet_count--;
		xil_printf("\r\nGoner ",alien_bullet_count);
	} else {
		bullets_draw_alien_bullets();
	}

}
