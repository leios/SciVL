/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef FFT_H
#define FFT_H

#include "../include/GL_setup.h"

// test fft functions
void fft_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void fft_fn(Param &par);
void fft_par(Param &par);
void fft_OGL(Param &par);

#endif
