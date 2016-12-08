#include<stdint.h>
#include "arduino.h"
#include "xil_io.h"
#include<stdbool.h>
#include<stdio.h>

// Initialization procedure for new arduino_t structs
void arduino_initialize(arduino_t *p, uint32_t baseAddress) {
	p->BaseAddress = baseAddress;
	p->InterruptPresent = true;
}


uint32_t arduino_get_pmod(arduino_t *p) {
	if(p == NULL) {
				return 0;
			}
		return Xil_In32(p->BaseAddress);
}

uint32_t arduino_get_switches(arduino_t *p) {
	if(p == NULL) {
				return 0;
			}
		return Xil_In32(p->BaseAddress+4);
}

