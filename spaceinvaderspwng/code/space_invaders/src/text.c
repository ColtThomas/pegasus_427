/*
 * text.c
 *
 *  Created on: Sep 28, 2016
 *      Author: superman
 */

#include "text.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>

// definition for the bitmap
#define packWord5(b4,b3,b2,b1,b0) \
		((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Text dimensions and initial positions
#define TEXT_WIDTH 5
#define TEXT_HEIGHT 7
#define TEXT_SPACING 2

#define TEXT_SCORE_INIT_X 2
#define TEXT_SCORE_INIT_Y 2
#define TEXT_SCORE_TXT_LEN 6
#define TEXT_SCORE_NUM_X TEXT_SCORE_INIT_X + \
		TEXT_WIDTH*TEXT_SCORE_TXT_LEN + \
		TEXT_SPACING*TEXT_SCORE_TXT_LEN
#define TEXT_SCORE_NUM_Y TEXT_SCORE_INIT_Y

#define TEXT_LIVES_INIT_X 200
#define TEXT_LIVES_INIT_Y 2
#define TEXT_LIFE_TXT_LEN 5
#define TEXT_SCORE_NUM_LEN 5
static const long SPACE_5x7[] =
{
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0),
		packWord5(0,0,0,0,0)
};

static const long L_5x7[] =
{
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,1,1)
};

static const long I_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,1,1,1,0)
};

static const long V_5x7[] =
{
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,0,1,0),
		packWord5(0,1,0,1,0),
		packWord5(0,0,1,0,0),
};

static const long E_5x7[] =
{
		packWord5(1,1,1,1,1),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,1,1),
};

