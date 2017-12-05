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

#include "../include/bits.h"
#include "../include/shape_functions.h"
#include "../include/aux_functions.h"
#include "../include/shaders.h"
#include "../include/operations.h"

void color_gates(Param &par){
    int index = par.shapes.size() - 16;
    glm::vec3 color1 = {1, 1, 1};
    glm::vec3 color2 = {1, 0, 0};
    for (int i = 0; i < 4; ++i){
        if (par.strings[6+i][0] == '0'){
            add_color_keyframe(par, par.shapes[index], color1, curr_time(par));
        }
        else{
            add_color_keyframe(par, par.shapes[index], color2, curr_time(par));
        }
        index++;

        if (par.strings[6+i][1] == '0'){
            add_color_keyframe(par, par.shapes[index], color1, curr_time(par));
        }
        else{
            add_color_keyframe(par, par.shapes[index], color2, curr_time(par));
        }
        index++;

        if (par.strings[10+i][0] == '0'){
            add_color_keyframe(par, par.shapes[index], color1, curr_time(par));
        }
        else{
            add_color_keyframe(par, par.shapes[index], color2, curr_time(par));
        }
        index++;
    }
}

void gate_change(Param &par, int change){
    if(change < 0 && par.curr_factor > 0){
        par.curr_factor += change;
        if (par.curr_factor == 3){
            int index = par.shapes.size() - 16;
            for(int i = index; i < par.shapes.size(); ++i){
                if(par.shapes[i].draw){
                    par.shapes[i].draw = false;
                }
                else{
                    par.shapes[i].draw = true;
                }
            }
        }
        if(par.curr_factor == 2){
            int index = par.shapes.size() - 16;
            for(int i = index; i < par.shapes.size(); ++i){
                if(par.shapes[i].draw){
                    par.shapes[i].draw = false;
                }
                else{
                    par.shapes[i].draw = true;
                }
            }

        }
    }
    if(change > 0 && par.curr_factor < 5){
        par.curr_factor += change;
        if (par.curr_factor == 3){
            int index = par.shapes.size() - 16;
            for(int i = index; i < par.shapes.size(); ++i){
                if(par.shapes[i].draw){
                    par.shapes[i].draw = false;
                }
                else{
                    par.shapes[i].draw = true;
                }
            }
        }
        if(par.curr_factor == 4){ 
            int index = par.shapes.size() - 16;
            for(int i = index; i < par.shapes.size(); ++i){
                if(par.shapes[i].draw){
                    par.shapes[i].draw = false;
                }
                else{
                    par.shapes[i].draw = true;
                }
            }

        }

    }

    if (par.strings[par.curr_factor] == "AND"){
        par.strings[10] = "0";
        par.strings[11] = "0";
        par.strings[12] = "0";
        par.strings[13] = "1";
    }

    if (par.strings[par.curr_factor] == "OR"){
        par.strings[10] = "0";
        par.strings[11] = "1";
        par.strings[12] = "1";
        par.strings[13] = "1";
    }

    if (par.strings[par.curr_factor] == "XOR"){
        par.strings[10] = "0";
        par.strings[11] = "1";
        par.strings[12] = "1";
        par.strings[13] = "0";
    }

    if (par.strings[par.curr_factor] == "NOT"){
        par.strings[10] = "1";
        par.strings[11] = "0";
    }

    if (par.strings[par.curr_factor] == "NAND"){
        par.strings[10] = "1";
        par.strings[11] = "1";
        par.strings[12] = "1";
        par.strings[13] = "0";
    }

    if (par.strings[par.curr_factor] == "NOR"){
        par.strings[10] = "1";
        par.strings[11] = "0";
        par.strings[12] = "0";
        par.strings[13] = "0";
    }
    color_gates(par);
}

void set_gates(Param &par){
    par.strings.push_back("AND");
    par.strings.push_back("OR");
    par.strings.push_back("XOR");
    par.strings.push_back("NOT");
    par.strings.push_back("NAND");
    par.strings.push_back("NOR");

    par.strings.push_back("00");
    par.strings.push_back("01");
    par.strings.push_back("10");
    par.strings.push_back("11");

    par.strings.push_back("0");
    par.strings.push_back("0");
    par.strings.push_back("0");
    par.strings.push_back("1");
}

