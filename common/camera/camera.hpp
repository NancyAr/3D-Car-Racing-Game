#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum struct CameraType {
    Orthographic,
    Perspective
};

class Camera {
private:
    const uint8_t V_DIRTY = 1, P_DIRTY = 2, VP_DIRTY = 4;
    glm::vec3 position, direction, up;
    CameraType type;
    float fovyAngle, orthoHeight, aspectRatio, zNear, zFar;
    uint8_t dirtyFlags;

    glm::mat4 V, P, VP;

public:
    Camera(){
        dirtyFlags = V_DIRTY | P_DIRTY | VP_DIRTY;
        up = {0, 1, 0};
    }

    
    void setupPerspective(float _fovyAngle, float _aspectRatio, float _zNear, float _zFar){
        this->type = CameraType::Perspective;
        this->fovyAngle = _fovyAngle;
        this->aspectRatio = _aspectRatio;
        this->zNear = _zNear;
        this->zFar = _zFar;
        dirtyFlags |= P_DIRTY | VP_DIRTY;
    }

    void setupOrthographic(float _orthoHeight, float _aspectRatio, float _zNear, float _zFar){
        this->type = CameraType::Orthographic;
        this->orthoHeight = _orthoHeight;
        this->aspectRatio = _aspectRatio;
        this->zNear = _zNear;
        this->zFar = _zFar;
        dirtyFlags |= P_DIRTY | VP_DIRTY;
    }

    void setType(CameraType _type){
        if(this->type != _type){
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->type = _type;
        }
    }
    void setOrthographicSize(float _orthoHeight){
        if(this->orthoHeight != _orthoHeight){
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->orthoHeight = _orthoHeight;
        }
    }
    void setFieldOfView(float _fovyAngle){
        if(this->fovyAngle != _fovyAngle){    
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->fovyAngle = _fovyAngle;
        }
    }
    void setAspectRatio(float _aspectRatio){
        if(this->aspectRatio != _aspectRatio){
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->aspectRatio = _aspectRatio;
        }
    }
    void setNearPlane(float _zNear){
        if(this->zNear != _zNear){
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->zNear = _zNear;
        }
    }
    void setFarPlane(float _zFar){
        if(this->zFar != _zFar){
            dirtyFlags |= P_DIRTY | VP_DIRTY;
            this->zFar = _zFar;
        }
    }
    void setPosition(glm::vec3 _position){
        if(this->position != _position){
            dirtyFlags |= V_DIRTY | VP_DIRTY;
            this->position = _position;
        }
    }
    void setDirection(glm::vec3 _direction){
        if(this->direction != _direction){
            dirtyFlags |= V_DIRTY | VP_DIRTY;
            this->direction = _direction;
        }
    }
    void setTarget(glm::vec3 target){
        glm::vec3 _direction = target - position;
        if(this->direction != _direction){
            dirtyFlags |= V_DIRTY | VP_DIRTY;
            this->direction = _direction;
        }
    }
    void setUp(glm::vec3 _up){
        if(this->up != _up){
            dirtyFlags |= V_DIRTY | VP_DIRTY;
            this->up = _up;
        }
    }

    glm::mat4 getProjectionMatrix(){
        if(dirtyFlags & P_DIRTY){
            if(type == CameraType::Orthographic){
                float orthoWidth = aspectRatio * orthoHeight;
                P = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, zNear, zFar);
            } else {
                P = glm::perspective(fovyAngle, aspectRatio, zNear, zFar);
            }
            dirtyFlags &= ~P_DIRTY;
        }
        return P;
    }

    glm::mat4 getViewMatrix(){
        if(dirtyFlags & V_DIRTY){
            V = glm::lookAt(position, position + direction, up);
            dirtyFlags &= ~V_DIRTY;
        }
        return V;
    }

    glm::mat4 getVPMatrix(){
        if(dirtyFlags & VP_DIRTY){
            VP = getProjectionMatrix() * getViewMatrix();
            dirtyFlags = 0;
        }
        return VP;
    }

    CameraType getType(){return type;}
    float getFieldOfView(){return fovyAngle;}
    float getOrthographicSize(){return orthoHeight;}
    float getAspectRatio(){return aspectRatio;}
    float getNearPlane(){return zNear;}
    float getFarPlane(){return zFar;}
    glm::vec3 getPosition(){return position;}
    glm::vec3 getDirection(){return direction;}
    glm::vec3 getOriginalUp(){return up;}

    glm::vec3 Right(){
        getViewMatrix();
        return {V[0][0],V[1][0],V[2][0]};
    }
    glm::vec3 Left(){
        getViewMatrix();
        return {-V[0][0],-V[1][0],-V[2][0]};
    }
    glm::vec3 Up(){
        getViewMatrix();
        return {V[0][1],V[1][1],V[2][1]};
    }
    glm::vec3 Down(){
        getViewMatrix();
        return {-V[0][1],-V[1][1],-V[2][1]};
    }
    glm::vec3 Front(){
        getViewMatrix();
        return {-V[0][2],-V[1][2],-V[2][2]};
    }
    glm::vec3 Back(){
        getViewMatrix();
        return {V[0][2],V[1][2],V[2][2]};
    }

    glm::vec3 fromWorldToDeviceSpace(glm::vec3 world){
        glm::vec4 clip = getVPMatrix() * glm::vec4(world, 1.0f);
        return glm::vec3(clip)/clip.w;
    }

    glm::vec3 fromDeviceToWorldSpace(glm::vec3 device){
        glm::vec4 clip = glm::inverse(getVPMatrix()) * glm::vec4(device, 1.0f);
        return glm::vec3(clip)/clip.w;
    }
};

#endif