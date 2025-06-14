#include "GLFW/GLFWWindow.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	int iconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);

}

GLFWWindow::GLFWWindow(int width, int height):
	m_WindowWidth(width), m_WindowHeight(height)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "OpenGL Engine Window", nullptr, nullptr);
	if (!m_Window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
}

GLFWWindow::~GLFWWindow() {

}

GLFWwindow* GLFWWindow::getWindowPointer() {
	return m_Window;
}


void GLFWWindow::newFrame() {

	// Set the clear color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void GLFWWindow::render() {
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
};
void GLFWWindow::shutdown() {
	glfwTerminate();
};

bool GLFWWindow::shouldDisplayClose() {
	return glfwWindowShouldClose(m_Window);
}