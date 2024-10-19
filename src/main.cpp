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

	const Shader shader("assets/shaders/obstacle.glsl");

	const std::vector<float> vertices = {
		-0.4f, -0.4f,
		 0.0f,  0.5f,
		 0.4f, -0.4f
	};

	const Model obstacleModel(vertices, shader);
	Drawable obstacle(obstacleModel);
	obstacle.xScale = 0.3f;
	obstacle.yScale = 0.3f;
	Drawable obstacle2(obstacleModel);
	obstacle2.xScale = 0.3f;
	obstacle2.yScale = 0.3f;

	obstacleModel.shader->setUniform("color", 1.f, 0.f, 0.f);

    while (!window.shouldClose()) {
		if(window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window.close();

		auto [winx, winy] = window.getWindowSize();
		if(winx > winy)
			obstacleModel.shader->setUniform("globalScale", winy/winx, 1.f);
		else 
			obstacleModel.shader->setUniform("globalScale", 1.f, winx/winy);

		obstacle.render();
		obstacle2.render();
		if(window.getKey(GLFW_KEY_A) == GLFW_PRESS)
			obstacle.x -= 0.01f;
		if(window.getKey(GLFW_KEY_D) == GLFW_PRESS)
			obstacle.x += 0.01f;
		if(window.getKey(GLFW_KEY_W) == GLFW_PRESS)
			obstacle.y += 0.01f;
		if(window.getKey(GLFW_KEY_S) == GLFW_PRESS)
			obstacle.y -= 0.01f;
		if(window.getKey(GLFW_KEY_Q) == GLFW_PRESS)
			obstacle.angle += 0.05f;
		if(window.getKey(GLFW_KEY_E) == GLFW_PRESS)
			obstacle.angle -= 0.05f;

		window.endFrame();
    }

    return 0;
}

