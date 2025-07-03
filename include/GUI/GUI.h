#pragma once
#include <Common/GLInclude.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


#include <Common/Structs.h>

class GUI {
private:
	GLFWwindow* m_GLFWWindow;
public:
	//Constructor
	GUI(GLFWwindow* glfwWindow);

	//Deconstructor
	~GUI();

	//Step Render
	void render(ParticleToGUITransfer particleEngineGui);
};