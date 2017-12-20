/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "../include/GL_setup.h"

// test anim functions
void convolution_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void convolution_fn(Param &par);
void convolution_par(Param &par);
void convolution_OGL(Param &par);

#endif
