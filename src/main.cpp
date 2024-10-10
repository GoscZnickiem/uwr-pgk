#include "test.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <map>
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

class Window {
public:
	Window() {
		const int width = 800;
		const int height = 600;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		ID = glfwCreateWindow(width, height, "Hello OpenGL", nullptr, nullptr);
		if(ID == nullptr) {
			std::cerr << "Window creation failed\n";
			exit(2);
		}


		glfwMakeContextCurrent(ID);

		if (glewInit() != GLEW_OK) {
			std::cerr << "GLEW initialization failed\n";
			exit(3);
		}

		glViewport(0, 0, width, height);
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(ID) != 0;
	}

	void swapBuffers() {
		glfwSwapBuffers(ID);
	}

private:
	GLFWwindow* ID;
};

int main() {
	test();
	initializeGLFW();
	Window window;

	std::map<int, std::string> m = {{5, "lol"}, {3, "a"}};
	if(m.contains(5)) std::cout << "a\n";

	std::cout << glGetString(GL_VERSION) << "\n";

	const char* vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(vertexShader, logLength, &logLength, message.data());
		std::cerr << "Compilaton of a shader failed. Error:\n" << message << "\n";
		return 10;
	}

	const char* fragmentShaderSource = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
			"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";

	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(fragmentShader, logLength, &logLength, message.data());
		std::cerr << "Compilaton of a shader failed. Error:\n" << message << "\n";
		return 10;
	}

	const uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(shaderProgram, logLength, &logLength, message.data());
		std::cerr << "Linking of shaders failed. Error:\n" << message << "\n";
		return 10;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);

	const std::array<float, 12> vertices = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	const std::array<uint32_t, 6> indices = {
		0, 1, 3,
		1, 2, 3
	};

	glBindVertexArray(vao);

	uint32_t vbo = 0;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	uint32_t ebo = 0;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		DON'T
	glBindVertexArray(0);

    while (!window.ShouldClose()) {
		glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		glBindVertexArray(0);

		window.swapBuffers();

        glfwPollEvents();
    }

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    return 0;
}

