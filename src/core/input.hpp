#ifndef _GZN_PGK_INPUT_
#define _GZN_PGK_INPUT_

#include <string>
#include <GLFW/glfw3.h>

namespace Input {

[[nodiscard]] bool isKeyPressed(const std::string& key);
std::pair<float, float> getMousePos();
void setMouseVisibility(bool visible);
[[nodiscard]] bool isMouseLocked();
void setMousePosLock(bool lock);

void setWindow(GLFWwindow* win);

};

#endif
