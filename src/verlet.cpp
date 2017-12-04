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

#include "../include/verlet.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"
#include "../include/platformer.h"

// Test functions using shader.h
void verlet_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_SPACE:
            if (is_down){
                par.bmap["move"] = true;
            }
            break;
        case SDLK_UP:
            if (is_down){
                par.bmap["move_up"] = true;
            }
            else{
                par.bmap["move_up"] = false;
            }
            break;
        case SDLK_DOWN:
            if (is_down){
                par.bmap["move_down"] = true;
            }
            else{
                par.bmap["move_down"] = false;
            }
            break;
        case SDLK_RIGHT:
           if (is_down){
               glm::vec3 trans = {0.1, 0, 0};
               par.v3map["vel"] += trans;
           }
           break;
        case SDLK_LEFT:
           if (is_down){
               glm::vec3 trans = {0.1, 0, 0};
               par.v3map["vel"] -= trans;
           }
           break;
        default:
            break;

    }

}

void verlet_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    move_verlet_obj(par);
    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void verlet_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    par.dmap["res"] = 100;
    par.dmap["dt"] = 0.005;

    par.bmap["move"] = false;
    par.bmap["move_up"] = false;
    par.bmap["move_down"] = false;

    par.positions.push_back({1.0, 1.0, 10.0});
    par.positions.push_back({1.0, -1.0, 10.0});
    par.positions.push_back({-1.0, -1.0, 2.0});
    par.positions.push_back({-1.0, 0.5, 1.0});

    par.v3map["prev_p"] = par.positions[par.positions.size() - 1];
    par.v3map["vel"] = {0,0,0};
    par.v3map["temp"] = {-1.0, 0.5, 0.0};
    par.v3map["acc"] = {0, 0, 0};

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void verlet_OGL(Param &par){
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
    Shape line, circle;
    std::vector<glm::vec3> array(4);

    array = par.positions;
    for (size_t i = 0; i < par.positions.size(); ++i){
        array[i][2] = 0.0f;
    }

    std::vector<glm::vec3> carray(4);
    carray[0] = {0.5, 0.0, 0.5};
    carray[1] = {0.0, 0.5, 0.5};
    carray[2] = {0.0, 0.0, 0.5};
    carray[3] = {1.0, 1.0, 1.0};

    // Working with the circle
    create_circle(circle, array[0], 0.5, carray[0], par.dmap["res"]);
    add_keyframes(par, circle, 0, 0.25);
    par.shapes.push_back(circle);

    create_circle(circle, array[1], 0.3, carray[1], par.dmap["res"]);
    add_keyframes(par, circle, 0.25, 0.5);
    par.shapes.push_back(circle);

    create_circle(circle, array[2], 1, carray[2], par.dmap["res"]);
    add_keyframes(par, circle, 0.5, 0.75);
    par.shapes.push_back(circle);

    create_circle(circle, array[3], 0.1, carray[3], par.dmap["res"]);
    add_keyframes(par, circle, 0.75, 1.0);
    par.shapes.push_back(circle);

}
