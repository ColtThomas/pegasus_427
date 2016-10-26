/*
 * soundMain.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */

#include <stdio.h>
#include "platform.h"
#include "unistd.h"
#include "xac97_l.h"
#include "xgpio.h"
//#include "screen.h"
#include "globals.h"
//#include "core.h"
#include "sound.h"
#include <xbasic_types.h>
#include <xio.h>
#include "xparameters.h"
int main()
{
	// calls initializer functions, then runs sount test
	init_platform();

	// AC-97 Inits per instruction from lab 5 overview

	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);

	// This will do what you think it will do
	xil_printf("\r\nderp");
	run_sound_test();
	xil_printf("\r\ndone");

	cleanup_platform();

	return 0;
}
