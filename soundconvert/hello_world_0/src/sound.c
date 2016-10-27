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
#include "xac97_l.h"
#include "xparameters.h"
#include "globals.h"
#define ENABLE 1
#define DISABLE 0
#define AC97_MAX_SAMPLES 256
// Plays the given .wav file.

static uint32_t indexExplosion;
static uint32_t indexShoot;
static uint8_t stateMarch;

void sound_init() {
	indexExplosion = 0;
	indexShoot = 0;
	stateMarch = 0;
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
	case 0:
		rate = getFastinvader1Rate();
		frames = getFastinvader1Frames();
		data = getFastinvader1Data();
		stateMarch++;
		break;
	case 1:
		rate = getFastinvader2Rate();
		frames = getFastinvader2Frames();
		data = getFastinvader2Data();
		stateMarch++;
		break;
	case 2:
		rate = getFastinvader3Rate();
		frames = getFastinvader3Frames();
		data = getFastinvader3Data();
		stateMarch++;
		break;
	case 3:
		rate = getFastinvader4Rate();
		frames = getFastinvader4Frames();
		data = getFastinvader4Data();
		stateMarch=0;
		break;
	default:
		break;
	}



	// Set the globals for the sound
	if(!globals_getSoundStatus()){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexExplosion); // Reset the index
		globals_setSoundStatus(true); // Call dibs on the AC97
	}
}
void sound_playSaucer() {

}
void sound_playAlienExplode(){

}
void sound_playTankExplode() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	rate = getExplosionRate();
	frames = getExplosionFrames();
	data = getExplosionData();

	// Set the globals for the sound
	if(!globals_getSoundStatus()){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexExplosion); // Reset the index
		globals_setSoundStatus(true); // Call dibs on the AC97
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
	if(!globals_getSoundStatus()){ // Should freeze things... waiting for the sound
		globals_setNextSoundSamples(data);
		globals_setCurrentSoundFrames(frames);
		globals_setCurrentFrameIndex(indexShoot); // Reset the index
		globals_setSoundStatus(true); // Call dibs on the AC97
	}
}
void sound_playSaucerExplode(){

}
