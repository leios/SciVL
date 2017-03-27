/*-------------shape_functions.cpp--------------------------------------------//
*
* Purpose: To keep all the functions for drawing different shapes for SciVL
*
*-----------------------------------------------------------------------------*/

#include "../include/shape_functions.h"

// Function to move shape
void move_shape(Shape &sh, glm::vec3 &translate){
    for (int i = 0; i < sh.vnum; ++i){
         sh.vertices[i * 6 + 0] += translate[0];
         sh.vertices[i * 6 + 1] += translate[1];
         sh.vertices[i * 6 + 2] += translate[2];
    }

    GLuint VAO, VBO, EBO;

    // Generating objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // Binding the vertex array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, sh.vertices, 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, 
                 sh.indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    sh.VAO = VAO;
    sh.VBO = VBO;
    sh.EBO = EBO;

}

// Function to transform shape
void transform_shape(Shape &sh, glm::mat3 &transform){
}


// Function to draw a single shape
void draw_shape(Param &par, Shape &sh){

    par.shmap["default"].Use();
    glBindVertexArray(sh.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sh.EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Function to draw all shapes in the par shape map
void draw_shapes(Param &par){
    for (auto &sh : par.shapes){
        draw_shape(par, sh);
    }

}

void create_rectangle(Shape &rect, glm::vec3 &pos, 
                      glm::vec3 &dim, glm::vec3 &color){

    rect.vertices = (GLfloat*)std::malloc(sizeof(GLfloat)*24);
    rect.vertices[0] = -dim[0] * 0.5f + pos[0];
    rect.vertices[1] = -dim[1] * 0.5f + pos[1];
    rect.vertices[2] = 0.0f;
    rect.vertices[3] = color[0];
    rect.vertices[4] = color[1];
    rect.vertices[5] = color[2];
    rect.vertices[6] = -dim[0] * 0.5f + pos[0];
    rect.vertices[7] = dim[1] * 0.5f + pos[1];
    rect.vertices[8] = 0.0f;
    rect.vertices[9] = color[0];
    rect.vertices[10] = color[1];
    rect.vertices[11] = color[2];
    rect.vertices[12] = dim[0] * 0.5f + pos[0];
    rect.vertices[13] = dim[1] * 0.5f + pos[1];
    rect.vertices[14] = 0.0f;
    rect.vertices[15] = color[0];
    rect.vertices[16] = color[1];
    rect.vertices[17] = color[2];
    rect.vertices[18] = dim[0] * 0.5f + pos[0];
    rect.vertices[19] = -dim[1] * 0.5f + pos[1];
    rect.vertices[20] = 0.0f;
    rect.vertices[21] = color[0];
    rect.vertices[22] = color[1];
    rect.vertices[23] = color[2];

    rect.indices = (GLuint*)std::malloc(sizeof(GLuint) * 6);
    rect.indices[0] = 0;
    rect.indices[1] = 1;
    rect.indices[2] = 3;
    rect.indices[3] = 2;
    rect.indices[4] = 1;
    rect.indices[5] = 3;

    GLuint VAO, VBO, EBO;

    // Generating objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Binding the vertex array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, rect.vertices, 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, 
                 rect.indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Setting square attributes
    rect.VAO = VAO;
    rect.VBO = VBO;
    rect.EBO = EBO;

    rect.vnum = 4;

}

// Function to create a square shape centered at the origin of length 1
Shape create_square(Param &par){
    Shape square;

    GLfloat rad = 0.5f;
    if (par.dmap.find("radius") != par.dmap.end()){
        rad = (GLfloat)par.dmap["radius"];
    }

    GLfloat vertices[] = {
        -rad, -rad, 0.0f, 1.0f, 0.0f, 1.0f,
        -rad, rad, 0.0f, 1.0f, 0.0f, 1.0f,
        rad, rad, 0.0f, 1.0f, 0.0f, 1.0f,
        rad, -rad, 0.0f, 1.0f, 0.0f, 1.0f
    };
    square.vertices = (GLfloat*)std::malloc(sizeof(vertices)*24);
    for (int i = 0; i < 24; ++i){
        square.vertices[i] = vertices[i];
    }

    square.indices = (GLuint*)std::malloc(sizeof(GLuint) * 6);
    square.indices = (GLuint*)std::malloc(sizeof(GLuint) * 6);
    square.indices[0] = 0;
    square.indices[1] = 1;
    square.indices[2] = 3;
    square.indices[3] = 2;
    square.indices[4] = 1;
    square.indices[5] = 3;


    GLuint VAO, VBO, EBO;

    // Generating objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Binding the vertex aray object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, square.vertices, 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 6, 
                 square.indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Setting square attributes
    square.VAO = VAO;
    square.VBO = VBO;
    square.EBO = EBO;

    square.vnum = 4;

    return square;
}


// function to draw a circle, must have a radius and position
void draw_circle(Param &par){

    // Pull appropriate variables from parameter list
    double radius = par.dmap["radius"];
    double pos_x = par.dmap["pos_x"];
    double pos_y = par.dmap["pos_y"];

    int res = par.imap["res"];

    // Using immediate mode, drawing a circle is easy using parametric eqns

    glBegin(GL_POLYGON);
    double theta = 0;
    for (int i = 0; i < res; i++){
        theta = -M_PI + 2*M_PI*i/(res-1);
        glVertex2f(pos_x + radius * cos(theta), pos_y + radius * sin(theta));
    }

    glEnd();
    
}
