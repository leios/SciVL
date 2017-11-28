/*-------------GL_setup.cpp---------------------------------------------------//
*
* Purpose: This holds all functions for setting up OpenGL to work with SDL
*
*-----------------------------------------------------------------------------*/

#include "../include/GL_setup.h"
#include "../include/distributions.h"
#include FT_FREETYPE_H


// Function to set parameters
void set_params(Param &par){
    par.set_fns(par.dist);
    par.par_fn(par);
}

// function to set the drawing functions in the Param struct
void Param::set_fns(std::string dist){
    if (dist == "platformer"){
        draw_fn = platformer_fn;
        key_fn = platformer_key;
        par_fn = platformer_par;
        OGL_fn = platformer_OGL;
    }
    else if (dist == "test_anim"){
        draw_fn = test_anim_fn;
        key_fn = test_anim_key;
        par_fn = test_anim_par;
        OGL_fn = test_anim_OGL;
    }
    else if (dist == "pong"){
        draw_fn = pong_fn;
        key_fn = pong_key;
        par_fn = pong_par;
        OGL_fn = pong_OGL;
    }
    else if (dist == "fourier"){
        draw_fn = fourier_fn;
        key_fn = fourier_key;
        par_fn = fourier_par;
        OGL_fn = fourier_OGL;
    }
    else if (dist == "fft"){
        draw_fn = fft_fn;
        key_fn = fft_key;
        par_fn = fft_par;
        OGL_fn = fft_OGL;
    }
    else if (dist == "verlet"){
        draw_fn = verlet_fn;
        key_fn = verlet_key;
        par_fn = verlet_par;
        OGL_fn = verlet_OGL;
    }
    else if (dist == "traverse"){
        draw_fn = traverse_fn;
        key_fn = traverse_key;
        par_fn = traverse_par;
        OGL_fn = traverse_OGL;
    }
    else if (dist == "euclid"){
        draw_fn = euclid_fn;
        key_fn = euclid_key;
        par_fn = euclid_par;
        OGL_fn = euclid_OGL;
    }
    else if (dist == "thanks"){
        draw_fn = thanks_fn;
        key_fn = thanks_key;
        par_fn = thanks_par;
        OGL_fn = thanks_OGL;
    }
    else{
        if (dist != "std"){
            std::cout << "Simulation type " << dist << " does not exist!"
                      << '\n' << "Using standard output." << '\n';
            std::cout << "Try one of these: " << '\n'
                      << "pong" << '\n'
                      << "verlet" << '\n'
                      << "fourier" << '\n';
        }
        draw_fn = std_fn;
        key_fn = std_key;
        par_fn = std_par;
        OGL_fn = std_OGL;
    }
}

// Function to set-up OGL
void setup_OGL(Param &par){

    par.OGL_fn(par);

}

// SDL initialization
void SDL_init(Param &par){

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Setting up SDL_OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    //glEnable(GL_MULTISAMPLE);

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

    if (par.fullscreen){
        SDL_SetWindowFullscreen(par.screen, SDL_WINDOW_FULLSCREEN);
    }

}

// Drawing to screen
void draw_screen(Param &par){

    par.draw_fn(par);
}

void key_down(Param &par,  SDL_Keysym* keysym, bool is_down){
    par.key_fn(par, keysym, is_down);
}

// Function to process events in game loop
void process_events(Param &par){

    // re-adjusting curr_time
    par.curr_time = std::chrono::high_resolution_clock::now();

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
                key_down(par, &event.key.keysym, true);
                break;
            }
            case SDL_KEYUP:
            {
                key_down(par, &event.key.keysym, false);
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
            (GLuint) face->glyph->advance.x
        };
        par.chmap.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Freeing freetype resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// Frees vertices and indicies
void Shape::destroy(){
    free(vertices);
    free(indices);
}

// Detroys everything in Param
void Param::destroy(){
    for (size_t i = 0; i < shapes.size(); ++i){
        shapes[i].destroy();
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(screen);
    SDL_Quit();

}

double curr_time(Param &par){
    std::chrono::high_resolution_clock::time_point time = 
        std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> ctime = std::chrono::duration_cast<std::chrono::duration<double>>(time -par.start_time);
    return ctime.count(); 
}
