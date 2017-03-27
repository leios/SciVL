/*-------------shape_functions.h----------------------------------------------//
*
* Purpose: To keep all the functions for drawing different shapes for SciVL
*
*-----------------------------------------------------------------------------*/

#ifndef SHAPE_FUNCTIONS_H
#define SHAPE_FUNCTIONS_H

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include "../include/distributions.h"

// Function to draw a single shape
void draw_shape(Param &par, std::string shapestr);

// Function to draw all shapes in the par shape map
void draw_shapes(Param &par);

// Function to create a rectangle at a given position with size and color
void create_rectangle(Shape &rect, glm::vec3 &pos, 
                      glm::vec3 &dim, glm::vec3 &color);

// Function to create a square shape centered at the origin
Shape create_square(Param &par);

// Function to move shape
void move_shape(Shape &sh, glm::vec3 &translate);

// Function to transform shape
void transform_shape(Shape &sh, glm::mat3 &transform);

// function to draw a circle, must have a radius and position
void draw_circle(Param &par);

// function to draw a square with a position and size
void draw_square(Param &par);

#endif
