#include "drawable.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

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

	const Shader obstacleShader("assets/shaders/obstacle.glsl");
	const Shader playerShader("assets/shaders/player.glsl");

	const std::vector<float> vertices = {
		-0.4f, -0.4f,
		 0.0f,  0.5f,
		 0.4f, -0.4f
	};

	const Model obstacleModel(vertices, obstacleShader);
	Drawable obstacle(obstacleModel);
	obstacle.xScale = 0.3f;
	obstacle.yScale = 0.3f;
	Drawable obstacle2(obstacleModel);
	obstacle2.xScale = 0.3f;
	obstacle2.yScale = 0.3f;
	obstacle2.y = 0.3f;

	const std::vector<float> verticesP = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
	};

	const Model playerModel(verticesP, {0, 1, 2, 2, 1, 3}, playerShader);

	Drawable player(playerModel);
	player.xScale = 0.1f;
	player.yScale = 0.1f;
	player.x = 0.5f;

    while (!window.shouldClose()) {
		if(window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window.close();

		if(window.getKey(GLFW_KEY_A) == GLFW_PRESS)
			player.x -= 0.01f;
		if(window.getKey(GLFW_KEY_D) == GLFW_PRESS)
			player.x += 0.01f;
		if(window.getKey(GLFW_KEY_W) == GLFW_PRESS)
			player.y += 0.01f;
		if(window.getKey(GLFW_KEY_S) == GLFW_PRESS)
			player.y -= 0.01f;
		if(window.getKey(GLFW_KEY_Q) == GLFW_PRESS)
			player.angle += 0.05f;
		if(window.getKey(GLFW_KEY_E) == GLFW_PRESS)
			player.angle -= 0.05f;

		obstacle.render();
		obstacle2.render();
		player.render();

		window.endFrame();
    }
}

