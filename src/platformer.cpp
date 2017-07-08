/*------------platformer.cpp--------------------------------------------------//
*
* Purpose: to hold all functions for a platformer game
*
*-----------------------------------------------------------------------------*/

#include <glm/mat3x3.hpp>
#include <glm/glm.hpp>
#include <fftw3.h>
#include "SDL.h"

#include "../include/shape_functions.h"
#include "../include/operations.h"
#include "../include/platformer.h"

// function to play the platformer game
void play_platformer(Param &par){
    // For this we will be using the verlet algorithm
    double y_prev = par.dmap["y_prev"];
    double y = par.dmap["y"];
    double dt = par.dmap["timestep"];
    double ymin = -1.0;
    double xmin = par.dmap["platf_xc"] - 0.5 * par.dmap["platf_l"];
    double xmax = par.dmap["platf_xc"] + 0.5 * par.dmap["platf_l"];
    double g;
    // we're tracking the position of the head
    if (par.dmap["x"] > xmin && par.dmap["x"] < xmax){
        ymin = -1 + par.dmap["platf_h"];
        if (par.dmap["y"] < ymin + 0.5) {
            glm::vec3 trans = {0, - par.dmap["y"] + (ymin + 0.5), 0.0};
            move_shape(par.shapes[1], trans);
            move_shape(par.shapes[0], trans);
            par.dmap["y"] = ymin + 0.5;
        }
    }
    else {
        ymin = -1.0;
    }
    if (par.dmap["y"] > ymin + 0.5) {

        g = par.dmap["grav"];
    }
    else {
        g = 0;
        if (par.dmap["yvel"] < 0) {
            par.dmap["yvel"] = 0;
        }
    }
    double yvel  = par.dmap["yvel"];

    double dy = yvel*dt + 0.5*g*dt*dt;
    yvel += g*dt;
    par.dmap["y"] += dy;
    par.dmap["yvel"] = yvel;


    //now we need to move the appropriate indices, 12, 13, 18, 19

    glm::vec3 trans = {0.0,dy, 0.0};
    move_shape(par.shapes[0], trans);
    move_shape(par.shapes[1], trans);
    // now we are going to move the character left and right
    //Uint8 *keystate = SDL_GetKeyState(NULL);
    //if (keystate[SDLK_LEFT]){
    if (par.bmap["mv_left"] == true){
        glm::vec3 trans = {-0.01, 0.0, 0.0};
        move_shape(par.shapes[1], trans);
        move_shape(par.shapes[0], trans);
        par.dmap["x"] -= 0.01;
    }

    if (par.bmap["mv_right"] == true){
        glm::vec3 trans = {0.01, 0.0, 0.0};
        move_shape(par.shapes[1], trans);
        move_shape(par.shapes[0], trans);
        par.dmap["x"] += 0.01;
    }
}
