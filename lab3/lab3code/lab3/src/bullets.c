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

static point_t tankBulletPos;
static point_t alienBulletPos;

static bool tankFired = false;

void bullets_fire_tank() {
	if(!tankFired) {
		tankFired = true;
		point_t tankBulletPos;
		int xOffset,yOffset;

		tankBulletPos.x = BULLET_START_X;
		tankBulletPos.y = BULLET_START_Y;

		xil_printf("\r\nCurrent pos: %d %d",tankBulletPos.x,tankBulletPos.y);

		globals_setTankBulletPosition(tankBulletPos);

		// draw the tank bullet
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
}

void bullets_fire_aliens(){
	point_t alienBulletPos;
	unsigned char bullet;
	alienBulletPos.x = 0;
	alienBulletPos.y = 0;
	bullet = '0';
	globals_setAlienBulletPosition(alienBulletPos,bullet);

	// draw the alien bullet
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
void bullets_update_position() {
	// Tank bullet update
	bullets_erase_tank_bullet();

	point_t tankBulletPos = globals_getTankBulletPosition();
	tankBulletPos.y -= BULLET_SPEED;
	globals_setTankBulletPosition(tankBulletPos);

	// See if the bullet hit anything, or the edge
	// <add the bunker hit>
	xil_printf("\r\nCurrent pos: %d %d",tankBulletPos.x,tankBulletPos.y);

	if(tankBulletPos.y>BULLET_START_Y) {
		tankFired = false;
	} else {
		bullets_draw_tank_bullet();
	}

}
