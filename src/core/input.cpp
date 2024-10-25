#include "input.hpp"

#include <map>

std::map<std::string, GLint> keys = {
	{"UP", GLFW_KEY_UP},
	{"DOWN", GLFW_KEY_DOWN},
	{"LEFT", GLFW_KEY_LEFT},
	{"RIGHT", GLFW_KEY_RIGHT},
	{"SPACE", GLFW_KEY_SPACE},
	{"ESCAPE", GLFW_KEY_ESCAPE}
};

static GLFWwindow* window;

bool Input::isKeyPressed(const std::string& key) {
	return glfwGetKey(window, keys.at(key)) == GLFW_PRESS;
}

void Input::setWindow(GLFWwindow* win) {
	window = win;
}
