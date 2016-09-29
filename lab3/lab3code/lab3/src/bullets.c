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


void bullets_fire_tank() {
	point_t tankBulletPos;
	tankBulletPos.x = globals_getTankPosition();
	tankBulletPos.y = 0;
	globals_setTankBulletPosition(tankBulletPos);
	// draw the tank bullet
	int x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (tank_bullet_3x5[y]) & (1 << x) ) {
				screen_draw_double_pixel(x,y,SCREEN_WHITE);
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

