/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "screen.h"
#include <stdbool.h>
#include<stdio.h>
#include<stdint.h>

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define ALIEN_WIDTH GLOBALS_ALIEN_WIDTH
#define ALIEN_HEIGHT GLOBALS_ALIEN_HEIGHT
#define TOP_ROW GLOBALS_TOP_ROW
#define MIDDLE_ROW GLOBALS_MIDDLE_ROW
#define BOTTOM_ROW GLOBALS_BOTTOM_ROW
#define ALIENS_PER_ROW GLOBALS_ALIENS_PER_ROW
#define ALIEN_SPACING GLOBALS_ALIEN_SPACING
#define BUFFER_WIDTH GLOBALS_BUFFER_WIDTH
#define ALIEN_BLOCK_HEIGHT GLOBALS_ALIEN_BLOCK_HEIGHT
#define ALIEN_BLOCK_WIDTH GLOBALS_ALIEN_BLOCK_WIDTH
#define ALIEN_MOVEMENT GLOBALS_ALIEN_MOVEMENT
#define ALIEN_ROWS GLOBALS_ALIEN_ROWS
#define ALIEN_COLUMNS GLOBALS_ALIEN_COLUMNS
#define LEGS_IN 1
#define BIT_MASK 0x1
#define X_OFFSET 1
#define COLUMN_OFFSET 1

#define ALIEN_LAND_POINT 130 // Y coordinate indicating
#define COLUMN_0 0
#define COLUMN_10 10

static bool aliens_legs_in;

static const int32_t alien_explosion_12x10[] =
{
		packword12(0,0,0,0,0,0,1,0,0,0,0,0),
		packword12(0,0,0,1,0,0,1,0,0,0,1,0),
		packword12(1,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,1,0,0,1,0,0,0,1,0,0,0),
		packword12(0,0,0,0,0,0,0,0,0,0,1,1),
		packword12(1,1,0,0,0,0,0,0,0,0,0,0),
		packword12(0,0,0,1,0,0,0,1,0,0,1,0),
		packword12(0,0,1,0,0,0,0,0,1,0,0,1),
		packword12(0,1,0,0,0,1,0,0,1,0,0,0),
		packword12(0,0,0,0,0,1,0,0,0,0,0,0)
};

static const int32_t alien_top_in_12x8[] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,0,0,0,0,0,1,0,0),
		packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};

static const int32_t alien_top_out_12x8[] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,0,1,0,0,1,0,0,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

static const int32_t alien_middle_in_12x8[] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,0,0,1,0,0,0,1,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,1,0,0),
		packword12(0,0,1,1,0,1,1,1,0,1,1,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,0,1,0,0,0,0,0,1,0,1),
		packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};

static const int32_t alien_middle_out_12x8[] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,1,0,0,1,0,0,0,1,0,0,1),
		packword12(0,1,0,1,1,1,1,1,1,1,0,1),
		packword12(0,1,1,1,0,1,1,1,0,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,1,1,1,1,1,1,0),
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};

static const int32_t alien_bottom_in_12x8[] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,0,0,1,1,1,0,0),
		packword12(0,1,1,0,0,1,1,0,0,1,1,0),
		packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};

static const int32_t alien_bottom_out_12x8[] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,0,1,1,0,0,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

// draws the aliens in their initial positions on the board.
void aliens_draw_initial() {
	int32_t i;
	int32_t x, y;
	point_t position;
	aliens_legs_in = LEGS_IN; // begin in legs-in guise
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		if(!globals_isDeadAlien(i)) { // alien is alive
			// get position of each alien. alienBlockPosition() gives top-left corner of alien block;
			// offset for each alien is then calculated based on the alien's number.
			position.x = globals_getAlienBlockPosition().x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = globals_getAlienBlockPosition().y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(x = 0; x < ALIEN_WIDTH; x++) {
				for(y = 0; y < ALIEN_HEIGHT; y++) {
					if(i < TOP_ROW) { // alien is in top row
						if(alien_top_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_WHITE);
						}
					}
					else if (i < MIDDLE_ROW) { // alien is in middle row
						if(alien_middle_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(x+position.x+X_OFFSET,y+position.y,SCREEN_WHITE); // this actually draws a mirror image, so the +1 corrects for that to avoid later movement problems.
						}
					}
					else  {// alien is in bottom row
						if(alien_bottom_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_WHITE);
						}
					}
				}
			}
		}
	}
}

// checks to see if all aliens in the given column are dead.
bool column_dead(int8_t column) {
	bool all_aliens_dead = true; // be optimistic
	int32_t i;
	for(i = 0; i < ALIEN_ROWS; i++) {
		if(!globals_isDeadAlien(column + i*ALIENS_PER_ROW)) {
			all_aliens_dead = false;
			break;
		}
	}
	return all_aliens_dead;
}

