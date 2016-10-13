#include "bullets.h"
#include "screen.h"
#include "globals.h"
#include "bunkers.h"
#include "tank.h"
#include "aliens.h"
// Consider ticking the tank before the bullets in case the tank collides with the bullets
void bulletHandler_tick() {
uint32_t i;
point_t alienBullets[GLOBALS_NUMBER_OF_ALIEN_BULLETS],nextPos,clear;
clear.x = 0;
clear.y = 0;
bool bulletIsAlien;
// We need to see if the bullet will move into a collision area
	for(i=0;i<GLOBALS_NUMBER_OF_ALIEN_BULLETS;i++){
		bulletIsAlien = true;
		alienBullets[i] = globals_getAlienBulletPosition(i);		// Get the position and then increment to
		nextPos = alienBullets[i];									// simulate the next position. We want
		nextPos.y += bullets_get_speed();							// to see if the next bullet will collide

		// Check bunker areas and update on a hit
		if (bunkers_check_hit(nextPos,bulletIsAlien)) {
//			xil_printf("\r\nHit detected!");

			// remove the bullet
			bullets_remove_alien_bullet(i);
			globals_setAlienBulletStatus(i,false);
		}
		// Check tank
		if(tank_check_hit(nextPos)) {
				xil_printf("\r\nTank Hit");

				// remove the bullet
				bullets_remove_alien_bullet(i);
				globals_setAlienBulletStatus(i,false);
		}
	}





	// Check tank areas
	nextPos = globals_getTankBulletPosition();
	bulletIsAlien = false;
	if (bunkers_check_hit(nextPos,bulletIsAlien)) {
//		xil_printf("\r\nHit detected!");

		// remove the bullet
		bullets_remove_tank_bullet();
		globals_setAlienBulletStatus(i,false);
		globals_setTankBulletPosition(clear);
	}

	//check alien hit
	if(aliens_check_hit(nextPos)) {
		bullets_remove_tank_bullet();
		globals_setAlienBulletStatus(i,false);
		globals_setTankBulletPosition(clear);
	}
	// Apply appropriate damage to bunkers; report alien hit or tank hit
	bunkers_update();


// Update the bullets
	bullets_update_position();

}
