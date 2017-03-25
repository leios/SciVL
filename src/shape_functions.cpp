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

void draw_square(Param &par, ){

    double pos_x = par.dmap["pos_x"];
    double pos_y = par.dmap["pos_y"];
    double radius = par.dmap["radius"];

/*
    GLuint VAO = par.dmap["VAO"];
    GLuint EBO = par.dmap["EBO"];
    GLuint VBO = par.dmap["VBO"];
*/

    // Generating objects
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    GLfloat vertices[] = {
        pos_x - radius, pos_y - radius, 0.0f, 1.0f, 0.0f, 1.0f,
        pos_x - radius, pos_y + radius, 0.0f, 1.0f, 0.0f, 1.0f,
        pos_x + radius, pos_y + radius, 0.0f, 1.0f, 0.0f, 1.0f,
        pos_x + radius, pos_y - radius, 0.0f, 1.0f, 0.0f, 1.0f
    };

    // List of indices to be bound to element buffer
    GLuint indices[] = {
        0, 1, 3,
        2, 1, 3
    };

    // Binding necessary elements
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    par.uimap["VAO"] = VAO;
    par.uimap["VBO"] = VBO;
    par.uimap["EBO"] = EBO;


}
