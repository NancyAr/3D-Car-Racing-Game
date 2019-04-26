#ifndef DIRECTIONAL_LIGHT_SCENE_HPP
#define DIRECTIONAL_LIGHT_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <glm/gtx/fast_trigonometry.hpp>
#include <scene.hpp>

#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>

enum TextureType {
    ALBEDO = 0,
    SPECULAR = 1,
    ROUGHNESS = 2,
    AMBIENT_OCCLUSION = 3,
    EMISSIVE = 4
};

class DirectionalLightScene : public Scene {
private:
    Shader* shader;
    Shader* skyShader;
    Shader* particleshader;
    Mesh* ground;
    Mesh* plane;
    Mesh* sky;
    Mesh* box;
    Mesh* box2;
    Mesh* box3;
    
    Mesh* part;
    Mesh* car;
    Texture2D* parttext[1];
    Texture2D* carTex[2];
    Texture2D* boxtex[5];
    Texture2D* boxtex2[5];
    Texture2D* metal[5];
    Texture2D* text[2];
    Texture2D* wood[5];
    Texture2D* asphalt[5];
    Texture2D* checkers[1];
    Texture2D* planetex[3];

    Texture2D* Text2DTextureID[2];
    Texture2D* Text2DVertexBufferID;         // Buffer containing the vertices
    Texture2D* Text2DUVBufferID;             //                       UVs
    Texture2D* Text2DShaderID;               // Program used to disaply the text
  int vertexPosition_screenspaceID; // Location of the program's "vertexPosition_screenspace" attribute
  int vertexUVID;                   // Location of the program's "vertexUV" attribute
  int Text2DUniformID;    


    Camera* camera;
    FlyCameraController* controller;
    GLuint mLoc, vpLoc, tintLoc;
    float carRotation;
 
    glm::vec3 Box1Position;
    glm::vec3 Box2Position;
    glm::vec3 Box3Position;
    glm::vec3 LinePosition;
    float sunYaw, sunPitch;
public:
   
    DirectionalLightScene(Application* app): Scene(app) {}
    //GLuint firstUnusedParticle();
    glm::vec3 CarPosition;
    void ParticleGeneratorDraw();
    void ParticleGeneratorUpdate(double dt,  GLuint newParticles, glm::vec3 offset);

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif