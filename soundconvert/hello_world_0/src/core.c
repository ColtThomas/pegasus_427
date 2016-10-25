/*
 * core.c
 *
 *  Created on: Oct 13, 2016
 *      Author: superman
 */

#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "aliens.h"
#include "tank.h"
#include "bunkers.h"
#include "text.h"
#include "bullets.h"
#include "bulletHandler.h"
#include "alienHandler.h"
#include "saucer.h"
#include <stdlib.h>

#define TIMER_SEC 100 // timer period is 10ms, so 100 periods is one second
#define NUMBER_OF_BUTTONS 5

#define FRAMES_PER_SECOND 30
#define TIMER_FRAME TIMER_SEC/FRAMES_PER_SECOND

#define ALIEN_FRAME_COUNT 10 // update aliens every this many frames
#define BULLET_FRAME_COUNT 1 // bullets update a little faster
#define TANK_FRAME_COUNT 1 // allow updating the tank every this many frames
#define TANK_DEATH_FRAME_COUNT 15 // change animation every this many frames
#define SAUCER_FRAME_COUNT 3
#define SAUCER_SCORE_FRAME_COUNT 30
#define ALIEN_DEATH_FRAME_COUNT 2 // change animation every this many frames

#define NEW_BULLET_MAX_RAND 50 // create a new bullet at a max of this many spaces
#define NEW_BULLET_SPACING 5 // ensures that new bullets are at least this many frames apart
#define NEW_BULLET_TIME (rand() % NEW_BULLET_MAX_RAND + 1)*NEW_BULLET_SPACING

#define NEW_SAUCER_MAX_RAND 8 // create a new saucer at a max of this many spaces
#define NEW_SAUCER_SPACING 60 // ensures that new saucer are at least this many frames apart
#define NEW_SAUCER_TIME (rand() % NEW_SAUCER_MAX_RAND + 1)*NEW_SAUCER_SPACING

// Masks used to identify the push buttons
const uint8_t BTN_MASKS[] = {0x01,0x02,0x04,0x08,0x10};
#define BTN_LEFT 3
#define BTN_FIRE 0
#define BTN_RIGHT 1
#define BTN_DOWN 2
#define BTN_UP 4

#define BOTTOM_LINE_Y SCREEN_HEIGHT - 10

u32 buttonStateReg; // Read the button values with this variable

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

bool game_started = false;

// This function is used to distinguish the up and down buttons from the rest
bool volume_button_pressed() {
	return (buttonStateReg & BTN_MASKS[BTN_UP]) || (buttonStateReg & BTN_MASKS[BTN_DOWN]);
}

// This function is used to distinguish the hour, minute and second buttons from the rest
bool game_button_pressed() {
	return (buttonStateReg & BTN_MASKS[BTN_LEFT]) || (buttonStateReg & BTN_MASKS[BTN_FIRE]) || (buttonStateReg & BTN_MASKS[BTN_RIGHT]);
}

bool left_button_pressed() {
	return buttonStateReg & BTN_MASKS[BTN_LEFT];
}

bool right_button_pressed() {
	return buttonStateReg & BTN_MASKS[BTN_RIGHT];
}

bool fire_button_pressed() {
	return buttonStateReg & BTN_MASKS[BTN_FIRE];
}

