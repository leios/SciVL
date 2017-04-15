/*-------------test_pong.cpp--------------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#include <glm/mat3x3.hpp>

#include "../include/test_pong.h"
#include "../include/shape_functions.h"

void play_pong(Param &par){

    // Update ball position
    par.dmap["pos_x"] += par.dmap["vel_x"] * par.dmap["timestep"];
    par.dmap["pos_y"] += par.dmap["vel_y"] * par.dmap["timestep"];

    // create a translation matrix
    glm::vec3 ball_vel = {par.dmap["vel_x"] * par.dmap["timestep"],
                          par.dmap["vel_y"] * par.dmap["timestep"], 0.0};
    move_shape(par.shapes[0], ball_vel);

    // Dealing with boundary conditions
    if (par.dmap["pos_y"]  + par.dmap["radius"] > 1 || 
        par.dmap["pos_y"]  - par.dmap["radius"] < -1){
        par.dmap["vel_y"] = - par.dmap["vel_y"];
    }

    // Deal with paddles, paddle size is 1
    if (par.dmap["pos_x"]  + par.dmap["radius"] > 0.9){
        if (par.dmap["pos_y"] < par.dmap["rbumper"] + 0.5 &&
            par.dmap["pos_y"] > par.dmap["rbumper"] - 0.5){
            std::cout << par.dmap["pos_y"] << '\t' 
                      << par.dmap["rbumper"] << '\n';
            par.dmap["vel_x"] = - 2.0 * par.dmap["vel_x"];
        }
        else{
            std::cout << "Red wins!" << '\n';
            par.end = 1;
        }
    } 
    else if (par.dmap["pos_x"]  - par.dmap["radius"] < -0.9){
        if (par.dmap["pos_y"] < par.dmap["lbumper"] + 0.5 &&
            par.dmap["pos_y"] > par.dmap["lbumper"] - 0.5){
            std::cout << par.dmap["pos_y"] << '\t' 
                      << par.dmap["lbumper"] << '\n';
            par.dmap["vel_x"] = - 2.0 * par.dmap["vel_x"];
        }
        else{
            std::cout << "Blue wins!" << '\n';
            par.end = 1;
        }
    }

}
