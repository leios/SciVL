/*-------------shape_functions.cpp--------------------------------------------//
*
* Purpose: To keep all the functions for drawing different shapes for SciVL
*
*-----------------------------------------------------------------------------*/

#include "../include/shape_functions.h"
#include "../include/shaders.h"

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

void draw_square(Param &par){

    // this should use shaders...
    Shader defaultShader("shaders/default.vtx", "shaders/default.frg");

    double pos_x = par.dmap["pos_x"];
    double pos_y = par.dmap["pos_y"];
    double radius = par.dmap["radius"];

    // Creating our list of vertices
    GLfloat vertices[] = {
        pos_x - radius, pos_y - radius, 0.0f,
        pos_x - radius, pos_y + radius, 0.0f,
        pos_x + radius, pos_y + radius, 0.0f
        //pos_x + radius, pos_y - radius, 0.0f
    };

    // Creating vertex array and vertex buffer objects
    GLuint VAO, VBO;

    // Generating objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Binding the vertex aray object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    defaultShader.Use();
    //glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
