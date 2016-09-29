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

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define TOP_ROW 11
#define MIDDLE_ROW 33
#define BOTTOM_ROW 55
#define ALIENS_PER_ROW 11
#define ALIEN_SPACING 16
#define BUFFER_WIDTH 10
#define ALIEN_BLOCK_HEIGHT 5*ALIEN_SPACING
#define ALIEN_BLOCK_WIDTH ALIENS_PER_ROW*ALIEN_SPACING
#define ALIEN_MOVEMENT 4
#define BIT_MASK 0x1
#define ALIEN_WIDTH_DIFF ALIEN_WIDTH - 1

static bool aliens_legs_in;


static const int alien_explosion_12x10[] =
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

static const int alien_top_in_12x8[] =
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

static const int alien_top_out_12x8[] =
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

static const int alien_middle_in_12x8[] =
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

static const int alien_middle_out_12x8[] =
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

static const int alien_bottom_in_12x8[] =
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

static const int alien_bottom_out_12x8[] =
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

void aliens_draw_initial() {
	int i;
	int x, y;
	point_t position;
	aliens_legs_in = BIT_MASK; // 1 for in
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		if(!globals_isDeadAlien(i)) { // alien is alive
			xil_printf("initialing alien %d\r\n",i);
			position.x = globals_getAlienBlockPosition().x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = globals_getAlienBlockPosition().y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(y = 0; y < ALIEN_HEIGHT; y++) {
				for(x = 0; x < ALIEN_WIDTH; x++) {
					if(i < TOP_ROW) {
						if(alien_top_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
//							xil_printf("print (%d,%d) at (%d,%d)\r\n",x,y,position.x+ALIEN_WIDTH_DIFF-x,y+position.y);
						}
					}
					else if (i < MIDDLE_ROW) {
						if(alien_middle_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE); // this actually draws a mirror image, so the +1 corrects for that to avoid later movement problems.
						}
					}
					else  {//if (i < BOTTOM_ROW) {
						if(alien_bottom_in_12x8[y] & (BIT_MASK << x)) {
							screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
						}
					}
				}
			}
		}
	}
}

void move_down(point_t position, point_t blockposition) {
	xil_printf("moving down\r\n");
	int x, y, i;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		xil_printf("moving down alien %d\r\n",i);
		if(!globals_isDeadAlien(i)) { //alien is alive, so it must move

			position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(y = 0; y < ALIEN_HEIGHT+ALIEN_MOVEMENT; y++) {
				for(x = 0; x < ALIEN_WIDTH; x++) {
					//		xil_printf("x = %d y = %d\r\n",x,y);
					if(i < TOP_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));

							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);

								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
								//									int i;
								//									for(i = 0; i < 100000; i++); // delay loop
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else if(i < MIDDLE_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));

							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);

								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
								//									int i;
								//									for(i = 0; i < 100000; i++); // delay loop
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));

							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);

								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
								//									int i;
								//									for(i = 0; i < 100000; i++); // delay loop
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line;
							int new_alien_line;
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
							//								xil_printf("y = %d x = %d and %x ?= %x\r\n",y,x,new_alien_line & (1<<x),old_alien_line & (1<<x));
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
				}
			}
		}

	}

}

void move_left(point_t position, point_t blockposition) {
	xil_printf("moving left\r\n");
	int x, y, i;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		if(!globals_isDeadAlien(i)) { //alien is alive, so it must move
		//	xil_printf("updating left alien %d\r\n",i);
			position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(y = 0; y < ALIEN_HEIGHT; y++) {
				for(x = 0; x < ALIEN_WIDTH+ALIEN_MOVEMENT; x++) {
					if(i < TOP_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_top_in_12x8[y];
							int new_alien_line = alien_top_out_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
				//					xil_printf("print (%d,%d) at (%d,%d)\r\n",x,y,position.x+ALIEN_WIDTH_DIFF-x,y+position.y);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
			//						xil_printf("blank (%d,%d) at (%d,%d)\r\n",x,y,position.x+ALIEN_WIDTH_DIFF-x,y+position.y);
								}
								//									int i;
								//									for(i = 0; i < 100000; i++); // delay loop
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_top_out_12x8[y];
							int new_alien_line = alien_top_in_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else if(i < MIDDLE_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_middle_in_12x8[y] ;
							int new_alien_line = alien_middle_out_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
									//										xil_printf("print x,y = %d,%d modified to %d,%d\r\n",x,y,position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
									//										xil_printf("blank x,y = %d,%d modified to %d,%d\r\n",x,y,position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
								}
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_middle_out_12x8[y] ;
							int new_alien_line = alien_middle_in_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_bottom_in_12x8[y] ;
							int new_alien_line = alien_bottom_out_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_bottom_out_12x8[y] ;
							int new_alien_line = alien_bottom_in_12x8[y] << ALIEN_MOVEMENT;
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
				}
			}
		}

	}

	;
}

