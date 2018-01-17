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

#include "../include/thanks.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// Test functions using shader.h
void thanks_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
    }

}

void thanks_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    glm::vec3 pos_text = {-.65, 0.8, 0};
    write_string(par, par.strings[0], pos_text, 1.0f, color);

    // Finding time
    //std::cout << curr_time(par) << '\n';

/*
    std::string massive_string = "";
    for (int i = 1; i < par.strings.size(); ++i){
        massive_string += par.strings[i] + " | ";
    }
    double offset = 0;
    offset = curr_time(par) * 0.25;
    pos_text[0] = 3*0.5 - offset;
    pos_text[1] = -0.8;
    write_string(par, massive_string, pos_text, 1.0f, color);
*/
    for (int i = 1; i < par.strings.size(); ++i){
        double offset = curr_time(par) * 0.25;
        pos_text[0] = i*0.25 - offset;
        if (i % 2 == 0){
            pos_text[0] -= 0.25;
            pos_text[1] = -0.9;
        }
        else{
            pos_text[1] = -0.8;
        }
        if (pos_text[0] > -2 && pos_text[0] < 2){
            write_string(par, par.strings[i], pos_text, 1.0f, color);
        }
    }
/*
    pos_text[0] -= 0.25;
    pos_text[1] -= 0.2;
    int half = par.strings.size() / 2;
    for (int i = 1; i < par.strings.size(); ++i){
        if (i == half+1){
            pos_text[0] += 1;
            pos_text[1] = 0.6;
        }
        write_string(par, par.strings[i], pos_text, 1.0f, color);
        pos_text[1] -= 0.15;
    }
*/

    draw_shapes(par);
    SDL_GL_SwapWindow(par.screen);

}

void thanks_par(Param &par){
    par.dist = "euclid";
    par.end = 0;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void thanks_OGL(Param &par){
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

    par.strings = find_names("CONTRIBUTORS.md");

}
