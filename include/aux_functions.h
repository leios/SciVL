/*-------------aux_function.h-------------------------------------------------//
*
* Purpose: Work with the test function to make a pong game
*
*-----------------------------------------------------------------------------*/

#ifndef AUX_FUNCTIONS_H
#define AUX_FUNCTIONS_H

#include "../include/GL_setup.h"

struct node{
    std::vector<node> children;
    glm::vec3 pos;
    int ID;
};

// Function to create simple tree
void create_tree(Param &par, node& root, int num_row, int num_child, 
                 double offset_x, int max_row, double radius,
                 glm::vec3 licolor, glm::vec3 cicolor, 
                 std::vector<Shape> &lines, std::vector<Shape> &circles);

// Function to do a depth-first search recursively
void DFS_recursive(const node& root);

// Function to do a depth-first search with a stack
void DFS_stack(const node& root);

// Function to do a breadth-first search with a queue
void BFS_queue(const node& root);

void play_pong(Param &par);

// Function to update fft arrays
void update_fft(Param &par);
void draw_fft(Param &par);

// Functions to move asteroid for verlet visualization
void find_verlet_acc(Param &par);
void move_verlet_obj(Param &par);

glm::vec3 mult(glm::vec3 vec, double val);

#endif
