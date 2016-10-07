/*
 * bulletHandler.h
 *
 *  Created on: Oct 7, 2016
 *      Author: superman
 */

#ifndef BULLETHANDLER_H_
#define BULLETHANDLER_H_

#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

// This will handle all button inputs and then update the bullets every tick. Also handles the
// collision of bullets and necessary communication of effects. These effects include:
// - Increment Score
// - Decrement Lives or tank hit flag
// - Returning the index of the killed alien? (we will see if we should handle it here.)
void bulletHandler_tick();

#endif /* BULLETHANDLER_H_ */
