/*-------------GL_setup.cpp---------------------------------------------------//
*
* Purpose: This holds all functions for setting up OpenGL to work with SDL
*
*-----------------------------------------------------------------------------*/

#include "../include/GL_setup.h"
#include "../include/distributions.h"


// Function to set parameters
Param set_params(std::string dist){
    Param par;
    par.set_fns();
    par.par_fns[dist](par);
    return par;
}

// function to set the drawing functions in the Param struct
void Param::set_fns(){
    draw_fns["std"] = std_fn;
    key_fns["std"] = std_key;
    par_fns["std"] = std_par;
    OGL_fns["std"] = std_OGL;

    draw_fns["example"] = example_fn;
    key_fns["example"] = example_key;
    par_fns["example"] = example_par;
    OGL_fns["example"] = example_OGL;

    draw_fns["test"] = test_fn;
    key_fns["test"] = test_key;
    par_fns["test"] = test_par;
    OGL_fns["test"] = test_OGL;

    draw_fns["test_shader"] = test_shader_fn;
    key_fns["test_shader"] = test_shader_key;
    par_fns["test_shader"] = test_shader_par;
    OGL_fns["test_shader"] = test_shader_OGL;
}

// Function to set-up OGL
void setup_OGL(Param &par){

    par.OGL_fns[par.dist](par);

}

// SDL initialization
void SDL_init(Param &par){
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Set the title bar
    SDL_WM_SetCaption("SDL Test", "SDL Test");

    // Create Window
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    int bpp = info->vfmt->BitsPerPixel;
    par.screen = SDL_SetVideoMode(par.width, par.height, bpp, 
                                           SDL_OPENGL);

    // Setting OGL specific values:
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


}

// Drawing to screen
void draw_screen(Param &par){

    par.draw_fns[par.dist](par);
}

void key_down(Param &par,  SDL_keysym* keysym){
    par.key_fns[par.dist](par, keysym);
}

// Function to process events in game loop
void process_events(Param &par){

    // Creating an event
    SDL_Event event;

    while( SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
            {
                par.end = 1;
                break;
            }
            case SDL_KEYDOWN:
            {
                key_down(par, &event.key.keysym);
                break;
            }
        }
    }
}
