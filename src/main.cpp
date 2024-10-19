#include "window.hpp"
#include "input.hpp"
#include "obstacle.hpp"
#include "player.hpp"

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
		if(Input::isKeyPressed("ESCAPE"))
			window.close();

		if(Input::isKeyPressed("LEFT"))
			player.transform.x -= 0.01f;
		if(Input::isKeyPressed("RIGHT"))
			player.transform.x += 0.01f;
		if(Input::isKeyPressed("UP"))
			player.transform.y += 0.01f;
		if(Input::isKeyPressed("DOWN"))
			player.transform.y -= 0.01f;

		obs.render();
		player.render();

		window.endFrame();
    }
}

