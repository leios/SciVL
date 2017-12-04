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

#include "../include/fft.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// Test functions using shader.h
void fft_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    if (!is_down){
        return;
    }
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:{
            par.end = 1;
            break;
        }
        case SDLK_b:
            if (is_down){
                break;
            }

        default:
            break;

    }

}

void fft_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    write_butterfly(par);

    SDL_GL_SwapWindow(par.screen);

}

void fft_par(Param &par){
    par.dist = "fft";
    par.end = 0;

    par.factors.push_back(1.0);
    par.imap["res"] = 100;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

    par.start_time = std::chrono::high_resolution_clock::now();

}

void fft_OGL(Param &par){
    glewExperimental = GL_TRUE;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
    line.rad = 0.002;
    std::vector<glm::vec3> array(2);
    array[0] = {-0.8,0.875,0};
    array[1] = {0.8,0.875,0};

    glm::vec3 licolor = {1.0, 1.0, 1.0};
    for (int i = 0; i < 8; ++i){
        create_line(line, array, licolor);
        add_keyframes(par, line, 1, 2);
        par.shapes.push_back(line);
        array[0][1] -= 0.25;
        array[1][1] -= 0.25;
    }

    int bnum = 4;
    double ypos = 0.875;
    double xpos = -0.75;
    double stride;
    for (int i = 0; i < 3; ++i){
        stride = 4 / bnum;
        std::cout << stride << '\n';

        for (int j = 0; j < bnum; ++j){
            for (int k = 0; k < stride; ++k){
                array[0] = {xpos, ypos, 0};
                array[1] = {xpos + 0.4, ypos - (0.25*stride), 0};
                create_line(line, array, licolor);
                add_keyframes(par, line, 1, 2);
                par.shapes.push_back(line);

                array[0] = {xpos, ypos - (0.25*stride), 0};
                array[1] = {xpos + 0.4, ypos, 0};
                create_line(line, array, licolor);
                add_keyframes(par, line, 1, 2);
                par.shapes.push_back(line);
                ypos -= 0.25;
            }

            ypos -= 0.25*stride;
        }

        xpos += 0.525;
        ypos = 0.875;
        bnum /= 2;
    }    
}
