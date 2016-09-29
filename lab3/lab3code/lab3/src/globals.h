/*
 * globals.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include<stdbool.h>

#define GLOBALS_NUMBER_OF_ALIEN_BULLETS 4
#define GLOBALS_NUMBER_OF_BUNKERS 4
#define GLOBALS_NUMBER_OF_BLOCKS_PER_BUNKER 9
#define GLOBALS_NUMBER_OF_ALIENS 55
typedef struct {unsigned short x; unsigned short y;} point_t;

void globals_init();

void globals_setTankPosition(unsigned short val);
unsigned short globals_getTankPosition();

void globals_setTankBulletPosition(point_t val);
point_t globals_getTankBulletPosition();

void globals_setAlienBlockPosition(point_t val);
point_t globals_getAlienBlockPosition();

void globals_setAlienBulletPosition(point_t val, unsigned char bullet);
point_t globals_getAlienBulletPosition(unsigned char bullet);

void globals_setBunkerErosionState(unsigned char state, unsigned char bunker, unsigned char block);
unsigned char globals_getBunkerErosionState(unsigned char bunker, unsigned char block);

void globals_killAlien(unsigned char alien);

bool globals_isDeadAlien(unsigned char alien);

void globals_setAlienBulletStatus(unsigned char bullet,bool status);
bool globals_getAlienBulletStatus(unsigned char bullet);

#endif /* GLOBALS_H_ */

/*tank position (x, y)
tank bullet position (x, y)
alien block position (x, y)
alien bullet-0 position (x, y, bullet_type)
alien bullet-1 position (x, y, bullet_type)
alien bullet-2 position (x, y, bullet_type)
alien bullet-3 position (x, y, bullet_type)
bunker 0 erosion state (right-most)
bunker 1 erosion state
bunker 2 erosion state
bunker 3 erosion state (left-most)
*/
