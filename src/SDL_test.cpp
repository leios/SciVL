/*-------------SDL_test.cpp---------------------------------------------------//
*
* Purpose: this file intends to quickly test how SDL works and will become the 
*          basis of I/O for the SciVL library
*
*-----------------------------------------------------------------------------*/

#include "../include/GL_setup.h"
#include "../include/shaders.h"

int main(){

    Param par = set_params("test_shader");
    SDL_init(par);

    // Defining OGL stuff
    setup_OGL(par);

    // this should use shaders...
    Shader defaultShader("shaders/default.vtx", "shaders/default.frg");

    double pos_x = par.dmap["pos_x"];
    double pos_y = par.dmap["pos_y"];
    double radius = par.dmap["radius"];

    // Creating our list of vertices
/*
    GLfloat vertices[] = {
        pos_x - radius, pos_y - radius, 0.0f,
        pos_x - radius, pos_y + radius, 0.0f,
        pos_x + radius, pos_y + radius, 0.0f
        //pos_x + radius, pos_y - radius, 0.0f
    };
*/
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!par.end){
        process_events(par);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        SDL_GL_SwapBuffers();
        
        //draw_screen(par);

    }

    // Clean-up
    SDL_Quit();

}
