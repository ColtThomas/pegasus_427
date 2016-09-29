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

void bullets_fire_tank();
void bullets_fire_aliens();
void bullets_update_position();
void bullets_erase_tank_bullet();
void bullets_draw_tank_bullet();
void bullets_draw_alien_bullet(unsigned char bullet,unsigned int type);
void bullets_erase_alien_bullet(unsigned char bullet,unsigned int type);
#endif /* BULLETS_H_ */