// This is invoked in response to a timer interrupt.
// It does: ??????????????????????????????????????????????????????????????????
void timer_interrupt_handler() {
	static int32_t frame_timer = 0;
	static int32_t frame_count = 0; // used to allow things to be every nth frame, by using mod.
	static int32_t bullet_time = -1;
	static int32_t saucer_time = -1;
	static int32_t saucer_score_timer = 0;
	//xil_printf("saucer_time %d\r\n",saucer_time);
	if(bullet_time == -1) {
		bullet_time = NEW_BULLET_TIME; // the next bullet will come in NEW_BULLET_TIME frames
	} // this should only ever run on the first tick
	if(saucer_time == -1) {
		saucer_time = NEW_SAUCER_TIME;
	//	xil_printf("new saucer time: %d\r\n",saucer_time);
	} // this too

	frame_timer++;

	if(frame_timer == TIMER_FRAME) { // one frame has gone by - redraw things
		// setup for new bullet and saucer
		if(game_started && !tank_is_dying()) {
			bullet_time--;
			saucer_time--;
		}
		// generate new bullet
		if(bullet_time == 0 && game_started && !tank_is_dying()) {
				bullet_time = NEW_BULLET_TIME;
				bullets_fire_aliens();
			}
		// spawn saucer
		if(saucer_time == 0 && game_started && !tank_is_dying()) {
			saucer_time = NEW_SAUCER_TIME;
			saucer_spawn();
		}

		// reset frame timer
		frame_timer = 0;

		 //move saucer, if spawned
		if(frame_count % SAUCER_FRAME_COUNT == 0 && game_started && !tank_is_dying() && saucer_is_spawned()) {
			saucer_update();
		}

		//update all bullet positions
				if(frame_count % BULLET_FRAME_COUNT == 0 && game_started && !tank_is_dying()) {
					bulletHandler_tick();
				}

		//move aliens and saucer
		if(frame_count % ALIEN_FRAME_COUNT == 0 && game_started && !tank_is_dying()) { //pause the aliens while the death animation goes on
			alienHandler_tick();
		}


		//check buttons
		// don't start the game until a button is pressed
		if(game_button_pressed() && !game_started) {
			game_started = true;
			srand(frame_count); // seed the random depending on how long the user waits before pressing the button to start the game.
		}
		//move tank left, if needed
		if(left_button_pressed() && frame_count % TANK_FRAME_COUNT == 0 && !tank_is_dying()) {
			tank_move_left();
		}
		//move tank right, if needed
		if(right_button_pressed() && frame_count % TANK_FRAME_COUNT == 0 && !tank_is_dying()) {
			tank_move_right();
		}
		// fire tank bullet
		if(fire_button_pressed()/* && !tank_is_dying()*/) {
			bullets_fire_tank();
		}
		// animate tank death
		if(tank_is_dying() && frame_count % TANK_DEATH_FRAME_COUNT == 0) {
			tank_update_death();
		}
		// animate alien death
		if(aliens_is_dying() && frame_count % ALIEN_DEATH_FRAME_COUNT == 0) {
			aliens_update_death();
		}

		// if needed, erase saucer score after given time
		if(text_saucer_score_begun()) {
			if(saucer_score_timer++ >= SAUCER_SCORE_FRAME_COUNT) { // leave the score there for a number of frames
				text_print_saucer_score(true); //erase saucer score
				saucer_score_timer = 0;
			}
		}

	//	xil_printf("%d\r\n",frame_count);
		frame_count++;
		//xil_printf("interrupt!");
	}
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
//	xil_printf("int");
	uint32_t intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	buttonStateReg = XGpio_DiscreteRead(&gpPB, 1); // read buttons
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
		//xil_printf("exit interrupt\r\n");
	}
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XGpio_InterruptClear(&gpPB, 0xFFFFFFFF); // ack this interrupt, but we do nothing with it.
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int32_t core_init (void) {
	//init_platform();
	// Initialize the GPIO peripherals.
	int32_t success;
	//time_t t;
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
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	// seed random for bullets
	//srand((unsigned) time(&t));

	//while(1);  // Program never ends. But at least the lab does.

	// cleanup_platform();

	return 0;
}

void draw_bottom_line() {
	int32_t x, y;
	y = BOTTOM_LINE_Y;
	for(x = 0; x < SCREEN_WIDTH; x++) {
		screen_draw_double_pixel(x, y, SCREEN_GREEN);
	}
}

void core_draw_initial() {
	tank_draw_initial();
	tank_draw_lives_initial();
	aliens_draw_initial();
	bunkers_draw_initial();
	text_draw_score();
	draw_bottom_line();
}

void core_run() {
	microblaze_enable_interrupts();
}

void core_end_game() {
	game_started = false;
	// disable interrupts to prevent any further control or motion, thus completely ending the game.
	XIntc_MasterDisable(XPAR_INTC_0_BASEADDR);
}
