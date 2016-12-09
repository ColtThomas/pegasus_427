/*
 * arduino.h
 *
 *  Created on: Nov 21, 2016
 *      Author: superman
 */

#include<stdint.h>
#include<stdbool.h>

#ifndef ARDUINO_H_
#define ARDUINO_H_

typedef struct {
	uint32_t BaseAddress;	/* Device base address */
	uint32_t InterruptPresent;	/* Are interrupts supported in h/w */
} arduino_t;

void arduino_initialize(arduino_t *p, uint32_t baseAddress);
// returns the current delay value
uint32_t arduino_get_pmod(arduino_t *p);
uint32_t arduino_get_switches(arduino_t *p);


#endif /* ARDUINO_H_ */
