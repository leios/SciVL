/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef TRAVERSE_H
#define TRAVERSE_H

#include "../include/GL_setup.h"

// STD functions

// test anim functions
void traverse_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void traverse_fn(Param &par);
void traverse_par(Param &par);
void traverse_OGL(Param &par);

#endif
