/*
 * bullets.c
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#include "bullets.h"
#include "screen.h"
#include "globals.h"


#define BULLET_HEIGHT 5
#define BULLET_WIDTH 3
#define BULLET_SPEED 4
#define BULLET_START_X globals_getTankPosition() + 6
#define BULLET_START_Y 220 - 2*BULLET_HEIGHT
#define BULLET_VARIATIONS 3
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

static bool bullet_alien_status[GLOBALS_NUMBER_OF_ALIEN_BULLETS];

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

static const int alien_bullet_zig_3x5[] =
{
		packword3(1,0,0),
		packword3(0,1,0),
		packword3(0,0,1),
		packword3(0,1,0),
		packword3(1,0,0)
};
static point_t tankBulletPos;
static point_t alienBulletPos;

static bool tankFired = false;
static bool alien_bullet_status[GLOBALS_NUMBER_OF_ALIEN_BULLETS];
static u8 alien_bullet_count = 0;
unsigned int alien_bullet_type[BULLET_VARIATIONS] = {
		0,0,0
};

u16 bullets_randMod11() {
	return rand() % 11;	// Random number generated to add random sequence square
}

unsigned int bullets_randMod3() {
	return rand() % 3;	// Random number generated to add random sequence square
}

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
	unsigned char i;
	bool launch=false;
	for(i=0;i<GLOBALS_NUMBER_OF_ALIEN_BULLETS;i++){
		if(!globals_getAlienBulletStatus(i) & !launch) {
			launch=true;
			alien_bullet_type[i]=bullets_randMod3();
			xil_printf("\r\nSpawn %d",i);

			// Reset global position on bullet
			point_t alienBulletPos = globals_getAlienBlockPosition();
			alienBulletPos.x += BULLET_ALIEN_HALFSPACE+ bullets_randMod11()*ALIEN_WIDTH;
			alienBulletPos.y += ALIEN_BLOCK_HEIGHT;
			globals_setAlienBulletPosition(alienBulletPos,i);


			// draw the tank bullet
			globals_setAlienBulletStatus(i,true);
			bullets_draw_alien_bullet(i,alien_bullet_type[i]);
		}
	}

	// Indicate that a single bullet was fired
//	if(alien_bullet_count<GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
//		alien_bullet_count++;
//	}
}

void bullets_draw_alien_bullet(unsigned char bullet,unsigned int type) {
	point_t alienBulletPos;
	int xOffset,yOffset;
	int x, y, i;
	alienBulletPos = globals_getAlienBulletPosition(bullet);
	//		xil_printf("\r\nAlien block position: %d %d",alienBulletPos.x,alienBulletPos.y);

//	xil_printf("\r\n Drawing bullet %d",i);
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if(type==0){
				if( (alien_bullet_down_3x5[y]) & (1 << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_WHITE);
				}
			} else if (type==1) {
				if( (alien_bullet_up_3x5[y]) & (1 << x) ) {
									xOffset = x + alienBulletPos.x;
									yOffset = y + alienBulletPos.y;
									screen_draw_double_pixel(xOffset,yOffset,SCREEN_WHITE);
								}
			} else {
				if( (alien_bullet_zig_3x5[y]) & (1 << x) ) {
									xOffset = x + alienBulletPos.x;
									yOffset = y + alienBulletPos.y;
									screen_draw_double_pixel(xOffset,yOffset,SCREEN_WHITE);
								}
			}


		}
	}

}

void bullets_erase_alien_bullet(unsigned char bullet,unsigned int type) {
//	xil_printf("\r\n Erasing bullet %d",bullet);

	point_t alienBulletPos = globals_getAlienBulletPosition(bullet);
	int xOffset,yOffset;
//	alienFired = false;
	int x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if(type==0){
				if( (alien_bullet_down_3x5[y]) & (1 << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
			} else if (type==1) {
				if( (alien_bullet_up_3x5[y]) & (1 << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
			} else {
				if( (alien_bullet_zig_3x5[y]) & (1 << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
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

	point_t alienBulletPos;
	unsigned char i;
	for(i=0;i<GLOBALS_NUMBER_OF_ALIEN_BULLETS; i++) {
		if(globals_getAlienBulletStatus(i)) {
			bullets_erase_alien_bullet(i,alien_bullet_type[i]);
			alienBulletPos = globals_getAlienBulletPosition(i);
			alienBulletPos.y += BULLET_SPEED;
			globals_setAlienBulletPosition(alienBulletPos,i);

			// See if the bullet hit anything, or the edge
			// <add the bunker hit>
	//		xil_printf("\r\nCurrent pos: %d %d",alienBulletPos.x,alienBulletPos.y);

			if((alienBulletPos.y>SCREEN_HEIGHT) & globals_getAlienBulletStatus(i)) {
//				alien_bullet_count--;
				globals_setAlienBulletStatus(i,false);
				xil_printf("\r\nGoner %d",i);
			} else {
				bullets_draw_alien_bullet(i,alien_bullet_type[i]);
			}
		}
	}
}
