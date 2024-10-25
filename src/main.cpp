#include "window.hpp"
#include "appdata.hpp"
#include "input.hpp"
#include "obstacle.hpp"
#include "player.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <random>

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

	std::vector<Obstacle> obstacles;
	Player player;

	Application(long unsigned int seed = static_cast<long unsigned int>(std::time(nullptr)), int size = 10)
		: gridSize(boardSize / static_cast<float>(size)), 
		  player( - boardSize / 2 + gridSize / 2, - boardSize / 2 + gridSize / 2, -0.78f, gridSize * 0.1f, gridSize * 0.3f) {

		std::cout << static_cast<long unsigned int>(std::time(nullptr)) << "\n";
		obstacles.reserve(static_cast<size_t>(size) * static_cast<size_t>(size));
		std::mt19937 gen(seed);


		std::uniform_real_distribution<float> angleDis(-3.14f, 3.14f);
		std::uniform_real_distribution<float> scaleDis(0.8f, 1.0f);

		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				if((i == 0 && j == 0) || (i == size - 1 && j == size - 1)) continue;
				const float scale = scaleDis(gen) * gridSize * 0.18f;
				obstacles.emplace_back(
					static_cast<float>(i) * gridSize - boardSize / 2 + gridSize / 2,
					static_cast<float>(j) * gridSize - boardSize / 2 + gridSize / 2, 
					angleDis(gen), scale, scale * 3);
			}
		}
	}

	void update() {
		player.update();
		if(Input::isKeyPressed("ESCAPE"))
			window.close();
	}

	void render() {
		for(auto& o : obstacles) o.render();
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

