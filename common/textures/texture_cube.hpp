#ifndef TEXTURE_CUBE_HPP
#define TEXTURE_CUBE_HPP

#include <glad/glad.h>

class TextureCube {
private:
    GLuint id;
    GLsizei s;

    //Delete copy constructor and assignment operation
    TextureCube(TextureCube const &) = delete;
    TextureCube & operator =(TextureCube const &) = delete;
public:
    TextureCube(){glGenTextures(1, &id); s=0;}
    ~TextureCube(){glDeleteTextures(1, &id);}

    void bind(){glBindTexture(GL_TEXTURE_CUBE_MAP, id);}

    void setup(GLint internalFormat, GLsizei sideLength, GLenum dataFormat, GLenum datatype, const GLvoid* data[6], GLint miplevel=0){
        s = sideLength;
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        //NOTE: All Faces enums are order so GL_TEXTURE_CUBE_MAP_NEGATIVE_X = 1 + GL_TEXTURE_CUBE_MAP_POSITIVE_X
        for(int face = 0; face < 6; face++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, miplevel, internalFormat, s, s, 0, dataFormat, datatype, data[face]);
    }

    GLsizei getSideLength(){return s;}
};

#endif