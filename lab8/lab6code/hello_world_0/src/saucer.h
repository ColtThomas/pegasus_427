/*
 * saucer.h
 *
 *  Created on: Oct 13, 2016
 *      Author: superman
 */

#ifndef SAUCER_H_
#define SAUCER_H_

#include "screen.h"
#include "globals.h"
#include <stdint.h>
#include <stdbool.h>

#define SAUCER_Y 15

// update the saucer's position, if it is spawned
void saucer_update();

// make a new saucer, if it does not already exist
void saucer_spawn();

// checks to see if the saucer has been hit, and processes saucer's death
bool saucer_check_hit(point_t pos);

// returns true if saucer is on screen
bool saucer_is_spawned();
#endif /* SAUCER_H_ */
