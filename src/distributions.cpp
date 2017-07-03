/*-------------distributions.cpp----------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fftw3.h>

#include "../include/distributions.h"
#include "../include/shape_functions.h"
#include "../include/test_pong.h"
#include "../include/shaders.h"
#include "../include/operations.h"

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
void test_pend_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
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

void test_pend_fn(Param &par){

    move_pendulum(par);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    glm::vec3 pos = {20.0f, 20.0f, 0.0f};
    write_string(par, "sample text", pos, 1.0f, glm::vec3(0.5, 0.0f, 0.5f));

    SDL_GL_SwapWindow(par.screen);

}

void test_pend_par(Param &par){
    par.dist = "test_pend";
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

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 34;

}

void test_pend_OGL(Param &par){
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
    float rad = (float)par.dmap["radius"];
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
    Shape line, circle;
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

}

