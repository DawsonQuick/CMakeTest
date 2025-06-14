#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

class GUI {
private:
	GLFWwindow* m_GLFWWindow;
public:
	//Constructor
	GUI(GLFWwindow* glfwWindow);

	//Deconstructor
	~GUI();

	//Step Render
	void render();
};