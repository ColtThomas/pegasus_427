/*
 * globals.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "globals.h"
#include<stdbool.h>
#include<stdint.h>
#include <stdlib.h>
#include "core.h"
//#include "core.h"

#define INITIAL_TANK_POS 152
#define INITIAL_ALIEN_X 10
#define INITIAL_ALIEN_Y 40


static uint16_t tankPosition;
static point_t tankBulletPosition;
static point_t alienBlockPosition;
static int32_t saucerPosition;
static point_t alienBulletPositions[GLOBALS_NUMBER_OF_ALIEN_BULLETS];
static uint8_t bunkerErosionStates[GLOBALS_NUMBER_OF_BUNKERS][GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER];
static bool aliens_dead[GLOBALS_NUMBER_OF_ALIENS];
static bool alien_bullet_status[GLOBALS_NUMBER_OF_ALIEN_BULLETS];
static bool gameOver;
static bool saucerSpawned;
static uint32_t alienBlockFrontLine;
static int8_t alienBlockRowCount;
static int8_t numberOfDeadAliens;
static uint32_t * currentSoundData; // Takes the next 256 samples ready to put into the FIFO
static bool soundStatus;
//initializes globals arrays and structs
void globals_init() {
	numberOfDeadAliens = 0;
	tankPosition = INITIAL_TANK_POS;
	tankBulletPosition.x = GLOBALS_NULL_LOCATION;
	tankBulletPosition.y = GLOBALS_NULL_LOCATION;
	alienBlockPosition.x = INITIAL_ALIEN_X;
	alienBlockPosition.y = INITIAL_ALIEN_Y;
	alienBlockFrontLine = 0;
	alienBlockRowCount = GLOBALS_ALIEN_ROWS;
	saucerPosition = GLOBALS_NULL_LOCATION;
	saucerSpawned = false;
	gameOver = false;
	int32_t i,j;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIEN_BULLETS; i++) {
		alienBulletPositions[i].x = GLOBALS_NULL_LOCATION;
		alienBulletPositions[i].y = GLOBALS_NULL_LOCATION;
//		alien_bullet_status[i] = false;
	}
	for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
		for(j = 0; j < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER; j++) {
			bunkerErosionStates[i][j] = 0; //init value, uneroded
		}
	}
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		aliens_dead[i] = false;
	}
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIEN_BULLETS; i++) {
		alien_bullet_status[i] = false;
	}
}

void globals_setTankPosition(uint16_t val) {
	tankPosition = val;
//	xil_printf("tank position is %d",tankPosition);
}
uint16_t globals_getTankPosition() {
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

void globals_setAlienBlockFrontLine(uint32_t val) {
	alienBlockFrontLine = val;
}
uint32_t globals_getAlienBlockFrontLine() {

	return alienBlockFrontLine;
}
void globals_setAlienBlockRowCount(int8_t val) {
	alienBlockRowCount = val;
}

int8_t globals_getAlienBlockRowCount() {
	return alienBlockRowCount;
}


void globals_setSaucerPosition(int32_t val) {
	saucerPosition = val;
}

int32_t globals_getSaucerPosition() {
	return saucerPosition;
}

void globals_setAlienBulletPosition(point_t val, uint8_t bullet) {
	if(bullet < GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
		alienBulletPositions[bullet] = val;
	}
}
point_t globals_getAlienBulletPosition(uint8_t bullet) {
	if(bullet < GLOBALS_NUMBER_OF_ALIEN_BULLETS) {
		return alienBulletPositions[bullet];
	}
	point_t null_point;
	null_point.x = GLOBALS_NULL_LOCATION; // this will actually be a very large number, since these are unsigned chars.
	null_point.y = GLOBALS_NULL_LOCATION; // this ensures that it's a large enough value to be off-screen
	return null_point;
}

void globals_setBunkerErosionState(uint8_t state, uint8_t bunker, uint8_t block) {
	if(bunker < GLOBALS_NUMBER_OF_BUNKERS && block < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER) {
		bunkerErosionStates[bunker][block] = state;
	}
}

uint8_t globals_getBunkerErosionState(uint8_t bunker, uint8_t block) {
	if(bunker < GLOBALS_NUMBER_OF_BUNKERS && block < GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER) {
		return bunkerErosionStates[bunker][block];
	}
	else return GLOBALS_NULL_LOCATION; // very large, thus improper, number, to indicate error.
}

void globals_killAlien(uint8_t alien) {
	if(alien < GLOBALS_NUMBER_OF_ALIENS) {
		aliens_dead[alien] = true;
		numberOfDeadAliens++;
	}
}

bool globals_isDeadAlien(uint8_t alien) {
	if(alien < GLOBALS_NUMBER_OF_ALIENS) return aliens_dead[alien];
	else return true;
}

void globals_setAlienBulletStatus(uint8_t bullet,bool status) {
	alien_bullet_status[bullet] = status;
}

bool globals_getAlienBulletStatus(uint8_t bullet) {
	return alien_bullet_status[bullet];
}

void globals_setGameStatus(bool status) {
	gameOver = status;
	core_end_game();
}

bool globals_isGameOver() {
	return gameOver;
}

void globals_toggleSaucer() {
	saucerSpawned = !saucerSpawned;
}

bool globals_saucerSpawned() {
	return saucerSpawned;
}

bool globals_allAliensDead() {
	return numberOfDeadAliens == GLOBALS_NUMBER_OF_ALIENS;
}

void globals_setNextSoundSamples(uint32_t * data) {
	currentSoundData = data;
}

uint32_t * globals_getNextSoundSamples() {
	return currentSoundData;
}

bool globals_getSoundStatus() {
	return soundStatus;
}
void globals_setSoundStatus(bool status){
	soundStatus = status;
}
