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

// Function to clear shapes
void clear_shapes(Param &par);

// Function to return location of vertex 
glm::vec3 vertex_location(Shape &sh, int id);

// Function to return color of vertex
glm::vec3 vertex_color(Shape &sh, int id);

// Function to move a single vertex
void move_vertex(Shape &sh, glm::vec3 &translate, int ind);

// Function to change color of all vertices
void change_color(Shape &sh, glm::vec3 &color);

// Function to draw a single shape
void draw_shape(Param &par, std::string shapestr);

// Function to animate a shape as it changes with time
void animate_line(Param &par, Shape &sh);

// Function to animate a circle as it changes with time
void animate_circle(Param &par, Shape &sh);

// Function to animate a rect as it changes with time
void animate_rect(Param &par, Shape &sh);

// Function to animate a shape moving with time
void animate_move(Param &par, Shape &sh);

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

// Function to create a circle for drawing
void create_circle(Shape &circle, glm::vec3 &pos, double radius, 
                   glm::vec3 color, int res);

// Function to animate the drawing of a circle
void grow_circle(Shape &circle, glm::vec3 &pos, double radius, 
                 glm::vec3 color, double draw_time);

// Function to create text quads
void create_quad(Shape &quad);

// function to create array shape
void create_line(Shape &line, glm::vec3 *array, int size, glm::vec3 &color);
void create_line(Shape &line, std::vector<glm::vec3> &array, glm::vec3 &color);

// Function to create odd integral shape from vector of vertices
void create_integral(Shape &integral, double *array, int size, 
                     const glm::vec3 pos, const glm::vec3 dim, 
                     const glm::vec3 &color);

// Function to update an array
void update_line(Shape &sh, glm::vec3 *new_array);
void update_integral(Shape &integral, double *new_array, double ydim, 
                     double pos);

// Function to add keyframes to a shape for drawing
void add_keyframes(Param &par, Shape &sh, double start_time, double end_time);
void add_color_keyframe(Param &par, Shape &sh, glm::vec3 &color, double time);
void add_move_keyframe(Param &par, Shape &sh, glm::vec3 &loc, double time);

// Functions to shift keyframes by a double value
void shift_keyframes(Param &par, Shape &sh, double time);
void shift_color_keyframes(Param &par, Shape &sh, double time);
void shift_move_keyframes(Param &par, Shape &sh, double time);

void create_human(Shape &human, const glm::vec3 loc, 
                  const glm::vec3 color, double radius);

#endif
