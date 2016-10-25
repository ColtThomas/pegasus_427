/*
 * soundMain.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */

#include <stdio.h>
#include "platform.h"
#include "unistd.h"
//#include "screen.h"
#include "globals.h"
//#include "core.h"
#include "sound.h"

int main()
{
//	int64_t idle_count;
	// calls initializer functions, then runs game core
	init_platform();
//	screen_init();
//	globals_init();
//	screen_clear();
//	core_init();
//	core_draw_initial();//run_test();
//	core_run();

	run_sound_test();

	cleanup_platform();

	return 0;
}
