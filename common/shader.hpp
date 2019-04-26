#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

private:
    //Shader Program Handle
    GLuint programID;

    //Delete copy constructor and assignment operation
    Shader(Shader const &) = delete;
    Shader & operator =(Shader const &) = delete;

public:

    //Create Shader Program
    Shader() { programID = glCreateProgram(); }
    
    //Delete Shader Program
    ~Shader() { glDeleteProgram(programID); }

    //Bind shader program in order to be used
    void use() { glUseProgram(programID); }

    //Get Program Handle
    GLuint getID() { return programID; }

    //Read shader from file, send it to GPU, compile it then attach it to shader
    void attach(std::string const &filename, GLenum type){

        std::ifstream file(filename.c_str());
        if (file.fail())
        {
            std::cerr << "UNABLE TO OPEN FILE: \"" << filename << "\"\n";
            return;
        }
        std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

        GLuint shaderID = glCreateShader(type); //Create shader of the given type

        const char * sourceCharArray = source.c_str();
        glShaderSource(shaderID, 1, &sourceCharArray, nullptr); //Send shader source code
        glCompileShader(shaderID); //Compile the shader code

        //Check and log for any error in the compilation process
        GLint status;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if(!status){
            GLint length;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
            char* logStr = new char[length];
            glGetShaderInfoLog(shaderID, length, nullptr, logStr);
            std::cerr << "ERROR IN " << filename << std::endl;
            std::cerr << logStr << std::endl;
            delete[] logStr;
        }
        assert(status);


        glAttachShader(programID, shaderID); //Attach shader to program
        glDeleteShader(shaderID); //Delete shader (the shader is already attached to the program so its object is no longer needed)
    }

    //Link Program (Do this after all shaders are attached)
    void link()
    {
        //Link
        glLinkProgram(programID);

        //Check and log for any error in the linking process
        GLint status;
        glGetProgramiv(programID, GL_LINK_STATUS, &status);
        if (!status)
        {
            GLint length;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
            char* logStr = new char[length];
            glGetProgramInfoLog(programID, length, nullptr, logStr);
            std::cerr << "LINKING ERROR" << std::endl;
            std::cerr << logStr << std::endl;
            delete[] logStr;
        }
        assert(status);
    }

    //Get the location of a uniform variable in the shader
    GLuint getUniformLocation(const std::string& uniform){return glGetUniformLocation(programID, uniform.c_str());}

    //A group of setter for uniform variables
    //NOTE: It is inefficient to call glGetUniformLocation every frame
    //So it is usually a better option to either cache the location
    //or explicitly define the uniform location in the shader
    void set(const std::string& uniform, GLfloat value){
        glUniform1f(getUniformLocation(uniform), value);
    }

    void set(const std::string& uniform, GLint value){
        glUniform1i(getUniformLocation(uniform), value);
    }

    void set(const std::string& uniform, GLboolean value){
        glUniform1i(getUniformLocation(uniform), value);
    }

    void set(const std::string& uniform, glm::vec2 value){
        glUniform2f(getUniformLocation(uniform), value.x, value.y);
    }

    void set(const std::string& uniform, glm::vec3 value){
        glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
    }

    void set(const std::string& uniform, glm::mat4 value){
        glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

};

#endif