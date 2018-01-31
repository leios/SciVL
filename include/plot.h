/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef PLOT_H
#define PLOT_H

#include "../include/GL_setup.h"

// test anim functions
void plot_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void plot_fn(Param &par);
void plot_par(Param &par);
void plot_OGL(Param &par);

#endif
