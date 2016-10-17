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

// Adds points to the score and redraws.
void text_add_score(uint32_t points);

// erases or prints saucer score value given in begin_saucer_score to the location where the saucer was when hit.
// if erase, also resets saucer_score_begun.
void text_print_saucer_score(bool erase);
// calls print_saucer_score and sets saucer_score_begun.
void text_begin_saucer_score(uint32_t points);
// if true, saucer score should be displayed on screen. set by begin_saucer_score, reset by print_saucer_score(true)
bool text_saucer_score_begun();

// prints losing message "WASTED" to screen
void text_game_over();
// prints winning message "AWESOME" to screen
void text_game_over_win();

#endif /* TEXT_H_ */
