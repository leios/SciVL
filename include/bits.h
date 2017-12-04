/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef BITS_H
#define BITS_H

#include "../include/GL_setup.h"

// test anim functions
void bits_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void bits_fn(Param &par);
void bits_par(Param &par);
void bits_OGL(Param &par);

#endif
