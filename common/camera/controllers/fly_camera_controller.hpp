#ifndef FLY_CAMERA_CONTROLLER_HPP
#define FLY_CAMERA_CONTROLLER_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <camera/camera.hpp>
#include <input/mouse.hpp>
#include <input/keyboard.hpp>
#include <scene.hpp>

class FlyCameraController {
private:
    Mouse* mouse;
    Keyboard* keyboard;
    Camera* camera;

    float yaw, pitch;
    glm::vec3 position;

    float yawSenstivity, pitchSenstivity;
    glm::vec3 positionSenstivity;

public:
    FlyCameraController(Scene* scene, Camera* camera){
        this->mouse = scene->getMouse();
        this->keyboard = scene->getKeyboard();
        this->camera = camera;
        if(this->mouse->isPressed(GLFW_MOUSE_BUTTON_1)) this->mouse->lockMouse();
        yawSenstivity = pitchSenstivity = 0.01f;
        positionSenstivity = {3.0f, 3.0f, 3.0f};
    }

    ~FlyCameraController(){
        this->mouse->unlockMouse();
    }

    void update(double delta_time){
        if(mouse->justPressed(GLFW_MOUSE_BUTTON_1)){
            mouse->lockMouse();
        } else if(mouse->justReleased(GLFW_MOUSE_BUTTON_1)){
            mouse->unlockMouse();
        }

        if(mouse->isPressed(GLFW_MOUSE_BUTTON_1)){
            glm::vec2 delta = mouse->getMouseDelta();
            pitch -= delta.y * pitchSenstivity;
            yaw += delta.x * yawSenstivity;
        }
        
        if(pitch < -glm::half_pi<float>() * 0.99f) pitch = -glm::half_pi<float>() * 0.99f;
        if(pitch >  glm::half_pi<float>() * 0.99f) pitch  = glm::half_pi<float>() * 0.99f;
        yaw = glm::wrapAngle(yaw);
        
        glm::vec3 front = camera->Front(), up = camera->Up(), right = camera->Right();

        if(keyboard->isPressed(GLFW_KEY_W)) position += front * ((float)delta_time * positionSenstivity.z);
        if(keyboard->isPressed(GLFW_KEY_S)) position -= front * ((float)delta_time * positionSenstivity.z);
        if(keyboard->isPressed(GLFW_KEY_Q)) position += up * ((float)delta_time * positionSenstivity.y);
        if(keyboard->isPressed(GLFW_KEY_E)) position -= up * ((float)delta_time * positionSenstivity.y);
        if(keyboard->isPressed(GLFW_KEY_D)) position += right * ((float)delta_time * positionSenstivity.x);
        if(keyboard->isPressed(GLFW_KEY_A)) position -= right * ((float)delta_time * positionSenstivity.x);

        camera->setDirection(glm::vec3(glm::cos(yaw), 0, glm::sin(yaw)) * glm::cos(pitch) + glm::vec3(0, glm::sin(pitch), 0));
        camera->setPosition(position);
    }

    float getYaw(){return yaw;}
    float getPitch(){return pitch;}
    glm::vec3 getPosition(){return position;}

    float getYawSenstivity(){return yawSenstivity;}
    float getPitchSenstivity(){return pitchSenstivity;}
    glm::vec3 getPositionSenstivity(){return positionSenstivity;}

    void setYaw(float _yaw){
        this->yaw = glm::wrapAngle(_yaw);  
    }
    void setPitch(float _pitch){
        const float v = 0.99f*glm::pi<float>()/2;
        if(_pitch > v) _pitch = v;
        else if(_pitch < -v) _pitch = -v;
        this->pitch = _pitch;  
    }
    void setPosition(glm::vec3 _pos){
        this->position = _pos;
    }

    void setYawSenstivity(float _senstivity){this->yawSenstivity = _senstivity;}
    void setPitchSenstivity(float _senstivity){this->pitchSenstivity = _senstivity;}
    void setPositionSenstivity(glm::vec3 _senstivity){this->positionSenstivity = _senstivity;}

};

#endif