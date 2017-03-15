/*-------------shape_functions.cpp--------------------------------------------//
*
* Purpose: To keep all the functions for drawing different shapes for SciVL
*
*-----------------------------------------------------------------------------*/

#include "../include/shape_functions.h"

// function to draw a circle, must have a radius and position
void draw_circle(Param &par){

    // Pull appropriate variables from parameter list
    double radius = par.dmap["radius"];
    double pos_x = par.dmap["pos_x"];
    double pos_y = par.dmap["pos_y"];

    int res = par.imap["res"];

    // Using immediate mode, drawing a circle is easy using parametric eqns

    glBegin(GL_POLYGON);
    double theta = 0;
    for (int i = 0; i < res; i++){
        theta = -M_PI + 2*M_PI*i/(res-1);
        glVertex2f(pos_x + radius * cos(theta), pos_y + radius * sin(theta));
    }

    glEnd();
    
}
