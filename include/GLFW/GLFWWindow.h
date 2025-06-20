#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
class GLFWWindow {
private:
	GLFWwindow* m_Window = nullptr;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
public:
	//Constructor
	GLFWWindow(int width, int height);

	//De-Constructor
	~GLFWWindow();

	//Access GLFW window pointer member variable
	GLFWwindow* getWindowPointer();

	void pollEvent();
	void newFrame();
	void render();
	void shutdown();
	bool shouldDisplayClose();

};