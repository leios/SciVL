/*-------------GL_setup.cpp---------------------------------------------------//
*
* Purpose: This holds all functions for setting up OpenGL to work with SDL
*
*-----------------------------------------------------------------------------*/

#include "../include/GL_setup.h"
#include "../include/distributions.h"
#include FT_FREETYPE_H


// Function to set parameters
Param set_params(std::string dist){
    Param par;
    par.set_fns();
    par.par_fns[dist](par);
    return par;
}

// function to set the drawing functions in the Param struct
void Param::set_fns(){
    draw_fns["std"] = std_fn;
    key_fns["std"] = std_key;
    par_fns["std"] = std_par;
    OGL_fns["std"] = std_OGL;

    draw_fns["example"] = example_fn;
    key_fns["example"] = example_key;
    par_fns["example"] = example_par;
    OGL_fns["example"] = example_OGL;

    draw_fns["test"] = test_fn;
    key_fns["test"] = test_key;
    par_fns["test"] = test_par;
    OGL_fns["test"] = test_OGL;

    draw_fns["test_shader"] = test_shader_fn;
    key_fns["test_shader"] = test_shader_key;
    par_fns["test_shader"] = test_shader_par;
    OGL_fns["test_shader"] = test_shader_OGL;

    draw_fns["test_fft"] = test_fft_fn;
    key_fns["test_fft"] = test_fft_key;
    par_fns["test_fft"] = test_fft_par;
    OGL_fns["test_fft"] = test_fft_OGL;

    draw_fns["test_pend"] = test_pend_fn;
    key_fns["test_pend"] = test_pend_key;
    par_fns["test_pend"] = test_pend_par;
    OGL_fns["test_pend"] = test_pend_OGL;
}

// Function to set-up OGL
void setup_OGL(Param &par){

    par.OGL_fns[par.dist](par);

}

// SDL initialization
void SDL_init(Param &par){

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Setting up SDL_OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    glEnable(GL_MULTISAMPLE);

    // Create Window
    par.screen = SDL_CreateWindow("SDL_TEST", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, par.width,
                                  par.height, 
                                  SDL_WINDOW_OPENGL);

    par.context = SDL_GL_CreateContext(par.screen);

    // Setting OGL specific values:
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


}

// Drawing to screen
void draw_screen(Param &par){

    par.draw_fns[par.dist](par);
}

void key_down(Param &par,  SDL_Keysym* keysym){
    par.key_fns[par.dist](par, keysym);
}

// Function to process events in game loop
void process_events(Param &par){

    // Creating an event
    SDL_Event event;

    while( SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
            {
                par.end = 1;
                break;
            }
            case SDL_KEYDOWN:
            {
                key_down(par, &event.key.keysym);
                break;
            }
        }
    }
}

// Dealing with text input and such
void write_string(Param &par, std::string text, glm::vec3 pos, GLfloat scale, 
                  glm::vec3 color){

    //Using the corresponding shader
    //Shader textShader = par.shmap["text"];
    //textShader.Use();

    par.shmap["text"].Use();

    glUniform3f(glGetUniformLocation(par.shmap["text"].Program, "textColor"),
                color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(par.text.VAO);

    std::string::const_iterator c;
    Character ch;
    GLfloat x_pos, y_pos;

    // setting up the character positions
    pos[0] = pos[0]*par.width*0.5 + par.width * 0.5;
    pos[1] = pos[1]*par.height*0.5 + par.height * 0.5;
    for (c = text.begin(); c != text.end(); c++){
        ch = par.chmap[*c];

        x_pos = pos[0] + ch.bearing.x * scale;
        y_pos = pos[1] - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        // Updating VBO for each character
        GLfloat vertices[6][4] = {
            {x_pos, y_pos + h, 0.0, 0.0},
            {x_pos, y_pos, 0.0, 1.0},
            {x_pos + w, y_pos, 1.0, 1.0},
            {x_pos, y_pos + h, 0.0, 0.0},
            {x_pos + w, y_pos, 1.0, 1.0},
            {x_pos + w, y_pos + h, 1.0, 0.0}
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texID);

        // Update contents of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, par.text.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance for next character 
        // advance is number of 1/64 pixels, so bitshift
        pos[0] += (ch.advance >> 6) * scale; 
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

// function to set up freetype
void setup_freetype(Param &par){

    FT_Library ft;
    if (FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << '\n';
    }
    
    FT_Face face;
    if (FT_New_Face(ft, par.font.c_str(), 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << '\n';; 
    }

    FT_Set_Pixel_Sizes(face, 0, par.font_size);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++){

        // Load Character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << '\n';
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Setting texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now to store the character for later
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        par.chmap.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Freeing freetype resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

