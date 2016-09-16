#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.


#define PB_DEBOUNCE_TIME 5 //timer ticks at 10ms, so 5 = 50ms
#define HALF_TIMER_SEC 50 // increment 2x per s
#define TIMER_SEC 100
#define NUMBER_OF_BUTTONS 5

// Masks used to identify the push buttons
const int BTN_MASKS[] = {0x01,0x02,0x04,0x08,0x10};
#define BTN_HR 3
#define BTN_MIN 0
#define BTN_SEC 1
#define BTN_DOWN 2
#define BTN_UP 4

// Constants for the clock display
#define MINSEC_MAX 59
#define HR_MAX 23

// Don't know if we are allowed to add any libraries so I manually define bool for kicks and giggles:
typedef int bool;
#define TRUE 1
#define FALSE 0

u32 buttonStateReg; // Read the button values with this variable

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

int hour = 0;
int minute = 0;
int second = 0;

// Counters for the various timers
int secondCount = 0; // General second counter
int halfSecondCount = 0; // Used in auto increment
int debounceCount = 0; 
int timerCount = 0; 
bool scrollDisable = FALSE; // debug variable for part 1

// Flags for the buttons
bool secondFlag = FALSE;
bool halfSecondFlag = FALSE;
bool debounceFlag = FALSE;
bool dirButtonHeld = FALSE;
bool autoIncrementFlag = FALSE;

// Function used to increment clock by 1 second
void increment_clock() {
	if(!buttonStateReg) { // only increment time when not setting clock
		second++;
		if(second > MINSEC_MAX) {
			second = 0;
			minute++;
			if(minute > MINSEC_MAX) {
				minute = 0;
				hour++;
				if(hour > HR_MAX) {
					hour = 0;
				}
			}
		}
	}
}

// Display function for the clock
void print_clock() {
	if(scrollDisable) {
		xil_printf("\b\b\b\b\b\b\b\b"); // Backspaces to avoid scrolling
	}
	xil_printf("\r%02d:%02d:%02d",hour,minute,second);
}

// Increments or decrements the saved time values
void reset_time() {
	bool time_changed = FALSE;
	if(buttonStateReg & BTN_MASKS[BTN_HR]) { // SET HOUR
		if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT HOUR
			hour++;
			time_changed = TRUE;
			if(hour > HR_MAX) hour = 0; // wrap-around at 23 for military time
		}
		if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT HOUR
			hour--;
			time_changed = TRUE;
			if(hour < 0) hour = HR_MAX; // wrap-around to 23 for military time
		}
	}

	if(buttonStateReg & BTN_MASKS[BTN_MIN]) { // SET MINUTE
		if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT MINUTE
			minute++;
			time_changed = TRUE;
			if(minute > MINSEC_MAX) minute = 0; // wrap-around
		}
		if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT MINUTE
			minute--;
			time_changed = TRUE;
			if(minute < 0) minute = MINSEC_MAX; // wrap-around
		}
	}

	if(buttonStateReg & BTN_MASKS[BTN_SEC]) { // SET SECOND
		if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT SECOND
			second++;
			time_changed = TRUE;
			if(second > MINSEC_MAX) second = 0; // wrap-around
		}
		if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT SECOND
			second--;
			time_changed = TRUE;
			if(second < 0) second = MINSEC_MAX; // wrap-around to 23 for military time
		}
	}
	if(time_changed) print_clock();
}

// Checks to see if any button is pressed at all
bool button_pressed() {
	return buttonStateReg>0;
}

// This function is used to distinguish the up and down buttons from the rest
bool inc_button_pressed() {
	return (buttonStateReg & BTN_MASKS[BTN_UP]) || (buttonStateReg & BTN_MASKS[BTN_DOWN]);
}

// This function is used to distinguish the hour, minute and second buttons from the rest
bool time_button_pressed() {
	return (buttonStateReg & BTN_MASKS[BTN_HR]) || (buttonStateReg & BTN_MASKS[BTN_MIN]) || (buttonStateReg & BTN_MASKS[BTN_SEC]);
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() {
	bool time_set = FALSE;

	secondFlag = FALSE;
	halfSecondFlag = FALSE;

	// Increment counters for the flags
	secondCount++;
	halfSecondCount++;

	// Second counter
	if(secondCount >= TIMER_SEC && !button_pressed()) {	// Only increment when no buttons are pushed
		secondCount = 0;
		dirButtonHeld = FALSE;
		secondFlag = TRUE;
		print_clock();
		increment_clock();

	} else if (secondCount >= TIMER_SEC && dirButtonHeld) { 
		// We wait a second as a directional button is held before auto incrementing
		secondCount = 0;
		secondFlag = TRUE; // Mark the first increment
		autoIncrementFlag = TRUE; // Let the rest of the software prepare to autoincrement
		halfSecondCount = 0; // Reset the counter to iterate every half second
		print_clock();
		reset_time();
	}

	// The half second counter goes off after the second long hold
	if(halfSecondCount >= HALF_TIMER_SEC) {
		halfSecondCount = 0;
		halfSecondFlag = TRUE;
	}

	// Auto increment counter clock update
	if(halfSecondCount >= HALF_TIMER_SEC && autoIncrementFlag) {
		print_clock();
		reset_time();
		halfSecondCount = 0;
	}

	// Button debounce logic
	if(debounceCount >= PB_DEBOUNCE_TIME && !debounceFlag) {
		// reset the counters so we don't get awkward timing hiccups
		debounceCount = 0;
		secondCount = 0;
		debounceFlag = TRUE; // Button ready to read
		reset_time();
	}

	// Debounce the buttons
	if(button_pressed()) {
		debounceCount++;
	} else {
		debounceCount = 0;
		debounceFlag = FALSE;
	}


	// Check to see if the button was held for a second
	if(!dirButtonHeld && debounceFlag && inc_button_pressed() && time_button_pressed()) {
		reset_time();
		print_clock();
		dirButtonHeld = TRUE; // This is key to knowing whether to increment or not
		secondCount = 0;
	}

	// We don't want to increment when only a time button is pressed.
	if(!inc_button_pressed()) {
		dirButtonHeld = FALSE;
		autoIncrementFlag = FALSE;
	}

}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.

  // Button implementation
  /*
   *  buttonStateReg will store the button states in the lower 5 bits.
   *  If you just print the value you will get:
   *  1 - center button
   *  2 - right button
   *  4 - down button
   *  8 - left button
   *  16 - up button
   *
   */
   // shows which button is being pressed

  // The lab requires us to not directly poll the buttons. We will do this by storing the value of the states into
  // buttonStateReg. Maybe we should have a flag variable that indicates a change in the buttons...
  buttonStateReg = currentButtonState; // Save the button state and then do something with it

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int main (void) {
    init_platform();
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
    		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

    while(1);  // Program never ends. But at least the lab does.

    cleanup_platform();

    return 0;
}
