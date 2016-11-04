/*
 * pitiful.c
 *
 *  Created on: Nov 3, 2016
 *      Author: superman
 */
#include<stdint.h>
#include "pitiful.h"
#include "xil_io.h"
#include<stdbool.h>
#include<stdio.h>


void pitiful_initialize(pitiful_t *p, uint32_t baseAddress) {
	p->BaseAddress = baseAddress;
	p->InterruptPresent = true;
}

bool pitiful_counter_enable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)|PITIFUL_COUNTER_ENABLE);
	return 1;
}
bool pitiful_counter_disable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)&~PITIFUL_COUNTER_ENABLE);
	return 1;

}
bool pitiful_interrupt_enable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	xil_printf("enable interrupt %x\r\n",p->BaseAddress);
	//uint32_t temp = Xil_In32(p->BaseAddress);
	xil_printf("read correct\r\n");
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)|PITIFUL_INTERRUPT_ENABLE);
	xil_printf("enabled.\r\n");
	return 1;
}
bool pitiful_interrupt_disable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)&~PITIFUL_INTERRUPT_ENABLE);
	return 1;
}
bool pitiful_counter_reload_enable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)|PITIFUL_COUNTER_RELOAD);
	return 1;
}
bool pitiful_counter_reload_disable(pitiful_t *p) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress, Xil_In32(p->BaseAddress)&~PITIFUL_COUNTER_RELOAD);
	return 1;
}

bool pitiful_set_delay(pitiful_t *p, uint32_t delay) {
	if(p == NULL) {
		return 0;
	}
	Xil_Out32(p->BaseAddress+4, delay);
	if(Xil_In32(p->BaseAddress+4) == delay) xil_printf("delay verified: %x\r\n",Xil_In32(p->BaseAddress+4));
	return 1;
}

bool pitiful_counter_enabled(pitiful_t *p) {
	if(p == NULL) {
			return 0;
		}
	uint32_t status = Xil_In32(p->BaseAddress);
	if(status & PITIFUL_COUNTER_ENABLE) return 1;
	else return 0;
}
bool pitiful_interrupt_enabled(pitiful_t *p) {
	if(p == NULL) {
				return 0;
			}
		uint32_t status = Xil_In32(p->BaseAddress);
		if(status & PITIFUL_INTERRUPT_ENABLE) return 1;
		else return 0;
}
bool pitiful_counter_reloading(pitiful_t *p) {
	if(p == NULL) {
				return 0;
			}
		uint32_t status = Xil_In32(p->BaseAddress);
		if(status & PITIFUL_COUNTER_RELOAD) return 1;
		else return 0;
}
uint32_t pitiful_get_delay(pitiful_t *p) {
	if(p == NULL) {
				return 0;
			}
		return Xil_In32(p->BaseAddress+4);
}
