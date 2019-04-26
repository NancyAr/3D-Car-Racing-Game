#include "application.hpp"

#include <scene.hpp>
#include <iostream>


Application::Application(const char* title, int width, int height, bool isFullscreen){

    //Try to initialize GLFW
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Set the opengl version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Use only opengl core feature (Ignore compatibility with older versions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Make window size fixed (User can't resize it)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    //Set Number of sample used in MSAA (0 = Disabled)
    glfwWindowHint(GLFW_SAMPLES, 16);

    //Enable Double Buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    //Set the bit-depths of the frame buffer
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    //Set Bits for Depth Buffer
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    //Set Bits for Stencil Buffer
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    //Set the refresh rate of the window (GLFW_DONT_CARE = Run as fast as possible)
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    //If the application is fullscreen, set the monitor to primary monitor
    //otherwise set the monitor to null (windowed)
    GLFWmonitor *monitor = isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, title, monitor, nullptr);
    
    //Check if window was created
    if(!window){
        std::cerr << "Failed to create Window" << std::endl;
        glfwTerminate(); //Remember to terminate GLFW
        exit(EXIT_FAILURE);
    }

    //Set the window to be the current OpenGL context 
    glfwMakeContextCurrent(window);
    //Load OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window); //Remember to destroy window
        glfwTerminate(); //Remember to terminate GLFW
        exit(EXIT_FAILURE);
    }

    keyboard = new Keyboard(window);
    mouse = new Mouse(window);

    createEventHandlers();
}

Application::~Application(){
    //Destroy window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::setScene(Scene* scene){
    this->currentScene = scene;
}

Scene* Application::getScene(){
    return currentScene;
}
    

void Application::run(){
    
    //Main application loop:
    //- Initialize Scene
    //- While window still open, do:
    //      - Update Scene
    //      - Draw Scene
    //      - Swap window buffers and poll input events (keyboard, mouse, etc)
    //- Finalize Scene

    currentScene->Initialize();
    
    double lastFrame = glfwGetTime();

    while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)){
        
        double currentFrame = glfwGetTime();

        currentScene->Update(currentFrame - lastFrame); //Send time difference between last and current frame
        
        currentScene->Draw();

        glfwSwapBuffers(window);

        keyboard->update();
        mouse->update();
        glfwPollEvents();

        lastFrame = currentFrame;
    }

    currentScene->Finalize();

}

void Application::createEventHandlers(){
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getKeyBoard()->keyEvent(key, scancode, action, mods);
            Scene* scene = app->getScene();
            if(scene) scene->KeyEvent(key, scancode, action, mods);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse()->cursorMoveEvent(xpos, ypos);
            Scene* scene = app->getScene();
            if(scene) scene->CursorMoveEvent(xpos, ypos);
        }
    });

    glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            Scene* scene = app->getScene();
            if(scene) scene->CursorEnterEvent(entered);
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse()->MouseButtonEvent(button, action, mods);
            Scene* scene = app->getScene();
            if(scene) scene->MouseButtonEvent(button, action, mods);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if(app){
            app->getMouse()->ScrollEvent(xoffset, yoffset);
            Scene* scene = app->getScene();
            if(scene) scene->ScrollEvent(xoffset, yoffset);
        }
    });
}