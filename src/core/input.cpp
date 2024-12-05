#include "input.hpp"

#include <iostream>
#include <map>
#include <stdexcept>

static std::map<std::string, char> keys = {
	{"UP", 0},
	{"DOWN", 0},
	{"LEFT", 0},
	{"RIGHT", 0},
	{"SPACE", 0},
	{"ESCAPE", 0},
	{"SHIFT", 0},
	{"TAB", 0},
	{"ENTER", 0},
	{"W", 0},
	{"S", 0},
	{"A", 0},
	{"D", 0},
	{"+", 0},
	{"-", 0}
};

static const std::map<GLint, std::string> glToString = {
	{GLFW_KEY_UP, "UP"},
	{GLFW_KEY_DOWN, "DOWN"},
	{GLFW_KEY_LEFT, "LEFT"},
	{GLFW_KEY_RIGHT, "RIGHT"},
	{GLFW_KEY_SPACE, "SPACE"},
	{GLFW_KEY_ESCAPE, "ESCAPE"},
	{GLFW_KEY_LEFT_SHIFT, "SHIFT"},
	{GLFW_KEY_TAB, "TAB"},
	{GLFW_KEY_ENTER, "ENTER"},
	{GLFW_KEY_W, "W"},
	{GLFW_KEY_S, "S"},
	{GLFW_KEY_A, "A"},
	{GLFW_KEY_D, "D"},
	{GLFW_KEY_EQUAL, "+"},
	{GLFW_KEY_MINUS, "-"}
};

static GLFWwindow* window;
static bool mouseLocked = false;
static bool mouseVisible = true;
static std::pair<float, float> prevMousePos;
static float scrollOffset = 0;

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

void update() {
	for(auto& k : keys) {
		const char s = k.second;
		k.second = ((s >> 1) & 1) | (s & 2);
	}
	scrollOffset = 0.f;
}

void processKeyCallback(GLint keyCode, GLint action) {
	try {
		const auto c = keys[glToString.at(keyCode)];
		const auto bit = (action == GLFW_PRESS || action == GLFW_REPEAT) * 2;
		keys[glToString.at(keyCode)] = static_cast<char>((c & ~2) | bit);
	} catch ([[maybe_unused]] std::out_of_range& e) {
		return;
	}
}

void processScrollCallback(double offset) {
	scrollOffset += static_cast<float>(offset);
}

bool isKeyPressed(const std::string& key) {
	return (keys[key] & 2) != 0;
}

bool isKeyClicked(const std::string& key) {
	return isKeyPressed(key) && !(keys[key] & 1);
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

[[nodiscard]] float getScroll() {
	return scrollOffset;
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
