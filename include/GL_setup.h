/*-------------GL_setup.h-----------------------------------------------------//
*
* Purpose: This holds all functions for setting up OpenGL with SDL
*
*-----------------------------------------------------------------------------*/

#ifndef GL_SETUP_H
#define GL_SETUP_H

// GLEW must be defined first
#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else  // __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <random>
#include <vector>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <ctime>
#include <chrono>

// For font rendering
#include <ft2build.h>
#include FT_FREETYPE_H  

// Struct to hold freetype character information
struct Character{
    // glyph texture ID
    GLuint texID;

    // Size of glyph
    glm::ivec2 size;

    // Offset from baseline to left / top of glyph
    glm::ivec2 bearing;

    // Offset to advance to next glyph
    GLuint advance;
};

#include "../include/shaders.h"
// Struct to hold all data for shape drawing
struct Shape{
    GLfloat* vertices;
    GLuint* indices;
    GLuint VAO, VBO, EBO;
    double rad = 0.01;

    // Integer for the number of vertices
    int vnum;
    int ind;

    // Stage of simulation
    int stage;

    double start_time, end_time;

    int rtype = GL_TRIANGLES;
    void destroy();
};

// struct to hold most data
struct Param{
    std::unordered_map<std::string, int> imap;
    std::unordered_map<std::string, double> dmap;
    std::unordered_map<std::string, GLuint> uimap;
    std::unordered_map<std::string, Shader> shmap;
    std::vector<Shape> shapes;
    int end, width, height;
    std::string dist;

    SDL_Window* screen;
    SDL_GLContext context;

    // Function pointers for all drawing functions
    typedef void (*functionPtr)(Param&);
    functionPtr draw_fn, par_fn, OGL_fn;

    // Function pointer for the key functions
    typedef void (*functionPtr_key)(Param&, SDL_Keysym*);
    functionPtr_key key_fn;

    // function to set all mapped functions
    void set_fns(std::string dist);

    // Text rendering parameters
    std::string font;
    int font_size;
    std::map<GLchar, Character> chmap;

    // Adding in a vector for fft factors
    std::vector<double> factors;
    int curr_factor = 0;

    Shape text;

    void destroy();

    bool fullscreen;

    std::chrono::high_resolution_clock::time_point start_time;

};

// Struct to hold all data for scenes
struct Scene{
    Param par;
    std::vector<Shape> shapes;

    std::chrono::high_resolution_clock::time_point start_time;
};

// Function to set parameters
void set_params(Param &par);

// OGL set-up function
void setup_OGL(Param &par);

// SDL initialization
void SDL_init(Param &par);

// Drawing to screen
void draw_screen(Param &par);

// Dealing with key presses
void key_down(Param &par, SDL_Keysym* keysym);

// Processing with events
void process_events(Param &par);

// Dealing with text input and such
void write_string(Param &par, std::string text, glm::vec3 pos, GLfloat scale, 
                  glm::vec3 color);

// function to set up freetype
void setup_freetype(Param &par);

#endif
