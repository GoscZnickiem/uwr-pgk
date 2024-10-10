#include "shader.hpp"
#include "window.hpp"
#include <array>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <string>

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

	std::cout << glGetString(GL_VERSION) << "\n";

	const Shader shader("assets/shaders/shader.glsl");

	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);

	const std::array<float, 18> vertices = {
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,

		-0.5f,  0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.5f,  0.0f, 0.0f,
	};

	glBindVertexArray(vao);

	uint32_t vbo = 0;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    while (!window.ShouldClose()) {
		glClearColor(0.7f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		window.swapBuffers();

        glfwPollEvents();

		if(window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			window.close();
		}
    }

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}

