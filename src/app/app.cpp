#include "app.h"

App::App():
m_GLFWWindow(2560,1440),
m_Gui(m_GLFWWindow.getWindowPointer()),
m_Renderer()
{
    std::cout<<"Starting App"<<std::endl;

    while(!m_GLFWWindow.shouldDisplayClose()){
        m_GLFWWindow.newFrame();
        m_Gui.render();
        m_Renderer.render();
        m_GLFWWindow.render();
    }

    m_GLFWWindow.shutdown();
}

App::~App(){

}