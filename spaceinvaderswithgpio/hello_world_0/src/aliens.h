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

// returns true if an alien has been hit by a bullet.
// also does a lot of the processing on that alien, including killing it.
bool aliens_check_hit(point_t pos);

// returns true if the aliens reach the bottom of the screen
bool aliens_landed(); // Find a place for this

// returns true if any alien is in the process of dying (i.e. his explosion is being animated)
bool aliens_is_dying();
// draw the next step in the explosion animation
void aliens_update_death();

#endif /* ALIENS_H_ */
