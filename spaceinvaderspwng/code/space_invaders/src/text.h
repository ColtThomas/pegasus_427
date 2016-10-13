/*
 * text.h
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "screen.h"
#include "globals.h"

// Used to write a char to the game screen at given coordinate
void text_write(unsigned char val, point_t coord,uint32_t color);

// Draws the scoreboard values to the top of the screen
void text_draw_score();

void text_increment_score();

void text_add_score(uint32_t points);

void text_print_saucer_score(uint32_t points);
void text_set_saucer_score(unsigned char score,uint32_t indx);
void text_game_over();
#endif /* TEXT_H_ */
