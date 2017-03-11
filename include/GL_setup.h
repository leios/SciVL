/*-------------GL_setup.h-----------------------------------------------------//
*
* Purpose: This holds all functions for setting up OpenGL with SDL
*
*-----------------------------------------------------------------------------*/

#ifndef GL_SETUP_H
#define GL_SETUP_H

// GLEW must be defined first
#define GLEW_STATIC
#include <GL/glew.h>


#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <string>
#include <unordered_map>

// struct to hold most data
struct Param{
    std::unordered_map<std::string, int> imap;
    int end, width, height;
    std::string dist;

    SDL_Surface* screen;

    typedef void (*functionPtr)(Param&);
    std::unordered_map<std::string, functionPtr> draw_fns;
    std::unordered_map<std::string, functionPtr> par_fns;
    std::unordered_map<std::string, functionPtr> OGL_fns;

    typedef void (*functionPtr_key)(Param&, SDL_keysym*);
    std::unordered_map<std::string, functionPtr_key> key_fns;

    // function to set all mapped functions
    void set_fns();

};

// Function to set parameters
Param set_params(std::string dist);

// OGL set-up function
void setup_OGL(Param &par);

// SDL initialization
void SDL_init(Param &par);

// Drawing to screen
void draw_screen(Param &par);

// Dealing with key presses
void key_down(Param &par, SDL_keysym* keysym);

// Processing with events
void process_events(Param &par);

#endif
