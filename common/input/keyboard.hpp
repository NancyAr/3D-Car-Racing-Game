#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <GLFW/glfw3.h>
#include <cstring>

class Keyboard {
private:
    bool CurrentKeyState[GLFW_KEY_LAST+1];
    bool PreviousKeyState[GLFW_KEY_LAST+1];

public:
    Keyboard(GLFWwindow* window){
        for(int key = 0; key <= GLFW_KEY_LAST; key++){
            CurrentKeyState[key] = PreviousKeyState[key] = glfwGetKey(window, key);
        }
    }
    void update(){
        std::memcpy(PreviousKeyState, CurrentKeyState, sizeof(PreviousKeyState));
    }
    void keyEvent(int key, int, int action, int){
        if(action == GLFW_PRESS){
            CurrentKeyState[key] = true;
        } else if(action == GLFW_RELEASE){
            CurrentKeyState[key] = false;
        }
    }

    bool isPressed(int key){return CurrentKeyState[key];}
    bool justPressed(int key){return CurrentKeyState[key] && !PreviousKeyState[key];}
    bool justReleased(int key){return !CurrentKeyState[key] && PreviousKeyState[key];}
};

#endif