/*
 * globals.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include<stdbool.h>
#include<stdint.h>

#define GLOBALS_NUMBER_OF_ALIEN_BULLETS 4
#define GLOBALS_NUMBER_OF_BUNKERS 4
#define GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER 9
#define GLOBALS_NUMBER_OF_ALIENS 55
#define GLOBALS_ALIEN_WIDTH 12
#define GLOBALS_ALIEN_HEIGHT 8
#define GLOBALS_TOP_ROW 11
#define GLOBALS_MIDDLE_ROW 33
#define GLOBALS_BOTTOM_ROW 55
#define GLOBALS_ALIENS_PER_ROW 11
#define GLOBALS_ALIEN_SPACING 16
#define GLOBALS_BUFFER_WIDTH 10
#define GLOBALS_ALIEN_BLOCK_HEIGHT 5*GLOBALS_ALIEN_SPACING
#define GLOBALS_ALIEN_BLOCK_WIDTH GLOBALS_ALIENS_PER_ROW*GLOBALS_ALIEN_SPACING
#define GLOBALS_ALIEN_MOVEMENT 4
#define GLOBALS_ALIEN_ROWS 5
#define GLOBALS_ALIEN_COLUMNS 11

// struct to hold x and y coordinates.
// All globals functions that return point_t, it represents the top-left corner of the item.
typedef struct {int16_t x; int16_t y;} point_t;

// sets up initial values for several globals
void globals_init();

// set and get the position of the tank
void globals_setTankPosition(uint16_t val);
unsigned short globals_getTankPosition();

// set and get the position of the tank bullet
void globals_setTankBulletPosition(point_t val);
point_t globals_getTankBulletPosition();

// set and get the position of the block of aliens
void globals_setAlienBlockPosition(point_t val);
point_t globals_getAlienBlockPosition();

// set and get the position of the specified alien bullet
void globals_setAlienBulletPosition(point_t val, uint8_t bullet);
point_t globals_getAlienBulletPosition(uint8_t bullet);

// set and get the erosion state of the specified bunker (0-3) and block (0-9)
void globals_setBunkerErosionState(uint8_t state, uint8_t bunker, uint8_t block);
uint8_t globals_getBunkerErosionState(uint8_t bunker, uint8_t block);

// mark the specified alien as dead.
// does not undraw! use aliens_killAlien for that
void globals_killAlien(uint8_t alien);

bool globals_isDeadAlien(uint8_t alien);

void globals_setAlienBulletStatus(uint8_t bullet,bool status);
bool globals_getAlienBulletStatus(uint8_t bullet);

#endif /* GLOBALS_H_ */
