/*
 * bunkers.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#include "bunkers.h"
#include "bullets.h"
#include "screen.h"
#include <stdbool.h>
#include <stdio.h>

// Used to define the bitmaps
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b23 << 23)|(b22<< 22)|(b21<< 21)|(b20<< 20)|(b19<< 19)|(b18<< 18)|(b17 << 17)|(b16<< 16)|(b15<< 15)|(b14<< 14)|(b13<< 13)|(b12<< 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword6(b5,b4,b3,b2,b1,b0) \
		((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

// Constants dealing with the bunker positioning
#define BUNKER_MARGIN_BOTTOM 60
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_SPACING 45

#define BUNKER_ONE 0
#define BUNKER_TWO 1
#define BUNKER_THREE 2
#define BUNKER_FOUR 3

// Boundary definitions for hit indication
#define BUNKER_LOWER_BOUND SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM
#define BUNKER_UPPER_BOUND SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM + BUNKER_HEIGHT
#define BUNKER_ONE_LEFT_BOUND BUNKER_SPACING
#define BUNKER_ONE_RIGHT_BOUND BUNKER_SPACING + BUNKER_WIDTH
#define BUNKER_TWO_LEFT_BOUND BUNKER_SPACING*2 + BUNKER_WIDTH
#define BUNKER_TWO_RIGHT_BOUND BUNKER_SPACING*2 + BUNKER_WIDTH*2
#define BUNKER_THREE_LEFT_BOUND BUNKER_SPACING*3 + BUNKER_WIDTH*2
#define BUNKER_THREE_RIGHT_BOUND BUNKER_SPACING*3 + BUNKER_WIDTH*3
#define BUNKER_FOUR_LEFT_BOUND BUNKER_SPACING*4 + BUNKER_WIDTH*3
#define BUNKER_FOUR_RIGHT_BOUND BUNKER_SPACING*4 + BUNKER_WIDTH*4

// More bunker css stuff for easy spacing and quadrant definitions
#define BUNKER_QUAD_WIDTH BUNKER_WIDTH/3
#define BUNKER_QUAD_HEIGHT BUNKER_HEIGHT/3
#define BUNKER_QUAD_X_1 0
#define BUNKER_QUAD_X_2 BUNKER_QUAD_WIDTH
#define BUNKER_QUAD_X_3 BUNKER_QUAD_WIDTH*2
#define BUNKER_QUAD_Y_1 0
#define BUNKER_QUAD_Y_2 BUNKER_QUAD_HEIGHT
#define BUNKER_QUAD_Y_3 BUNKER_QUAD_HEIGHT*2

// Used to define the quadrants of the bunker
#define BUNKER_QUAD_ZERO 0
#define BUNKER_QUAD_ONE 1
#define BUNKER_QUAD_TWO 2
#define BUNKER_QUAD_THREE 3
#define BUNKER_QUAD_FOUR 4
#define BUNKER_QUAD_FIVE 5
#define BUNKER_QUAD_SIX 6
#define BUNKER_QUAD_SEVEN 7
#define BUNKER_QUAD_EIGHT 8
#define BUNKER_QUAD_NINE 9
#define BUNKER_QUAD_VOID 0

// Used as damage status of the bunker quadrants
#define BUNKER_DMG_0 0
#define BUNKER_DMG_1 1
#define BUNKER_DMG_2 2
#define BUNKER_DMG_3 3
#define BUNKER_DMG_4 4

// Since some of the bitmaps are smaller we have to use modulo
// operations to avoid any illegal memory access
#define BUNKER_MOD 6
#define BUNKER_QUANTITY 4

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
		{0,0,0,0,0,0,0,4,0},
		{0,0,0,0,0,0,0,4,0},
		{0,0,0,0,0,0,0,4,0},
		{0,0,0,0,0,0,0,4,0}
};

// Initialization function for the bunkers
void bunkers_draw_initial() {
	int32_t i;
	int32_t x, y;
	int32_t xOffset;
	int32_t yOffset;
	// Iterate though all 4 bunkers
	for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
		
		//Trace the x and y coordinates of the bitmaps
		for(x = 0; x < BUNKER_WIDTH; x++) {
			for(y = 0; y < BUNKER_HEIGHT; y++) {
				if(bunker_24x18[y] & (1 << x)) { // If the coordinate matches the bitmap, we draw
					// Offset puts the bunkers right where we want them, evenly spaced like a champ
					xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
					yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
					screen_draw_double_pixel(xOffset,yOffset,SCREEN_GREEN);
				}
			}
		}
	}

}

uint32_t bunkers_get_quadrant(uint32_t xQuad,uint32_t yQuad,bool debug){
	uint32_t quadIndex = 0;
	// Each bunker is divided into 9 quadrants. These quadrants will help us apply
	// damage to each patch on the bunker as they get hit. The quad index is assigned
	// as the pixel coordinate crosses quadrant boundaries
	if(debug) {xil_printf("\r\nQuad map: %d %d",xQuad,yQuad);}
	if((xQuad>=BUNKER_QUAD_X_1) & (xQuad<BUNKER_QUAD_X_2)) {
		if((yQuad>=BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
			quadIndex = BUNKER_QUAD_ZERO;
		}
		else if((yQuad>=BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
			quadIndex = BUNKER_QUAD_THREE;
		}
		else if((yQuad>=BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
			quadIndex = BUNKER_QUAD_SIX;
		}
	}
	else if((xQuad>=BUNKER_QUAD_X_2) & (xQuad<BUNKER_QUAD_X_3)) {
		if((yQuad>=BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
			quadIndex = BUNKER_QUAD_ONE;
		}
		else if((yQuad>=BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
			quadIndex = BUNKER_QUAD_FOUR;
		}
		else if((yQuad>=BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
			quadIndex = BUNKER_QUAD_SEVEN;
		}
	}
	else if((xQuad>=BUNKER_QUAD_X_3) & (xQuad<BUNKER_QUAD_X_3+BUNKER_QUAD_WIDTH)) {
		if((yQuad>=BUNKER_QUAD_Y_1) & (yQuad<BUNKER_QUAD_Y_2)) {
			quadIndex = BUNKER_QUAD_TWO;
		}
		else if((yQuad>=BUNKER_QUAD_Y_2) & (yQuad<BUNKER_QUAD_Y_3)) {
			quadIndex = BUNKER_QUAD_FIVE;
		}
		else if((yQuad>=BUNKER_QUAD_Y_3) & (yQuad<BUNKER_QUAD_Y_3+BUNKER_QUAD_HEIGHT)) {
			quadIndex = BUNKER_QUAD_EIGHT;
		}
	}
	else { // Value is void when the pixel is in no quadrant at all
		xil_printf("\r\nmeow");
		quadIndex = BUNKER_QUAD_VOID;
	}
	return quadIndex;
}

// Might want to consider splitting this to update a specific bunker to save render time
void bunkers_update() {
		// We do a lot of iteration though each bunker and the quadrants of each one
		int32_t i;
		int32_t x, y;
		int32_t xOffset,yOffset,xQuad,yQuad,quadIndex,quadDamage = 0;
		// Iterate though each bunker
		for(i = 0; i < GLOBALS_NUMBER_OF_BUNKERS; i++) {
			for(y = 0; y < BUNKER_HEIGHT; y++) {
				for(x = 0; x < BUNKER_WIDTH; x++) {
					// Get the positioning of each bunker
					xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
					yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;

					// Used to determine if coordinate is in a quadrant
					xQuad = x;
					yQuad = y;

					quadIndex = bunkers_get_quadrant(xQuad,yQuad,false);


					quadDamage = bunkerStatus[i][quadIndex]; // Indicates which damage block to render
					switch(quadDamage) {
					case BUNKER_DMG_0: // No damage, so we don't do anything to save execution time
						break;
					case BUNKER_DMG_1: // Mildly damaged bunker bitmap generation
						if((bunkerDamage0_6x6[y%BUNKER_MOD] & (1 << (x%BUNKER_MOD)))) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
						}
						break;
					case BUNKER_DMG_2:// Moderately damaged bunker bitmap generation
						if((bunkerDamage1_6x6[y%BUNKER_MOD] & (1 << (x%BUNKER_MOD)))) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
						}
						break;
					case BUNKER_DMG_3: // Heavily damaged bunker bitmap generation
						if((bunkerDamage2_6x6[y%BUNKER_MOD] & (1 << (x%BUNKER_MOD)))) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
						}
						break;
					case BUNKER_DMG_4: // annihilation, or destroyed quadrant
						if((bunkerDamage3_6x6[y%BUNKER_MOD] & (1 << (x%BUNKER_MOD)))) {
							xOffset = x + BUNKER_SPACING + i*BUNKER_SPACING + i* BUNKER_WIDTH;
							yOffset = y + SCREEN_HEIGHT - BUNKER_MARGIN_BOTTOM;
							screen_draw_double_pixel(xOffset,yOffset,SCREEN_BLACK);
						}
						break;
					}
				}
			}
		}
}

// Function call used to inflict damage on a particular bunker's quadrant
bool bunker_damage(int32_t bunkerNum, int32_t quadrant) {
//		xil_printf("\r\nDamage Applied");
		if(bunkerStatus[bunkerNum][quadrant]<BUNKER_QUANTITY) {
			bunkerStatus[bunkerNum][quadrant]++; // The higher the stored value, the higher the damage
			return true;
		}
		return false;
}

// function translates the given position to the quadrant position
point_t bunkers_get_relative_pos(point_t pos, uint32_t bunker){
	point_t relativePoint;
	relativePoint.x = pos.x - BUNKER_SPACING - bunker*BUNKER_SPACING - bunker* BUNKER_WIDTH;
	relativePoint.y = pos.y - SCREEN_HEIGHT + BUNKER_MARGIN_BOTTOM; // had a problem where I would get negative y values without the speed put in the equation
//	+bullets_get_speed()
	return relativePoint;
}

// This will be useful when doing hits; this is for future implementations of collision detection
bool bunkers_check_hit(point_t pos,bool bulletIsAlien) {
	uint32_t quadrant;
	int32_t offset = (bulletIsAlien)? bullets_get_height() : -bullets_get_height()-bullets_get_speed(); // consider a global
	point_t relativePoint;
	pos.y += offset; // both types of bullets approach from different sides, so they have different offset
	/*
	 * Here is an idea for the offset prob: just add the offset to the initial pos value and then
	 * remove respective offset from bunkers_get_relative_pos as well as the boundaries in the
	 * following conditionals. Basically anything that takes into consideration the bullet speed
	 * or bullet height
	 */

