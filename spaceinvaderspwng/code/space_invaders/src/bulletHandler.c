#include "bullets.h"
#include "screen.h"
#include "globals.h"
#include "bunkers.h"



void bulletHandler_tick() {
uint32_t i;
point_t alienBullets[GLOBALS_NUMBER_OF_ALIEN_BULLETS],nextPos;
// We need to see if the bullet will move into a collision area
	for(i=0;i<GLOBALS_NUMBER_OF_ALIEN_BULLETS;i++){

		alienBullets[i] = globals_getAlienBulletPosition(i);		// Get the position and then increment to
		nextPos = alienBullets[i];									// simulate the next position. We want
		nextPos.y += bullets_get_speed();							// to see if the next bullet will collide

		// Check bunker areas and update on a hit
		if (bunkers_check_hit(nextPos)) {
//			xil_printf("\r\nHit detected!");

			// remove the bullet
			bullets_remove_alien_bullet(i);
			globals_setAlienBulletStatus(i,false);
		}
		// Check alien areas

	}





	// Check tank areas

	// Apply appropriate damage to bunkers; report alien hit or tank hit
	bunkers_update();


// Update the bullets
	bullets_update_position();

}
