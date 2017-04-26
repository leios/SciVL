/*-------------distributions.cpp----------------------------------------------//
*
* Purpose: to hold all possible distributions for SDL and OGL stuff
*
*-----------------------------------------------------------------------------*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fftw3.h>

#include "../include/distributions.h"
#include "../include/shape_functions.h"
#include "../include/test_pong.h"
#include "../include/shaders.h"
#include "../include/operations.h"

// STD functions
void std_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
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

    SDL_GL_SwapWindow(par.screen);

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


void example_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
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

    SDL_GL_SwapWindow(par.screen);

}

void example_par(Param &par){
    par.set_fns();
    par.width = 640;
    par.height = 480;
    par.dist = "example";
    par.end = 0;

}

// Test functions
void test_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
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

    SDL_GL_SwapWindow(par.screen);

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
void test_shader_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
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
    write_string(par, "sample text", pos, 1.0f, glm::vec3(0.5, 0.0f, 0.5f));

    SDL_GL_SwapWindow(par.screen);

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

#ifdef __APPLE__
    par.font = "/Library/Fonts/Arial.ttf";
#else
    par.font = "/usr/share/fonts/TTF/arial.ttf";
#endif
    par.font_size = 48;

}

void test_shader_OGL(Param &par){
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

    // this should use shaders...
    Shader defaultShader;
    defaultShader.Load("shaders/default.vtx", "shaders/default.frg");
    par.shmap["default"] = defaultShader;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(10);

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

    Shape line;
    std::vector<glm::vec3> array(3);
    array[0] = {-0.5, 0.5, 0.0};
    array[1] = {0.5, 0.5, 0.0};
    array[2] = {0.0, 0.0, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

}

// Test functions using shader.h
void test_fft_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;

        case SDLK_LEFT:
            par.factors[par.curr_factor] -= 0.1;
            update_fft(par);
            break;
        case SDLK_RIGHT:
            par.factors[par.curr_factor] += 0.1;
            update_fft(par);
            break;
        case SDLK_DOWN:
            if (par.curr_factor < 2){
                par.curr_factor += 1;
                if (par.curr_factor > par.factors.size()-1){
                    par.factors.push_back((double)par.curr_factor + 1);
                }
                glm::vec3 trans = {0.0, -0.2, 0.0};
                move_shape(par.shapes[par.shapes.size()-3], trans);
            }
            update_fft(par);
            break;
        case SDLK_UP:
            if (par.curr_factor > 0){
                par.curr_factor -= 1;
                glm::vec3 trans = {0.0, 0.2, 0.0};
                move_shape(par.shapes[par.shapes.size()-3], trans);
            }
            update_fft(par);
            break;

        default:
            break;

    }

}

void test_fft_fn(Param &par){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    draw_fft(par);

    SDL_GL_SwapWindow(par.screen);

}

void test_fft_par(Param &par){
    par.set_fns();
    //par.width = 3000;
    //par.height = 3000;
    par.width = 1000;
    par.height = 1000;
    par.dist = "test_fft";
    par.end = 0;


    par.factors.push_back(1.0);
    par.imap["res"] = 100;

#ifdef __APPLE__
    par.font = "/Library/Fonts/Arial.ttf";
#else
    par.font = "/usr/share/fonts/TTF/arial.ttf";
#endif
    par.font_size = 48;

}

void test_fft_OGL(Param &par){
    glewExperimental = GL_TRUE;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    // Creating the xy axis for plotting
    Shape line;
    std::vector<glm::vec3> array(3);
    array[0] = {-0.95, 0.85, 0.0};
    array[1] = {-0.95, -0.1, 0.0};
    array[2] = {-0.05, -0.1, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    array[0] = {0.05, 0.85, 0.0};
    array[1] = {0.05, -0.1, 0.0};
    array[2] = {0.95, -0.1, 0.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    // Creating the box around the text for our current factor that we are using
    std::vector<glm::vec3> box(9);
    box[0] = {-0.7, -0.3, 0.0};
    box[1] = {0.0, -0.3, 0.0};
    box[2] = {0.7, -0.3, 0.0};
    box[3] = {0.7, -0.4, 0.0};
    box[4] = {0.7, -0.45, 0.0};
    box[5] = {0.0, -0.45, 0.0};
    box[6] = {-0.7, -0.45, 0.0};
    box[7] = {-0.7, -0.4, 0.0};
    box[8] = {-0.7, -0.3, 0.0};

    glm::vec3 box_color = {1.0, 1.0, 1.0};
    create_line(line, box, box_color);
    par.shapes.push_back(line);

    int res = par.imap["res"];

    // Creating the two arrays for plotting 
    fftw_complex *wave, *ftwave;
    wave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);
    ftwave = ( fftw_complex* ) fftw_malloc(sizeof (fftw_complex ) * res);

    // Creating the plan for fft'ing
    fftw_plan plan;
    for (int i = 0; i < res; ++i){
        wave[i][0] = sin(par.factors[0]*2*M_PI*i/(double)res);
        wave[i][1] = 0;
    }

    normalize(wave, res);

    // Performing fft
    plan = fftw_plan_dft_1d(res, wave, ftwave, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    fftw_destroy_plan(plan);

    normalize(ftwave, res);

    // now creating a sinusoidal wave
    std::vector<glm::vec3> sinarr(res);
    std::vector<glm::vec3> fftarr(res / 2);
    for (size_t i = 0; i < sinarr.size(); ++i){
        sinarr[i].x = -0.95 + 0.9 * (double)i / sinarr.size();
        sinarr[i].y = (wave[i][0]) * 0.5 * 0.9 + 0.4;
        sinarr[i].z = 0;
        if (i < res / 2){
            fftarr[i].x = 0.05 + 0.9 * 2 * (double)i / sinarr.size();
            fftarr[i].y = (abs2(ftwave[i])) * 0.5 * 0.9 + 0.4;
            fftarr[i].z = 0;
        }
        //std::cout << wave[i][0] << '\t' << ftwave[i][0] << '\n';
    }
    create_line(line, sinarr, licolor);
    par.shapes.push_back(line);
    create_line(line, fftarr, licolor);
    par.shapes.push_back(line);

}

// Test functions using shader.h
void test_pend_key(Param &par, SDL_Keysym* Keysym){
    switch(Keysym->sym){
        case SDLK_ESCAPE:
        case SDLK_q:
            par.end = 1;
            break;
        case SDLK_LEFT:
            if(par.shapes[1].vertices[0] - par.dmap["radius"] > -1){
                glm::vec3 trans = {-0.05, 0.0, 0.0};
                move_shape(par.shapes[1], trans);
                //move_shape(par.shapes[0], trans);
                move_vertex(par.shapes[0], trans, 0);
                move_vertex(par.shapes[0], trans, 1);
                par.dmap["theta"] += asin(0.05/0.5);
            }
            break;
        case SDLK_RIGHT:
            if(par.shapes[1].vertices[0] + par.dmap["radius"] < 1){
                glm::vec3 trans = {0.05, 0.0, 0.0};
                move_shape(par.shapes[1], trans);
                //move_shape(par.shapes[0], trans);
                move_vertex(par.shapes[0], trans, 0);
                move_vertex(par.shapes[0], trans, 1);
                par.dmap["theta"] -= asin(0.05/0.5);
            }
            break;
        default:
            break;

    }

}

void test_pend_fn(Param &par){

    move_pendulum(par);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_shapes(par);
    glm::vec3 pos = {20.0f, 20.0f, 0.0f};
    write_string(par, "sample text", pos, 1.0f, glm::vec3(0.5, 0.0f, 0.5f));

    SDL_GL_SwapWindow(par.screen);

}

void test_pend_par(Param &par){
    par.set_fns();
    par.width = 500;
    par.height = 500;
    par.dist = "test_pend";
    par.end = 0;

    par.dmap["alpha"] = 0.0;
    par.dmap["theta"] = 0.0;
    par.dmap["theta_prev"] = 0.0;
    par.dmap["radius"] = 0.1;
    par.dmap["timestep"] = 0.05;
    par.imap["res"] = 50;

#ifdef __APPLE__
    par.font = "/Library/Fonts/Arial.ttf";
#else
    par.font = "/usr/share/fonts/TTF/arial.ttf";
#endif
    par.font_size = 48;

}

void test_pend_OGL(Param &par){
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

    // Creating a line to work with
    Shape line;
    std::vector<glm::vec3> array(2);
    array[0] = {0.0, 0.0, 0.0};
    array[1] = {0.0, -0.5, 0.0};

    glm::vec3 licolor = {1.0, 0.0, 1.0};

    create_line(line, array, licolor);
    par.shapes.push_back(line);

    // Creating a circle to work with
    Shape circle;
    float rad = (float)par.dmap["radius"];
    glm::vec3 cloc = {0.0, 0.0, 0.0},
              ccolor = {1.0, 0.0, 1.0};
    create_circle(circle, cloc, rad, ccolor, par.imap["res"]); 
    par.shapes.push_back(circle);

}

