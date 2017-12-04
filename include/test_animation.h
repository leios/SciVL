/*-------------distributions.h------------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#ifndef TEST_ANIM_H
#define TEST_ANIM_H

#include "../include/GL_setup.h"

// test anim functions
void test_anim_key(Param &par, SDL_Keysym* Keysym, bool is_down);
void test_anim_fn(Param &par);
void test_anim_par(Param &par);
void test_anim_OGL(Param &par);

#endif
