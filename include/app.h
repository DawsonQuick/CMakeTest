#pragma once
#include <iostream>
#include <memory>
#include "Renderer/Renderer.h"
#include "GLFW/GLFWWindow.h"
#include "IO/IO.h"
#include "Camera/CameraFactory.h"
#include "GUI/GUI.h"
#include "ParticleEngine/ParticleEngine.h"

class App {
    private:
        GLFWWindow m_GLFWWindow;
        GUI m_Gui;
        Renderer m_Renderer;
        IO m_IO;
        ParticleEngine m_ParticleEngine;

        std::unique_ptr<ICamera> m_Camera;
        std::unique_ptr<ICameraControls> m_CameraControls;
    public:
    //Constuctor
    App();
    ~App();
};