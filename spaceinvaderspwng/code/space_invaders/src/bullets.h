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

uint32_t bullets_get_speed();

uint32_t bullets_get_height();


// fires bullet from tank
void bullets_fire_tank();

// fires a bullet from a random alien
void bullets_fire_aliens();

// moves all bullets one position. alien bullets move down, tank bullet moves up.
void bullets_update_position(); // Mind mid air collision with bullets

// Removes tank bullet and sets the globals to indicate likewise
void bullets_remove_tank_bullet();

// draws tank bullet in incremented position
void bullets_draw_tank_bullet();

// draws specified alien bullet in incremented position
void bullets_draw_alien_bullet(uint8_t bullet,uint32_t type);

// Removes alien bullet and sets the globals to indicate likewise
void bullets_remove_alien_bullet(uint8_t bullet);
#endif /* BULLETS_H_ */


