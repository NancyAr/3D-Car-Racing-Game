#ifndef MOUSE_HPP
#define MOUSE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <cstring>

class Mouse {
private:
    GLFWwindow* window;
    glm::vec2 currentMousePosition, previousMousePosition;
    bool currentMouseButtons[GLFW_MOUSE_BUTTON_LAST+1], previousMouseButtons[GLFW_MOUSE_BUTTON_LAST+1];
    double scrollDelta;

public:
    Mouse(GLFWwindow* window){
        this->window = window;
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        previousMousePosition = currentMousePosition = glm::vec2((float)x,(float)y);
        for(int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; button++){
            currentMouseButtons[button] = previousMouseButtons[button] = glfwGetMouseButton(window, button);
        }
        scrollDelta = 0;
    }

    void update(){
        previousMousePosition = currentMousePosition;
        std::memcpy(previousMouseButtons, currentMouseButtons, sizeof(previousMouseButtons));
        scrollDelta = 0;
    }

    glm::vec2 getMousePosition(){ return currentMousePosition; }
    glm::vec2 getMouseDelta(){ return currentMousePosition - previousMousePosition; }

    bool isPressed(int button){ return currentMouseButtons[button]; }
    bool justPressed(int button){ return currentMouseButtons[button] && !previousMouseButtons[button]; }
    bool justReleased(int button){ return !currentMouseButtons[button] && previousMouseButtons[button]; }

    double getScrollDelta(){return scrollDelta;}

    void cursorMoveEvent(double xpos, double ypos){ currentMousePosition.x = (float)xpos; currentMousePosition.y = (float)ypos; }
    void MouseButtonEvent(int button, int action, int){
        if(action == GLFW_PRESS) currentMouseButtons[button] = true;
        else if(action == GLFW_RELEASE) currentMouseButtons[button] = false;
    }
    void ScrollEvent(double, double yoffset){ scrollDelta += yoffset; }

    void lockMouse() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
    void unlockMouse() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

};

#endif