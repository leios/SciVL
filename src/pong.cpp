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

#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"
#include "../include/pong.h"

// Test functions using shader.h
void pong_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    if (!is_down){
        return;
    }
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;

        case SDLK_UP:
            if (par.dmap["rbumper"] < 0.5){
                par.dmap["rbumper"] += 0.05;
                glm::vec3 trans = {0.0, 0.05, 0.0};
                move_shape(par.shapes[2], trans);
            }
            break;
        case SDLK_DOWN:
            if (par.dmap["rbumper"] > -0.5){
                par.dmap["rbumper"] -= 0.05;
                glm::vec3 trans = {0.0, -0.05, 0.0};
                move_shape(par.shapes[2], trans);
            }
            break;
        case SDLK_w:
            if (par.dmap["lbumper"] < 0.5){
                par.dmap["lbumper"] += 0.05;
                glm::vec3 trans = {0.0, 0.05, 0.0};
                move_shape(par.shapes[1], trans);
            }
            break;
        case SDLK_s:
            if (par.dmap["lbumper"] > -0.5){
                par.dmap["lbumper"] -= 0.05;
                glm::vec3 trans = {0.0, -0.05, 0.0};
                move_shape(par.shapes[1], trans);
            }
            break;
        default:
            break;

    }

}

void pong_fn(Param &par){

    play_pong(par);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    glm::vec3 pos = {20.0f, 20.0f, 0.0f};
    write_string(par, "sample text", pos, 1.0f, glm::vec3(0.5, 0.0f, 0.5f));

    SDL_GL_SwapWindow(par.screen);

}

void pong_par(Param &par){
    par.dist = "pong";
    par.end = 0;

    par.dmap["rbumper"] = 0.0;
    par.dmap["lbumper"] = 0.0;
    par.dmap["radius"] = 0.1;
    par.dmap["pos_x"] = 0.0;
    par.dmap["pos_y"] = 0.0;
    par.dmap["vel_y"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["vel_x"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["timestep"] = 0.05;
    par.imap["res"] = 100;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void pong_OGL(Param &par){
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

    // this should use shaders...
    Shader defaultShader;
    defaultShader.Load("shaders/default.vtx", "shaders/default.frg");
    par.shmap["default"] = defaultShader;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(10);

    glEnable(GL_POINT_SMOOTH);
    glPointSize(10);

    Shader textShader;
    textShader.Load("shaders/text.vtx", "shaders/text.frg");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(par.width), 
                                      0.0f, static_cast<GLfloat>(par.height));
    textShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 
                       1, GL_FALSE, glm::value_ptr(projection));
    par.shmap["text"] = textShader;
    setup_freetype(par);
    create_quad(par.text);

    Shape circle;
    float rad = (float)par.dmap["radius"];
    glm::vec3 cloc = {0.0, 0.0, 0.0},
              ccolor = {0.0, 1.0, 0.0};
    create_circle(circle, cloc, rad, ccolor, 100); 
    //rect = create_square(par);
    par.shapes.push_back(circle);

    Shape rect;
    glm::vec3 lloc = {-1.0, 0.0, 0.0}, 
              lsize = {0.2, 1.0, 0.0}, 
              lcolor = {0.0, 0.0, 1.0};
    create_rectangle(rect, lloc, lsize, lcolor); 
    par.shapes.push_back(rect);

    glm::vec3 rloc = {1.0, 0.0, 0.0}, 
              rsize = {0.2, 1.0, 0.0}, 
              rcolor = {1.0, 0.0, 0.0};
    create_rectangle(rect, rloc, rsize, rcolor); 
    par.shapes.push_back(rect);

}
