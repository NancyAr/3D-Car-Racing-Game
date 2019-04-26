#ifndef VERTEX_H
#define VERTEX_H

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 uv;
    glm::vec3 normal;

    bool operator==(const Vertex& other) const {
        return position == other.position && 
               color == other.color &&
               uv == other.uv &&
               normal == other.normal;
    }
};

namespace std {
    //A Simple method to combine two hash values
    inline size_t hash_combine(size_t h1, size_t h2){ return h1 ^ (h2 << 1); }
    
    //A Hash function for struct Vertex
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return hash_combine(hash<glm::vec3>()(vertex.position), 
                   hash_combine(hash<glm::vec4>()(vertex.color),
                   hash_combine(hash<glm::vec2>()(vertex.uv),hash<glm::vec3>()(vertex.normal))));
        }
    };
}

#endif