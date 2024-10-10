#ifndef _GZN_PGK_WINDOW_
#define _GZN_PGK_WINDOW_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	bool ShouldClose();
	void swapBuffers();
	int getKey(int key) const;
	void close();

private:
	GLFWwindow* ID;
};

#endif
