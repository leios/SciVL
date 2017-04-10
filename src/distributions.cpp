/*-------------distributions.cpp----------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/distributions.h"
#include "../include/shape_functions.h"
#include "../include/test_pong.h"
#include "../include/shaders.h"

// STD functions
void std_key(Param &par, SDL_keysym* keysym){
    switch(keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        default:
            break;
    }

}

void std_fn(Param &par){
    // OpenGL stuff
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapBuffers();

}

void std_par(Param &par){
    par.set_fns();
    par.width = 640;
    par.height = 480;
    par.dist = "std";
    par.end = 0;

}

void std_OGL(Param &par){
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\n';
        exit(1);
    }

    glViewport(0,0,640,480);

}

// Example functions -- from https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html

void example_OGL(Param &par){
    float ratio = (float) par.width / (float) par.height;

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, par.width, par.height );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\n';
        exit(1);
    }

    glViewport(0,0,640,480);

}


void example_key(Param &par, SDL_keysym* keysym){
    switch(keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        default:
            break;
    }

}

void example_fn(Param &par){

    /* Our angle of rotation. */
    static float angle = 0.0f;

    static GLfloat v0[] = { -1.0f, -1.0f,  1.0f };
    static GLfloat v1[] = {  1.0f, -1.0f,  1.0f };
    static GLfloat v2[] = {  1.0f,  1.0f,  1.0f };
    static GLfloat v3[] = { -1.0f,  1.0f,  1.0f };
    static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
    static GLfloat v5[] = {  1.0f, -1.0f, -1.0f };
    static GLfloat v6[] = {  1.0f,  1.0f, -1.0f };
    static GLfloat v7[] = { -1.0f,  1.0f, -1.0f };
    static GLubyte red[]    = { 255,   0,   0, 255 };
    static GLubyte green[]  = {   0, 255,   0, 255 };
    static GLubyte blue[]   = {   0,   0, 255, 255 };
    static GLubyte white[]  = { 255, 255, 255, 255 };
    static GLubyte yellow[] = {   0, 255, 255, 255 };
    static GLubyte black[]  = {   0,   0,   0, 255 };
    static GLubyte orange[] = { 255, 255,   0, 255 };
    static GLubyte purple[] = { 255,   0, 255,   0 };

    /* Clear the color and depth buffers. */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* We don't want to modify the projection matrix. */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    /* Move down the z-axis. */
    glTranslatef( 0.0, 0.0, -5.0 );

    /* Rotate. */
    glRotatef( angle, 0.0, 1.0, 0.0 );

    if( ++angle > 360.0f ) {
        angle = 0.0f;
    }

    /* Send our triangle data to the pipeline. */
    glBegin( GL_TRIANGLES );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( black );
    glVertex3fv( v5 );

    glEnd( );

    SDL_GL_SwapBuffers();

}

void example_par(Param &par){
    par.set_fns();
    par.width = 640;
    par.height = 480;
    par.dist = "example";
    par.end = 0;

}

// Test functions
void test_key(Param &par, SDL_keysym* keysym){
    switch(keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;

        case SDLK_UP:
            if (par.dmap["rbumper"] < 0.5){
                par.dmap["rbumper"] += 0.05;
            }
            break;
        case SDLK_DOWN:
            if (par.dmap["rbumper"] > -0.5){
                par.dmap["rbumper"] -= 0.05;
            }
            break;
        case SDLK_w:
            if (par.dmap["lbumper"] < 0.5){
                par.dmap["lbumper"] += 0.05;
            }
            break;
        case SDLK_s:
            if (par.dmap["lbumper"] > -0.5){
                par.dmap["lbumper"] -= 0.05;
            }
            break;
        default:
            break;

    }

}

void test_fn(Param &par){

    play_pong(par);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    glLineWidth(30);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBegin(GL_POLYGON);
        glVertex2f(1.0f, 0.5f + par.dmap["rbumper"]);
        glVertex2f(1.0f, -0.5f + par.dmap["rbumper"]);
        glVertex2f(0.9f, -0.5f + par.dmap["rbumper"]);
        glVertex2f(0.9f, 0.5f + par.dmap["rbumper"]);
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.5f + par.dmap["lbumper"]);
        glVertex2f(-1.0f, -0.5f + par.dmap["lbumper"]);
        glVertex2f(-0.9f, -0.5f + par.dmap["lbumper"]);
        glVertex2f(-0.9f, 0.5f + par.dmap["lbumper"]);
    glEnd();

    glColor3f(0.0,1.0,0.0);
    draw_circle(par);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glFlush(); 

    SDL_GL_SwapBuffers();

}

