/*
 * globals.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "globals.h"
#include<stdbool.h>
#define INITIAL_TANK_POS 152

static unsigned short tankPosition;
static point_t tankBulletPosition;
static point_t alienBlockPosition;
static point_t alienBulletPositions[GLOBALS_NUMBER_OF_ALIEN_BULLETS];
static unsigned char bunkerErosionStates[GLOBALS_NUMBER_OF_BUNKERS][GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER];
static bool aliens_dead[GLOBALS_NUMBER_OF_ALIENS];

//initializes globals arrays and structs
void globals_init() {
	tankPosition = INITIAL_TANK_POS;
	tankBulletPosition.x = -1;
	tankBulletPosition.y = -1;
	alienBlockPosition.x = -1;
	alienBlockPosition.y = -1;
	int i,j;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIEN_BULLETS; i++) {
		alienBulletPositions[i].x = -1;
		alienBulletPositions[i].y = -1;
	}
	for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
		for(j = 0; j < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER; j++) {
			bunkerErosionStates[i][j] = 0; //init value, uneroded
		}
	}
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		aliens_dead[i] = false;
	}
}

void globals_setTankPosition(unsigned short val) {
	tankPosition = val;
//	xil_printf("tank position is %d",tankPosition);
}
unsigned short globals_getTankPosition() {
	return tankPosition;
}

void globals_setTankBulletPosition(point_t val) {
	tankBulletPosition = val;
}
point_t globals_getTankBulletPosition() {
	return tankBulletPosition;
}

void globals_setAlienBlockPosition(point_t val) {
	alienBlockPosition = val;
}
point_t globals_getAlienBlockPosition() {
	return alienBlockPosition;
}

void globals_setAlienBulletPosition(point_t val, unsigned char bullet) {
	if(bullet < GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
		alienBulletPositions[bullet] = val;
	}
}
point_t globals_getAlienBulletPosition(unsigned char bullet) {
	if(bullet < GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
		return alienBulletPositions[bullet];
	}
	point_t null_point;
	null_point.x = -1; // this will actually be a very large number, since these are unsigned chars.
	null_point.y = -1; // this ensures that it's a large enough value to be off-screen
	return null_point;
}

void globals_setBunkerErosionState(unsigned char state, unsigned char bunker, unsigned char block) {
	if(bunker < GLOBALS_NUMBER_OF_BUNKERS && block < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER) {
		bunkerErosionStates[bunker][block] = state;
	}
}

unsigned char globals_getBunkerErosionState(unsigned char bunker, unsigned char block) {
	if(bunker < GLOBALS_NUMBER_OF_BUNKERS && block < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER) {
		return bunkerErosionStates[bunker][block];
	}
	else return -1; // very large, thus improper, number, to indicate error.
}
