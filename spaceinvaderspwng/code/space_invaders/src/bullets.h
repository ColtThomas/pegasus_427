/*
 * bullets.h
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#ifndef BULLETS_H_
#define BULLETS_H_

#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

// fires bullet from tank
void bullets_fire_tank();

// fires a bullet from a random alien
void bullets_fire_aliens();

// moves all bullets one position. alien bullets move down, tank bullet moves up.
void bullets_update_position();

// clears the current tank bullet
void bullets_erase_tank_bullet();

// draws tank bullet in incremented position
void bullets_draw_tank_bullet();

// draws specified alien bullet in incremented position
void bullets_draw_alien_bullet(uint8_t bullet,uint32_t type);

// erases current specified alien bullet
void bullets_erase_alien_bullet(uint8_t bullet,uint32_t type);
#endif /* BULLETS_H_ */


