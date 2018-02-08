/*-------------stable_marriage.cpp--------------------------------------------//
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

/*
struct person{
    int ID, mate;
};

void propose(Param &par){
}

void animate_gale_shapley(Param &par){
}
*/

// Test functions using shader.h
void stable_key(Param &par, SDL_Keysym* Keysym, bool is_down){
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

void stable_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    double ypos = 0.725;
    glm::vec3 text_color = {1, 1, 1};

    SDL_GL_SwapWindow(par.screen);

}

void stable_par(Param &par){
    par.dist = "stable";
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

void stable_OGL(Param &par){
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

    Shape human;
    glm::vec3 loc = {-0.25, 0.85, 0};
    glm::vec3 color1 = {0.25, 0.25, 1};
    glm::vec3 color2 = {1, 0, 1};
    double dt = 0;
    for (int i = 0; i < 4; ++i){
        create_human(human, loc, color1, 0.075);
        add_keyframes(par, human, 1+dt,2+dt);
        par.shapes.push_back(human);

        dt += 0.5;

        loc[0] += 0.5;
        create_human(human, loc, color2, 0.075);
        add_keyframes(par, human, 1+dt,2+dt);
        par.shapes.push_back(human);
        dt += 0.5;

        loc[0] -= 0.5;
        loc[1] -= 0.5;
    }
    
}
