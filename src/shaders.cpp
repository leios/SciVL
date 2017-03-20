/*----------------------------------------------------------------------------//
*
* Purpose: This file holds the shader constructor for the struct in shaders.h
*
*   Notes: This file was heavily influenced by:
*              https://learnopengl.com/#!Getting-started/Shaders
*-----------------------------------------------------------------------------*/

#include "../include/shaders.h"

Shader::Shader(const GLchar* vertexSourcePath, 
               const GLchar* fragmentSourcePath){

    // Step 1: Retreive the vertex / fragment source code from filepath
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // checks if ifstream files can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | 
                           std::ifstream::badbit);

    fShaderFile.exceptions(std::ifstream::failbit | 
                           std::ifstream::badbit);

    try{
        // Opens files
        vShaderFile.open(vertexSourcePath);
        fShaderFile.open(fragmentSourcePath);

        // Reading buffer contents into stream
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream to GLchar*
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR: Shader files not successfully read" << '\n';
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    // Step 2: Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infolog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Print compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cout << "Error: Vertex shader compilation failed!" << infolog 
                  << '\n';
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Print compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        std::cout << "Error: Fragement shader compilation failed!" << infolog 
                  << '\n';
    }

    // now for the program, itself

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);

    glLinkProgram(this->Program);

    // print linking errors
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(this->Program, 512, NULL, infolog);
        std::cout << "Error: Shader program linking failed!" << infolog << '\n';
    }

    // Now to delete unnecessary shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

// Now for the Use function
void Shader::Use(){
    glUseProgram(this->Program);
}
