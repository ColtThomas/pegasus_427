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
#define packWord5(b4,b3,b2,b1,b0) \
		((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define TEXT_WIDTH 5
#define TEXT_HEIGHT 7
#define TEXT_SPACING 2

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
void text_write(unsigned char val, point_t coord){
	int x, y;
//	long character;
	switch(val){
	case 'L':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(L_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'I':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(I_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'V':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(V_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'E':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(E_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'S':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(S_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'C':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(C_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'O':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(O_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'R':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(R_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'G':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(G_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'A':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(A_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case 'M':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(M_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '0':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Zero_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '1':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(One_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '2':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Two_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '3':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Three_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '4':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Four_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '5':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Five_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '6':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Six_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '7':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Seven_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '8':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Eight_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	case '9':
		for(y = 0; y < TEXT_HEIGHT; y++) {
			for(x = 0; x < TEXT_WIDTH; x++) {
				if(Nine_5x7[y] & (1 << (TEXT_WIDTH-x))) {
					screen_draw_double_pixel(x,y,SCREEN_WHITE);
				}
			}
		}
		break;
	}
}

void text_draw_score(){
	// Draw out the text for the score

}