void draw_gate(Param &par){
    glm::vec3 color = {1, 1, 1};

    glm::vec3 text_pos = {-0.8, 0.45, 0};
    write_string(par, "IN", text_pos, 1.0f, color);

    text_pos = {-0.45, 0.45, 0};
    write_string(par, "OUT", text_pos, 1.0f, color);

    if (par.strings[par.curr_factor] != "NOT"){
        for (int i = 0; i < 4; ++i){
            text_pos = {-0.8, 0.2 - 0.25*i, 0};
            write_string(par, par.strings[i+6], text_pos, 1.0f, color);

            text_pos = {-0.35, 0.2 - 0.25*i, 0};
            write_string(par, par.strings[i+10], text_pos, 1.0f, color);
        }
    }
    else{
        text_pos = {-0.8, 0.2 - 0.25, 0};
        write_string(par, "0", text_pos, 1.0f, color);

        text_pos = {-0.35, 0.2 - 0.25, 0};
        write_string(par, par.strings[10], text_pos, 1.0f, color);

        text_pos = {-0.8, 0.2 - 0.5, 0};
        write_string(par, "1", text_pos, 1.0f, color);

        text_pos = {-0.35, 0.2 - 0.5, 0};
        write_string(par, par.strings[11], text_pos, 1.0f, color);
    }

    text_pos = {0.5, 0.8, 0};
    write_string(par, par.strings[par.curr_factor], text_pos, 1.0f, color);
}

// Test functions using shader.h
void bits_key(Param &par, SDL_Keysym* Keysym, bool is_down){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_g:
            if (is_down){
                par.bmap["is_gate"] = true;
                for (int i = 1; i < par.shapes.size()-4; ++i){
                    par.shapes[i].draw = true;
                }
            }
            break;
        case SDLK_b:
            if (is_down){
                par.bmap["is_gate"] = false;
                for (int i = 1; i < par.shapes.size(); ++i){
                    par.shapes[i].draw = false;
                }
            }
            break;
        case SDLK_f:
            if (!par.bmap["is_gate"]){
                if (is_down){
                    par.bmap["is_int"] = false;
                }
            }
            break;
        case SDLK_i:
            if (!par.bmap["is_gate"]){
                if (is_down){
                    par.bmap["is_int"] = true;
                }
            }
            break;
        case SDLK_LEFT:{
            if (!par.bmap["is_gate"]){
                if(is_down){
                    if (par.bmap["is_int"]){
                        par.factors[0] = (int)par.factors[0] << 1;
                        par.shapes[0].draw = true;
                    }
                }
                else{
                    par.shapes[0].draw = false;
                }
            }
            else{
                if(is_down){
                    gate_change(par, -1);
                }
            }
            break;
        }
        case SDLK_RIGHT:{
            if (!par.bmap["is_gate"]){
                if(is_down){
                    if (par.bmap["is_int"]){
                        par.factors[0] = (int)par.factors[0] >> 1;
                        glm::vec3 vloc = vertex_location(par.shapes[0], 0);
                        if (vloc[0] < 0){
                            glm::vec3 translate = {1.6, 0, 0};
                            move_shape(par.shapes[0], translate);
                            par.shapes[0].draw = true;
                        }
                    }
                }
                else{
                    glm::vec3 translate = {-1.6, 0, 0};
                    move_shape(par.shapes[0], translate);
                    par.shapes[0].draw = false;
                }
            }
            else{
                if (is_down){
                    gate_change(par, 1);
                }
            }
            break;
        }
        case SDLK_DOWN:{
            if (!par.bmap["is_gate"]){
                if(is_down){
                    if (par.bmap["is_int"]){
                        par.factors[0] -= 1;
                    }
                    else{
                        par.factors[0] -= 0.1;
                    }
                }
            }
            break;
        }
        case SDLK_UP:{
            if (!par.bmap["is_gate"]){
                if(is_down){
                    if (par.bmap["is_int"]){
                        par.factors[0] += 1;
                    }
                    else{
                        par.factors[0] += 0.1;
                    }
                }
            }
            break;
        }

        default:
            break;

    }

}

