/*-------------distributions.cpp----------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <fftw3.h>

#include "../include/test_animation.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// Test functions using shader.h
void test_anim_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    if (!is_down){
        return;
    }
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        default:
            break;

    }

}

void test_anim_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void test_anim_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    par.dmap["res"] = 100;

}

void test_anim_OGL(Param &par){
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\t' 
                  << glewGetErrorString(glewInit()) << '\n';
        exit(1);
    }

    glViewport(0,0,par.width,par.height);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // this should use shaders...
    Shader defaultShader;
    defaultShader.Load("shaders/default.vtx", "shaders/default.frg");
    par.shmap["default"] = defaultShader;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);

    glEnable(GL_POINT_SMOOTH);
    glPointSize(10);

    // Creating a simple line
    Shape line, circle, rect;
    std::vector<glm::vec3> array(4);

    array[0] = {0.0, 0.0, 0.0};
    array[1] = {0.5, -0.5, 0.0};
    array[2] = {0.5, 0.5, 0.0};
    array[3] = {-0.5, 0.5, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};
    glm::vec3 ccolor = {0.0, 0.0, 1.0};

    create_line(line, array, licolor);

    add_keyframes(par, line, 1, 7);
    glm::vec3 loc = {0,0,0};
    add_move_keyframe(par, line, loc, 7);
    loc = {1,1,0};
    add_move_keyframe(par, line, loc, 8);
    loc = {-1,-1,0};
    add_move_keyframe(par, line, loc, 12);
    par.shapes.push_back(line);

    // Working with the circle
    create_circle(circle, array[0], 0.25, ccolor, par.dmap["res"]);
    add_keyframes(par, circle, 7,8);

    par.shapes.push_back(circle);

    glm::vec3 rect_size = {0.25, 0.25, 0};
    glm::vec3 recolor = {0.0, 1.0, 0.0};
    create_rectangle(rect, array[1], rect_size, recolor); 
    add_keyframes(par, rect, 8,9);
    par.shapes.push_back(rect);

    // Creating a set of values to test an integral
    int length = 100;
    double *int_array;
    int_array = (double *)malloc(sizeof(double)*length);
    for (int i = 0; i < length; ++i){
        int_array[i] = sin(i*2*M_PI/(double)length);
    }

    Shape integral;
    create_integral(integral, int_array, length, {0,0.5,0}, {1,0.5,0}, {0,1,1});
    par.shapes.push_back(integral);
}
