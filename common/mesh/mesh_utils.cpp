#include "mesh_utils.hpp"
#include "vertex.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <glm/gtc/color_space.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include <unordered_map>

#define WHITE {1,1,1,1}

Mesh* MeshUtils::Box(){
glm::vec4 grey = {0.75f, 0.75f, 0.75f, 0.75f};
    glm::vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};

    glm::vec4 upper_face_color = grey;
    glm::vec4 lower_face_color = grey;
    glm::vec4 right_face_color = white;
    glm::vec4 left_face_color  = white;
    glm::vec4 front_face_color = white;
    glm::vec4 back_face_color  = white;

    std::vector<Vertex> vertices = {
        //Upper Face
        {{-0.5f,  0.5f, -0.5f}, upper_face_color},
        {{-0.5f,  0.5f,  0.5f}, upper_face_color},
        {{ 0.5f,  0.5f,  0.5f}, upper_face_color},
        {{ 0.5f,  0.5f, -0.5f}, upper_face_color},
        //Lower Face
        {{-0.5f, -0.5f, -0.5f}, lower_face_color},
        {{ 0.5f, -0.5f, -0.5f}, lower_face_color},
        {{ 0.5f, -0.5f,  0.5f}, lower_face_color},
        {{-0.5f, -0.5f,  0.5f}, lower_face_color},
        //Right Face
        {{ 0.5f, -0.5f, -0.5f}, right_face_color},
        {{ 0.5f,  0.5f, -0.5f}, right_face_color},
        {{ 0.5f,  0.5f,  0.5f}, right_face_color},
        {{ 0.5f, -0.5f,  0.5f}, right_face_color},
        //Left Face
        {{-0.5f, -0.5f, -0.5f}, left_face_color},
        {{-0.5f, -0.5f,  0.5f}, left_face_color},
        {{-0.5f,  0.5f,  0.5f}, left_face_color},
        {{-0.5f,  0.5f, -0.5f}, left_face_color},
        //Front Face
        {{-0.5f, -0.5f,  0.5f}, front_face_color},
        {{ 0.5f, -0.5f,  0.5f}, front_face_color},
        {{ 0.5f,  0.5f,  0.5f}, front_face_color},
        {{-0.5f,  0.5f,  0.5f}, front_face_color},
        //Back Face
        {{-0.5f, -0.5f, -0.5f}, back_face_color},
        {{-0.5f,  0.5f, -0.5f}, back_face_color},
        {{ 0.5f,  0.5f, -0.5f}, back_face_color},
        {{ 0.5f, -0.5f, -0.5f}, back_face_color},
    };
    std::vector<GLuint> indices = {
        //Upper Face
        0, 1, 2, 2, 3, 0,
        //Lower Face
        4, 5, 6, 6, 7, 4,
        //Right Face
        8, 9, 10, 10, 11, 8,
        //Left Face
        12, 13, 14, 14, 15, 12,
        //Front Face
        16, 17, 18, 18, 19, 16,
        //Back Face
        20, 21, 22, 22, 23, 20, 
    };

    Mesh* mesh = new Mesh();
    mesh->setup(vertices, indices);
    return mesh;
}
Mesh* MeshUtils::Sphere(int long_segments, int lat_segments){
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for(int lat = 0; lat <= lat_segments; lat++){
        float v = (float)lat / lat_segments; 
        float pitch = v * glm::pi<float>() - glm::half_pi<float>();
        float cos = glm::cos(pitch), sin = glm::sin(pitch);
        for(int lng = 0; lng <= long_segments; lng++){
            float u = (float)lng/long_segments; 
            float yaw = u * glm::two_pi<float>();
            glm::vec3 pos = {cos * glm::cos(yaw), sin, cos * glm::sin(yaw)};
            vertices.push_back({pos, WHITE, {u, v}, pos});
        }   
    }

    for(int lat = 1; lat <= lat_segments; lat++){
        int start = lat*(long_segments+1);
        for(int lng = 1; lng <= long_segments; lng++){
            int prev_lng = lng-1;
            indices.push_back(lng + start);
            indices.push_back(lng + start - long_segments - 1);
            indices.push_back(prev_lng + start - long_segments - 1);
            indices.push_back(prev_lng + start - long_segments - 1);
            indices.push_back(prev_lng + start);
            indices.push_back(lng + start);
        }
    }

    Mesh* mesh = new Mesh();
    mesh->setup(vertices, indices);
    return mesh;
}

