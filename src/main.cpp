#include "core/window.hpp"
#include "core/input.hpp"
#include "appdata.hpp"
#include "obstacle.hpp"
#include "obstacleCollection.hpp"
#include "player.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <chrono>
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

struct Application {
	Window window{};

	const float boardSize = 1.8f;
	const float gridSize;

	ObstacleCollection obstacles;
	Player player;

	Application(long unsigned int seed = static_cast<long unsigned int>(std::time(nullptr)), int size = 10)
		: gridSize(boardSize / static_cast<float>(size)), 
		  obstacles(seed, size, boardSize, gridSize),
		  player( - boardSize / 2 + gridSize / 2, - boardSize / 2 + gridSize / 2, -0.78f, gridSize * 0.1f, gridSize * 0.3f) {
	}

	void update() {
		player.update();
		if(Input::isKeyPressed("ESCAPE"))
			window.close();
	}

	void render() {
		obstacles.render();
		player.render();

		window.endFrame();
	}

	void run() {
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeBetweenFrames{};
		double lag = 0.0;

		const double fpsInterval = 1.;
		double fpsTimer = 0.0;
		uint fpsCount = 0;

		const auto timePerUpdate = AppData::data().timePerUpdate;

		while (!window.shouldClose()) {
			now = std::chrono::steady_clock::now();
			timeBetweenFrames = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime);
			lastFrameTime = std::chrono::steady_clock::now();

			lag += timeBetweenFrames.count();
			fpsTimer += timeBetweenFrames.count();

			while(lag >= timePerUpdate ) {
				lag -= timePerUpdate;
				update();
			}

			render();

			fpsCount++;
			if (fpsTimer >= fpsInterval) {
				fpsTimer = 0.;
				std::cout << "FPS: " << static_cast<uint>(static_cast<double>(fpsCount) / fpsInterval) << "\n";
				fpsCount = 0;
			}
		}
	}
};

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main() {
	initializeGLFW();
	Application app;

	glDebugMessageCallback(MessageCallback, nullptr);

	app.run();
}