static const long S_5x7[] =
{
		packWord5(0,1,1,1,1),
		packWord5(1,0,0,0,0),
		packWord5(0,1,1,1,0),
		packWord5(0,0,0,0,1),
		packWord5(0,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long C_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long O_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long R_5x7[] =
{
		packWord5(1,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
};

static const long Zero_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,1,1),
		packWord5(1,0,1,0,1),
		packWord5(1,1,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long One_5x7[] =
{
		packWord5(0,0,1,0,0),
		packWord5(0,1,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(1,1,1,1,1),
};

static const long Two_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(0,0,0,0,1),
		packWord5(0,0,1,1,0),
		packWord5(0,1,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,1,1),
};

static const long Three_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(0,0,0,0,1),
		packWord5(0,0,1,1,0),
		packWord5(0,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long Four_5x7[] =
{
		packWord5(0,0,0,1,1),
		packWord5(0,0,1,0,1),
		packWord5(0,1,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,1,1,1,1),
		packWord5(0,0,0,0,1),
		packWord5(0,0,0,0,1),
};

static const long Five_5x7[] =
{
		packWord5(1,1,1,1,1),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,1,0),
		packWord5(0,0,0,0,1),
		packWord5(0,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long Six_5x7[] =
{
		packWord5(0,0,1,1,0),
		packWord5(0,1,0,0,0),
		packWord5(1,0,0,0,0),
		packWord5(1,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long Seven_5x7[] =
{
		packWord5(1,1,1,1,1),
		packWord5(1,0,0,0,1),
		packWord5(0,0,0,0,1),
		packWord5(0,0,0,1,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
		packWord5(0,0,1,0,0),
};

static const long Eight_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long Nine_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,1),
		packWord5(0,0,0,0,1),
		packWord5(0,0,0,1,0),
		packWord5(0,1,1,0,0),
};

static const long G_5x7[] =
{
		packWord5(0,1,1,1,1),
		packWord5(1,0,0,0,0),
		packWord5(1,0,1,1,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(0,1,1,1,0),
};

static const long A_5x7[] =
{
		packWord5(0,1,1,1,0),
		packWord5(1,0,0,0,1),
		packWord5(1,1,1,1,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
};

static const long M_5x7[] =
{
		packWord5(1,0,0,0,1),
		packWord5(1,1,0,1,1),
		packWord5(1,0,1,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
		packWord5(1,0,0,0,1),
};

static unsigned char currentScore[TEXT_SCORE_NUM_LEN] = {'0',' ',' ',' ',' '};


// Plug in a char and a coordinate, and that char will write to the screen
void text_write(unsigned char val, point_t coord,bool erase){
	int32_t x, y;

	// This is a long switch statement that could be optimized; it takes the char input
	// and draws the respective char on the gamescreen
	switch(val){
	case ' ':
		// Optimize this by sticking it into a function
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(SPACE_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'L':
		// Optimize this by sticking it into a function
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(L_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'I':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(I_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'V':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(V_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'E':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(E_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'S':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(S_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'C':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(C_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'O':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(O_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'R':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(R_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'G':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(G_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'A':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(A_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case 'M':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(M_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_WHITE);
				}
			}
		}
		break;
	case '0':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Zero_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '1':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(One_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '2':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Two_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '3':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Three_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '4':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Four_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '5':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Five_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '6':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Six_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '7':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Seven_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '8':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Eight_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	case '9':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Nine_5x7[y] & (1 << (TEXT_WIDTH-x-1))) {
					screen_draw_double_pixel(x+coord.x,y+coord.y,(erase)?SCREEN_BLACK:SCREEN_GREEN);
				}
			}
		}
		break;
	}
}
void text_increment_score(){
	uint32_t i;

	point_t currentPoint;
	currentPoint.x = TEXT_SCORE_NUM_X;
	currentPoint.y = TEXT_SCORE_NUM_Y;
	for(i=0;i<TEXT_SCORE_NUM_LEN;i++){
		text_write(currentScore[i], currentPoint,true);
		currentPoint.x += TEXT_WIDTH+TEXT_SPACING;
	}

	for(i=0;i<TEXT_SCORE_NUM_LEN;i++) {
		if(currentScore[i]=='9'){
			currentScore[i]='1';
			currentScore[i+1]='0';
			break;
		}
		else if(currentScore[i]==' '){
			break;
		}
		else {

			currentScore[i]++;
			break;
		}

	}


	for(i=0;i<TEXT_SCORE_NUM_LEN;i++){
		text_write(currentScore[i], currentPoint,false);
		currentPoint.x += TEXT_WIDTH+TEXT_SPACING;
	}
}
void text_add_score(uint32_t points) {
	uint32_t i;
	for(i=0;i<points/10;i++) {

	}

	switch(points){
		case 10:
			break;
		case 20:
			break;
		case 40:
			break;
		case 50:
			break;
		case 100:
			break;
		case 150:
			break;
		case 200:
			break;
		case 250:
			break;
		case 300:
			break;
	}
}

void text_draw_score(){
	
	// Draw out text for the lives
	point_t currentPoint;
	unsigned char livesMessage[TEXT_LIFE_TXT_LEN] = {
			'L','I','V','E','S'
	};
	
	// Set the initial point of the string
	currentPoint.x = TEXT_LIVES_INIT_X;
	currentPoint.y = TEXT_LIVES_INIT_Y;
	int32_t i;
	for(i=0;i<TEXT_LIFE_TXT_LEN;i++){;
			text_write(livesMessage[i], currentPoint,false);
			currentPoint.x += TEXT_WIDTH+TEXT_SPACING;
		}
	
	// Draw out the text for the score
	unsigned char scoreMessage[TEXT_SCORE_TXT_LEN] = {
			'S','C','O','R','E',' '
	};
	// Initial coordinate for the score board
	currentPoint.x = TEXT_SCORE_INIT_X;
	currentPoint.y = TEXT_SCORE_INIT_Y;
	for(i=0;i<TEXT_SCORE_TXT_LEN;i++){
		text_write(scoreMessage[i], currentPoint,false);
		currentPoint.x += TEXT_WIDTH+TEXT_SPACING;
	}

	// Draw out the score itself
	currentPoint.x = TEXT_SCORE_NUM_X;
	currentPoint.y = TEXT_SCORE_NUM_Y;
	for(i=0;i<TEXT_SCORE_NUM_LEN;i++){
		text_write(currentScore[i], currentPoint,false);
		currentPoint.x += TEXT_WIDTH+TEXT_SPACING;
	}
}
