/*
 * clockControl.c
 *
 *  Created on: Oct 3, 2014
 *      Author: Dallen Ford
 */

#include<stdio.h>
#include<stdint.h>
#include "supportFiles/display.h"
#include "clockDisplay.h"

// These #defines are based on a 50ms tick period.
#define AD_TIMER_EXPIRED 1 //50ms
#define AUTO_TIMER_EXPIRED 10 //0.5 seconds
#define RATE_TIMER_EXPIRED 1 //50ms
#define CLOCK_TIMER_EXPIRED 20 //1 second
#define TIMER_RESET 0

// Enumeration of the state machine's states, and instantiation of that enum.
enum clockControl_States {init_st, never_touched_st,
	waiting_for_touch_st, ad_timer_running_st,
	auto_timer_running_st, rate_timer_running_st,
	rate_timer_expired_st, add_second_to_clock_st
} clock_state = init_st;

// Ticks the state machine.
// Performs state actions, then state transitions.
void clockControl_tick() {

	// Timer variables to be used by the state machine.
	static uint32_t adTimer = TIMER_RESET;
	static uint32_t autoTimer = TIMER_RESET;
	static uint32_t rateTimer = TIMER_RESET;
	static uint32_t clockTimer = TIMER_RESET;

	// State actions
	switch(clock_state) {
	case init_st:
		break;
	case never_touched_st:
		break;
	case waiting_for_touch_st:
		adTimer = TIMER_RESET;
		autoTimer = TIMER_RESET;
		rateTimer = TIMER_RESET;
		clockTimer++;
		break;
	case ad_timer_running_st:
		adTimer++;
		break;
	case auto_timer_running_st:
		autoTimer++;
		break;
	case rate_timer_running_st:
		rateTimer++;
		break;
	case rate_timer_expired_st:
		rateTimer = TIMER_RESET;
		break;
	case add_second_to_clock_st:
		clockDisplay_advanceTimeOneSecond();
		clockTimer++;
		break;
	default:
		break;
	}

	// State Transitions and Mealy actions
	switch(clock_state) {
	case init_st:
		clock_state = never_touched_st;
		break;
	case never_touched_st:
		if(display_isTouched()) {
			display_clearOldTouchData();
			clock_state = ad_timer_running_st;
		}
	case waiting_for_touch_st:
		if(clockTimer == CLOCK_TIMER_EXPIRED) {
			clock_state = add_second_to_clock_st;
			clockTimer = TIMER_RESET;
		}
		else if(display_isTouched()) {
			display_clearOldTouchData();
			clock_state = ad_timer_running_st;
		}

		break;
	case ad_timer_running_st:
		if(!display_isTouched() && adTimer == AD_TIMER_EXPIRED) {
			clockDisplay_performIncDec();
			clock_state = waiting_for_touch_st;
		}
		else if(display_isTouched() && adTimer == AD_TIMER_EXPIRED)
			clock_state = auto_timer_running_st;
		break;
	case auto_timer_running_st:
		if(!display_isTouched()) {
			clockDisplay_performIncDec();
			clock_state = waiting_for_touch_st;
		}
		else if(display_isTouched() && autoTimer == AUTO_TIMER_EXPIRED) {
			clockDisplay_performIncDec();
			clock_state = rate_timer_running_st;
		}
		break;
	case rate_timer_running_st:
		if(!display_isTouched())
			clock_state = waiting_for_touch_st;
		else if(display_isTouched() && rateTimer == RATE_TIMER_EXPIRED)
			clock_state = rate_timer_expired_st;
		break;
	case rate_timer_expired_st:
		if(!display_isTouched())
			clock_state = waiting_for_touch_st;
		else if(display_isTouched()) {
			clockDisplay_performIncDec();
			clock_state = rate_timer_running_st;
		}
		break;
	case add_second_to_clock_st:
		clock_state = waiting_for_touch_st;
		break;
	default:
		printf("State error.");
		break;
	}
}


