#include "window.hpp"
#include <iostream>

Window::Window() {
	const int width = 800;
	const int height = 600;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	ID = glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr);
	if(ID == nullptr) {
		std::cerr << "Window creation failed\n";
		exit(2);
	}

	glfwMakeContextCurrent(ID);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failed\n";
		exit(3);
	}
	glViewport(0, 0, width, height);
	glfwSetInputMode(ID, GLFW_STICKY_KEYS, GL_TRUE);
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(ID) != 0;
}

void Window::swapBuffers() {
	glfwSwapBuffers(ID);
}

int Window::getKey(int key) const {
	return glfwGetKey(ID, key);
}

void Window::close() {
	glfwSetWindowShouldClose(ID, GL_TRUE);
}
