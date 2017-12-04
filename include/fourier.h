/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef FOURIER_H
#define FOURIER_H

#include "../include/GL_setup.h"

// test fft functions
void fourier_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void fourier_fn(Param &par);
void fourier_par(Param &par);
void fourier_OGL(Param &par);

#endif
