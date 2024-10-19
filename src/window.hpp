#ifndef _GZN_PGK_WINDOW_
#define _GZN_PGK_WINDOW_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utility>

class Window {
public:
	Window();

	void endFrame();
	void close();
	int getKey(int key) const; // temp
	bool shouldClose();
	std::pair<float, float> getWindowSize();

private:
	GLFWwindow* m_ID;
};

#endif
