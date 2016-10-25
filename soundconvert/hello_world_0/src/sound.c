/*
 * sound.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */
#include "sound/explosion.h"
#include "xac97_l.h"

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
	// Hard reset done in the main

	// await the codec
	XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);
	// enable the VRA

	// Set the frequency

	// Set the volume

	//clear FIFO

	// set control bit to cause the IN_FIFO interrupts

	//add AC97 mask to general INT masks

	sound_play();
}
