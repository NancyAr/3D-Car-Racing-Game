#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad.h>

class Texture2D {
private:
    GLuint id;
    GLsizei w, h;

    //Delete copy constructor and assignment operation
    Texture2D(Texture2D const &) = delete;
    Texture2D & operator =(Texture2D const &) = delete;
public:
    Texture2D(){glGenTextures(1, &id); w=h=0;}
    ~Texture2D(){glDeleteTextures(1, &id);}

    //Bind the texture to the target GL_TEXTURE_2D
    void bind(){glBindTexture(GL_TEXTURE_2D, id);}

    //Allocate texture memory
    //- internalFormat: the data format of the texture on the GPU
    //- width & height: size of the texture
    //- dataFormat: the format of the data sent in the "data" argument
    //- datatype: the type of the data sent in the "data" argument
    //- data: the data that we want to fill the texture with (use null if you don't care)
    //- miplevel: the mip level we want to insert the data in (usually 0 since we usually use glGenerateMipmap to create the rest)
    void setup(GLint internalFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum datatype, const GLvoid* data, GLint miplevel=0){
        w = width; h = height;
        //Bind the texture first
        glBindTexture(GL_TEXTURE_2D, id);
        //Send data to GPU texture memory
        glTexImage2D(GL_TEXTURE_2D, miplevel, internalFormat, width, height, 0, dataFormat, datatype, data);
    }

    GLsizei getWidth(){return w;}
    GLsizei getHeight(){return h;}
};

#endif