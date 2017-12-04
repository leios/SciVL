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

#include "../include/bits.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// Test functions using shader.h
void bits_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_f:
            if (is_down){
                par.bmap["is_int"] = false;
            }
            break;
        case SDLK_i:
            if (is_down){
                par.bmap["is_int"] = true;
            }
            break;
        case SDLK_c:
            if (is_down){
            }
            break;
        case SDLK_LEFT:{
            if(is_down){
                if (par.bmap["is_int"]){
                    int index = par.shapes.size() - 1;
                    par.factors[0] = (int)par.factors[0] << 1;
                    par.shapes[index].draw = true;
                }
            }
            else{
                par.shapes[par.shapes.size() -1].draw = false;
            }
            break;
        }
        case SDLK_RIGHT:{
            if(is_down){
                if (par.bmap["is_int"]){
                    par.factors[0] = (int)par.factors[0] >> 1;
                    int index = par.shapes.size() - 1;
                    glm::vec3 vloc = vertex_location(par.shapes[index], 0);
                    if (vloc[0] < 0){
                        glm::vec3 translate = {1.6, 0, 0};
                        move_shape(par.shapes[index], translate);
                        par.shapes[index].draw = true;
                    }
                }
            }
            else{
                glm::vec3 translate = {-1.6, 0, 0};
                move_shape(par.shapes[par.shapes.size() -1], translate);
                par.shapes[par.shapes.size() -1].draw = false;
            }
            break;
        }
        case SDLK_DOWN:{
            if(is_down){
                if (par.bmap["is_int"]){
                    par.factors[0] -= 1;
                }
                else{
                    par.factors[0] -= 0.1;
                }
            }
            break;
        }
        case SDLK_UP:{
            if(is_down){
                if (par.bmap["is_int"]){
                    par.factors[0] += 1;
                }
                else{
                    par.factors[0] += 0.1;
                }
            }
            break;
        }

        default:
            break;

    }

}

void bits_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 color1 = {1.0f, 1.0f, 1.0f};
    glm::vec3 color2 = {0.5f, 0.0f, 0.5f};
    glm::vec3 color3 = {0.25f, 0.25f, 1.0f};

    draw_shapes(par);
    write_bits(par);
    glm::vec3 pos_text = {-0.5f, -.7f, 0.0f};

    write_string(par, "Value is: ", pos_text, .5f, color1);

    pos_text[0] += 0.5;

    if (par.bmap["is_int"]){
        pos_text[0] = -0.9;
        pos_text[1] -= 0.001;
        write_string(par, "<<", pos_text, 1.0f, color1);
        pos_text[0] = 0.7;
        write_string(par, ">>", pos_text, 1.0f, color1);
        pos_text[1] += 0.001;
        pos_text[0] = 0;
        write_string(par, std::to_string((int)par.factors[0]), 
                     pos_text, .5f, color1);
    }
    else{
        write_string(par, std::to_string(par.factors[0]), 
                     pos_text, .5f, color1);
    }

    pos_text = {-1.0f, -0.99f, 0.0f};
    write_string(par, "f -- floats; i -- ints; up -- value up; down -- value down; left -- bitshift left; right -- bitshift right", 
                 pos_text, 0.25f, color1);

    SDL_GL_SwapWindow(par.screen);

}

void bits_par(Param &par){
    par.dist = "bits";
    par.end = 0;

    par.bmap["is_int"] = true;

    par.factors.push_back(0);
    par.imap["gcd"] = 3;
    par.curr_factor = 0;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 16;

}

void bits_OGL(Param &par){
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

    Shape line;

    // Creating the box around the text for our current factor that we are using
    std::vector<glm::vec3> box(6);
    box[0] = {-0.92, -0.69, 0.0};
    box[1] = {-0.92, -0.59, 0.0};
    box[2] = {-0.68, -0.59, 0.0};
    box[3] = {-0.68, -0.73, 0.0};
    box[4] = {-0.92, -0.73, 0.0};
    box[5] = {-0.92, -0.69, 0.0};

    glm::vec3 box_color = {1.0, 1.0, 1.0};
    create_line(line, box, box_color);
    line.draw = false;
    par.shapes.push_back(line);

}
