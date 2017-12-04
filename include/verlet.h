/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef VERLET_H
#define VERLET_H

#include "../include/GL_setup.h"
// test anim functions
void verlet_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void verlet_fn(Param &par);
void verlet_par(Param &par);
void verlet_OGL(Param &par);

#endif
