#ifndef _GZN_PGK_CORE_WINDOW_
#define _GZN_PGK_CORE_WINDOW_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <utility>

class Window {
public:
	Window();

	void endFrame();
	void close();
	bool shouldClose();
	std::pair<float, float> getWindowSize();

private:
	GLFWwindow* m_ID;

	std::function<void(int, int)> m_resizeCallback;
};

#endif
