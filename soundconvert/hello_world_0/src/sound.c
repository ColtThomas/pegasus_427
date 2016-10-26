/*
 * sound.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */
#include "explosion.h"
#include "xac97_l.h"
#include "xparameters.h"

#define ENABLE 1
#define DISABLE 0
// Plays the given .wav file.


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
