#include "Model.hpp"
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

	const Shader shader("assets/shaders/shader.glsl");

	const std::vector<float> vertices = {
		-0.5f, -0.6f,
		 0.0f,  0.0f,
		 0.5f, -0.6f
	};

	const Model obstacle(vertices, shader);
	obstacle.bind();

    while (!window.ShouldClose()) {
		if(window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window.close();

		obstacle.render();

		window.endFrame();
    }

    return 0;
}

