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

#include <random>
#include <algorithm>

#include "../include/test_animation.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

struct person{
    int ID, mate;
};

struct character{
    double rad;
    glm::vec3 color, loc;
};

void move_to_partners(Param &par, std::vector<int> pairs,
                      std::vector<character> characters, double time){
    for (int i = 0; i < pairs.size(); ++i){
        glm::vec3 temp_loc = characters[pairs[i]+4].loc;
        temp_loc[0] -= 0.15;
        glm::vec3 origin = {0,0,0};
        temp_loc[0] -= characters[i].loc[0];
        temp_loc[1] -= characters[i].loc[1];
        temp_loc[2] = 0;

        add_move_keyframe(par, par.shapes[i], origin, time);
        add_move_keyframe(par, par.shapes[i], temp_loc, time+1);
    }
}

std::vector<person> find_preference(Param &par, int n, 
                                    std::vector<character> characters,
                                    double &time){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<person> preferences(4*n*n);
    person noone;
    noone.ID = -1;
    noone.mate = 0;
    for(int i = 0; i < 2*n; ++i){
        std::vector<person> ind_pref(n);
        for (int j = 0; j < n; ++j){
            ind_pref[j].ID = j;
            ind_pref[j].mate = 0;
        }
        std::shuffle(ind_pref.begin(), ind_pref.end(), gen);
        for (int j = 0; j < 2*n; ++j){
            int index = i*2*n + j;
            if (i < n){
                if(j < n){
                    preferences[index] = ind_pref[j];
                }
                else{
                    preferences[index] = noone;
                }
            }
            else{
                if(j >= n){
                    preferences[index] = ind_pref[j-n];
                }
                else{
                    preferences[index] = noone;
                }
            }
        }
    }

    // Animating preferences
    double xloc = -0.9;
    for (int i = 0; i < 8; ++i){
        if (i == 4){
            xloc += 1.5;
            time += 3;
        }
        for (int j = 0; j < 4; ++j){
            int index, char_index;
            if (i < 4){
                index = i*2*n + j;
                char_index = preferences[index].ID+n;
            }
            else{
                index = i*2*n + n;
                char_index = preferences[index].ID;
                std::vector<int> list(n);
                for(int k = 0; k < n; ++k){
                    for(int l = 0; l < n; ++l){
                        if(preferences[index+k].ID == l){
                            list[k] = l;
                        }
                    }
                }
                char_index = list[j];
            }
            glm::vec3 temp_loc = {xloc + j*0.1, characters[i].loc[1], 0};
            Shape human;
            create_human(human, temp_loc, characters[char_index].color,
                         characters[char_index].rad * 0.5);
            add_keyframes(par, human, time + j*0.5, time +j*0.5 + 1);
            par.shapes.push_back(human);

        }
    }

    time += 4;
    return preferences;
}

void propose(Param &par, std::vector<person> preferences, person man,
             person &woman, std::vector<int> &pairs, 
             std::vector<character> characters, double &time, int j){
    int n = sqrt(preferences.size()/4);
    for (int i = 0; i < n; ++i){
        add_move_keyframe(par, par.shapes[i], par.shapes[i].locations.back(), time);
    }

    int move_id = -1;
    int mid_0 = -1;
    for (int i = 0; i < n; ++i){
        if (pairs[i] == woman.ID){
            mid_0 = i;
        }
    }
    std::cout << "mid 0 is: " << mid_0 << '\n';
    if (mid_0 >= 0){
        std::cout << "check\n";
        int rank_0 = preferences[(woman.ID+n)*2*n + mid_0+n].ID;
        int rank_1 = man.ID;
        std::cout << rank_0 << '\t' << rank_1 << "\n";
        std::cout << mid_0 << '\t' << j << "\n\n";
        if (rank_1 < rank_0){
            pairs[j] = woman.ID;
            pairs[mid_0] = -1;
            move_id = mid_0;
        }
        else if (rank_1 == rank_0){
        }
        else{
            move_id = j;
        }
    }
    else{
        pairs[j] = woman.ID;
    }

/*
    time += 1;
    if (move_id >= 0){
        std::cout << "move id is: " << move_id << '\n';
        glm::vec3 temp_loc = par.shapes[move_id].locations.back();
        temp_loc[0] -= temp_loc[0] - characters[move_id].loc[0];
        temp_loc[1] -= temp_loc[1] - characters[move_id].loc[1];
        temp_loc[2] = 0;

        add_move_keyframe(par, par.shapes[move_id], temp_loc, time);
    }
*/


}

