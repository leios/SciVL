/*-------------aux_functions.cpp----------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#include <glm/mat3x3.hpp>
#include <glm/glm.hpp>
#include <fftw3.h>
#include <stack>
#include <queue>
#include <fstream>
#include "SDL.h"

#include "../include/aux_functions.h"
#include "../include/shape_functions.h"
#include "../include/operations.h"

void play_pong(Param &par){

    // Update ball position
    par.dmap["pos_x"] += par.dmap["vel_x"] * par.dmap["timestep"];
    par.dmap["pos_y"] += par.dmap["vel_y"] * par.dmap["timestep"];

    // create a translation matrix
    glm::vec3 ball_vel = {par.dmap["vel_x"] * par.dmap["timestep"],
                          par.dmap["vel_y"] * par.dmap["timestep"], 0.0};
    move_shape(par.shapes[0], ball_vel);

    // Dealing with boundary conditions
    if (par.dmap["pos_y"]  + par.dmap["radius"] > 1 || 
        par.dmap["pos_y"]  - par.dmap["radius"] < -1){
        par.dmap["vel_y"] = - par.dmap["vel_y"];
    }

    // Deal with paddles, paddle size is 1
    if (par.dmap["pos_x"]  + par.dmap["radius"] > 0.9){
        if (par.dmap["pos_y"] < par.dmap["rbumper"] + 0.5 &&
            par.dmap["pos_y"] > par.dmap["rbumper"] - 0.5){
            std::cout << par.dmap["pos_y"] << '\t' 
                      << par.dmap["rbumper"] << '\n';
            par.dmap["vel_x"] = - 2.0 * par.dmap["vel_x"];
        }
        else{
            std::cout << "Red wins!" << '\n';
            par.end = 1;
        }
    } 
    else if (par.dmap["pos_x"]  - par.dmap["radius"] < -0.9){
        if (par.dmap["pos_y"] < par.dmap["lbumper"] + 0.5 &&
            par.dmap["pos_y"] > par.dmap["lbumper"] - 0.5){
            std::cout << par.dmap["pos_y"] << '\t' 
                      << par.dmap["lbumper"] << '\n';
            par.dmap["vel_x"] = - 2.0 * par.dmap["vel_x"];
        }
        else{
            std::cout << "Blue wins!" << '\n';
            par.end = 1;
        }
    }

}

// Function to update fft arrays
void update_fft(Param &par){

    int res = par.imap["res"];

    // Creating the two arrays for plotting 
    fftw_complex *wave, *ftwave;
    wave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);
    ftwave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);

    // Creating the plan for fft'ing
    fftw_plan plan;
    for (int i = 0; i < res; ++i){
        for (size_t j = 0; j < par.factors.size(); ++j){
            if (j == 0){
                wave[i][0] = sin(par.factors[0]*2*M_PI*i/(double)res);
                wave[i][1] = 0;
            }
            else{
                wave[i][0] += sin(par.factors[j]*2*M_PI*i/(double)res);
            }
        }
    }

    normalize(wave, res);

    // Performing fft
    plan = fftw_plan_dft_1d(res, wave, ftwave, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    fftw_destroy_plan(plan);

    normalize(ftwave, res);

    // now creating a sinusoidal wave
    glm::vec3 *sinarr;
    glm::vec3 *fftarr;
    sinarr = (glm::vec3*)malloc(sizeof(glm::vec3)*res);
    fftarr = (glm::vec3*)malloc(sizeof(glm::vec3)*res / 2);
    for (int i = 0; i < res; ++i){
        sinarr[i].x = -0.95 + 0.9 * (double)i / res;
        sinarr[i].y = (wave[i][0]) * 0.5 * 0.9 + 0.4;
        sinarr[i].z = 0;
        if (i < res / 2){
            fftarr[i].x = 0.05 + 0.9 * 2 * (double)i / res;
            fftarr[i].y = (abs2(ftwave[i])) * 0.5 * 0.9 + 0.4;
            fftarr[i].z = 0;
        }
    }
    update_line(par.shapes[par.shapes.size()-2], sinarr);
    update_line(par.shapes[par.shapes.size()-1], fftarr);

}

// function to draw all necessary fft visualizations to screen
void draw_fft(Param &par){
    glm::vec3 pos = {-0.6f, 0.9f, 0.0f};
    write_string(par, "REAL", pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));
    pos = {0.2f, 0.9f, 0.0f};
    write_string(par, "FREQUENCY", pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));

    // Writing out all frequencies
    for (size_t i = 0; i < par.factors.size(); ++i){
        pos = {-0.4, -0.4 - (double)i*0.2, 0.0};
        write_string(par,"FREQUENCY " +std::to_string(i) + " is : "
                         +std::to_string(par.factors[i]),
                     pos, 1.0f, glm::vec3(0.25, 0.25, 1.0));
    }

}

// Functions to move asteroid for verlet visualization
void find_verlet_acc(Param &par){

    // Finding rad to find acc
    glm::vec3 acc = {0,0,0};
    glm::vec3 rad = {0,0,0};
    double mag_rad, mag_acc;
    for (size_t i = 0; i < par.positions.size() -1; ++i){
        rad = par.positions[i] - par.positions[par.positions.size() -1];
        mag_rad = rad[0]*rad[0] + rad[1]*rad[1];
        rad = {rad[0] / mag_rad, rad[1]/mag_rad, 0};

        mag_acc = par.positions[i][2] / mag_rad;
        acc[0] += rad[0]*mag_acc;
        acc[1] += rad[1]*mag_acc;
    }
    acc[2] = 0.0;

    par.v3map["acc"] = acc;
}

void move_verlet_obj(Param &par){

    if (par.bmap["move"]){
        //find_verlet_acc(par);

        // Grabbing necessary values
        glm::vec3 mult_acc, mult_vel, vel, pos, acc;
        acc = par.v3map["acc"];
        vel = par.v3map["vel"];
        size_t index = par.positions.size()-1;
        pos = par.positions[index];
        double dt = par.dmap["dt"];

        // Creating variable for verlet integration
        // Verlet integration
        par.v3map["temp"] = par.positions[index];
        par.positions[index] += vel*(float)dt + acc*(float)(0.5*dt*dt);
        find_verlet_acc(par);
        acc = par.v3map["acc"];
        vel += acc*(float)(dt*0.5);
        par.v3map["prev_p"] = par.v3map["temp"];

        // Creating translation vector and moving shape
        glm::vec3 trans = par.positions[index] - par.v3map["prev_p"];
        //std::cout << vel[0] << '\t' << vel[1] << '\n';
        move_shape(par.shapes[index], trans);

        par.v3map["vel"] = vel;
    }
    else{
        if (par.bmap["move_up"]){
            glm::vec3 trans = {0.0, 0.01, 0.0};
            int index = par.positions.size() -1;
            move_shape(par.shapes[index], trans);
            par.positions[index] += trans;
        }
        if (par.bmap["move_down"]){
            glm::vec3 trans = {0.0, -0.01, 0.0};
            int index = par.positions.size() -1;
            move_shape(par.shapes[index], trans);
            par.positions[index] += trans;
        }
    }
}

glm::vec3 mult(glm::vec3 vec, double val){
    return {vec[0]*val, vec[1]*val, vec[2]*val};
}

// Function to create simple tree
void create_tree(Param &par, node& root, int num_row, int num_child,
                 double offset_x, int max_row, double radius, int &id,
                 glm::vec3 licolor, glm::vec3 cicolor, double time,
                 std::vector<Shape> &lines, std::vector<Shape> &circles){

    // Creating the circle for this node
    Shape line, circle;
    glm::vec3 position = {offset_x, 
                          -(((2-2*radius)/(max_row))*(max_row - num_row) 
                          - 1 + radius), 0};

    create_circle(circle, position, radius, cicolor, 100);
    add_keyframes(par, circle, time, time + 0.5);
    circles.push_back(circle);

    // Aux variables for drawing
    double offset;
    double box_size;
    int new_id;
    std::vector<glm::vec3> array(2);

    // Defining node parameters
    root.ID = id;
    id += 1;
    root.pos = position;

    // Returning is leaf node
    //par.positions.push_back(root.pos);
    if (num_row == 0){
        return;
    }
    root.children.reserve(num_child);
    for (int i = 0; i < num_child; ++i){
        node child;
        box_size = abs((2-2*radius) / (pow(num_child, max_row-num_row)));
        offset = offset_x -0.5*box_size + (i+0.5)*box_size / num_child;
        create_tree(par, child, num_row - 1, num_child, offset, max_row, 
                    radius, id, licolor, cicolor, time+0.5, lines, circles);
        root.children.push_back(child);

        // Creating array for drawing line
        array[0] = position;
        array[1] = {offset, 
                    -(((2-2*radius)/(max_row))*(max_row - num_row+1) 
                     - 1 + radius), 0};
        create_line(line, array, licolor);
        add_keyframes(par, line, time, time + 0.5);
        lines.push_back(line);

    }
}

// Function to do a depth-first search recursively
void DFS_recursive(Param &par, const node& root, double &time){

    int id = par.shapes.size()/2 + root.ID;
    glm::vec3 new_color = {0,0,1};
    glm::vec3 old_color = vertex_color(par.shapes[id],0);
    add_color_keyframe(par, par.shapes[id], new_color, time);
    time += 1;
    add_color_keyframe(par, par.shapes[id], old_color, time);
    if (root.children.size() == 0){
        return;
    }
    for (size_t i = 0; i < root.children.size(); ++i){
        DFS_recursive(par, root.children[i], time);
    }
}

// Function to do a depth-first search with a stack
void DFS_stack(const node& root){

    std::stack<node> s;
    s.push(root);
    node temp;

    while (s.size() > 0){
        temp = s.top();
        s.pop();
        for (size_t i = 0; i < temp.children.size(); ++i){
            s.push(temp.children[i]);
        }
        std::cout << temp.ID << '\n';
    }
}

// Function to do a breadth-first search with a queue
void BFS_queue(Param &par, const node& root, double &time){
    std::queue<node> q;
    q.push(root);
    node temp;



    while (q.size() > 0){
        temp = q.front();
        q.pop();
        for (size_t i = 0; i < temp.children.size(); ++i){
            q.push(temp.children[i]);
        }
        int id = par.shapes.size()/2 + temp.ID;
        glm::vec3 new_color = {0,0,1};
        glm::vec3 old_color = vertex_color(par.shapes[id],0);
        add_color_keyframe(par, par.shapes[id], new_color, time);
        time += 1;
        add_color_keyframe(par, par.shapes[id], old_color, time);

    }
}

// Euclidean algorithm with mod
void euclid_mod(Param &par, int a, int b){

    int max_val;
    if (a > b){
        max_val = a;
    }
    else{
        max_val = b;
    }

    std::vector<int> divisors;
    int temp;
    while (b != 0){
        divisors.push_back(a);
        divisors.push_back(b);
        temp = b;
        b = a%b;
        a = temp;
    }

    divisors.push_back(a);
    divisors.push_back(0);

    par.imap["gcd"] = a;

    Shape line;
    std::vector<glm::vec3> pos(2);
    glm::vec3 color = {0.5,0,0.5};
    glm::vec3 color2 = {0.25,0.25,1.0};
    double time = curr_time(par);
    for (int i = 0; i < divisors.size(); i+=2){
        pos[0] = {i*1.8/divisors.size()-0.9-line.rad,-0.45,0};
        pos[1] = {i*1.8/divisors.size()-0.9-line.rad,
                  1.45*divisors[i]
                  /(double)max_val-0.45,0};
        create_line(line, pos, color);
        add_keyframes(par, line, time, time+1);
        par.shapes.push_back(line);

        pos[0] = {i*1.8/divisors.size()-0.9+line.rad,-0.45,0};
        pos[1] = {i*1.8/divisors.size()-0.9+line.rad,
                 1.45*divisors[i+1]
                  /(double)max_val-0.45,0};
        if(abs(pos[0][1] - pos[1][1]) != 0.0){ 
            create_line(line, pos, color2);
            add_keyframes(par, line, time, time+1);
            par.shapes.push_back(line);
        }
        time += 1;

    }

    std::swap(par.shapes[par.shapes.size()-1], par.shapes[1]);

}

// Euclidean algorithm with subtraction
void euclid_sub(Param &par, int a, int b){

    int max_val;
    if (a > b){
        max_val = a;
    }
    else{
        max_val = b;
    }

    std::vector<int> divisors;

    divisors.push_back(a);
    divisors.push_back(b);

    while (a != b){
        if (a > b){
            a = a - b;
        }
        else{
            b = b - a;
        }
        divisors.push_back(a);
        divisors.push_back(b);
    }

    par.imap["gcd"] = a;

    Shape line;
    double x_pos, y_pos;
    std::vector<glm::vec3> pos(2);
    glm::vec3 color = {0.5,0,0.5};
    glm::vec3 color2 = {0.25,0.25,1.0};
    double time = curr_time(par);
    for (int i = 0; i < divisors.size(); i+=2){

        // Line a
        pos[0] = {i*1.8/divisors.size()-0.9-line.rad,-0.45,0};
        pos[1] = {i*1.8/divisors.size()-0.9-line.rad,
                  1.45*divisors[i]/(double)max_val-0.45,0};
        create_line(line, pos, color);
        add_keyframes(par, line, time, time+1);
        par.shapes.push_back(line);

        // Line b
        pos[0] = {i*1.8/divisors.size()-0.9+line.rad,-0.45,0};
        pos[1] = {i*1.8/divisors.size()-0.9+line.rad,
                  1.45*divisors[i+1]/(double)max_val-0.45,0};
        create_line(line, pos, color2);
        add_keyframes(par, line, time, time+1);
        par.shapes.push_back(line);

        time += 1.1;
    }

    std::swap(par.shapes[par.shapes.size()-1], par.shapes[1]);

}

// Euclidean clear screen
void euclid_clear(Param &par){

    std::vector<Shape> elements(2);
    elements[0] = par.shapes[0];
    elements[1] = par.shapes[par.shapes.size()-1];
    par.shapes = elements;

}

// Function to write text on butterfly diagram
void write_butterfly(Param &par){

    std::vector<int> reverse_order = {0,4,2,6,1,5,3,7};
    double ypos = 0.85;
    double xpos = -0.35;
    double offset;
    std::string temp, val_string;
    for (int i = 0; i < 8; ++i){
        // Lowercase a
        temp = "a[";
        temp.append(std::to_string(reverse_order[i]));
        temp.append("]");
        glm::vec3 pos = {-0.975,ypos,0};
        write_string(par, temp, pos, 1.0f, glm::vec3(1, 1, 1));

        // Capital A
        temp = "A[";
        temp.append(std::to_string(i));
        temp.append("]");
        pos = {0.825,ypos,0};
        write_string(par, temp, pos, 1.0f, glm::vec3(1, 1, 1));

        xpos = -0.35;
        for (int j = 0; j < 3; ++j){
            if (i%(int)pow(2,j+1) >= (int)pow(2,j+1)/2){
                val_string = "-W";
                offset = 0.08;
            }
            else{
                val_string = "W";
                offset = 0.06;
            }
            pos = {xpos,ypos-0.03,0};
            write_string(par, val_string, pos, 0.75f, glm::vec3(1, 1, 1));

            pos = {xpos+offset,ypos-0.035,0};
            temp = std::to_string((int)pow(2,j+1));
            write_string(par, temp, pos, 0.5f, glm::vec3(1, 1, 1));

            pos = {xpos+offset,ypos-0.005,0};
            temp = std::to_string((i%(int)pow(2,j))%(int)pow(2,j+1));
            write_string(par, temp, pos, 0.5f, glm::vec3(1, 1, 1));
            xpos += 0.525;
        }
        ypos -= 0.25;
    }
}

// function to find all the people we need to thank
std::vector<std::string> find_names(std::string filename){

    std::ifstream input(filename);

    std::vector<std::string> names;

    std::string line;
    while (std::getline(input, line)){
        names.push_back(line);
    }

    input.close();

    return names;

}

