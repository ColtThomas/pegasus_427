#include "bullets.h"
#include "screen.h"
#include "globals.h"
#include "bunkers.h"
#include "tank.h"
#include "aliens.h"
#include "saucer.h"

void alienHandler_tick() {
	if(!aliens_landed()){
//		bunkers_update(); // Need speed? Remove this. Eroding bunkers takes time. Order matters!
		aliens_update_position();
		//saucer_update();
	}

	// verify if the aliens are over the bunker
//	point_t testPoint = globals_getAlienBlockPosition();
//	xil_printf("\r\nblock pos: %d %d",testPoint.x,testPoint.y );


}
