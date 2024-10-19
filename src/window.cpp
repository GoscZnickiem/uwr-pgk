#include "window.hpp"
#include "shader.hpp"

#include <iostream>

Window::Window() {
	const int width = 800;
	const int height = 600;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	m_ID = glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr);
	if(m_ID == nullptr) {
		std::cerr << "Window creation failed\n";
		exit(2);
	}

	glfwMakeContextCurrent(m_ID);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failed\n";
		exit(3);
	}
	glViewport(0, 0, width, height);
	glfwSetInputMode(m_ID, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_ID, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	glfwSetFramebufferSizeCallback(m_ID, []([[maybe_unused]] GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		float winx = static_cast<float>(w);
		float winy = static_cast<float>(h);
		if(winx > winy)
			Shader::setGlobalUniform("scale", winy/winx, 1.f);
		else 
			Shader::setGlobalUniform("scale", 1.f, winx/winy);
	});
}

void Window::endFrame() {
	glfwSwapBuffers(m_ID);
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::close() {
	glfwSetWindowShouldClose(m_ID, GL_TRUE);
}

int Window::getKey(int key) const {
	return glfwGetKey(m_ID, key);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_ID) != 0;
}

std::pair<float, float> Window::getWindowSize() {
	int w = 0; int h = 0;
	glfwGetWindowSize(m_ID, &w, &h);
	return {static_cast<float>(w), static_cast<float>(h)};
}

