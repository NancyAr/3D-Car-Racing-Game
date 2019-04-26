#include <glad/glad.h>
#include "vertex.h"

template<typename T>
inline void defineAttributePointer(){
    //Force defining a specialization for vertex structs
    static_assert(false, "Type has no attribute pointer definition");
}

template<>
inline void defineAttributePointer<Vertex>(){
    //Assumes Location 0: Position, 1: Color, 2: UV, 3: Normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);
}