#include "scene.hpp"
#include <application.hpp>

void Scene::Initialize(){}
void Scene::Update(double){}
void Scene::Draw(){}
void Scene::Finalize(){}

void Scene::KeyEvent(int, int, int, int){}
void Scene::CursorMoveEvent(double, double){}
void Scene::CursorEnterEvent(int){}
void Scene::MouseButtonEvent(int, int, int){}
void Scene::ScrollEvent(double, double){}

Keyboard* Scene::getKeyboard(){ return application->getKeyBoard(); }
Mouse* Scene::getMouse(){ return application->getMouse(); }