void move_right(point_t position, point_t blockposition) {
	xil_printf("moving right\r\n");
	int x, y, i;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		if(!globals_isDeadAlien(i)) { //alien is alive, so it must move
//			xil_printf("updating alien %d\r\n",i);
			position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(y = 0; y < ALIEN_HEIGHT; y++) {
				for(x = 0; x < ALIEN_WIDTH+ALIEN_MOVEMENT; x++) {
					if(i < TOP_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_top_in_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_top_out_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									//screen_draw_double_pixel(position.x+ALIEN_WIDTH-x,y+position.y,SCREEN_WHITE);
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
//									xil_printf("print (%d,%d) at (%d,%d)\r\n",x,y,position.x+ALIEN_WIDTH_DIFF-x,y+position.y);

								}
								else {
									//screen_draw_double_pixel(position.x+ALIEN_WIDTH-x,y+position.y,SCREEN_BLACK);
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
//									xil_printf("blank (%d,%d) at (%d,%d)\r\n",x,y,position.x+ALIEN_WIDTH_DIFF-x,y+position.y);
								}
								//									int i;
								//									for(i = 0; i < 100000; i++); // delay loop
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_top_out_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_top_in_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else if(i < MIDDLE_ROW) {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_middle_in_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_middle_out_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
									//										xil_printf("print x,y = %d,%d modified to %d,%d\r\n",x,y,position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
									//										xil_printf("blank x,y = %d,%d modified to %d,%d\r\n",x,y,position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
								}
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_middle_out_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_middle_in_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
					else {
						if(aliens_legs_in) { // switch from legs in to legs out
							int old_alien_line = alien_bottom_in_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_bottom_out_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
						else { // switch from legs out to legs in
							int old_alien_line = alien_bottom_out_12x8[y] << ALIEN_MOVEMENT;
							int new_alien_line = alien_bottom_in_12x8[y];
							if((new_alien_line & (BIT_MASK <<x)) != (old_alien_line & (BIT_MASK <<x))) { //some changes has occured to this pixel
								if((new_alien_line) &(BIT_MASK <<x)) {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_WHITE);
								}
								else {
									screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH_DIFF-x,y+position.y,SCREEN_BLACK);
								}
							}
						}
					}
				}
			}
		}

	}
}

void aliens_update_position() {
	//static bool moved_down = true;
	static bool moving_left = false; // start moving right
	point_t blockposition = globals_getAlienBlockPosition();
	point_t position;
	if ((blockposition.x <= BUFFER_WIDTH && moving_left) || (blockposition.x >= SCREEN_WIDTH-ALIEN_BLOCK_WIDTH-BUFFER_WIDTH && !moving_left)) { // on side, needs to move down;
		//move down
		move_down(position, blockposition);
		printf("blockposition.y = %d\r\n",blockposition.y);
		blockposition.y += ALIEN_MOVEMENT;
		printf(", new y = %d\r\n",blockposition.y);
		globals_setAlienBlockPosition(blockposition);
		moving_left = !moving_left;
		//xil_printf("end move down\r\n");
	}
	else if(moving_left) {
		//move left
		xil_printf("moving left\r\n");
		move_left(position, blockposition);
		blockposition.x -= ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
		if(blockposition.x <= BUFFER_WIDTH && moving_left) xil_printf("about to move down\r\n");
		//move left
	}
	else {
		move_right(position, blockposition);
		blockposition.x += ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
		if(blockposition.x >= SCREEN_WIDTH-ALIEN_BLOCK_WIDTH-BUFFER_WIDTH) xil_printf("about to move down\r\n");
		//move right
	}
	xil_printf("resetting aliens_legs\r\n");
	aliens_legs_in = !aliens_legs_in;
}
