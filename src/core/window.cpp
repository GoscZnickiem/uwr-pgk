#include "appdata.hpp"
#include "window.hpp"
#include "input.hpp"
#include "shader.hpp"

#include <iostream>

void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam) {
    std::cout << "OpenGL Debug Message:\n";
    std::cout << "Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
    }
    std::cout << "\nType: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability Issue"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance Issue"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Marker"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
    }
    std::cout << "\nID: " << id;
    std::cout << "\nSeverity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Notification"; break;
    }
    std::cout << "\nMessage: " << message << "\n" << std::endl;
}

Window::Window(int width, int height) {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	m_ID = glfwCreateWindow(width, height, "PGK", nullptr, nullptr);
	if(m_ID == nullptr) {
		std::cerr << "Window creation failed\n";
		exit(2);
	}

	glfwMakeContextCurrent(m_ID);
	glfwSwapInterval(1);
	glCullFace(GL_BACK);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failed\n";
		exit(3);
	}
	glViewport(0, 0, width, height);
	glfwSetInputMode(m_ID, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_ID, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLDebugCallback, nullptr);

	glfwSetFramebufferSizeCallback(m_ID, []([[maybe_unused]] GLFWwindow* window, int w, int h) {
		AppData::Data().atResize(w, h);
	});

	glfwSetKeyCallback(m_ID, []([[maybe_unused]] GLFWwindow* window, int key,[[maybe_unused]] int scancode, int action,[[maybe_unused]] int mods) {
		Input::processKeyCallback(key, action);
	});

	glfwSetScrollCallback(m_ID, []([[maybe_unused]] GLFWwindow* window,[[maybe_unused]] double xOffset, double yOffset) {
		Input::processScrollCallback(yOffset);
	});

	Input::setWindow(m_ID);
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

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_ID) != 0;
}

std::pair<int, int> Window::getWindowSize() {
	int w = 0; int h = 0;
	glfwGetWindowSize(m_ID, &w, &h);
	return {w, h};
}
