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

#define SAUCER_Y 15

void saucer_update();
void saucer_spawn();
bool saucer_check_hit(point_t pos);
bool saucer_is_spawned();
#endif /* SAUCER_H_ */
