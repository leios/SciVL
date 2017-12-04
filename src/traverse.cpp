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

#include "../include/traverse.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// Test functions using shader.h
void traverse_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_d:
            if (is_down){
                double ctime = curr_time(par);
                DFS_recursive(par, par.tree, ctime);
                break;
            }
        case SDLK_b:
            if (is_down){
                double ctime = curr_time(par);
                BFS_queue(par, par.tree, ctime);
                break;
            }
            break;
        default:
            break;

    }

}

void traverse_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void traverse_par(Param &par){
    par.dist = "traverse";
    par.end = 0;

    int row_num = 2;
    int child_num = 3;

    par.imap["row_num"] = row_num;
    par.imap["child_num"] = child_num;
    par.imap["node_num"] = (pow(child_num,row_num+1)-1)/(row_num);

    par.dmap["res"] = 100;
    par.dmap["radius"] = 0.09;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void traverse_OGL(Param &par){
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

    // Create a simple tree and drawing the tree
    node root;
    int row_num = par.imap["row_num"];
    int child_num = par.imap["child_num"];
    int node_num = par.imap["node_num"];
    par.shapes.reserve(node_num*2 -1);
    glm::vec3 licolor = {1.0, 1.0, 1.0};
    glm::vec3 cicolor = {1.0, 0, 1.0};
    std::vector<Shape> lines, circles;
    lines.reserve(node_num - 1);
    circles.reserve(node_num);
    int id = 0;
    create_tree(par, root, row_num, child_num, 0, row_num, par.dmap["radius"],
                id, licolor, cicolor, 0, lines, circles);

    par.tree = root;

    // We need the circles to come after the lines for appropriate visualization
    for (size_t i = 0; i < lines.size(); ++i){
        par.shapes.push_back(lines[i]);
    }
    for (size_t i = 0; i < circles.size(); ++i){
        par.shapes.push_back(circles[i]);
    }

}