// changes the point to be checked for stopping alien movement on the left,
// based on whether the left column(s) of aliens are dead.
void move_left_border(int8_t column, int32_t *left_border) {
	if(!column_dead(column) || column > ALIEN_COLUMNS) return;
	else {
		*left_border -= ALIEN_SPACING;
		move_left_border(column+COLUMN_OFFSET, left_border);
	}
}

// changes the point to be checked for stopping alien movement on the right,
// based on whether the right column(s) of aliens are dead.
void move_right_border(int8_t column, int32_t *right_border) {
	if(!column_dead(column) || column < COLUMN_0) return;
	else {
		*right_border += ALIEN_SPACING;
		move_right_border(column-COLUMN_OFFSET, right_border);
	}
}

// moves all the aliens, as a block.
// initially moves them all to the right;
// when the reach the right edge, moves them down;
// then moves them left;
// then down, then right, etc.
void aliens_update_position() {
	static bool moving_left = false; // begin by moving right
	point_t blockposition = globals_getAlienBlockPosition(); // get current position
	point_t position; // temp to hold new position
	int32_t left_border = BUFFER_WIDTH; // initial left boundary line
	int32_t right_border = SCREEN_WIDTH-ALIEN_BLOCK_WIDTH-BUFFER_WIDTH; // initial right boundary line
	move_left_border(COLUMN_0, &left_border); // adjust left boundary if needed
	move_right_border(COLUMN_10, &right_border); // adjust left boundary if needed

	if ((blockposition.x <= left_border && moving_left) || (blockposition.x >= right_border && !moving_left)) { // on side, needs to move down;
		//move down
		int32_t x, y, i;
		for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
			if(!globals_isDeadAlien(i)) { //alien is alive, so it must move; if alien is dead, moving would redraw it, so we do nothing.
				position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
				position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
				for(y = 0; y < ALIEN_HEIGHT+ALIEN_MOVEMENT; y++) {
					for(x = 0; x < ALIEN_WIDTH; x++) {
						if(i < TOP_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_top_in_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_top_in_12x8[y];
									new_alien_line = alien_top_out_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_top_out_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some change has occurred to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) { // this is now a 1, so draw it
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);

									}
									else { // this is now a 0, so blank it
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_top_out_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_top_out_12x8[y];
									new_alien_line = alien_top_in_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_top_in_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else if(i < MIDDLE_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_middle_in_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_middle_in_12x8[y];
									new_alien_line = alien_middle_out_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_middle_out_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_middle_out_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_middle_out_12x8[y];
									new_alien_line = alien_middle_in_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_middle_in_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_bottom_in_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_bottom_in_12x8[y];
									new_alien_line = alien_bottom_out_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_bottom_out_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line;
								int32_t new_alien_line;
								if(y < ALIEN_MOVEMENT) {
									old_alien_line = alien_bottom_out_12x8[y];
									new_alien_line = 0;
								}
								else if(y < ALIEN_HEIGHT) {
									old_alien_line = alien_bottom_out_12x8[y];
									new_alien_line = alien_bottom_in_12x8[y-ALIEN_MOVEMENT];
								}
								else { // y > ALIEN_HEIGHT
									old_alien_line = 0;
									new_alien_line = alien_bottom_in_12x8[y-ALIEN_MOVEMENT];
								}
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
					}
				}
			}
		}
		blockposition.y += ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
		moving_left = !moving_left;
	}
	else if(moving_left) {
		int32_t x, y, i;
		for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
			if(!globals_isDeadAlien(i)) { //alien is alive, so it must move
				position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
				position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
				for(y = 0; y < ALIEN_HEIGHT; y++) {
					for(x = 0; x < ALIEN_WIDTH+ALIEN_MOVEMENT; x++) {
						if(i < TOP_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_top_in_12x8[y];
								int32_t new_alien_line = alien_top_out_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_top_out_12x8[y];
								int32_t new_alien_line = alien_top_in_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else if(i < MIDDLE_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_middle_in_12x8[y] ;
								int32_t new_alien_line = alien_middle_out_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_middle_out_12x8[y] ;
								int32_t new_alien_line = alien_middle_in_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_bottom_in_12x8[y] ;
								int32_t new_alien_line = alien_bottom_out_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_bottom_out_12x8[y] ;
								int32_t new_alien_line = alien_bottom_in_12x8[y] << ALIEN_MOVEMENT;
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
					}
				}
			}
		}
		blockposition.x -= ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
	}
	else {
		int32_t x, y, i;
		for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
			if(!globals_isDeadAlien(i)) { //alien is alive, so it must move
				position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
				position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
				for(y = 0; y < ALIEN_HEIGHT; y++) {
					for(x = 0; x < ALIEN_WIDTH+ALIEN_MOVEMENT; x++) {
						if(i < TOP_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_top_in_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_top_out_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_top_out_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_top_in_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else if(i < MIDDLE_ROW) {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_middle_in_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_middle_out_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_middle_out_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_middle_in_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						else {
							if(aliens_legs_in) { // switch from legs in to legs out
								int32_t old_alien_line = alien_bottom_in_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_bottom_out_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
							else { // switch from legs out to legs in
								int32_t old_alien_line = alien_bottom_out_12x8[y] << ALIEN_MOVEMENT;
								int32_t new_alien_line = alien_bottom_in_12x8[y];
								if((new_alien_line & (BIT_MASK<<x)) != (old_alien_line & (BIT_MASK<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(BIT_MASK<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-X_OFFSET-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
					}
				}
			}
		}
		blockposition.x += ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
	}
	aliens_legs_in = !aliens_legs_in;
}

// kill and blank the given alien
void aliens_kill_alien(uint8_t alien) {
	globals_killAlien(alien); // kills the alien in the globals.
	// but... we still have to undraw him.
	point_t position;
	int32_t x, y;
	position.x = globals_getAlienBlockPosition().x + ALIEN_SPACING*(alien%ALIENS_PER_ROW);
	position.y = globals_getAlienBlockPosition().y + ALIEN_SPACING*(alien/ALIENS_PER_ROW);
	for(x = 0; x < ALIEN_WIDTH; x++) {
		for(y = 0; y < ALIEN_HEIGHT; y++) {
			if(alien < TOP_ROW && aliens_legs_in) {
				if(alien_top_in_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_BLACK);
				}
			}
			else if (alien < MIDDLE_ROW && aliens_legs_in) {
				if(alien_middle_in_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x+X_OFFSET,y+position.y,SCREEN_BLACK); // this actually draws a mirror image, so the +1 corrects for that to avoid later movement problems.
				}
			}
			else if (alien < BOTTOM_ROW && aliens_legs_in) {
				if(alien_bottom_in_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_BLACK);
				}
			}
			if(alien < TOP_ROW && !aliens_legs_in) {
				if(alien_top_out_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_BLACK);
				}
			}
			else if (alien < MIDDLE_ROW && !aliens_legs_in) {
				if(alien_middle_out_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x+X_OFFSET,y+position.y,SCREEN_BLACK); // this actually draws a mirror image, so the +1 corrects for that to avoid later movement problems.
				}
			}
			else if (alien < BOTTOM_ROW && !aliens_legs_in) {
				if(alien_bottom_out_12x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_BLACK);
				}
			}
		}
	}
}

// Returns the alien that the point corresponds to
bool aliens_get_alien_select(point_t currentPos) { // change uint8_t
	point_t blockPos = globals_getAlienBlockPosition();

	point_t relativePos;
	relativePos.x = currentPos.x-blockPos.x;
	relativePos.y = currentPos.y-blockPos.y;

	// Remember that the bullet has to go though spaces
	// Now you have relative point > , <=
	uint32_t i;
	uint8_t row = 0;
	bool hit = false;
	// Check the row
	for(i=1; i <= GLOBALS_ALIEN_ROWS; i++) {
		if((relativePos.y>=GLOBALS_ALIEN_SPACING*i) &&(relativePos.y<GLOBALS_ALIEN_SPACING*i+GLOBALS_ALIEN_HEIGHT)) {
			row = (i)*GLOBALS_ALIENS_PER_ROW;
			hit=true;
			xil_printf("\r\nYAY");
		}
	}
	if(!hit) {return hit;}
	else{hit = false;}
	xil_printf("\r\nRow %d hit",row);
	// Check the column
	uint8_t column = 0;
	for(i=1; i <= GLOBALS_ALIEN_COLUMNS ; i++) {
		if((relativePos.x>=GLOBALS_ALIEN_SPACING*i) &&(relativePos.x<GLOBALS_ALIEN_SPACING*i+GLOBALS_ALIEN_WIDTH)) {
			column = i;
			hit = true;
		}
	}

	if(!hit) {return hit;}
	xil_printf("\r\nColumn %d hit",column);
	xil_printf("\r\nAlien number: %d",column+row);
	uint8_t alien = column+row;
	if(hit && !globals_isDeadAlien(alien)) {
		aliens_kill_alien(alien);
	} else {
		hit = false;
	}

	return hit;
}

bool aliens_check_hit(point_t pos){
	point_t alienPos = globals_getAlienBlockPosition();
	if((pos.x>=alienPos.x) && (pos.x<=alienPos.x+GLOBALS_ALIEN_BLOCK_WIDTH)) {
		if((pos.y>=alienPos.y) && (pos.y<=alienPos.y+GLOBALS_ALIEN_BLOCK_HEIGHT)) {
			xil_printf("\r\nBullet Hit");

			// Figure which alien box it is in
			return aliens_get_alien_select(pos);
		}
	}
	return false;
}

bool aliens_landed() {
	point_t pos = globals_getAlienBlockPosition();
	if(pos.y>=ALIEN_LAND_POINT){
		xil_printf("\r\nGAME OVER");
		globals_setGameStatus(true);
		return true;
	} else {
		return false;
	}
}

