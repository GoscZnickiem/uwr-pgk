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
static bool mouseLocked = false;
static bool mouseVisible = true;
std::pair<float, float> prevMousePos;

void updateMouseState() {
	if(mouseLocked) {
		prevMousePos = Input::getMousePos();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if(glfwRawMouseMotionSupported() == GLFW_TRUE) {
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	} else {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		glfwSetInputMode(window, GLFW_CURSOR, mouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}
}

namespace Input {

bool isKeyPressed(const std::string& key) {
	return glfwGetKey(window, keys.at(key)) == GLFW_PRESS;
}

std::pair<float, float> getMousePos() {
	double xPos = 0;
	double yPos = 0;
	glfwGetCursorPos(window, &xPos, &yPos);
	std::pair<float, float> pos = {static_cast<float>(xPos), static_cast<float>(yPos)};
	if(mouseLocked) {
		std::pair<float, float> res = {pos.first - prevMousePos.first, pos.second - prevMousePos.second};
		prevMousePos = pos;
		return res;
	}
	return pos;
}

void setMouseVisibility(bool visible) {
	if(visible != mouseVisible) {
		mouseVisible = visible;
		updateMouseState();
	}
}

[[nodiscard]] bool isMouseLocked() {
	return mouseLocked;
}

void setMousePosLock(bool lock) {
	if(lock != mouseLocked) {
		mouseLocked = lock;
		updateMouseState();
	}
}

void setWindow(GLFWwindow* win) {
	window = win;
}

}
