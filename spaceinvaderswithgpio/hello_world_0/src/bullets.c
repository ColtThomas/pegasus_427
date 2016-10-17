/*
 * bullets.c
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#include "bullets.h"
#include "screen.h"
#include "globals.h"
#include "bunkers.h"


#define BULLET_HEIGHT 5
#define BULLET_WIDTH 3

#define BULLET_SPEED 4

#define BULLET_START_X globals_getTankPosition() + 6
#define BULLET_START_Y 220 - 2*BULLET_HEIGHT

#define BULLET_VARIATIONS 4

#define BULLET_Y_BOUNDARY 10 + BULLET_HEIGHT

#define ALIEN_WIDTH GLOBALS_ALIEN_SPACING
#define ALIEN_HEIGHT GLOBALS_ALIEN_SPACING

#define TOP_ROW GLOBALS_TOP_ROW
#define MIDDLE_ROW GLOBALS_MIDDLE_ROW
#define BOTTOM_ROW GLOBALS_BOTTOM_ROW

#define ALIENS_PER_ROW GLOBALS_ALIENS_PER_ROW
#define ALIEN_SPACING GLOBALS_ALIEN_SPACING

#define BUFFER_WIDTH GLOBALS_BUFFER_WIDTH

#define ALIEN_BLOCK_HEIGHT GLOBALS_ALIEN_BLOCK_HEIGHT
#define ALIEN_BLOCK_WIDTH GLOBALS_ALIEN_BLOCK_WIDTH
#define ALIEN_MOVEMENT GLOBALS_ALIEN_MOVEMENT
#define ALIEN_ROWS GLOBALS_ALIEN_ROWS
#define ALIEN_COLUMNS GLOBALS_ALIEN_COLUMNS
#define BIT_MASK 0x1
#define TYPE_0 0
#define TYPE_1 1
#define TYPE_2 2
#define TYPE_3 3
#define BULLET_ALIEN_HALFSPACE ALIEN_WIDTH/2 - 1
#define packword3(b2,b1,b0) \
		( (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )



static const int32_t tank_bullet_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const int32_t alien_bullet_up_3x5[] =
{
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const int32_t alien_bullet_down_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0)
};

static const int32_t alien_bullet_zig_3x5[] =
{
		packword3(0,1,0),
		packword3(0,0,1),
		packword3(0,1,0),
		packword3(1,0,0),
		packword3(0,1,0)
};


static const int32_t alien_bullet_zig_inv3x5[] =
{
		packword3(0,1,0),
		packword3(1,0,0),
		packword3(0,1,0),
		packword3(0,0,1),
		packword3(0,1,0)
};

static bool tankFired = false;

// Used to see how far up we need to fire up the alien block
static uint32_t alienBulletHeightPos[ALIEN_COLUMNS] = {0,0,0,0,0,0,0,0,0,0,0};

void bullets_update_bullets_pos(uint8_t alien) { // redo this function
	uint32_t j;
	bool update = false;
	uint32_t column = alien % ALIEN_COLUMNS;
//	xil_printf("\r\nChange on column %d",column);
//	bool rowCheck = false; // This is used to make sure that we only fire from the bottom
	for(j=ALIEN_ROWS;j>0;j--) {
		if(globals_isDeadAlien(column+(j-1)*ALIEN_COLUMNS)) {
			alienBulletHeightPos[column] = ALIEN_ROWS - j + 1;
			update = true;
//			xil_printf("\r\nHeight modified column: %d height: %d",column,alienBulletHeightPos[column]);
		} else {
			break;
		}
	}

}



uint32_t bullets_get_speed() {
	return BULLET_SPEED;
}

uint32_t bullets_get_height() {
	return BULLET_HEIGHT;
}


uint32_t alien_bullet_type[BULLET_VARIATIONS] = {
		TYPE_0,TYPE_0,TYPE_0,TYPE_0
};

uint16_t bullets_randMod11() {
	return rand() % 11;	// Random number generated to add random sequence square
}

uint32_t bullets_randMod4() {
	return rand() % 4;	// Random number generated to add random sequence square
}



void bullets_fire_tank() {
	if(!tankFired & !globals_isGameOver()) {
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
	int32_t xOffset,yOffset;
//	tankFired = false;
	int32_t x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (tank_bullet_3x5[y]) & (BIT_MASK << x) ) {
				xOffset = x + tankBulletPos.x;
				yOffset = y + tankBulletPos.y;
				screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
			}

		}
	}
}

void bullets_draw_tank_bullet() {
	point_t tankBulletPos = globals_getTankBulletPosition();
	int32_t xOffset,yOffset;
	int32_t x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if( (tank_bullet_3x5[y]) & (BIT_MASK << x) ) {
				xOffset = x + tankBulletPos.x;
				yOffset = y + tankBulletPos.y;
				if(screen_double_color_pixel(xOffset,yOffset)!=SCREEN_HOTPINK) {
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_HOTPINK);
				}
			}

		}
	}
}

// Figure out what to do with the alien 0x0 stuff
void bullets_fire_aliens(){
	uint8_t i;
	bool launch=false;
	for(i=0;i<GLOBALS_NUMBER_OF_ALIEN_BULLETS;i++){
		if(!globals_getAlienBulletStatus(i) & !launch & !globals_isGameOver()) {
			launch=true;
			alien_bullet_type[i]=bullets_randMod4();
//			xil_printf("\r\nSpawn %d",i);

			// Reset global position on bullet
			point_t alienBulletPos = globals_getAlienBlockPosition();
			uint32_t alienRandColumn = bullets_randMod11();
			if(alienBulletHeightPos[alienRandColumn]<ALIEN_ROWS) {
				uint32_t alienRandHeight = alienBulletHeightPos[alienRandColumn];
				alienBulletPos.x += BULLET_ALIEN_HALFSPACE+ alienRandColumn*ALIEN_WIDTH;
				alienBulletPos.y += ALIEN_BLOCK_HEIGHT - ALIEN_HEIGHT *alienRandHeight ;
				if(!bunkers_check_hit(alienBulletPos,0))	{
					globals_setAlienBulletPosition(alienBulletPos,i);
	//				xil_printf("\r\nSpawn bullet: %d %d Column: %d Row: %d",alienBulletPos.x,alienBulletPos.y,alienRandColumn,alienRandHeight);

					// draw the alien bullet
					globals_setAlienBulletStatus(i,true);
					bullets_draw_alien_bullet(i,alien_bullet_type[i]);
				} else {
					launch=false;
				}
			}
		}
	}


}

void bullets_draw_alien_bullet(uint8_t bullet,uint32_t type) {
	point_t alienBulletPos;
	int32_t xOffset,yOffset;
	int32_t x, y;
	alienBulletPos = globals_getAlienBulletPosition(bullet);
	//		xil_printf("\r\nAlien block position: %d %d",alienBulletPos.x,alienBulletPos.y);

//	xil_printf("\r\n Drawing bullet %d",i);
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if(type==TYPE_0){
				if( (alien_bullet_down_3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					if(screen_double_color_pixel(xOffset,yOffset)!=SCREEN_HOTPINK) {
						screen_draw_double_pixel(xOffset,yOffset,SCREEN_HOTPINK);
					}
				}
			} else if (type==TYPE_1) {
				if( (alien_bullet_up_3x5[y]) & (BIT_MASK << x) ) {
									xOffset = x + alienBulletPos.x;
									yOffset = y + alienBulletPos.y;
									if(screen_double_color_pixel(xOffset,yOffset)!=SCREEN_HOTPINK) {
										screen_draw_double_pixel(xOffset,yOffset,SCREEN_HOTPINK);
									}
								}
			} else if (type==TYPE_2) {
				if( (alien_bullet_zig_inv3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					if(screen_double_color_pixel(xOffset,yOffset)!=SCREEN_HOTPINK) {
						screen_draw_double_pixel(xOffset,yOffset,SCREEN_HOTPINK);
					}
				}
			} else {
				if( (alien_bullet_zig_3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					if(screen_double_color_pixel(xOffset,yOffset)!=SCREEN_HOTPINK) {
						screen_draw_double_pixel(xOffset,yOffset,SCREEN_HOTPINK);
					}
				}
			}


		}
	}

}

void bullets_erase_alien_bullet(uint8_t bullet,uint32_t type) {
//	xil_printf("\r\n Erasing bullet %d",bullet);

	point_t alienBulletPos = globals_getAlienBulletPosition(bullet);
	int32_t xOffset,yOffset;
//	alienFired = false;
	int32_t x, y;
	for(y = 0; y < BULLET_HEIGHT; y++) {
		for(x = 0; x < BULLET_WIDTH; x++) {
			if(type==TYPE_0){
				if( (alien_bullet_down_3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
			} else if (type==TYPE_1) {
				if( (alien_bullet_up_3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
			} else if (type==TYPE_2) {
				if( (alien_bullet_zig_inv3x5[y]) & (BIT_MASK << x) ) {
					xOffset = x + alienBulletPos.x;
					yOffset = y + alienBulletPos.y;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
				}
			}else {
				if( (alien_bullet_zig_3x5[y]) & (BIT_MASK << x) ) {
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
	point_t tankBulletPos = globals_getTankBulletPosition();

	if(tankBulletPos.y>BULLET_START_Y || tankBulletPos.y < BULLET_Y_BOUNDARY) {
		tankFired = false;
		bullets_remove_tank_bullet();
	} else {
		bullets_erase_tank_bullet();

		tankBulletPos.y -= BULLET_SPEED;
		globals_setTankBulletPosition(tankBulletPos);
		bullets_draw_tank_bullet();
	}

	// Alien bullets update

	point_t alienBulletPos;
	uint8_t i;
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
//				xil_printf("\r\nGoner %d",i);
			} else {
				bullets_draw_alien_bullet(i,alien_bullet_type[i]);
			}
		}
	}
}


void bullets_remove_tank_bullet() {
	bullets_erase_tank_bullet();
	tankFired = false;
}

void bullets_remove_alien_bullet(uint8_t bullet) {
	point_t clear;
	clear.x = 0;
	clear.y = 0;

	bullets_erase_alien_bullet(bullet,alien_bullet_type[bullet]);
	globals_setAlienBulletStatus(bullet,false);
	globals_setAlienBulletPosition(clear,bullet);
}

void bullets_rotate() {
	uint32_t i;
	for(i=0;i<BULLET_VARIATIONS;i++){
		bullets_erase_alien_bullet(i,alien_bullet_type[i]);
		switch(alien_bullet_type[i]){
		case TYPE_0:
			alien_bullet_type[i] = TYPE_1;
			break;
		case TYPE_1:
			alien_bullet_type[i] = TYPE_0;
			break;
		case TYPE_2:
			alien_bullet_type[i] = TYPE_3;
			break;
		default:
			alien_bullet_type[i] = TYPE_2;
			break;
		}
		bullets_draw_alien_bullet(i,alien_bullet_type[i]);
	}
}
