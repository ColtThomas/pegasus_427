/*
 * soundMain.c
 *
 *  Created on: Oct 24, 2016
 *      Author: superman
 */

#include "unistd.h"
#include "xac97_l.h"
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
//#include "screen.h"
#include "globals.h"
#include "xac97_l.h"
//#include "core.h"
#include "sound.h"
#include <xbasic_types.h>
#include <xio.h>
#include "xparameters.h"
#include "explosion.h"

XGpio gpAC97;   // This is a handle for the AC97.
//XPAR_AXI_AC97_0_BASEADDR
//XPAR_AXI_AC97_0_INTERRUPT_MASK
//XPAR_AXI_INTC_0_AXI_AC97_0_INTERRUPT_INTR

#define AC97_MAX_SAMPLES 256
#define ENABLE 1
#define DISABLE 0

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

void ac97_interrupt_handler() {
	uint32_t * data;
	uint32_t rate;
	uint32_t frames;
	uint32_t i;
	rate = getExplosionRate();
	frames = getExplosionFrames();
	data = getExplosionData();
	for(i=0;i<AC97_MAX_SAMPLES;i++){
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, data[i]);
	}
	xil_printf("\r\nFired");
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	xil_printf("\r\ncalled");

	// Check the AC97 interrupts.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){
//		XIntc_AckIntr(XPAR_AXI_AC97_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
		ac97_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
		XAC97_EnableInterupts(XPAR_AXI_AC97_0_BASEADDR);
	}

}


// Remember that you comment out the core_end_game(); in globals.c. Fix that when you tie things together
int main()
{
	// calls initializer functions, then runs sound test
	init_platform();

	/*
	 * Initialization for the AC97
	 */

	uint32_t rate;
	rate = getExplosionRate();
	// AC-97 Inits per instruction from lab 5 overview
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);

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
//	XAC97_mSe?tControl(XPAR_AXI_AC97_0_BASEADDR,AC97_ENABLE_IN_FIFO_INTERRUPT);
	xil_printf("\r\n7");
	//add AC97 mask to general INT masks
    XAC97_EnableInterupts(XPAR_AXI_AC97_0_BASEADDR);

    /*
     * Now we need to enable the interrupt controller that also handles
     * the FIT and the GPIO. The interrupts from the AC97 will be picked
     * up by the interrupt controller
     */

    // Initialize the GPIO peripherals.
	int success;
	//print("dude, this does something\n\r");
	success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK | XPAR_AXI_AC97_0_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();






	// This will do what you think it will do
//	xil_printf("\r\nderp");
//	run_sound_test();
	while(1);

	xil_printf("\r\ndone");

	cleanup_platform();

	return 0;
}
