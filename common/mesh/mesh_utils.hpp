#ifndef MESH_UTILS_HPP
#define MESH_UTILS_HPP

#include "mesh.hpp"


class MeshUtils {
public:
    static Mesh* Box();
    static Mesh* ParticleMesh();
    static Mesh* Sphere(int long_segments=24, int lat_segments=16);
    static Mesh* Plane(glm::vec2 uv1 = {0,0}, glm::vec2 uv2 = {1,1});
    static Mesh* SubdividedPlane(glm::ivec2 resolution = {512, 512});
    static Mesh* LoadObj(const char* filename);
};

#endif