void test_par(Param &par){
    par.set_fns();
    par.width = 500;
    par.height = 500;
    par.dist = "test";
    par.end = 0;

    par.dmap["rbumper"] = 0.0;
    par.dmap["lbumper"] = 0.0;
    par.dmap["radius"] = 0.1;
    par.dmap["pos_x"] = 0.0;
    par.dmap["pos_y"] = 0.0;
    par.dmap["vel_y"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["vel_x"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["timestep"] = 0.05;
    par.imap["res"] = 100;

}

void test_OGL(Param &par){
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\n';
        exit(1);
    }

    glViewport(0,0,par.width,par.height);

}

// Test functions using shader.h
void test_shader_key(Param &par, SDL_keysym* keysym){
    switch(keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;

        case SDLK_UP:
            if (par.dmap["rbumper"] < 0.5){
                par.dmap["rbumper"] += 0.05;
                glm::vec3 trans = {0.0, 0.05, 0.0};
                move_shape(par.shapes[2], trans);
            }
            break;
        case SDLK_DOWN:
            if (par.dmap["rbumper"] > -0.5){
                par.dmap["rbumper"] -= 0.05;
                glm::vec3 trans = {0.0, -0.05, 0.0};
                move_shape(par.shapes[2], trans);
            }
            break;
        case SDLK_w:
            if (par.dmap["lbumper"] < 0.5){
                par.dmap["lbumper"] += 0.05;
                glm::vec3 trans = {0.0, 0.05, 0.0};
                move_shape(par.shapes[1], trans);
            }
            break;
        case SDLK_s:
            if (par.dmap["lbumper"] > -0.5){
                par.dmap["lbumper"] -= 0.05;
                glm::vec3 trans = {0.0, -0.05, 0.0};
                move_shape(par.shapes[1], trans);
            }
            break;
        default:
            break;

    }

}

void test_shader_fn(Param &par){

    play_pong(par);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    glm::vec3 pos = {20.0f, 20.0f, 0.0f};
    write_string(par, "sample text", pos, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    SDL_GL_SwapBuffers();

}

void test_shader_par(Param &par){
    par.set_fns();
    par.width = 500;
    par.height = 500;
    par.dist = "test_shader";
    par.end = 0;

    par.dmap["rbumper"] = 0.0;
    par.dmap["lbumper"] = 0.0;
    par.dmap["radius"] = 0.1;
    par.dmap["pos_x"] = 0.0;
    par.dmap["pos_y"] = 0.0;
    par.dmap["vel_y"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["vel_x"] = ((rand() % 1000) * 0.0001 - 0.5) * 0.1;
    par.dmap["timestep"] = 0.05;
    par.imap["res"] = 100;

    par.font = "/usr/share/fonts/TTF/ProggyClean.ttf";
    par.font_size = 48;

    setup_freetype(par);

}

void test_shader_OGL(Param &par){
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "You dun goofed!" << '\n';
        exit(1);
    }

    glViewport(0,0,par.width,par.height);

    // this should use shaders...
    Shader defaultShader;
    defaultShader.Load("shaders/default.vtx", "shaders/default.frg");
    par.shmap["default"] = defaultShader;

    Shader textShader;
    textShader.Load("shaders/text.vtx", "shaders/text.frg");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(par.width), 
                                      0.0f, static_cast<GLfloat>(par.height));
    textShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 
                       1, GL_FALSE, glm::value_ptr(projection));
    par.shmap["text"] = textShader;
    create_quad(par.text);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shape circle;
    float rad = (float)par.dmap["radius"];
    glm::vec3 cloc = {0.0, 0.0, 0.0},
              ccolor = {0.0, 1.0, 0.0};
    create_circle(circle, cloc, rad, ccolor, 100); 
    //rect = create_square(par);
    par.shapes.push_back(circle);

    Shape rect;
    glm::vec3 lloc = {-1.0, 0.0, 0.0}, 
              lsize = {0.2, 1.0, 0.0}, 
              lcolor = {0.0, 0.0, 1.0};
    create_rectangle(rect, lloc, lsize, lcolor); 
    par.shapes.push_back(rect);

    glm::vec3 rloc = {1.0, 0.0, 0.0}, 
              rsize = {0.2, 1.0, 0.0}, 
              rcolor = {1.0, 0.0, 0.0};
    create_rectangle(rect, rloc, rsize, rcolor); 
    par.shapes.push_back(rect);

}

