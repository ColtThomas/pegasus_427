/*
 * sound.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */
#include "sound/explosion.h"
#include "sound/shoot.h"
#include "sound/fastinvader1.h"
#include "sound/fastinvader2.h"
#include "sound/fastinvader3.h"
#include "sound/fastinvader4.h"
#include "sound/ufo_lowpitch.h"
#include "sound/ufo_highpitch.h"
#include "sound/invaderkilled.h"
#include "xac97_l.h"
#include "xparameters.h"
#include "globals.h"
#include <stdio.h>

#define ENABLE 1
#define DISABLE 0
#define AC97_MAX_SAMPLES 256
#define AC97_VOL_INC_VAL AC97_LEFT_VOL_ATTN_1_5_DB

// Plays the given .wav file.

enum stateMarch_t {march_state_one, march_state_two, march_state_three, march_state_four } stateMarch;

static uint32_t indexExplosion;
static uint32_t indexShoot;
static uint32_t indexSaucer = 0;
static uint32_t indexInvader = 0;
static uint32_t indexKill = 0;
static uint32_t currentVolume;
void sound_init() {
	indexExplosion = 0;
	indexShoot = 0;
	stateMarch = 0;
	indexSaucer = 0;
	indexInvader = 0;
	indexKill = 0;
	currentVolume = AC97_VOL_MAX;
}
void sound_play() {
	// For now, we will try to load the explosion.c file
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getExplosionRate();
	frames = getExplosionFrames();
	data = getExplosionData();
	uint32_t i;
	for(i=0;i<frames;i++) {
		XAC97_WriteFifo(XPAR_AXI_AC97_0_BASEADDR,data[i] );
	}
//	XAC97_WriteFifo(Xuint32 BaseAddress, Xuint32 sample);
}

void run_sound_test() {
	uint32_t rate;
	rate = getExplosionRate();
	// Hard reset done in the main
	xil_printf("\r\n1");

	// await the codec
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	xil_printf("\r\n2");
	// enable the VRA
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, ENABLE);
	xil_printf("\r\n3");
	// Set the frequency on the DAC
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR,AC97_PCM_DAC_Rate,rate );
	xil_printf("\r\n4");
	// Set the volume - base address, master volume address, volume level
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR,AC97_MasterVol, AC97_VOL_MAX);
	xil_printf("\r\n5");
	//clear FIFO
	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
	xil_printf("\r\n6");
	// set control bit to cause the IN_FIFO interrupts
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR,AC97_ENABLE_IN_FIFO_INTERRUPT);
	xil_printf("\r\n7");
	//add AC97 mask to general INT masks


	sound_play();
	sound_play();
	sound_play();
	sound_play();
	xil_printf("\r\nDone son");

}

void sound_playMarch() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;

	switch(stateMarch) {
	case march_state_one:
		rate = getFastinvader1Rate();
		frames = getFastinvader1Frames();
		data = getFastinvader1Data();
		stateMarch++;
		break;
	case march_state_two:
		rate = getFastinvader2Rate();
		frames = getFastinvader2Frames();
		data = getFastinvader2Data();
		stateMarch++;
		break;
	case march_state_three:
		rate = getFastinvader3Rate();
		frames = getFastinvader3Frames();
		data = getFastinvader3Data();
		stateMarch++;
		break;
	case march_state_four:
		rate = getFastinvader4Rate();
		frames = getFastinvader4Frames();
		data = getFastinvader4Data();
		stateMarch=march_state_one;
		break;
	default:
		break;
	}



	// Set the globals for the sound
	if(globals_getSoundStatus() < alien_march){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexInvader); // Reset the index
		globals_setSoundStatus(alien_march); // Call dibs on the AC97
	}
}
void sound_playSaucer() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getUfo_lowpitchRate();
	frames = getUfo_lowpitchFrames();
	data = getUfo_lowpitchData();

	// Set the globals for the sound
	if(globals_getSoundStatus() < saucer_move){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexSaucer); // Reset the index
		globals_setSoundStatus(saucer_move); // Call dibs on the AC97
	}
}
void sound_playAlienExplode(){
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getInvaderkilledRate();
	frames = getInvaderkilledFrames();
	data = getInvaderkilledData();

	// Set the globals for the sound
	if(globals_getSoundStatus() < alien_explode){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexKill); // Reset the index
		globals_setSoundStatus(alien_explode); // Call dibs on the AC97
	}
}
void sound_playTankExplode() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getExplosionRate();
	frames = getExplosionFrames();
	data = getExplosionData();

	// Set the globals for the sound
	if(globals_getSoundStatus() < tank_explode){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexExplosion); // Reset the index
		globals_setSoundStatus(tank_explode); // Call dibs on the AC97
	}
}

void sound_playShoot() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getShootRate();
	frames = getShootFrames();
	data = getShootData();

	// Set the globals for the sound
	if(globals_getSoundStatus() < bullet_shoot){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexShoot); // Reset the index
		globals_setSoundStatus(bullet_shoot); // Call dibs on the AC97
	}
}
void sound_playSaucerExplode(){
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getUfo_highpitchRate();
	frames = getUfo_highpitchFrames();
	data = getUfo_highpitchData();

	// Set the globals for the sound
	if(globals_getSoundStatus() < saucer_explode){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexSaucer); // Reset the index
		globals_setSoundStatus(saucer_explode); // Call dibs on the AC97
	}
}
//AC97_VOL_MIN
//AC97_VOL_MAX
void sound_VolumeUp() {
	//AC97_VOL_INC_VAL
	if(currentVolume>AC97_VOL_MAX && currentVolume<=AC97_VOL_MIN) {
		xil_printf("\r\nvolume: %d",currentVolume);

		if(currentVolume<AC97_VOL_INC_VAL) {
			currentVolume=AC97_VOL_MAX; // in case of overflow
		} else {
			currentVolume = currentVolume-AC97_VOL_INC_VAL;
		}

		xil_printf("\r\nvolume: %d",currentVolume);

		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR,AC97_MasterVol, currentVolume);
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, currentVolume);
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, currentVolume);

	} else {
		currentVolume=AC97_VOL_MAX;
	}
}

void sound_VolumeDown() {
	if(currentVolume>=AC97_VOL_MAX && currentVolume<AC97_VOL_MIN) {
		xil_printf("\r\nvolume: %d",currentVolume);

		currentVolume = currentVolume+AC97_VOL_INC_VAL;
		xil_printf("\r\nvolume: %d",currentVolume);

		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR,AC97_MasterVol, currentVolume);
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, currentVolume);
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, currentVolume);
	} else {
		currentVolume=AC97_VOL_MIN;
	}
}
