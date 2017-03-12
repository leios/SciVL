/*-------------SDL_test.cpp---------------------------------------------------//
*
* Purpose: this file intends to quickly test how SDL works and will become the 
*          basis of I/O for the SciVL library
*
*-----------------------------------------------------------------------------*/

#include "../include/GL_setup.h"

int main(){

    Param par = set_params("test");
    SDL_init(par);

    // Defining OGL stuff
    setup_OGL(par);

    while (!par.end){
        process_events(par);

        draw_screen(par);
    }

    // Clean-up
    SDL_Quit();

}
