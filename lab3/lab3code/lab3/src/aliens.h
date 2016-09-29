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

void aliens_draw_initial();

void aliens_update_position();

void aliens_kill_alien(unsigned char alien);

#endif /* ALIENS_H_ */
