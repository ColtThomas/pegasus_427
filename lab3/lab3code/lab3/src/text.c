/*
 * text.c
 *
 *  Created on: Sep 28, 2016
 *      Author: superman
 */

#define packWord5(b4,b3,b2,b1,b0) \
		((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

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