std::vector<int> gale_shapley(Param &par, std::vector<person> preferences,
                              std::vector<character> characters, double &time){
    int n = sqrt(preferences.size()/4);
    std::vector<int> pairs(n);
    std::vector<int> mate_list(4);
    int paired = 0;

    for (int i = 0; i < n; ++i){
        glm::vec3 origin = {0,0,0};
        pairs[i] = -1;
        mate_list[i] = 0;
        add_move_keyframe(par, par.shapes[i], origin, time);
    }
    while(!paired){
        for (int j = 0; j < n; ++j){
            if (pairs[j] < 0){
                for (auto &pair : pairs){
                    std::cout << pair << '\n';
                }
                person woman = preferences[j*2*n + mate_list[j]];
                person man = preferences[(woman.ID+n)*2*n + j+n];
                std::cout << "ID's are:\n";
                std::cout << man.ID << '\t' << woman.ID << '\n';
            
                glm::vec3 temp_loc = characters[woman.ID + 4].loc;
                temp_loc[0] -= 0.15;
                temp_loc[0] -= characters[j].loc[0];
                temp_loc[1] -= characters[j].loc[1];
                temp_loc[2] = 0;

                time += 1;
                add_move_keyframe(par, par.shapes[j], temp_loc, time);

                propose(par, preferences, man,woman,pairs,characters,time,j);
                mate_list[j] += 1;
            }
        }
        paired = 1;
        for(int j = 0; j < pairs.size(); ++j){
            if (pairs[j] < 0){
                paired = 0;
            }
        }
    }

    return pairs;
}

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

    //initializing all characters< '\n';
    std::vector<character> characters(8);
    double xval = -0.25;
    double yval = 0.85;
    Shape human;
    double time = 0;
    for (int i = 0; i < 8; ++i){
        characters[i].loc = {xval, yval, 0};
        characters[i].rad = 0.075;
        if (i < 4){
            characters[i].color = {0.25, 0.25, 1};
        }
        else{
            characters[i].color = {1, 0, 1};
        }
        yval -= 0.5;
        if (i == 3){
            xval += 0.5;
            yval += 2;
        }

        switch(i){
            case 0:
                characters[i].rad *= 0.8;
                break;
            case 1:
                characters[i].rad *= 0.9;
                characters[i].color = {0.5, 0.5, 1};
                break;
            case 2:
                characters[i].rad *= 0.75;
                characters[i].color[0] = 0;
                characters[i].color[1] = 0.5;
                characters[i].color[2] = 1;
                break;
            case 3:
                characters[i].color[0] -= 0.25;
                characters[i].color[1] -= 0.25;
                characters[i].color[2] -= 0.5;
                break;
            case 4:
                characters[i].rad *= 0.6;
                characters[i].color[1] += 0.5;
                break;
            case 5:
                characters[i].rad *= 0.8;
                characters[i].rad *= 0.8;
                break;
            case 6:
                characters[i].rad *= 0.8;
                characters[i].color[2] -= 0.25;
                break;
            case 7:
                characters[i].rad *= 0.9;
                characters[i].color[2] -= 0.5;
                characters[i].color[1] += 0.25;
                break;
        }

        create_human(human, characters[i].loc, characters[i].color,
                     characters[i].rad);
        add_keyframes(par, human, 1+time,2+time);
        par.shapes.push_back(human);
        if (i == 3){
            time += 1;
        }
        time += 0.5;

    }

    time = 8;

    std::vector<person> preferences = find_preference(par, 4, characters, time);
    std::cout << par.shapes.size() << '\n';
    std::vector<int> pairs = gale_shapley(par, preferences, characters, time);
    std::cout << "The pairs are: " << '\n';
    for (int i = 0; i < pairs.size(); ++i){
        std::cout << pairs[i] << '\n';
    }

    //move_to_partners(par, pairs, characters, time);
}