void bits_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    draw_shapes(par);
    if (!par.bmap["is_gate"]){
        write_bits(par);
        glm::vec3 pos_text = {-0.5f, -.7f, 0.0f};
    
        write_string(par, "Value is: ", pos_text, .5f, color);
    
        pos_text[0] += 0.5;
    
        if (par.bmap["is_int"]){
            pos_text[0] = -0.9;
            pos_text[1] -= 0.001;
            write_string(par, "<<", pos_text, 1.0f, color);
            pos_text[0] = 0.7;
            write_string(par, ">>", pos_text, 1.0f, color);
            pos_text[1] += 0.001;
            pos_text[0] = 0;
            write_string(par, std::to_string((int)par.factors[0]), 
                         pos_text, .5f, color);
        }
        else{
            write_string(par, std::to_string(par.factors[0]), 
                         pos_text, .5f, color);
        }
    
        pos_text = {-1.0f, -0.99f, 0.0f};
        write_string(par, "f -- floats; i -- ints; up -- value up; down -- value down; left -- bitshift left; right -- bitshift right", 
                     pos_text, 0.25f, color);
    }
    else{
        draw_gate(par);
    }

    SDL_GL_SwapWindow(par.screen);

}

void bits_par(Param &par){
    par.dist = "bits";
    par.end = 0;

    par.bmap["is_int"] = true;
    par.bmap["is_gate"] = false;

    par.factors.push_back(0);
    par.imap["gcd"] = 3;
    par.curr_factor = 0;

    par.font = "fonts/LinLibertine_Rah.ttf";
    par.font_size = sqrt(par.width*par.width + par.height*par.height) / 16;

}

void bits_OGL(Param &par){
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

    // Creating the box around the text for our current factor that we are using
    std::vector<glm::vec3> box(6);
    box[0] = {-0.92, -0.69, 0.0};
    box[1] = {-0.92, -0.59, 0.0};
    box[2] = {-0.68, -0.59, 0.0};
    box[3] = {-0.68, -0.73, 0.0};
    box[4] = {-0.92, -0.73, 0.0};
    box[5] = {-0.92, -0.69, 0.0};

    glm::vec3 box_color = {1.0, 1.0, 1.0};
    create_line(line, box, box_color);
    line.draw = false;
    par.shapes.push_back(line);

    // Creating all the endpoints for the gate visualization
    std::vector<glm::vec3> endpoints(2);
    endpoints[0] = {-0.5, 0.6, 0};    
    endpoints[1] = {-0.5, -0.8, 0};    
    create_line(line, endpoints, box_color);
    line.draw = false;
    par.shapes.push_back(line);

    endpoints[0] = {-0.9, 0.4, 0};    
    endpoints[1] = {-0.1, 0.4, 0};    
    create_line(line, endpoints, box_color);
    line.draw = false;
    par.shapes.push_back(line);

    set_gates(par);

    // Lines for gate input and output
    for (int i = 0; i < 4; ++i){
        endpoints[0] = {0.3, 0.25-i*0.25, 0};    
        endpoints[1] = {0.55, 0.25-i*0.25, 0};    
        create_line(line, endpoints, box_color);
        line.draw = false;
        par.shapes.push_back(line);

        endpoints[0] = {0.3, 0.2-i*0.25, 0};    
        endpoints[1] = {0.55, 0.2-i*0.25, 0};    
        create_line(line, endpoints, box_color);
        line.draw = false;
        par.shapes.push_back(line);

        endpoints[0] = {0.55, 0.225-i*0.25, 0};    
        endpoints[1] = {0.8, 0.225-i*0.25, 0};    
        create_line(line, endpoints, box_color);
        line.draw = false;
        par.shapes.push_back(line);
    }

    // for NOT gate
    glm::vec3 color2 = {1, 0, 0};
    for (int i = 0; i < 2; ++i){
        glm::vec3 temp_color = box_color;
        if(i == 1){
            temp_color = color2;
        }
        endpoints[0] = {0.3, -0.025-i*0.25, 0};    
        endpoints[1] = {0.55, -0.025-i*0.25, 0};    
        create_line(line, endpoints, temp_color);
        line.draw = false;
        par.shapes.push_back(line);

        temp_color = color2;
        if(i == 1){
            temp_color = box_color;
        }
        endpoints[0] = {0.55, -0.025-i*0.25, 0};    
        endpoints[1] = {0.8, -0.025-i*0.25, 0};    
        create_line(line, endpoints, temp_color);
        line.draw = false;
        par.shapes.push_back(line);

    }

    color_gates(par);

}
