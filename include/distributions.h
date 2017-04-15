/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef DIST_H
#define DIST_H

#include "../include/GL_setup.h"

// STD functions
void std_key(Param &par, SDL_Keysym* Keysym);
void std_fn(Param &par);
void std_par(Param &par);
void std_OGL(Param &par);

// Example functions
void example_key(Param &par, SDL_Keysym* Keysym);
void example_fn(Param &par);
void example_par(Param &par);
void example_OGL(Param &par);

// STD functions
void test_key(Param &par, SDL_Keysym* Keysym);
void test_fn(Param &par);
void test_par(Param &par);
void test_OGL(Param &par);

// STD functions
void test_shader_key(Param &par, SDL_Keysym* Keysym);
void test_shader_fn(Param &par);
void test_shader_par(Param &par);
void test_shader_OGL(Param &par);

#endif
