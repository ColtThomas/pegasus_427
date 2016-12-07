/*
 * pitiful.h
 *
 *  Created on: Nov 3, 2016
 *      Author: superman
 */
#include<stdint.h>
#include<stdbool.h>

#ifndef PITIFUL_H_
#define PITIFUL_H_

#define PITIFUL_COUNTER_ENABLE 0x01
#define PITIFUL_INTERRUPT_ENABLE 0x02
#define PITIFUL_COUNTER_RELOAD 0x04

typedef struct {
	uint32_t BaseAddress;	/* Device base address */
//	u32 IsReady;		/* Device is initialized and ready */
	uint32_t InterruptPresent;	/* Are interrupts supported in h/w */
//	int IsDual;		/* Are 2 channels supported in h/w */
} pitiful_t;

void pitiful_initialize(pitiful_t *p, uint32_t baseAddress);

// each set function returns a status code:
// 0 if unsuccessful
// 1 if successful

bool pitiful_counter_enable(pitiful_t *p);
bool pitiful_counter_disable(pitiful_t *p);
bool pitiful_interrupt_enable(pitiful_t *p);
bool pitiful_interrupt_disable(pitiful_t *p);
bool pitiful_counter_reload_enable(pitiful_t *p);
bool pitiful_counter_reload_disable(pitiful_t *p);

bool pitiful_set_delay(pitiful_t *p, uint32_t delay);

// these return 0 if not enabled, 1 if enabled
bool pitiful_counter_enabled(pitiful_t *p);
bool pitiful_interrupt_enabled(pitiful_t *p);
bool pitiful_counter_reloading(pitiful_t *p);
// returns the current delay value
uint32_t pitiful_get_delay(pitiful_t *p);

#endif /* PITIFUL_H_ */
