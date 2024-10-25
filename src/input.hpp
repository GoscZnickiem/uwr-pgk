#ifndef _GZN_PGK_INPUT_
#define _GZN_PGK_INPUT_

#include <string>
#include <GLFW/glfw3.h>

namespace Input {

bool isKeyPressed(const std::string& key);

void setWindow(GLFWwindow* win);

};

#endif