Mesh* MeshUtils::Plane(glm::vec2 uv1, glm::vec2 uv2){
    Mesh* mesh = new Mesh();
    mesh->setup<Vertex>({
        {{-1, 0,  1},WHITE,uv1,{0,1,0}},
        {{ 1, 0,  1},WHITE,{uv2.x, uv1.y},{0,1,0}},
        {{ 1, 0, -1},WHITE,uv2,{0,1,0}},
        {{-1, 0, -1},WHITE,{uv1.x, uv2.y},{0,1,0}},
    },{
        0, 1, 2, 2, 3, 0
    });
    return mesh;
}


Mesh* MeshUtils::SubdividedPlane(glm::ivec2 resolution){
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    glm::ivec2 it; glm::vec3 pos = {0,0,0}; glm::vec2 uv;
    for(it.x = 0; it.x <= resolution.x; it.x++){
        uv.s = ((float)it.x) / resolution.x;
        pos.x = uv.s - 0.5f;
        for(it.y = 0; it.y <= resolution.y; it.y++){
            uv.t = ((float)it.y) / resolution.y;
            pos.z = uv.t - 0.5f; 
            vertices.push_back({pos, WHITE, uv, {0,1,0}});
        }   
    }

    GLuint index = resolution.y + 1;
    for(it.x = 0; it.x < resolution.x; it.x++){
        index++;
        for(it.y = 0; it.y < resolution.y; it.y++){
            indices.push_back(index);
            indices.push_back(index - 1);
            indices.push_back(index - 1 - (resolution.x+1));
            indices.push_back(index - 1 - (resolution.x+1));
            indices.push_back(index - (resolution.x+1));
            indices.push_back(index);
            index++;
        }
    }

    Mesh* mesh = new Mesh();
    mesh->setup(vertices, indices);
    return mesh;
}

Mesh* MeshUtils::LoadObj(const char* filename){
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::unordered_map<Vertex, GLuint> vertex_map;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename)) {
        throw std::runtime_error(err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex = {};

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            if(index.texcoord_index >= 0){
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }
            
            vertex.color = {
                attrib.colors[3 * index.vertex_index + 0],
                attrib.colors[3 * index.vertex_index + 0],
                attrib.colors[3 * index.vertex_index + 0],
                1.0f
            };

            auto it = vertex_map.find(vertex);
            if(it == vertex_map.end()){
                GLuint new_vertex_index = (GLuint)vertices.size();
                vertex_map[vertex] = new_vertex_index;
                indices.push_back(new_vertex_index);
                vertices.push_back(vertex);
            } else {
                indices.push_back(it->second);
            }
        }
    }

    Mesh* mesh = new Mesh();
    mesh->setup(vertices, indices);
    return mesh;
}

    // Set up mesh and attribute properties
/*
 Mesh* MeshUtils::ParticleMesh()
 {
    Mesh* mesh = new Mesh();
    std::vector<Vertex> vertices =
    { {0.0f, 1.0f, 0.0f, 1.0f},
        {1, 0, 1, 0},
        {0, 0, 0, 0},

        {0, 1, 0, 1},
        {1, 1, 1, 1},
        {1, 0, 1, 0}
    };

    std::vector<GLuint> indices = {
        0,1,2,2,3,0
    };
    mesh->setup(vertices,indices);
    return mesh;
 }*/