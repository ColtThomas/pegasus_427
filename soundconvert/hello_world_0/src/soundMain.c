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
int main()
{
	// calls initializer functions, then runs sount test
	init_platform();

	// AC-97 Inits per instruction from lab 5 overview

//	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_ENABLE_RESET_AC97);
//	usleep(100);
//	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_DISABLE_RESET_AC97);
//	usleep(100);
//	XAC97_SoftReset(XPAR_AUDIO_CODEC_BASEADDR);

	XAC97_HardReset(XPAR_AUDIO_CODEC_BASEADDR);

	// This will do what you think it will do
	run_sound_test();

	cleanup_platform();

	return 0;
}
