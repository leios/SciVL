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

        default:
            break;

    }

}

void fft_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

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
    std::vector<glm::vec3> array(2);
    array[0] = {-0.8,0.875,0};
    array[1] = {0.8,0.875,0};

    glm::vec3 licolor = {1.0, 1.0, 1.0};
    for (int i = 0; i < 8; ++i){
        create_line(line, array, licolor);
        add_keyframes(par, line, 0, 1);
        par.shapes.push_back(line);
        array[0][1] -= 0.25;
        array[1][1] -= 0.25;
    }
}


// Test functions using shader.h
void fourier_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    if (!is_down){
        return;
    }
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:{
            par.end = 1;
            break;
        }

        case SDLK_LEFT:{
            
            par.factors[par.curr_factor] -= 0.1;
            update_fft(par);
            break;
        }
        case SDLK_RIGHT:{
            par.factors[par.curr_factor] += 0.1;
            update_fft(par);
            break;
        }
        case SDLK_DOWN:{
            if (par.curr_factor < 2){
                par.curr_factor += 1;
                if (par.curr_factor > par.factors.size()-1){
                    par.factors.push_back((double)par.curr_factor + 1);
                }
                glm::vec3 trans = {0.0, -0.2, 0.0};
                move_shape(par.shapes[par.shapes.size()-3], trans);
            }
            update_fft(par);
            break;
        }
        case SDLK_UP:{
            if (par.curr_factor > 0){
                par.curr_factor -= 1;
                glm::vec3 trans = {0.0, 0.2, 0.0};
                move_shape(par.shapes[par.shapes.size()-3], trans);
            }
            update_fft(par);
            break;
        }

        default:
            break;

    }

}

void fourier_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    draw_fft(par);

    SDL_GL_SwapWindow(par.screen);

}

void fourier_par(Param &par){
    par.dist = "fourier";
    par.end = 0;


    par.factors.push_back(1.0);
    par.imap["res"] = 100;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

    par.start_time = std::chrono::high_resolution_clock::now();

}

void fourier_OGL(Param &par){
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

    // Creating the xy axis for plotting
    Shape line;
    std::vector<glm::vec3> array(3);
    array[0] = {-0.95, 0.85, 0.0};
    array[1] = {-0.95, -0.1, 0.0};
    array[2] = {-0.05, -0.1, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    array[0] = {0.05, 0.85, 0.0};
    array[1] = {0.05, -0.1, 0.0};
    array[2] = {0.95, -0.1, 0.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    // Creating the box around the text for our current factor that we are using
    std::vector<glm::vec3> box(6);
    box[0] = {-0.7, -0.4, 0.0};
    box[1] = {-0.7, -0.3, 0.0};
    box[2] = {0.7, -0.3, 0.0};
    box[3] = {0.7, -0.45, 0.0};
    box[4] = {-0.7, -0.45, 0.0};
    box[5] = {-0.7, -0.4, 0.0};

    glm::vec3 box_color = {1.0, 1.0, 1.0};
    create_line(line, box, box_color);
    par.shapes.push_back(line);

    int res = par.imap["res"];

    // Creating the two arrays for plotting 
    fftw_complex *wave, *ftwave;
    wave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);
    ftwave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);

    // Creating the plan for fft'ing
    fftw_plan plan;
    for (int i = 0; i < res; ++i){
        wave[i][0] = sin(par.factors[0]*2*M_PI*i/(double)res);
        wave[i][1] = 0;
    }

    normalize(wave, res);

    // Performing fft
    plan = fftw_plan_dft_1d(res, wave, ftwave, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    fftw_destroy_plan(plan);

    normalize(ftwave, res);

    // now creating a sinusoidal wave
    std::vector<glm::vec3> sinarr(res);
    std::vector<glm::vec3> fftarr(res / 2);
    for (size_t i = 0; i < sinarr.size(); ++i){
        sinarr[i].x = -0.95 + 0.9 * (double)i / sinarr.size();
        sinarr[i].y = (wave[i][0]) * 0.5 * 0.9 + 0.4;
        sinarr[i].z = 0;
        if (i < res / 2){
            fftarr[i].x = 0.05 + 0.9 * 2 * (double)i / sinarr.size();
            fftarr[i].y = (abs2(ftwave[i])) * 0.5 * 0.9 + 0.4;
            fftarr[i].z = 0;
        }
        //std::cout << wave[i][0] << '\t' << ftwave[i][0] << '\n';
    }
    create_line(line, sinarr, licolor);
    par.shapes.push_back(line);
    create_line(line, fftarr, licolor);
    par.shapes.push_back(line);

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

// Test functions using shader.h
void test_anim_key(Param &par, SDL_Keysym* Keysym, bool is_down){
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

void test_anim_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void test_anim_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    par.dmap["res"] = 100;

}

void test_anim_OGL(Param &par){
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
    Shape line, circle, rect;
    std::vector<glm::vec3> array(2);

    array[0] = {0.0, 0.0, 0.0};
    array[1] = {0.5, -0.5, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};
    glm::vec3 ccolor = {0.0, 0.0, 1.0};

    create_line(line, array, licolor);

    add_keyframes(par, line, 1, 2);
    par.shapes.push_back(line);

    // Working with the circle
    create_circle(circle, array[0], 0.25, ccolor, par.dmap["res"]);
    add_keyframes(par, circle, 2,3);

    par.shapes.push_back(circle);

    glm::vec3 rect_size = {0.25, 0.25, 0};
    glm::vec3 recolor = {0.0, 1.0, 0.0};
    create_rectangle(rect, array[1], rect_size, recolor); 
    add_keyframes(par, rect, 3,4);
    par.shapes.push_back(rect);

}

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
                break;
            }
        case SDLK_m:
            if (is_down){
                euclid_clear(par);
                euclid_mod(par, par.factors[0], par.factors[1]);
                break;
            }
        case SDLK_c:
            if (is_down){
                euclid_clear(par);
                break;
            }
        case SDLK_LEFT:{
            if(is_down){
                par.factors[par.curr_factor] -= 1;
                break;
            }
        }
        case SDLK_RIGHT:{
            if(is_down){
                par.factors[par.curr_factor] += 1;
                break;
            }
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
                break;
            }
        }
        case SDLK_UP:{
            if(is_down){
                if (par.curr_factor > 0){
                    par.curr_factor -= 1;
                    glm::vec3 trans = {0.0, 0.15, 0.0};
                    move_shape(par.shapes[0], trans);
                }
                break;
            }
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

