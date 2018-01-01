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
    par.bmap["linear"] = true;

    // setting all appropriate shapes to draw
    for (int i = 0; i < par.shapes.size(); ++i){
        par.shapes[i].draw = (i < par.imap["shape_number"]);
    }
    double offset = par.dmap["offset"];
    for (Shape& sh : par.shapes){
        shift_keyframes(par, sh, curr_time(par) - offset);
        shift_color_keyframes(par, sh, curr_time(par) - offset);
        shift_move_keyframes(par, sh, curr_time(par) - offset);
    }
    par.dmap["offset"] = curr_time(par);
}

// Function to animate fft convolution
void animate_fft_conv(Param &par){
    std::cout << "animating fft conv..." << '\n';
    par.bmap["linear"] = false;
    // setting all appropriate shapes to draw
    for (int i = 0; i < par.shapes.size(); ++i){
        par.shapes[i].draw = (i > par.imap["shape_number"]);
    }
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

    double ypos = 0.725;
    glm::vec3 text_color = {1, 1, 1};

    if (par.bmap["linear"]){
        for (int i = 0; i < 2; ++i){
            glm::vec3 pos_text = {-0.9, ypos, 0};
            ypos -= 0.5;
            if (i == 0){
                write_string(par, "Signal 1:", pos_text, 1, text_color);
            }
            if (i == 1){
                write_string(par, "Signal 2:", pos_text, 1, text_color);
            }
        }
    }
    else{
    }

    SDL_GL_SwapWindow(par.screen);

}

void convolution_par(Param &par){
    par.dist = "test_anim";
    par.end = 0;

    par.dmap["offset"] = 0.0;
    par.imap["shape_number"] = 9;
    par.bmap["linear"] = true;

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
        //sig1[i] = sin(2*M_PI*i/n) * (double)i/n;
        //sig2[i] = cos(2*M_PI*i/n) * (double)i/n;
    }
    par.vdmap["sig1"] = sig1;
    par.vdmap["sig2"] = sig2;

    std::vector<double> sig_temp(n);
    for (int i = 0; i < n; ++i){
        sig_temp[i] = sig2[n-i-1];
    }
    par.vdmap["sig2_flip"] = sig_temp;

    par.imap["res"] = n;

    conv(par);

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 16;

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

    // Creating a simple line
    Shape line;
    std::vector<glm::vec3> array(2);

    double ypos = 0.75;
    for (int i = 0; i < 4; ++i){
        if (i < 2){
            array[0] = {-0.0, ypos, 0.0};
            array[1] = {0.9, ypos, 0.0};
        }
        else{
            array[0] = {-0.9, ypos, 0.0};
            array[1] = {0.9, ypos, 0.0};
        }
        ypos -= 0.5;

        glm::vec3 licolor = {0.5, 0.5, 0.5};
    
        create_line(line, array, licolor);
    
        par.shapes.push_back(line);
    }

    ypos = 0.75;
    std::vector<double> signal;
    for (int i = 0; i < 4; ++i){
        if(i == 0){
             signal = par.vdmap["sig1"];
        }
        else if(i == 1){
             signal = par.vdmap["sig2"];
        }
        else if(i == 2){
             signal = par.vdmap["sig1"];
        }
        else if(i == 3){
             signal = par.vdmap["sig3"];
        }
        std::vector<glm::vec3> conv_arr(signal.size());
        if (i < 2){
            for (int i = 0; i < signal.size(); ++i){
                conv_arr[i][0] = 0.0 + i*0.9/(signal.size()-1);
                conv_arr[i][1] = ypos + signal[i]*0.1;
                conv_arr[i][2] = 0;
            }
        }
        else{
            for (int i = 0; i < signal.size(); ++i){
                conv_arr[i][0] = -0.9 + i*1.8/(signal.size()-1);
                conv_arr[i][1] = ypos + signal[i]*0.2;
                conv_arr[i][2] = 0;
            }
        }
        ypos -= 0.5;
        glm::vec3 licolor = {1.0, 1.0, 1.0};
    
        create_line(line, conv_arr, licolor);
        if (i < 2){
            add_keyframes(par, line, 1, 2);
        }
        else if (i == 2){
            add_keyframes(par, line, 2, 3);
        }
        else if (i == 3){
            add_keyframes(par, line, 3, 7);
        }
    
        par.shapes.push_back(line);

    }

    // Adding the 5th waveform to move across the 3rd waveform
    Shape sh;
    signal = par.vdmap["sig2_flip"];
    std::vector<glm::vec3> conv_arr(signal.size());
    for (int i = 0; i < signal.size(); ++i){
        conv_arr[i][0] = -2.9 + i*1.8/(signal.size()-1);
        conv_arr[i][1] = -0.25 + signal[i]*0.2;
        conv_arr[i][2] = 0;
    }
    glm::vec3 licolor = {0.5, 0.5, 1.0};
    create_line(sh, conv_arr, licolor);
    glm::vec3 loc = {0,0,0};
    add_move_keyframe(par, sh, loc, 3);
    loc = {4,0,0};
    add_move_keyframe(par, sh, loc, 7);

    par.shapes.push_back(sh);

}
