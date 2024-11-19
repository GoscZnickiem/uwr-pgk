#ifndef _GZN_PGK_CORE_INPUT_
#define _GZN_PGK_CORE_INPUT_

#include <string>
#include <GLFW/glfw3.h>

namespace Input {

void update();
void processKeyCallback(GLint keyCode, GLint action);
void processScrollCallback(double offset);

[[nodiscard]] bool isKeyPressed(const std::string& key);
[[nodiscard]] bool isKeyClicked(const std::string& key);

std::pair<float, float> getMousePos();
[[nodiscard]] float getScroll();

void setMouseVisibility(bool visible);
[[nodiscard]] bool isMouseLocked();
void setMousePosLock(bool lock);

void setWindow(GLFWwindow* win);

};

#endif
