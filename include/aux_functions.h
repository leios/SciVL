/*-------------test_pong.h----------------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#ifndef AUX_FUNCTIONS_H
#define AUX_FUNCTIONS_H

#include "../include/GL_setup.h"

void play_pong(Param &par);

// Function to update fft arrays
void update_fft(Param &par);
void draw_fft(Param &par);

// Functions to move asteroid for verlet visualization
void find_verlet_acc(Param &par);
void move_verlet_obj(Param &par);

#endif
