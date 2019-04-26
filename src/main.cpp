#include <application.hpp>
#include "1_directional_light/directional_light_scene.hpp"


//NOTE: OpenGL is pure C so any class you see is not part of OpenGL
// All OpenGL functions start with "gl" and All constants and datatypes start with "GL"
// All GLFW functions start with "glfw" and All constants and datatypes start with "GLFW" 

int main()
{
    //Start an windowed OpenGL application with size 640x480 and title "3D" 
    Application* app = new Application("3D", 1360, 786, false);
    
    //Create a scene and set it as the main scene in the application
    Scene* scene = new DirectionalLightScene(app);
    app->setScene(scene);
    
    
    //Start the application loop
    app->run();

    //Remember to deallocate everything before closing
    delete scene;
    delete app;

    return 0;
}