//	if(!bulletIsAlien){xil_printf("\r\npos: %d %d",pos.x,pos.y);}
	if((pos.y <= BUNKER_UPPER_BOUND) && (pos.y >= BUNKER_LOWER_BOUND)) {
//		xil_printf("\r\nIn bounds");
		if((pos.x <= BUNKER_ONE_RIGHT_BOUND) & (pos.x >= BUNKER_ONE_LEFT_BOUND)){
			relativePoint = bunkers_get_relative_pos(pos, BUNKER_ONE);
//			xil_printf("\r\nPos: %d %d",relativePoint.x,relativePoint.y);
			quadrant = bunkers_get_quadrant(relativePoint.x,relativePoint.y,true);
//			xil_printf("\r\nBunker 1 hit... quadrant %d",quadrant);
			return bunker_damage(BUNKER_ONE, quadrant);
		}
		else if ((pos.x <= BUNKER_TWO_RIGHT_BOUND) & (pos.x >= BUNKER_TWO_LEFT_BOUND)){
			relativePoint = bunkers_get_relative_pos(pos, BUNKER_TWO);
//			xil_printf("\r\nPos: %d %d",relativePoint.x,relativePoint.y);
			quadrant = bunkers_get_quadrant(relativePoint.x,relativePoint.y,true);
//			xil_printf("\r\nBunker 2 hit... quadrant %d",quadrant);
			return bunker_damage(BUNKER_TWO, quadrant);
		}
		else if ((pos.x <= BUNKER_THREE_RIGHT_BOUND) & (pos.x >= BUNKER_THREE_LEFT_BOUND)){
			relativePoint = bunkers_get_relative_pos(pos, BUNKER_THREE);
//			xil_printf("\r\nPos: %d %d",relativePoint.x,relativePoint.y);
			quadrant = bunkers_get_quadrant(relativePoint.x,relativePoint.y,true);
//			xil_printf("\r\nBunker 3 hit... quadrant %d",quadrant);
			return bunker_damage(BUNKER_THREE, quadrant);
		}
		else if ((pos.x <= BUNKER_FOUR_RIGHT_BOUND) & (pos.x >= BUNKER_FOUR_LEFT_BOUND)){
			relativePoint = bunkers_get_relative_pos(pos, BUNKER_FOUR);
//			xil_printf("\r\nPos: %d %d",relativePoint.x,relativePoint.y);
			quadrant = bunkers_get_quadrant(relativePoint.x,relativePoint.y,true);
//			xil_printf("\r\nBunker 4 hit... quadrant %d",quadrant);
			return bunker_damage(BUNKER_FOUR, quadrant);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


