/*
 * aliens.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef ALIENS_H_
#define ALIENS_H_

#include "screen.h"
#include "globals.h"
#include <stdint.h>

// draws the aliens in their initial positions on the board.
void aliens_draw_initial();

// moves all the aliens, as a block.
// initially moves them all to the right;
// when the reach the right edge, moves them down;
// then moves them left;
// then down, then right, etc.
void aliens_update_position();

// kill and blank the given alien
void aliens_kill_alien(uint8_t alien);

bool aliens_check_hit(point_t pos);

bool aliens_landed(); // Find a place for this

bool aliens_is_dying();
void aliens_update_death();

#endif /* ALIENS_H_ */
