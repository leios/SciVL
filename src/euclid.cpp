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

#include "../include/euclid.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"


// Test functions using shader.h
void euclid_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_s:
            if (is_down){
                euclid_clear(par);
                euclid_sub(par, par.factors[0], par.factors[1]);
            }
            break;
        case SDLK_m:
            if (is_down){
                euclid_clear(par);
                euclid_mod(par, par.factors[0], par.factors[1]);
            }
            break;
        case SDLK_c:
            if (is_down){
                euclid_clear(par);
            }
            break;
        case SDLK_LEFT:{
            if(is_down){
                par.factors[par.curr_factor] -= 1;
            }
            break;
        }
        case SDLK_RIGHT:{
            if(is_down){
                par.factors[par.curr_factor] += 1;
            }
            break;
        }
        case SDLK_DOWN:{
            if(is_down){
                if (par.curr_factor < 1){
                    par.curr_factor += 1;
                    if (par.curr_factor > par.factors.size()-1){
                        par.factors.push_back((double)par.curr_factor + 1);
                    }
                    glm::vec3 trans = {0.0, -0.15, 0.0};
                    move_shape(par.shapes[0], trans);
                }
            }
            break;
        }
        case SDLK_UP:{
            if(is_down){
                if (par.curr_factor > 0){
                    par.curr_factor -= 1;
                    glm::vec3 trans = {0.0, 0.15, 0.0};
                    move_shape(par.shapes[0], trans);
                }
            }
            break;
        }

        default:
            break;

    }

}

void euclid_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 color1 = {1.0f, 1.0f, 1.0f};
    glm::vec3 color2 = {0.5f, 0.0f, 0.5f};
    glm::vec3 color3 = {0.25f, 0.25f, 1.0f};

    draw_shapes(par);
    glm::vec3 pos_text = {-.35f, -.55f, 0.0f};
    write_string(par, "Euclidean Algorithm", 
                 pos_text, 1.0f, color1);

    pos_text = {-.1f, -0.7f, 0.0f};
    write_string(par, "A: "+std::to_string((int)par.factors[0]), 
                 pos_text, 1.0f, color2);

    pos_text = {-.1f, -0.85f, 0.0f};
    write_string(par, "B: "+std::to_string((int)par.factors[1]), 
                 pos_text, 1.0f, color3);

    pos_text = {-1.0f, -0.99f, 0.0f};
    write_string(par, "s - Subtraction-based method; m - Modulus-based method", 
                 pos_text, 0.5f, color1);

    pos_text = {0.5f, 0.9f, 0.0f};
    write_string(par, "GCD is: "+std::to_string((int)par.imap["gcd"]), 
                 pos_text, 1.0f, color1);

    SDL_GL_SwapWindow(par.screen);

}

void euclid_par(Param &par){
    par.dist = "euclid";
    par.end = 0;

    par.factors.push_back(3*8);
    par.factors.push_back(3*9);
    par.imap["gcd"] = 3;
    par.curr_factor = 0;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void euclid_OGL(Param &par){
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

    // Creating line that stretches across screen
    Shape line;
    std::vector<glm::vec3> pos(2);
    glm::vec3 color = {1,1,1};

    // Creating the box around the text for our current factor that we are using
    std::vector<glm::vec3> box(6);
    box[0] = {-0.7, -0.7, 0.0};
    box[1] = {-0.7, -0.6, 0.0};
    box[2] = {0.7, -0.6, 0.0};
    box[3] = {0.7, -0.75, 0.0};
    box[4] = {-0.7, -0.75, 0.0};
    box[5] = {-0.7, -0.7, 0.0};

    glm::vec3 box_color = {1.0, 1.0, 1.0};
    create_line(line, box, box_color);
    par.shapes.push_back(line);

    pos[0] = {-1,-0.45,0};
    pos[1] = {1,-0.45,0};
    create_line(line, pos, color);
    par.shapes.push_back(line);
}
