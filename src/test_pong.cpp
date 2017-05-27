/*-------------test_pong.cpp--------------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#include <glm/mat3x3.hpp>
#include <glm/glm.hpp>
#include <fftw3.h>

#include "../include/test_pong.h"
#include "../include/shape_functions.h"
#include "../include/operations.h"

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

// function to swing pendulum
void move_pendulum(Param &par){

    // For this we will be using the verlet algorithm
    double y_prev = par.dmap["y_prev"];
    double y = par.dmap["y"];
    double dt = par.dmap["timestep"];
    double g;
    
    if (par.dmap["y"] >= -0.5) {
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


}

// Function to update fft arrays
void update_fft(Param &par){

    int res = par.imap["res"];

    // Creating the two arrays for plotting 
    fftw_complex *wave, *ftwave;
    wave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);
    ftwave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);

    // Creating the plan for fft'ing
    fftw_plan plan;
    for (int i = 0; i < res; ++i){
        for (size_t j = 0; j < par.factors.size(); ++j){
            if (j == 0){
                wave[i][0] = sin(par.factors[0]*2*M_PI*i/(double)res);
                wave[i][1] = 0;
            }
            else{
                wave[i][0] += sin(par.factors[j]*2*M_PI*i/(double)res);
            }
        }
    }

    normalize(wave, res);

    // Performing fft
    plan = fftw_plan_dft_1d(res, wave, ftwave, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    fftw_destroy_plan(plan);

    normalize(ftwave, res);

    // now creating a sinusoidal wave
    glm::vec3 *sinarr;
    glm::vec3 *fftarr;
    sinarr = (glm::vec3*)malloc(sizeof(glm::vec3)*res);
    fftarr = (glm::vec3*)malloc(sizeof(glm::vec3)*res / 2);
    for (int i = 0; i < res; ++i){
        sinarr[i].x = -0.95 + 0.9 * (double)i / res;
        sinarr[i].y = (wave[i][0]) * 0.5 * 0.9 + 0.4;
        sinarr[i].z = 0;
        if (i < res / 2){
            fftarr[i].x = 0.05 + 0.9 * 2 * (double)i / res;
            fftarr[i].y = (abs2(ftwave[i])) * 0.5 * 0.9 + 0.4;
            fftarr[i].z = 0;
        }
    }
    update_line(par.shapes[par.shapes.size()-2], sinarr);
    update_line(par.shapes[par.shapes.size()-1], fftarr);

}

// function to draw all necessary fft visualizations to screen
void draw_fft(Param &par){
    glm::vec3 pos = {-0.6f, 0.9f, 0.0f};
    write_string(par, "REAL", pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));
    pos = {0.2f, 0.9f, 0.0f};
    write_string(par, "FREQUENCY", pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));

    // Writing out all frequencies
    for (size_t i = 0; i < par.factors.size(); ++i){
        pos = {-0.4, -0.4 - (double)i*0.2, 0.0};
        write_string(par,"FREQUENCY " +std::to_string(i) + " is : "
                         +std::to_string(par.factors[i]),
                     pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));
    }

}
