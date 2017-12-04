/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef DIST_H
#define DIST_H

#include "../include/GL_setup.h"

// STD functions
void std_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void std_fn(Param &par);
void std_par(Param &par);
void std_OGL(Param &par);

// test pend functions
void platformer_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void platformer_fn(Param &par);
void platformer_par(Param &par);
void platformer_OGL(Param &par);

#endif
