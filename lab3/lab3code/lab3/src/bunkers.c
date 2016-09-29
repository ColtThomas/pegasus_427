/*
 * bunkers.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#include "bunkers.h"
#include "screen.h"
#include <stdbool.h>
#include <stdio.h>

#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b23 << 23)|(b22<< 22)|(b21<< 21)|(b20<< 20)|(b19<< 19)|(b18<< 18)|(b17 << 17)|(b16<< 16)|(b15<< 15)|(b14<< 14)|(b13<< 13)|(b12<< 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword6(b5,b4,b3,b2,b1,b0) \
		((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

#define BUNKER_MARGIN_BOTTOM 60
#define BUNKER_MARGIN_RIGHT 60
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_SPACING 45

// Boundary definitions for hit indication
#define BUNKER_LOWER_BOUND SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM
#define BUNKER_UPPER_BOUND SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM - BUNKER_HEIGHT
#define BUNKER_ONE_LEFT_BOUND BUNKER_SPACING
#define BUNKER_ONE_RIGHT_BOUND BUNKER_SPACING + BUNKER_WIDTH
#define BUNKER_TWO_LEFT_BOUND BUNKER_SPACING*2 + BUNKER_WIDTH
#define BUNKER_TWO_RIGHT_BOUND BUNKER_SPACING*2 + BUNKER_WIDTH*2
#define BUNKER_THREE_LEFT_BOUND BUNKER_SPACING*3 + BUNKER_WIDTH*2
#define BUNKER_THREE_RIGHT_BOUND BUNKER_SPACING*3 + BUNKER_WIDTH*3
#define BUNKER_FOUR_LEFT_BOUND BUNKER_SPACING*4 + BUNKER_WIDTH*3
#define BUNKER_FOUR_RIGHT_BOUND BUNKER_SPACING*4 + BUNKER_WIDTH*4

#define BUNKER_QUAD_WIDTH BUNKER_WIDTH/3
#define BUNKER_QUAD_HEIGHT BUNKER_HEIGHT/3
#define BUNKER_QUAD_X_1 0
#define BUNKER_QUAD_X_2 BUNKER_QUAD_WIDTH
#define BUNKER_QUAD_X_3 BUNKER_QUAD_WIDTH*2
#define BUNKER_QUAD_Y_1 0
#define BUNKER_QUAD_Y_2 BUNKER_QUAD_HEIGHT
#define BUNKER_QUAD_Y_3 BUNKER_QUAD_HEIGHT*2

// Shape of the entire bunker.
static const int bunker_24x18[] =
{
packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const int bunkerDamage0_6x6[] = {
	packword6(0,1,1,0,0,0),
	packword6(0,0,0,0,0,1),
	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,0,0,0,1,0)
};

static const int bunkerDamage1_6x6[] = {
	packword6(1,1,1,0,1,0),
	packword6(1,0,1,0,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(0,1,1,1,0,1),
	packword6(0,1,1,0,1,0)
};

static const int bunkerDamage2_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,0,1,1,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),
	packword6(0,1,1,1,0,1),
	packword6(1,1,1,1,1,1)
};

static const int bunkerDamage3_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1)
};


u32 bunkerStatus[4][9] = { // global values to change
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1}
};

void bunkers_draw_initial() {
	int i;
	int x, y;
	int xOffset;
	int yOffset;
	for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
		for(x = 0; x < BUNKER_WIDTH; x++) {
			for(y = 0; y < BUNKER_HEIGHT; y++) {
				if(bunker_24x18[y] & (1 << x)) {
					xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
					yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
				}
			}
		}
	}

}

// Might want to consider splitting this to update a specific bunker to save render time
void bunkers_update() {
	int i,j;
		int x, y;
		int xOffset,yOffset,xQuad,yQuad,quadIndex,quadDamage = 0; // lml..O,O..lml

		for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
			xil_printf("\r\nBunker %d",i);
			for(y = 0; y < BUNKER_HEIGHT; y++) {
				for(x = 0; x < BUNKER_WIDTH; x++) {
					// Figure out how to distinguish a quadrant
					xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
					yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;

					// Used to determine if coordinate is in a quadrant
					//BUNKER_QUAD_WIDTH BUNKER_QUAD_HEIGHT
					xQuad = x;// + i*BUNKER_WIDTH - (i)*BUNKER_SPACING;
					yQuad = y;// +SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;

//					if(i==3){xil_printf("\n\rOffset %d %d",xQuad,yQuad);}

					// Quadrant indicator
					if((xQuad>BUNKER_QUAD_X_1) & (xQuad<BUNKER_QUAD_X_2)) {
						if((yQuad>BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
							quadIndex = 0;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
							quadIndex = 3;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
							quadIndex = 6;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
					}
					else if((xQuad>BUNKER_QUAD_X_2) & (xQuad<BUNKER_QUAD_X_3)) {
						if((yQuad>BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
							quadIndex = 1;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
							quadIndex = 4;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
							quadIndex = 7;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
					}
					else if((xQuad>BUNKER_QUAD_X_3) & (xQuad<BUNKER_QUAD_X_3+BUNKER_QUAD_WIDTH)) {
						if((yQuad>BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
							quadIndex = 2;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
							quadIndex = 5;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
						else if((yQuad>BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
							quadIndex = 8;
//							xil_printf("\n\rquadIndex: %d",quadIndex);
						}
					}
					else {
						quadIndex = 0;
//						xil_printf("\n\rquadIndex: %d",quadIndex);
					}

					quadDamage = bunkerStatus[i][quadIndex]; // Indicates which damage block to render
//					if(i==3) {xil_printf(" quadDamage: %d i: %d quadIndex: %d",quadDamage,i,quadIndex);}
					switch(quadDamage) {
					case 0: // no damage
//						if(bunker_24x18[y] & (1 << x)) {
//							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
//							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
//							screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
//						}
						break;
					case 1:
						if((bunkerDamage0_6x6[y%6] & (1 << (x%6)))) {
//							xil_printf(" yas ");
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
						}
//						else {
////							xil_printf(" nope %d ",y);
////							screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
//						}
						break;
					case 2:
						if(bunker_24x18[y] & (1 << x)) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
						}
						break;
					case 3:
						if(bunker_24x18[y] & (1 << x)) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
						}
						break;
					case 4: // annihilation
						if(bunker_24x18[y] & (1 << x)) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
						}
						break;
					}

//					for(j = 0 ; j < 500000; j++){}
//					if(bunker_24x18[y] & (1 << x)) {
//						xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
//						yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
//						screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
//					}
				}
			}
		}
}

// This will be useful when doing hits
bool bunkers_check_hit(int x, int y) {
	if((y <= BUNKER_UPPER_BOUND) & (y >= BUNKER_LOWER_BOUND)) {
		if((x <= BUNKER_ONE_RIGHT_BOUND) & (x >= BUNKER_ONE_LEFT_BOUND)){
			return true;
		}
		else if ((x <= BUNKER_TWO_RIGHT_BOUND) & (x >= BUNKER_TWO_LEFT_BOUND)){
			return true;
		}
		else if ((x <= BUNKER_THREE_RIGHT_BOUND) & (x >= BUNKER_THREE_LEFT_BOUND)){
			return true;
		}
		else if ((x <= BUNKER_FOUR_RIGHT_BOUND) & (x >= BUNKER_FOUR_LEFT_BOUND)){
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
