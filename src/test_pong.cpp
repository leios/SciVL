/*-------------test_pong.cpp--------------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#include "../include/test_pong.h"

void play_pong(Param &par){

    // Update ball position
    par.dmap["pos_x"] += par.dmap["vel_x"] * par.dmap["timestep"];
    par.dmap["pos_y"] += par.dmap["vel_y"] * par.dmap["timestep"];

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
            par.dmap["vel_x"] = - 1.05 * par.dmap["vel_x"];
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
            par.dmap["vel_x"] = - 1.05 * par.dmap["vel_x"];
        }
        else{
            std::cout << "Blue wins!" << '\n';
            par.end = 1;
        }
    }
}
