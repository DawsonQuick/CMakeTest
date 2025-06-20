#include "app.h"

App::App():
m_GLFWWindow(2560,1440),
m_Gui(m_GLFWWindow.getWindowPointer()),
m_Renderer(),
m_IO(IOType::GLFW)
{
    std::cout<<"Starting App"<<std::endl;

    m_Camera = createCameraObject(CameraType::FirstPerson, m_IO.getInterface());
    m_CameraControls = createCameraControls(CameraType::FirstPerson, m_Camera.get());
    m_CameraControls->bindKeys();

    m_IO.setCameraUpdateCallBack([this](int windWidth, int windHeight, double cursorX, double cursorY){ m_Camera->update(windWidth, windHeight, cursorX, cursorY);});
    while(!m_GLFWWindow.shouldDisplayClose()){

        m_Camera->validateCameraInfo();

        

        m_GLFWWindow.newFrame();
        m_Gui.render();
        m_IO.getInterface()->processDisplayUpdate(m_GLFWWindow.getWindowPointer(), 0, false);
        m_Renderer.render(m_Camera->getActiveCameraInfo().view, m_Camera->getActiveCameraInfo().projection);
        m_GLFWWindow.render();
    }

    m_GLFWWindow.shutdown();
}

App::~App(){

}