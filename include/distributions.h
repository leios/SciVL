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

// test shader functions
void pong_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void pong_fn(Param &par);
void pong_par(Param &par);
void pong_OGL(Param &par);

// test fft functions
void fourier_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void fourier_fn(Param &par);
void fourier_par(Param &par);
void fourier_OGL(Param &par);

// test pend functions
void platformer_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void platformer_fn(Param &par);
void platformer_par(Param &par);
void platformer_OGL(Param &par);

// test anim functions
void test_anim_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void test_anim_fn(Param &par);
void test_anim_par(Param &par);
void test_anim_OGL(Param &par);

// test anim functions
void verlet_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void verlet_fn(Param &par);
void verlet_par(Param &par);
void verlet_OGL(Param &par);

// test anim functions
void traverse_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void traverse_fn(Param &par);
void traverse_par(Param &par);
void traverse_OGL(Param &par);

// test anim functions
void euclid_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void euclid_fn(Param &par);
void euclid_par(Param &par);
void euclid_OGL(Param &par);

#endif
