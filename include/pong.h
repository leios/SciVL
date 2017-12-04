/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef PONG_H
#define PONG_H

#include "../include/GL_setup.h"

// test shader functions
void pong_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void pong_fn(Param &par);
void pong_par(Param &par);
void pong_OGL(Param &par);

#endif
