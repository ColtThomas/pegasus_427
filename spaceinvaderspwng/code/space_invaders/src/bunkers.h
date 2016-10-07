/*
 * bunkers.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_



#include "screen.h"
#include "globals.h"

// Initializes the bunkers
void bunkers_draw_initial();

// Verifies if the coordinates are located on the bunker; returns 
// true for a collision
bool bunkers_check_hit(point_t pos);

// Checks the bunker damage and updates the bunker status
void bunkers_update();

// Applies one unit of damage to given quadrant of any bunker
void bunker_damage(int32_t bunkerNum, int32_t quadrant);

#endif /* BUNKERS_H_ */
