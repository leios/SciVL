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
void convolution_key(Param &par, SDL_Keysym* Keysym, bool is_down){
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

void convolution_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void convolution_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    par.dmap["res"] = 100;

}

void convolution_OGL(Param &par){
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
    Shape line;
    std::vector<glm::vec3> array(2);

    double ypos = 0.6;
    for (int i = 0; i < 3; ++i){
        array[0] = {-0.9, ypos, 0.0};
        array[1] = {0.9, ypos, 0.0};
        ypos -= 0.6;
    
        glm::vec3 licolor = {1.0, 1.0, 1.0};
    
        create_line(line, array, licolor);
    
        add_keyframes(par, line, 0, 1);
        par.shapes.push_back(line);
    }

    int n = 128;
    std::vector<glm::vec3> conv_arr(n);
    ypos = 0.66;
    for (int i = 0; i < 3; ++i){
        for (int i = 0; i < n; ++i){
            conv_arr[i][0] = -0.9 + i*1.8/(n-1);
            conv_arr[i][1] = ypos;
            conv_arr[i][2] = 0;
        }
        ypos -= 0.6;
        glm::vec3 licolor = {1.0, 1.0, 1.0};
    
        create_line(line, conv_arr, licolor);
    
        par.shapes.push_back(line);

    }   

}
