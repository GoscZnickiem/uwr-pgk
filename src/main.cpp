#include "obstacle.hpp"
#include "player.hpp"
#include "window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

void initializeGLFW() {
	if (glfwInit() == 0) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}
	std::atexit([](){
		glfwTerminate();
	});
}

int main() {
	initializeGLFW();
	Window window;

	Obstacle obs{0, 0, 0, 0.1f, 0.1f};
	Player player{-0.5f, -0.5f};

    while (!window.shouldClose()) {
		if(window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window.close();

		if(window.getKey(GLFW_KEY_A) == GLFW_PRESS)
			player.transform.x -= 0.01f;
		if(window.getKey(GLFW_KEY_D) == GLFW_PRESS)
			player.transform.x += 0.01f;
		if(window.getKey(GLFW_KEY_W) == GLFW_PRESS)
			player.transform.y += 0.01f;
		if(window.getKey(GLFW_KEY_S) == GLFW_PRESS)
			player.transform.y -= 0.01f;
		if(window.getKey(GLFW_KEY_Q) == GLFW_PRESS)
			player.transform.angle += 0.05f;
		if(window.getKey(GLFW_KEY_E) == GLFW_PRESS)
			player.transform.angle -= 0.05f;

		obs.render();
		player.render();

		window.endFrame();
    }
}

