/*-------------convolution.cpp------------------------------------------------//
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

// Function to animate linear convolution
void animate_linear_conv(Param &par){
    std::cout << "animating linear conv..." << '\n';
}

// Function to animate fft convolution
void animate_fft_conv(Param &par){
    std::cout << "animating fft conv..." << '\n';
}

// Function to perform a convolution
void conv(Param &par){
    double sum = 0;
    std::vector<std::vector<double>> signals(3);
    signals[0] = par.vdmap["sig1"];
    signals[1] = par.vdmap["sig2"];

    int n = signals[0].size() + signals[1].size();
    signals[2].reserve(n);

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < i; ++j){
            if (i-j < signals[1].size()-1){
                if (j < signals[0].size()){
                    sum += signals[0][j] * signals[1][i-j];
                }
            }
        }
        signals[2].push_back(sum);
        sum = 0;
    }

    normalize(signals[2].data(),n);

    par.vdmap["sig3"] = signals[2];
}

// Function to perform FFT-based convolution
void fft_conv(Param &par){
    int n = par.imap["res"];

    std::vector<std::vector<double>> signals(3);
    signals[0] = par.vdmap["sig1"];
    signals[1] = par.vdmap["sig2"];
    signals[2] = par.vdmap["sig3"];

    // Creating the two arrays for fft'ing
    std::vector<fftw_complex *> waves(3), ftwaves(3);
    for (int i = 0; i < 3; ++i){
        waves[i] = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * n);
        ftwaves[i] = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * n);
    }

    for (int i = 0; i < n; ++i){
        waves[0][i][0] = signals[0][i];
        waves[0][i][1] = 0;
        waves[1][i][0] = signals[1][i];
        waves[1][i][1] = 0;
    }

    fftw_plan plan;
    plan = fftw_plan_dft_1d(n, waves[0], ftwaves[0], 
                            FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    plan = fftw_plan_dft_1d(n, waves[1], ftwaves[1], 
                            FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    for (int i = 0; i < n; ++i){
        ftwaves[2][i][0] = ftwaves[0][i][0] * ftwaves[1][i][0] 
                        + ftwaves[0][i][1] * ftwaves[1][i][1];
        ftwaves[2][i][1] = ftwaves[0][i][1] * ftwaves[1][i][0] 
                        + ftwaves[0][i][0] * ftwaves[1][i][1];
    }

    plan = fftw_plan_dft_1d(n, ftwaves[2], waves[2], 
                            FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    for (int i = 0; i < n; ++i){
        signals[2][i] = waves[2][i][0]; 
    }
    normalize(signals[2].data(),n);

    par.vdmap["sig3"] = signals[2];

}

// Test functions using shader.h
void convolution_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    if (!is_down){
        return;
    }
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_f:
            if (is_down){
                animate_fft_conv(par);            
            }
            break;
        case SDLK_l:
            if (is_down){
                animate_linear_conv(par);
            }
            break;
        default:
            break;

    }

}

void convolution_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);

    SDL_GL_SwapWindow(par.screen);

}

void convolution_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    // Creating signals to work with
    int n = 1024;
    std::vector<double> sig1(n), sig2(n);
    for (int i = 0; i < n; ++i){
        if (i > 2*n / 5 && i < 3*n/5){
            sig1[i] = 1.0;
        }
        if (i > 2*n / 5 && i < 3*n/5){
            sig2[i] = 1.0;
        }
/*
        sig1[i] = sin(2*M_PI*i/n);
        sig2[i] = cos(2*M_PI*i/n);
*/
    }
    par.vdmap["sig1"] = sig1;
    par.vdmap["sig2"] = sig2;
    par.imap["res"] = n;

    conv(par);

}

void convolution_OGL(Param &par){
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
    Shape line;
    std::vector<glm::vec3> array(2);

    double ypos = 0.6;
    for (int i = 0; i < 3; ++i){
        array[0] = {-0.9, ypos, 0.0};
        array[1] = {0.9, ypos, 0.0};
        ypos -= 0.6;

        glm::vec3 licolor = {0.5, 0.5, 0.5};
    
        create_line(line, array, licolor);
    
        par.shapes.push_back(line);
    }

    ypos = 0.6;
    std::vector<double> signal;
    for (int i = 0; i < 3; ++i){
        if(i == 0){
             signal = par.vdmap["sig1"];
        }
        else if(i == 1){
             signal = par.vdmap["sig2"];
        }
        else if(i == 2){
             signal = par.vdmap["sig3"];
        }
        std::vector<glm::vec3> conv_arr(signal.size());
        for (int i = 0; i < signal.size(); ++i){
            conv_arr[i][0] = -0.9 + i*1.8/(signal.size()-1);
            conv_arr[i][1] = ypos + signal[i]*0.3;
            conv_arr[i][2] = 0;
        }
        ypos -= 0.6;
        glm::vec3 licolor = {1.0, 1.0, 1.0};
    
        create_line(line, conv_arr, licolor);
        add_keyframes(par, line, 1, 2);
    
        par.shapes.push_back(line);

    }   

}
