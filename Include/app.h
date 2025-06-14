#pragma once
#include <iostream>

#include "Renderer/Renderer.h"
#include "GLFW/GLFWWindow.h"
#include "GUI/GUI.h"

class App {
    private:
        GLFWWindow m_GLFWWindow;
        GUI m_Gui;
        Renderer m_Renderer;
    public:
    //Constuctor
    App();
    ~App();
};