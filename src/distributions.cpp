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

#include "../include/distributions.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"
#include "../include/platformer.h"

// STD functions
void std_key(Param &par, SDL_Keysym* Keysym, bool is_down){
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

void std_fn(Param &par){
    // OpenGL stuff
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glm::vec3 text_color = {1, 0, 0};
    glm::vec3 pos = {0, 0, 0};
    write_string(par, "whatever", pos, 1.0f, text_color);

    SDL_GL_SwapWindow(par.screen);

}

void std_par(Param &par){
    par.dist = "std";
    par.end = 0;

}

void std_OGL(Param &par){
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\n';
        exit(1);
    }

    glViewport(0,0,640,480);

}

// Test functions using shader.h
void platformer_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_SPACE:
            if (is_down){
                par.shapes[3].draw = true;
                std::chrono::duration<double> t;
                t = std::chrono::duration_cast<std::chrono::duration<double>>
                (par.curr_time - par.start_time);
                add_keyframes(par, par.shapes[3], t.count(), t.count()+1);
                par.dmap["shot_height"] = par.dmap["y"];

            }
            break;

        case SDLK_LEFT:
            if (is_down){
                if(par.shapes[1].vertices[0] - par.dmap["radius"] > -1){
                    par.bmap["mv_left"] = true;
                }
            }
            else{
                par.bmap["mv_left"] = false;
            }
            break;
        case SDLK_RIGHT:
            if (is_down){
                if(par.shapes[1].vertices[0] + par.dmap["radius"] < 1){
                    par.bmap["mv_right"] = true;
                }
            }
            else{
                par.bmap["mv_right"] = false;
            }
            break;
        case SDLK_UP:
            if (!is_down){
                return;
            }
            if(par.shapes[1].vertices[0] + par.dmap["radius"] < 1){
                par.dmap["yvel"] = 0.2;
            }
            break;
        default:
            break;

    }

}

void platformer_fn(Param &par){

    play_platformer(par);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    glm::vec3 pos = {-0.6f, 0.9f, 0.0f};
    write_string(par, "REAL", pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));

    SDL_GL_SwapWindow(par.screen);

}

void platformer_par(Param &par){
    par.dist = "platformer";
    par.end = 0;

    par.dmap["alpha"] = 0.0;
    par.dmap["grav"] = -0.05;
    par.dmap["xvel"] = 0.0;
    par.dmap["yvel"] = 0.0;
    par.dmap["x"] = 0.0;
    par.dmap["y"] = 0.0;
    par.dmap["y_prev"] = 0.0;
    par.dmap["theta"] = 0.0;
    par.dmap["theta_prev"] = 0.0;
    par.dmap["radius"] = 0.1;
    par.dmap["timestep"] = 0.05;
    par.imap["res"] = 50;
    par.bmap["mv_left"] = false;
    par.bmap["mv_right"] = false;
    par.dmap["platf_h"] = 0;
    par.dmap["platf_l"] = 0;
    par.dmap["laser_pos"] = 0 + par.dmap["radius"];
    par.dmap["shot_height"] = 0;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void platformer_OGL(Param &par){
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

    // Creating a line to work with
    Shape line;
    std::vector<glm::vec3> array(2);
    array[0] = {0.0, 0.0, 0.0};
    array[1] = {0.0, -0.5, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    // Creating a circle to work with
    Shape circle;
    double rad = par.dmap["radius"];
    glm::vec3 cloc = {0.0, 0.0, 0.0},
              ccolor = {1.0, 0.0, 1.0};
    create_circle(circle, cloc, rad, ccolor, par.imap["res"]); 
    par.shapes.push_back(circle);

    Shape platform;
    glm::vec3 ploc = {0.0, -0.9, 0.0}, 
              psize = {0.5, 0.2, 0.0}, 
              pcolor = {0.0, 0.0, 1.0};
    par.dmap["platf_h"] = psize[1];
    par.dmap["platf_l"] = psize[0];
    par.dmap["platf_xc"] = ploc[0];

    create_rectangle(platform, ploc, psize, pcolor); 
    par.shapes.push_back(platform);

    // Creating a laser to play with
    Shape laser;
    double offset = 0.05; 
    array[0] = {rad + offset, 0.0, 0.0};
    array[1] = {rad + offset*5, 0.0, 0.0};

    glm::vec3 lascolor = {0.0, 1.0, 1.0};

    create_line(laser, array, lascolor);
    laser.draw = false;
    par.shapes.push_back(laser);

}
