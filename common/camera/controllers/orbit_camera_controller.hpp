#ifndef ORBIT_CAMERA_CONTROLLER_HPP
#define ORBIT_CAMERA_CONTROLLER_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <camera/camera.hpp>
#include <Input/mouse.hpp>
#include <scene.hpp>

class OrbitCameraController {
private:
    Mouse* mouse;
    Camera* camera;

    float yaw, pitch, distance;
	glm::vec3 origin;
    float yawSenstivity, pitchSenstivity, distanceSenstivity;

public:
    OrbitCameraController(Scene* scene, Camera* camera){
        this->mouse = scene->getMouse();
        this->camera = camera;
        if(this->mouse->isPressed(GLFW_MOUSE_BUTTON_1)) this->mouse->lockMouse();
		origin = {0,0,0};
        yawSenstivity = pitchSenstivity = 0.01f;
        distanceSenstivity = 2.0f;
    }

    ~OrbitCameraController(){
        this->mouse->unlockMouse();
    }

    void update(double){
        if(mouse->justPressed(GLFW_MOUSE_BUTTON_1)){
            mouse->lockMouse();
        } else if(mouse->justReleased(GLFW_MOUSE_BUTTON_1)){
            mouse->unlockMouse();
        }

        if(mouse->isPressed(GLFW_MOUSE_BUTTON_1)){
            glm::vec2 delta = mouse->getMouseDelta();
            pitch += delta.y * pitchSenstivity;
            yaw += delta.x * yawSenstivity;
        }
        
        if(pitch < -glm::half_pi<float>() * 0.99f) pitch = -glm::half_pi<float>() * 0.99f;
        if(pitch >  glm::half_pi<float>() * 0.99f) pitch  = glm::half_pi<float>() * 0.99f;
        yaw = glm::wrapAngle(yaw);
        
        distance += (float)mouse->getScrollDelta() * distanceSenstivity;
        if(distance < 0) distance = 0;

        camera->setPosition(origin + distance * (glm::vec3(glm::cos(yaw), 0, glm::sin(yaw)) * glm::cos(pitch) + glm::vec3(0, glm::sin(pitch), 0)));
        camera->setTarget(origin);
    }

    float getYaw(){return yaw;}
    float getPitch(){return pitch;}
    float getDistance(){return distance;}
    glm::vec3 getOrigin(){return origin;}

    float getYawSenstivity(){return yawSenstivity;}
    float getPitchSenstivity(){return pitchSenstivity;}
    float getDistanceSenstivity(){return distanceSenstivity;}

    void setYaw(float _yaw){
        this->yaw = glm::wrapAngle(_yaw);  
    }
    void setPitch(float _pitch){
        const float v = 0.99f*glm::pi<float>()/2;
        if(_pitch > v) _pitch = v;
        else if(_pitch < -v) _pitch = -v;
        this->pitch = _pitch;  
    }
    void setDistance(float _distance){
        this->distance = glm::max(0.0f, _distance);
    }
	void setOrigin(glm::vec3 _origin){
        this->origin = _origin;
    }

    void setYawSenstivity(float _senstivity){this->yawSenstivity = _senstivity;}
    void setPitchSenstivity(float _senstivity){this->pitchSenstivity = _senstivity;}
    void setDistanceSenstivity(float _senstivity){this->distanceSenstivity = _senstivity;}

};

#endif