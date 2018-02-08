/*-------------plot.cpp-------------------------------------------------------//
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
void plot_key(Param &par, SDL_Keysym* Keysym, bool is_down){
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

void plot_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    double ypos = 0.725;
    glm::vec3 text_color = {1, 1, 1};

    SDL_GL_SwapWindow(par.screen);

}

void plot_par(Param &par){
    par.dist = "plot";
    par.end = 0;

    // Creating signals to work with
    int n = 128;
    std::vector<double> signal(n);
    for (int i = 0; i < n; ++i){
        signal[i] = sin(2*M_PI*cos(8*M_PI*i/n)/n);
        if (signal[i] > 8){
            signal[i] = 8;
        }
        else if(signal[i] < -8){
            signal[i] = -8;
        }
    }
    normalize(signal.data(),n);
    par.vdmap["signal"] = signal;

    par.imap["res"] = n;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 16;

}

void plot_OGL(Param &par){
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
    std::vector<glm::vec3> array(2);

    glm::vec3 color = {0.5, 0.5, 0.5};
    array[0] = {-0.9, 0.9, 0};
    array[1] = {-0.9, -0.9, 0};

    create_line(line, array, color);
    add_keyframes(par, line, 1, 2);
    par.shapes.push_back(line);

    array[0] = {-0.9, 0, 0};
    array[1] = {0.9, 0, 0};

    create_line(line, array, color);
    add_keyframes(par, line, 1, 2);
    par.shapes.push_back(line);

    int n = par.imap["res"];
    std::vector<double> signal = par.vdmap["signal"];
    std::vector<glm::vec3> v3signal(n);

    color = {1,1,1};
    for (int i = 0; i < n; ++i){
        v3signal[i][0] = -0.9 + i*1.8 / n;
        v3signal[i][1] = signal[i] * 0.9;
        v3signal[i][2] = -0.9 + i*1.8 / n;
    }

    create_line(line, v3signal, color);
    add_keyframes(par, line, 2, 3);
    par.shapes.push_back(line);
}
