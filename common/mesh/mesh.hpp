#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "vertex_attribute_pointer.h"

class Mesh {
private:
    //VAO: Vertex Array Object
    //VBO: Vertex Buffer Object
    //EBO: Elements Buffer Object
    //ElementCount: store number of vertices
    GLuint VAO, VBO, EBO, ElementCount;

    Mesh(Mesh const &) = delete;
    Mesh & operator =(Mesh const &) = delete;

public:
    Mesh(){ VAO = VBO = EBO = (GLuint)0; ElementCount = 0; }
    ~Mesh(){
        //Delete Objects if created
        if(VAO != 0) glDeleteVertexArrays(1, &VAO);
        if(VBO != 0) glDeleteBuffers(1, &VBO);
        if(EBO != 0) glDeleteBuffers(1, &EBO);
    }
    
    template<typename T>
    void setup(const std::vector<T> &vertices, const std::vector<GLuint> &indices){
        ElementCount = indices.size();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
        defineAttributePointer<T>();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    
    void draw(){
        if(VAO == 0) return; //Check if VAO is created
        //Bind Vertex Array
        glBindVertexArray(VAO);
        //DrawElements uses the element buffer to create triangles to be drawn
        glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, (void*)0);
        //Unbind Vertex Array
        glBindVertexArray(0);
    }
};

#endif