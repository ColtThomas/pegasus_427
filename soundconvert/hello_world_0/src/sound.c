/*
 * sound.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */
#include "sound/explosion.h"
// Plays the given .wav file.


void sound_play() {
	// For now, we will try to load the explosion.c file
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getExplosionRate();
	frames = getExplosionFrames();
	data = getExplosionData();

}

void run_sound_test() {

}
